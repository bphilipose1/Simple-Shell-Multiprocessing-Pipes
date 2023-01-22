#include <iostream>
#include <cstring>
#include <string>
using namespace std;
int main(int argc, char* argv[])    {
    //creating buffer for tokens
   
    const int max_NUM = 20;
    const int max_SZ = 20;
    string buf[max_SZ][max_NUM];
    int rowCnt = 0;
    int clmnCnt = 0;
   

    string inputCMD;
    //char tempChar;
    //cout << "myshell$";
    //cin >> inputCMD;

    //test input
    inputCMD = "helllo worlllld | worlld hello | wold hllo fls | f kdk | -li";
    inputCMD += " ";    // space indicates end of string
    string token;


    for (auto tempTok : inputCMD)    {  // itereates through each character from input string
        if (tempTok == ' ' && token.length() == 0)  {//ignore space after | symbol
            continue;  
        }
        else if (tempTok == ' ' && token.length() != 0) {   //space after a word indicates it has been put fully in token therefore add to 2D array
            cout << "Inserted: " << token << " At index(" << rowCnt << ", " << clmnCnt << ") ";
            buf[rowCnt][clmnCnt] = token;
            clmnCnt++;
            token = ""; //clears up token for next word
        }
        else if(tempTok == '|') {   //if pipe is read it adds "\0" to signify end of the row and changes write to new row to signify new process command
            clmnCnt++;
            buf[rowCnt][clmnCnt] = "\0";
            rowCnt++;
            clmnCnt = 0;
            cout << "" << endl;
        }
        else    {   //iterates through characters of a word
            token += tempTok;//combines characters to form a token 
        }
    }

    //testing 2D array contents
    cout << "\ntime to test array :(" << rowCnt << ")("<< clmnCnt <<")" << endl;
    for (int i = 0; i <= rowCnt; i++)
    {
        cout << "|";
        for (int j = 0; j <= clmnCnt; j++)
        {
            if(buf[i][j] == "\0")
                continue;
            cout << buf[i][j] << "|";
        }
        cout << "" << endl;
    }
    

}