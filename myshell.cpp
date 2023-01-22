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
    cout << "myshell$";
    getline(cin, inputCMD);

    //test input COPY AND PASTE WHEN PROMPTED :  ls -alF / | grep bin | tr a-z A-Z | rev | cat -n
    inputCMD += " ";    // space indicates end of string
    string token;


    for (auto tempTok : inputCMD)    {  // itereates through each character from input string
        if (tempTok == ' ' && token.length() == 0)  {//do nothing if it reads space and nothing is in token (also deals with extra spaces between command tokens)
            cout << "With \"" << tempTok << "\" it ended in CASE 1" << endl;
            continue;  
            
        }
        else if (tempTok == ' ' && token.length() != 0) {   //space after a word indicates it has been put fully in token therefore add to 2D array
            cout << "With \"" << tempTok << "\" it ended in CASE 2" << endl;
            buf[rowCnt][clmnCnt] = token;
            clmnCnt++;
            token = ""; //clears up token for next word
            
        }
        else if(tempTok == '|') {   //if pipe is read it adds "\0" to signify end of the row and changes write to new row to signify new process command
            cout << "With \"" << tempTok << "\" it ended in CASE 3" << endl;
            clmnCnt++;
            buf[rowCnt][clmnCnt] = "\0";
            rowCnt++;
            clmnCnt = 0;
            
        }
        else    {   //iterates through characters of a word
            cout << "With \"" << tempTok << "\" it ended in CASE 4" << endl;
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