#include <iostream>
#include <cstring>
#include <string>
using namespace std;
int main(int argc, char* argv[])    {
    //creating buffer for tokens
    const int max_NUM = 20;
    const int max_SZ = 20;
    string buf[max_SZ][max_NUM];

    //counters to detail buf utilized size  **includes \0 at end of row**
    int rowCnt = 0;
    int clmnCnt = 0;
   

    string inputCMD;
    cout << "myshell$";
    getline(cin, inputCMD);

    inputCMD += " ";  // added space indicating end of string
    string token;


    for (auto tempTok : inputCMD)    {  // itereates through each character from input string
        if (tempTok == ' ' && token.length() == 0)  {//do nothing if it reads space and nothing is in token (also deals with extra spaces between command tokens)          
            continue;  
        }
        else if (tempTok == ' ' && token.length() != 0) {   //space after a word indicates it has been put fully in token therefore add to 2D array
            buf[rowCnt][clmnCnt] = token;
            clmnCnt++;
            token = ""; //clears up token for next word        
        }
        else if(tempTok == '|') {   //if pipe is read it adds "\0" to signify end of the row and changes write to new row to signify new process command
            clmnCnt++;
            buf[rowCnt][clmnCnt] = "\0";
            rowCnt++;
            clmnCnt = 0;        
        }
        else    {   //adds parsed characters to rebuild word as a token from input string
            token += tempTok;//combines characters to form a token        
        }
    }
}