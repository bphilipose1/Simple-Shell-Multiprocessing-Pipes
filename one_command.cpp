#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

using namespace std;

int main() {
    while (true) {
        cout << "myshell$ ";
        string input;
        getline(cin, input);

        char* cmd = strtok(&input[0], " "); // parse command name
        char* args[20]; // array to hold arguments
        args[0] = cmd;

        // parse arguments
        for (int i = 1; i < 20; i++) {
            args[i] = strtok(NULL, " ");
            if (args[i] == NULL)
                break;
        }

        int pid = fork();
        if (pid == 0) {
            execvp(cmd, args); // child process executing the command
            exit(0);
        } else {
            int status;
            waitpid(pid, &status, 0); // parent process waiting for child to terminate
            cout << "process " << pid << " exits with " << status << endl;
        }
    }
    return 0;
}
