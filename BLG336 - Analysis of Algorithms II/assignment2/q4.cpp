// https://www.hackerrank.com/challenges/sherlock-and-the-beast/problem

#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

/*
 * Complete the 'decentNumber' function below.
 *
 * The function accepts INTEGER n as parameter.
 */

void decentNumber(int n) {
    int count5 = 0;
    int count3 = 0;
    int counter = 0;

    while(n >= 0){
        if(n % 5 == 0){
            count3 = n;
            count5 = counter * 3; 
        }
        n -= 3;
        counter += 1;
    }

    string res;
    res += string(count5, '5');
    res += string(count3, '3');


    /*
    for(int i = 0; i < count3; i++){
        res += 3*pow(10,i);
    }
    for(int i = count3; i < count5+count3; i++){
        res += 5*pow(10,i);
    }
        */
    if(res.size() == 0) cout << "-1" << "\n";
    else cout << res << "\n";

}

int main()
{
    string t_temp;
    getline(cin, t_temp);

    int t = stoi(ltrim(rtrim(t_temp)));

    for (int t_itr = 0; t_itr < t; t_itr++) {
        string n_temp;
        getline(cin, n_temp);

        int n = stoi(ltrim(rtrim(n_temp)));

        decentNumber(n);
    }

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}
