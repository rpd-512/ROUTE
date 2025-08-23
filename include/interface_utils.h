#ifndef INTERFACE_UTILS
#define INTERFACE_UTILS

#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>
#include "types.h"

// -------------------------
// Command hierarchy
// -------------------------

static const map<string, vector<string>> command_tree = {
    {"", {"help","exit","clear","load","save","list","set","simulate","export","plot","generate","compare","route"}},
    {"load", {"topology","config","algorithm"}},
    {"save", {"config","topology"}},
    {"list", {"algorithms","nodes"}},
    {"set", {"population","iteration","c_latency","c_energy"}},
    {"plot", {"topology","network"}},
    {"generate", {"population","topology"}},
};

// -------------------------
// Helpers
// -------------------------

void show_start(){
    cout << " ____   ___  _   _ _____ _____ " << endl;
    cout << "|  _ \\ / _ \\| | | |_   _| ____|" << endl;
    cout << "| |_) | | | | | | | | | |  _|  " << endl;
    cout << "|  _ <| |_| | |_| | | | | |___ " << endl;
    cout << "|_| \\_\\\\___/ \\___/  |_| |_____|" << endl;

    cout << "Route Optimization Using Tunable Evolution" << endl << endl;
}

inline string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

inline vector<string> split(const string& s) {
    vector<string> tokens;
    size_t start = 0, end;
    while ((end = s.find(' ', start)) != string::npos) {
        tokens.emplace_back(s.substr(start, end - start));
        start = end + 1;
    }
    if (start < s.size()) tokens.emplace_back(s.substr(start));
    return tokens;
}

inline char* dupstr(const string& s) {
    char* r = (char*)malloc(s.size() + 1);
    strcpy(r, s.c_str());
    return r;
}

inline vector<string> generate_matches(const vector<string>& options, const char* text) {
    vector<string> matches;
    string_view prefix(text ? text : "");
    for (const auto& opt : options) {
        if (prefix.empty() || opt.find(prefix) == 0)
            matches.push_back(opt);
    }
    sort(matches.begin(), matches.end());
    return matches;
}

inline string build_key(const vector<string>& tokens, size_t end) {
    if (tokens.empty() || end == 0) return tokens[0];
    string key = tokens[0];
    for (size_t i = 1; i < end; ++i)
        key += "." + tokens[i];
    return key;
}

// -------------------------
// Readline completion
// -------------------------

static vector<string> last_matches;

inline char* generator(const char* text, int state) {
    return (state < last_matches.size()) ? dupstr(last_matches[state]) : nullptr;
}

inline char** completer(const char* text, int start, int end) {
    rl_attempted_completion_over = 1;

    string buf(rl_line_buffer ? rl_line_buffer : "");
    string trimmed = trim(buf);
    vector<string> tokens = split(trimmed);

    // Determine token index at cursor
    size_t token_index = 0;
    size_t pos = 0;
    for (; token_index < tokens.size() && pos + tokens[token_index].size() < start; ++token_index)
        pos += tokens[token_index].size() + 1;

    string key = (token_index == 0) ? "" : build_key(tokens, token_index);

    // Select options
    vector<string> options;
    if (key.empty())
        options = command_tree.at("");
    else if (command_tree.count(key))
        options = command_tree.at(key);

    last_matches = generate_matches(options, text);

    return last_matches.empty() ? nullptr : rl_completion_matches(text, generator);
}

#endif // INTERFACE_UTILS
