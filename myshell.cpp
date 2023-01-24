#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
using namespace std;

void createPipes(int pipAmnt, int pipefd[][2]) { // create pipe function
    for (int i = 0; i < pipAmnt; i++){
        if (pipe(pipefd[i]) < 0) {
            perror("pipe"); // CAN BE REMOVED 
            exit(1);
        }
    }
}


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
    const int totalCMD = rowCnt + 1;
    int pipefd[rowCnt][2];
    createPipes(totalCMD-1, pipefd);
    

    for (int i = 0; i < totalCMD; i++) {
        int pid = fork();
        if (pid == 0) { //if program is in child process
            vector<string> tempArr;
            int processTokenAmnt=0;
            int z = 0;
            while(buf[i][z]!="\0")  {   //stores a row into a temp vector to make processing easier
                tempArr.push_back(buf[i][z]);
                processTokenAmnt++;
                z++;
            }



            
            char* argv[processTokenAmnt]; //sets argv size and adds 1 for NULL at end
            
            for (int j = 0; j < processTokenAmnt; j++) {  //iterates through the cmd array
                argv[j] = &tempArr[j][0];   //stores the address for each element in cmd array into argv
                cout<<argv[j]<<endl; //TESTING!!!!
            }
            argv[processTokenAmnt] = NULL;


            //Setting up Pipes and closing uneeded pipes
            if (i != 0) {   //dont setup pipe for the first command
                dup2(pipefd[i - 1][0], 0);
            }
            else if (i != totalCMD - 1) {   // dont setup pipe for the second command
                dup2(pipefd[i][1], 1);
            }
            for (int j = 0; j < totalCMD-1; j++) {
                close(pipefd[j][0]);
                close(pipefd[j][1]);
            }

            //sets last array element to NULL to signify end of array
            execvp(argv[0], argv);  //morphs child process to do command that is passed in argv
            exit(0);   //exits child process
        
        }
    }


    /* remove below not urs*/
    for (int i = 0; i < totalCMD-1; i++) {  //closes pipes in parent process
        close(pipefd[i][0]);
        close(pipefd[i][1]);
    }
    for (int i = 0; i < totalCMD; i++) {    //tells parent to wait for child execution
        int status;
        int pid = wait(&status);
        cout << "process " << pid << " exits with " << status << endl;
    }

    /*end of it*/

}
