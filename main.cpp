#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <pwd.h>
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

std::vector<std::string> commandHistory; // Store command history

void showTime() {
  auto now = std::chrono::system_clock::now();
  std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
  std::tm *localTime = std::localtime(&currentTime);

  std::cout << "\r\033[1;34m[" << std::setfill('0') << std::setw(2)
            << localTime->tm_hour << ":" << std::setfill('0') << std::setw(2)
            << localTime->tm_min << ":" << std::setfill('0') << std::setw(2)
            << localTime->tm_sec << "]\033[0m" << std::flush;
}

std::string getCurrentDir() {
  char buffer[1024];
  if (getcwd(buffer, sizeof(buffer)) != nullptr) {
    return std::string(buffer);
  }
  return "";
}

void displayPrompt() {
  struct passwd *pw = getpwuid(geteuid()); // Get user info
  std::string username = (pw) ? pw->pw_name : "user"; // Fallback if NULL

  std::cout << "\n\033[1;32m" << username << "@" << getCurrentDir() << "\033[0m$ ";
  std::cout.flush(); // Force output immediately
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
    showTime();
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
