#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

const std::vector<std::string> BUILTINS = {"echo", "exit", "pwd", "type"};

std::string find_in_path(const std::string& command);
std::string join(const std::vector<std::string>& parts, size_t start);
void run_external(const std::string& path, const std::vector<std::string>& parts);
std::vector<std::string> split(const std::string& s);
std::vector<std::string> split(const std::string& s, char delim);

int main() {
  while (true) {
    std::cout << "$ " << std::flush;

    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> parts = split(input);
    if (parts.empty()) {
      continue;
      
    } else if (parts[0] == "exit") {
      break;

    } else if (parts[0] == "echo") {
      std::cout << join(parts, 1) << std::endl;

    } else if (parts[0] == "type") {
      for (size_t i = 1; i < parts.size(); ++i) {
        const std::string& command = parts[i];

        if (std::find(BUILTINS.begin(), BUILTINS.end(), command) != BUILTINS.end()) {
          std::cout << command << " is a shell builtin\n";
          continue;
        }

        std::string path = find_in_path(command);
        if (!path.empty()) {
          std::cout << command << " is " << path << '\n';
          continue;
        }

        std::cout << command << ": not found\n";
      }

    } else if (parts[0] == "pwd") {
      std::cout << std::filesystem::current_path().string() << "\n";

    } else {
      std::string path = find_in_path(parts[0]);
      if (!path.empty()) {
        run_external(path, parts);
      } else {
        std::cerr << parts[0] << ": command not found\n";
      }
    }

  }

  return 0;
}

std::string find_in_path(const std::string& command) {
  const char* path_env = std::getenv("PATH");
  if (path_env == nullptr) return "";

  std::vector<std::string> dirs = split(path_env, ':');

  for (const std::string& dir : dirs) {
    std::string full_path = dir + "/" + command;
    if (access(full_path.c_str(), X_OK) == 0) return full_path;
  }
  return "";
}

std::string join(const std::vector<std::string>& parts, size_t start) {
  std::string result;
  for (size_t i = start; i < parts.size(); ++i) {
    if (i > start) result += " ";
    result += parts[i];
  }
  return result;
}

void run_external(const std::string& path, const std::vector<std::string>& parts) {
  pid_t pid = fork();

  if (pid < 0) {
    std::cerr << "fork failed\n";
    return;
  }

  if (pid == 0) {
    // child
    std::vector<char*> argv;
    for (const std::string& s : parts) {
      argv.push_back(const_cast<char*>(s.c_str()));
    }
    argv.push_back(nullptr);

    execv(path.c_str(), argv.data());

    // only reached if execv failed
    std::cerr << parts[0] << ": failed to exeute\n";
    _exit(1);
  }
  // parent
  int status;
  waitpid(pid, &status, 0);
}

std::vector<std::string> split(const std::string& s) {
  std::istringstream iss(s);
  std::vector<std::string> parts;
  std::string item;
  while (iss >> item) {
    parts.push_back(item);
  }
  return parts;
}

std::vector<std::string> split(const std::string& s, char delim) {
  std::istringstream iss(s);
  std::vector<std::string> parts;
  std::string item;
  while (std::getline(iss, item, delim)) {
    parts.push_back(item);
  }
  return parts;
}
