#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <pwd.h>
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <cstdlib>
#include <limits.h>

std::vector<std::string> commandHistory; // Store command history

// Function to get the current working directory (shortened)
std::string getCurrentDir() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd))) {
        std::string fullPath = cwd;
        size_t pos = fullPath.find_last_of("/");
        return (pos != std::string::npos) ? "~" + fullPath.substr(pos) : fullPath;
    }
    return "unknown";
}

// Function to get the current Git branch & status
std::string getGitStatus() {
    FILE* pipe = popen("git status --porcelain 2>/dev/null", "r");
    if (!pipe) return "";

    char buffer[128];
    bool isClean = true;

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        isClean = false;
    }
    pclose(pipe);

    // Get branch name
    FILE* branchPipe = popen("git branch --show-current 2>/dev/null", "r");
    if (!branchPipe) return "";
    
    std::string branch;
    if (fgets(buffer, sizeof(buffer), branchPipe) != nullptr) {
        branch = buffer;
    }
    pclose(branchPipe);
    
    branch.erase(branch.find_last_not_of(" \n\r\t") + 1); // Trim whitespace

    if (!branch.empty()) {
        return "\033[1;35m " + branch + (isClean ? " ✔" : " ✗") + " ";
    }
    return "";
}

// Function to get current time
std::string getCurrentTime() {
    time_t now = time(0);
    struct tm* t = localtime(&now);
    char buffer[10];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", t);
    return std::string(buffer);
}

// Function to display the unique shell prompt
void displayPrompt() {
    struct passwd *pw = getpwuid(geteuid());
    std::string username = (pw && pw->pw_name) ? pw->pw_name : "user";

    char hostname[HOST_NAME_MAX];
    gethostname(hostname, sizeof(hostname));

    std::string currentDir = getCurrentDir();
    std::string gitStatus = getGitStatus();
    std::string currentTime = getCurrentTime();

    // Print a unique, stylish prompt with time on the same line
    std::cout << "\n\033[1;36m" << username << "@" << hostname; // Cyan username@hostname
    std::cout << " \033[1;34m🕒 " << currentTime << " "; // Blue time
    std::cout << "\033[1;33m" << currentDir << " "; // Yellow directory
    std::cout << gitStatus; // Magenta Git branch if available
    std::cout << "\n\033[1;32m🚀 ➜ \033[0m"; // Green input arrow
    std::cout.flush();
}



void executeCommand(std::vector<std::string> &args) {
  std::vector<char *> c_args;
  for (const auto &arg : args) {
    c_args.push_back(const_cast<char *>(arg.c_str()));
  }
  c_args.push_back(nullptr);

  execvp(c_args[0], c_args.data());
  std::cerr << "Error: Command not found!\n";
  exit(EXIT_FAILURE);
}

// Handle built-in commands like 'cd'
bool handleBuiltInCommands(const std::vector<std::string> &args) {
  if (args.empty())
    return false;

  if (args[0] == "cd") {
    if (args.size() < 2) {
      std::cerr << "cd: Missing directory argument\n";
    } else {
      if (chdir(args[1].c_str()) != 0) {
        std::cerr << "cd: No such directory: " << args[1] << std::endl;
      }
    }
    return true;
  } else if (args[0] == "history") {
    std::cout << "\nCommand History:\n";
    for (size_t i = 0; i < commandHistory.size(); ++i) {
      std::cout << i + 1 << ". " << commandHistory[i] << std::endl;
    }
    return true;
  }

  return false;
}

int main() {
  std::string input;
  std::cout << "\033[1;32mWelcome to FSH (Flex Shell)\033[0m\n";

  while (true) {
    displayPrompt();

    std::getline(std::cin, input);
    if (input.empty())
      continue;

    commandHistory.push_back(input);

    if (input == "exit")
      break;

    std::vector<std::string> args;
    std::istringstream stream(input);
    std::string token;
    while (stream >> token)
      args.push_back(token);

    if (handleBuiltInCommands(args))
      continue; // Execute built-in commands without forking

    pid_t pid = fork();
    if (pid < 0) {
      std::cerr << "Fork failed!" << std::endl;
      continue;
    }

    if (pid == 0)
      executeCommand(args);
    else
      wait(nullptr);
  }

  return 0;
}
