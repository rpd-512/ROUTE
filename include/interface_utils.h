#ifndef INTERFACE_UTILS
#define INTERFACE_UTILS

#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Top-level commands
std::vector<std::string> top_level_cmds = {"help", "exit", "load", "run", "stats", "export"};

// Subcommands for "load"
std::vector<std::string> load_subcmds = {"config", "dataset", "topology"};

// Trim helper (to ignore spaces)
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

char* dupstr(const std::string& s) {
    char* r = (char*)malloc(s.size() + 1);
    strcpy(r, s.c_str());
    return r;
}

// Custom completion function
char** completer(const char* text, int start, int end) {
    rl_attempted_completion_over = 1;

    std::string buf(rl_line_buffer);
    std::string trimmed = trim(buf);

    std::vector<std::string> matches;

    // If this is the first word → suggest top-level commands
    if (start == 0) {
        for (auto& cmd : top_level_cmds) {
            if (cmd.find(text) == 0) matches.push_back(cmd);
        }
    }
    // If first word is "load" → suggest subcommands
    else {
        std::string first = trimmed.substr(0, trimmed.find(" "));
        if (first == "load") {
            for (auto& sub : load_subcmds) {
                if (sub.find(text) == 0) matches.push_back(sub);
            }
        }
    }

    if (matches.empty()) return nullptr;

    // Convert to readline format
    char** completions = (char**)malloc((matches.size() + 1) * sizeof(char*));
    for (size_t i = 0; i < matches.size(); i++) {
        completions[i] = dupstr(matches[i]);
    }
    completions[matches.size()] = nullptr;
    return completions;
}

/*
int main() {
    rl_attempted_completion_function = completer;

    char* input;
    while ((input = readline("(ROUTE)~$ ")) != nullptr) {
        if (*input) add_history(input);

        std::string cmd = trim(input);
        free(input);

        if (cmd == "exit") break;
        else if (cmd == "help") std::cout << "Available: help, init, run, stats, export, load\n";
        else if (cmd.rfind("load", 0) == 0) std::cout << "Loading: " << cmd << "\n";
        else std::cout << "You typed: " << cmd << "\n";
    }
    return 0;
}
    */

#endif //