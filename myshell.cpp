// myshell.cpp
// Jan 24 2023 
// A simple shell program
 

#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

void createPipes(int pipAmnt, int pipefd[][2]) { // create pipe function
    for (int i = 0; i < pipAmnt; i++){
        if (pipe(pipefd[i]) < 0) {
            perror("pipe\n");  
            exit(1);
        }
    }
}


int main(int argc, char* argv[])    {
    //creating buffer for tokens
    const int max_NUM = 20;
    const int max_SZ = 20;
    string buf[max_SZ][max_NUM];

    int rowCnt = 0;
    int clmnCnt = 0;

   

    string inputCMD;
    cout << "myshell$ ";
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

    int totalCMD = rowCnt + 1;
    const int maxPipes = max_NUM - 1;   //maximum pipes program will need (max # of tokens in command - 1)
    int pipefd[maxPipes][2];          
    int neededPipes = totalCMD - 1; //number of necessary pipes needed to connect processes
    createPipes(neededPipes, pipefd);

    for (int i = 0; i < totalCMD; i++) {
        int pid = fork();
        if (pid == 0) { //if program is in child process
            vector<string> tempArr; 

            char* argv[max_SZ]; //sets argv size to maximum needed to avoid variable array flag

            int processTokenAmnt = 0; //used to know the amount of tokens in command
            int z = 0;

            while(buf[i][z]!="\0")  {   //stores a row into a temp vector to make processing easier
                tempArr.push_back(buf[i][z]);
                processTokenAmnt++;
                z++;
            }
         
            for (int j = 0; j < processTokenAmnt; j++) {  //iterates through the cmd array                
                argv[j] = strdup(tempArr[j].c_str());   //stores the address for each element in cmd array into argv
            }

            argv[processTokenAmnt] = NULL;//sets last array element to NULL to signify end of array
            
            //Setting up Pipes and closing uneeded pipes
            if (i != 0) {   //dont setup pipe for the first command
                dup2(pipefd[i - 1][0], 0);
            }
            if (i != totalCMD-1) {   // dont setup pipe for the final command
                dup2(pipefd[i][1], 1);
            }
            for (int j = 0; j < neededPipes; j++) {    //closes all extra open file descriptors 
                close(pipefd[j][0]);
                close(pipefd[j][1]);
            }
            
            if(execvp(argv[0], argv) < 0){//morphs child process to do command according to parameters passed in argv
                perror("Execute Erorr!\n"); 
                exit(1);
            }  
            exit(0);   //exits child process
        
        } else if(pid < 0){
            perror("Fork Error!\n");
            exit(1);
        }
    }

    for (int i = 0; i < neededPipes; i++) {  //closes pipes in parent process
        close(pipefd[i][1]);
        close(pipefd[i][0]);
    }
    for (int i = 0; i < totalCMD; i++) {    //tells parent to wait for child execution
        int retStatus;
        int proc_pid = wait(&retStatus);
        cout << "process " << proc_pid << " exits with " << retStatus << endl;
    }

    return 0;
}
