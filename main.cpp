#include <iostream>
#include <unistd.h>     // For fork(), execvp(), etc.
#include <sys/wait.h>   // For wait()
#include <cstring>      // For strtok()
#include <cstdlib>      // For exit()

void displayPrompt() {
    std::cout << "-> ";
}

int main() {
    system("clear");
    std::cout << "\033[1;32mWelcome to FSH (Flex Shell)\033[0m\n";
    char command[1024];  // To store user input

    while (true) {
        displayPrompt();
        std::cin.getline(command, 1024); // Read input

        if (std::strcmp(command, "exit") == 0) { // Exit command
            break;
        }

        char *args[64]; // Array to hold command and arguments
        char *token = std::strtok(command, " ");
        int index = 0;

        while (token != nullptr) {
            args[index++] = token; // Split the command into arguments
            token = std::strtok(nullptr, " ");
        }
        args[index] = nullptr; // Null-terminate the array

        pid_t pid = fork(); // Create a child process

        if (pid < 0) {
            std::cerr << "Fork failed!" << std::endl;
            continue;
        }

        if (pid == 0) {
            // Child process: Execute the command
            if (execvp(args[0], args) < 0) {
                std::cerr << "Error executing command!" << std::endl;
            }
            exit(0); // Exit child process after execution
        } else {
            // Parent process: Wait for the child to finish
            wait(nullptr);
        }
    }

    return 0;
}

