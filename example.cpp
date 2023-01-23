#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>

using namespace std;

int main() {
    while (true) {
        cout << "myshell$ ";
        string input;
        getline(cin, input);

        vector<vector<string>> commands; // vector to hold commands and arguments
        vector<string> args;

        string token;
        for (auto c : input) {
            if (c == '|' || c == ' ') {
                if (!token.empty()) {
                    args.push_back(token);
                }
                if (c == '|') {
                    commands.push_back(args);
                    args.clear();
                }
                token.clear();
            } else {
                token += c;
            }
        }

        if (!token.empty()) {
            args.push_back(token);
        }
        commands.push_back(args);

        int num_pipes = commands.size() - 1; // number of pipes needed
        int pipefd[num_pipes][2]; // array to hold pipe file descriptors

        for (int i = 0; i < num_pipes; i++) {
            if (pipe(pipefd[i]) < 0) {
                perror("pipe");
                exit(1);
            }
        }

        for (int i = 0; i < commands.size(); i++) {
            int pid = fork();
            if (pid == 0) {
                if (i != 0) {
                    dup2(pipefd[i - 1][0], 0);
                }
                if (i != commands.size() - 1) {
                    dup2(pipefd[i][1], 1);
                }
                for (int j = 0; j < num_pipes; j++) {
                    close(pipefd[j][0]);
                    close(pipefd[j][1]);
                }
                vector<string> cmd = commands[i];   //takes first row of 2D array and stores into cmd array (signifies all tokens in 1 process)   EX: echo Hello world (1 process) 3 tokens
                char* argv[cmd.size() + 1]; //sets argv size to match size of 1 row in 2D array
                for (int j = 0; j < cmd.size(); j++) {  //iterates through the cmd array
                    argv[j] = &cmd[j][0];   //stores the address for each element in cmd array into argv
                }
                argv[cmd.size()] = NULL;    //sets last array element to NULL to signify end of array
                execvp(argv[0], argv);  //morphs child process to do command that is passed in argv
                exit(0);    //exits child process
            }
        }
        for (int i = 0; i < num_pipes; i++) {
            close(pipefd[i][0]);
            close(pipefd[i][1]);
        }
        for (int i = 0; i < commands.size(); i++) {
            int status;
            int pid = wait(&status);
            cout << "process " << pid << " exits with " << status << endl;
        }
    }
    return 0;
}
