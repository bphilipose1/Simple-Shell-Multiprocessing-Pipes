#include <iostream>
#include <cstring>
#include <string>
using namespace std;
int main(int argc, char* argv[])    {
    //creating buffer for tokens
    const int max_NUM = 20;
    const int max_SZ = 20;
    char buf[max_SZ][max_NUM];
    int rowCnt = 0;
    int clmnCnt = 0;


    string inputCMD;
    char tempChar;
    cout << "myshell$";
    cin >> inputCMD;

    const int charlength = inputCMD.length() + 1;

    for(int it; it<charlength; it++) { //cycles through each string in argv
        tempChar = inputCMD[it];
        if(tempChar == " ")  {//put \0 for space
        buf[rowCnt][clmnCnt] = NULL;
        clmnCnt++;
        }

        else if(tempChar = "|") {//start new row in buf 2D array
        rowCnt++;
        }

        else {//add to new column in buf 2D Array
        *buf[rowCnt][clmnCnt] = tempChar;
        clmnCnt++; 
        }

        it++;
    }
    cout << buf[0][0] << endl;
    


}