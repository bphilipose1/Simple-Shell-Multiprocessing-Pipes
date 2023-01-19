#include<iostream>
#include<string>
#include  <bits/stdc++.h>
#include<vector>

using namespace std;

int main(){
    string line = "Hello world!";
    vector <string> tokens;
    stringstream check1(line);
    string something;

    while (getline(check1,something,' '))
    {
        tokens.push_back(something);
    }
    for(int i = 0; i < tokens.size();i++)
    cout << tokens[i] << endl;
    return 0;
}
