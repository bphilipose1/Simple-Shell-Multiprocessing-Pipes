#include<iostream>
#include<sys/wait.h>
#include<string>
#include<cstring>
#include <unistd.h>

using namespace std;

string**parsing(string input);
void pipeing(int num_of_pipes,int pipefd[][2]);
void execute(string** commands, int num_of_pipe, int pipefd[][2]);
void deallocate(string** arr, int num_of_pipes);

int main(int argc, char* argv[]){
    cout <<"myshell$ ";
    string input;
    getline(cin, input);

    string**command = parsing(input);
    int num_of_pipes = sizeof(command);
    int pipe_desc[num_of_pipes][2];
    pipeing(num_of_pipes,pipe_desc);
    execute(command,num_of_pipes,pipe_desc);
    deallocate(command, num_of_pipes);
    
    return 0;
}

string**parsing(string inputCMD){
    const int max_NUM = 20;
    const int max_SZ = 20;
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
    return buf;
}

void pipeing(int num_of_pipes,int pipefd[][2]){
    // creating pipes
    for (int i = 0; i < num_of_pipes; i++) {
        if (pipe(pipefd[i]) < 0) {
            perror("pipe");
            exit(1);
        }
    }
} 
void execute(string** commands, int num_of_pipes, int pipefd[][2]) {
    int command_count = num_of_pipes + 1;
    for (int i = 0; i < command_count; i++) {
        int pid = fork();
        if (pid == 0) {
            // Debugging code to check the contents of the 2D array
            cout << "Command " << i << ": ";
            for (int j = 0; j < commands[i]->length(); j++) {
                cout << commands[i][j] << " ";
            }
            cout << endl;

            if (i != 0) {
                dup2(pipefd[i - 1][0], 0);
            }
            if (i != command_count - 1) {
                dup2(pipefd[i][1], 1);
            }
            for (int j = 0; j < num_of_pipes; j++) {
                close(pipefd[j][0]);
                close(pipefd[j][1]);
            }
            // convert commands[i] to a char** array for execvp
            int size = commands[i]->length();
            char** argv = new char*[size + 1];
            for (int j = 0; j < size; j++) {
                argv[j] = const_cast<char*>(commands[i][j].c_str());
            }
            argv[size] = NULL;
            // execute command
            if(execvp(argv[0], argv) < 0)
            {
                cout<<"execvp failed!"<<endl;
                exit(1);
            }
            exit(0);
        }
    }
}



void deallocate(string** arr, int num_of_pipes) {
    for (int i = 0; i <= num_of_pipes; i++) {
        delete[] arr[i];
    }
    delete[] arr;
}

