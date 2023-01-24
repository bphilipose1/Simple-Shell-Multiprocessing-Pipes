#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

#define max_NUM 20
#define max_SZ 20

using namespace std;

void createPipes(int pipAmnt , int pipefd[][2]);
string**parsing(string input,int &totalCMD);
void execute(string**,int,int);



void createPipes(int pipAmnt, int pipefd[][2]) { // create pipe function
    for (int i = 0; i < pipAmnt; i++){
        if (pipe(pipefd[i]) < 0) {
            perror("pipe"); // CAN BE REMOVED 
            exit(1);
        }
    }
}



string**parsing(string inputCMD,int &totalCMD){
    
    string** buf = new string*[max_SZ];
    for (int i = 0 ; i < max_SZ;i++){
        buf[i] = new string[max_NUM];
    }

    //counters to detail buf utilized size  **includes \0 at end of row**
    int rowCnt = 0;
    int clmnCnt = 0;
   
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
    totalCMD = rowCnt + 1;
    return buf;
}

void execute(string**buf ,int totalCMD,int neededPipes,int pipefd[][2]){
    for (int i = 0; i < totalCMD; i++) {
        int pid = fork();
        if (pid == 0) { //if program is in child process
            vector<string> tempArr; 
            char* argv[max_SZ]; //sets argv size to maximum needed to avoid variable array flag

            int processTokenAmnt=0; //used to know the amount of tokens in command
            int z = 0;
            while(buf[i][z]!="\0")  {   //stores a row into a temp vector to make processing easier
                tempArr.push_back(buf[i][z]);
                processTokenAmnt++;
                z++;
            }
            
            for (int j = 0; j < processTokenAmnt; j++) {  //iterates through the cmd array
                argv[j] = &tempArr[j][0];   //stores the address for each element in cmd array into argv
            }
            argv[processTokenAmnt] = NULL;//sets last array element to NULL to signify end of array
            
            //Setting up Pipes and closing uneeded pipes

            if (i != 0) {   //dont setup pipe for the first command
                dup2(pipefd[i - 1][0], 0);
            }
            else if (i != totalCMD-1) {   // dont setup pipe for the final command
                dup2(pipefd[i][1], 1);
            }
            for (int j = 0; j < neededPipes; j++) {    //closes all extra open file descriptors 
                close(pipefd[j][0]);
                close(pipefd[j][1]);
            }

            
            execvp(argv[0], argv);  //morphs child process to do command according to parameters passed in argv
            exit(0);   //exits child process
        
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


}

int main(int argc , char *argv[]){
    int totalCMD = 0;

    cout << "myshell$ ";
    string inputCMD;
    getline(cin,inputCMD);

    string**Command = parsing(inputCMD,totalCMD);
    const int maxPipes = max_NUM -1;
    int pipefd[maxPipes][2];
    int neededPipes = totalCMD - 1;
    createPipes(neededPipes,pipefd);
    execute(Command,totalCMD,neededPipes,pipefd);

    return 0;


}


