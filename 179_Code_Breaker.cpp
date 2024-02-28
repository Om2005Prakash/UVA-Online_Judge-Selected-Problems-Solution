#include <iostream>
#include <string>
#include <set>
#include <map>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

using namespace std;

map<int, set<int>> check(string plaintext, string ciphertext) {
    int flag = 0;
    int reset=0;
    map<int, set<int>> Possible;
    int diff = ciphertext.length() - plaintext.length();
    for (int i = 1; i <= plaintext.length(); i++) {
        reset=0;
        int temp = i - (plaintext.length() % i);
        if (temp >= diff) {
            // cout << "Checking Periodicity " << i << endl;
            string s_plain = plaintext.substr(0, i);
            string s_cipher = ciphertext.substr(0, i);
            for (int m = 0; m < i; m++) {
                if (count(s_plain.begin(), s_plain.end(), s_plain[m]) != count(s_cipher.begin(), s_cipher.end(), s_plain[m])) {
                    reset=1;
                    break;
                }
            }
            if (reset == 1) continue;
            // cout<< "First group identitcal" <<endl;
            for (int i = 0; i < s_plain.length(); i++) {
                Possible[i] = {};
                for (int j = 0; j < s_plain.length(); j++) {
                    if (s_cipher[j] == s_plain[i]) {
                        Possible[i].insert(j);
                    }
                }
            }
            // cout << "For " << i << " 0 group is consistent, Moving On" << endl;
            for (const auto& i:Possible){
                for (auto& num:i.second){
                    // cout << i.first << num <<endl;
                }
            }
            if (ceil((float) plaintext.length() / i) == 1) return Possible;
            for (int j = 1; j < ceil((float) plaintext.length() / i); j++) {
                // cout<< "Checking " << j << " group" <<endl;
                string s_plain = plaintext.substr(j * i, i);
                string s_cipher = ciphertext.substr(j * i, i);
                set<int> update;
                // cout << "s_plain.length()= " << s_plain.length() << "and i = " << i << "and j= "<< j << endl;
                for (int k = 0; k < s_plain.length(); k++) {
                    update.clear();
                    for (auto &elem: Possible[k]) {
                        if (s_cipher[elem] == s_plain[k]) {
                            update.insert(elem);
                        }
                    }
                    if (update.size() == 0){
                        reset = 1 ;
                        break;
                    }
                    if (reset == 1) break;
                    Possible[k] = update;
                    for (auto& num:update){

                        // cout << "Updated "<< k << " index" << num <<endl;
                    }
                }
                if (j == (ceil((float) plaintext.length() / i)-1)) {
                    flag = 1;
                    break;
                }
                if (reset == 1) break;
            }
            if (reset == 1) continue;
            if (flag == 1) {
                return Possible;
            }
        }
    }
    // cout << "cming out" << endl;
    Possible.clear();
    return Possible;
}

string decipher(string inp , vector<int> mapper){
    string answer(mapper.size()*ceil((float) inp.length()/mapper.size()),'?');
    inp.resize(answer.length(),'?');
    // cout << inp << endl;
    int i=0;
    int j=0;
    for (i=0; i<inp.length(); i++)
    {
        if(i>j) j=i;
        if(mapper[i%mapper.size()]+mapper.size()*((int) i/mapper.size())<inp.size()){
            answer[i]=inp[mapper[i%mapper.size()]+mapper.size()*((int) i/mapper.size())];
        }
    }
    return answer;
}

string compute(string plain , string cipher1 , string cipher2){
    map<int, set<int>> Sol;
    if (plain.length() != cipher1.length()) {
        return cipher2;
    } else {
        Sol = check(plain, cipher1);
        if (Sol.size() == 0){
            return cipher2;
        }
        vector<int> mapper;
        mapper.resize(Sol.size());
        // cout << "Mapper Started" << endl;
        for (const auto& i:Sol){
            for (auto& num:i.second){
                mapper[i.first]=num;
                // cout << i.first << num <<endl;
            }
        }
        return decipher(cipher2 , mapper);
    }
}

int main(){
    vector<string> input_p;
    vector<string> input_c1;
    vector<string> input_c2;
    string plain;
    string cipher1;
    string cipher2;
    while (1)
    {
        getline(cin, plain);
        if (plain == "#"){
            break;
        }
        getline(cin, cipher1);
        getline(cin, cipher2);
        input_p.push_back(plain);
        input_c1.push_back(cipher1);
        input_c2.push_back(cipher2);
    }
    for(int j=0; j<input_c1.size(); j++){
        cout << compute(input_p[j] , input_c1[j] , input_c2[j]) <<endl;
    }
    return 0;
}