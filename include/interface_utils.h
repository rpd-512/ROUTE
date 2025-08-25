#ifndef INTERFACE_UTILS
#define INTERFACE_UTILS

#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <filesystem>
#include <cstring>
#include "types.h"
#include "../metaheuristics/geneticAlgorithm.h"

namespace fs = filesystem;

void clear_screen() {
    #ifdef _WIN32
        int ret = system("cls");
    #else
        int ret = system("clear");
    #endif
        (void)ret;  // explicitly mark unused
}


vector<string> list_dir(const string& typed_path_str, const string& ext_filter = "") {
    vector<string> results;

    fs::path typed_path(typed_path_str);
    fs::path base_dir = typed_path.has_parent_path() ? typed_path.parent_path() : ".";
    string prefix = typed_path.filename().string();

    if (!fs::exists(base_dir) || !fs::is_directory(base_dir)) return results;

    for (auto& entry : fs::directory_iterator(base_dir)) {
        string name = entry.path().filename().string();
        if (!prefix.empty() && name.find(prefix) != 0) continue;

        if (entry.is_directory()) {
            results.push_back((base_dir / name).string() + "/");
        } else if (ext_filter.empty() || entry.path().extension() == ext_filter) {
            results.push_back((base_dir / name).string());
        }
    }

    sort(results.begin(), results.end());
    return results;
}

// -------------------------
// Command hierarchy
// -------------------------

static const map<string, vector<string>> command_tree = {
    {"", {"help","exit","clear","load","save","list","set","show","simulate","export","plot","generate","compare","route","about"}},
    {"load", {"topology","config","algorithm"}},
    {"save", {"config","topology","convergence"}},
    {"list", {"algorithms","nodes"}},
    {"set", {"population","iteration","c_latency","c_energy"}},
    {"show", {"population","iteration","c_latency","c_energy"}},
    {"plot", {"topology","network","convergence"}},
    {"generate", {"population","topology"}},
};

set<string> valid_set_keys = {"population", "iteration", "c_latency", "c_energy"};

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

void show_help() {
    cout << "\nAvailable commands:\n";

    cout << "load         - Load topology, config, or algorithm\n";
    cout << "  topology   - Load a network topology file\n";
    cout << "  config     - Load a configuration file\n";
    cout << "  algorithm  - Load an algorithm module\n";

    cout << "save         - Save config, topology, or convergence hash\n";
    cout << "  config     - Save current configuration\n";
    cout << "  topology   - Save current network topology\n";
    cout << "  convergence- Save current convergence hash\n";

    cout << "list         - List available items\n";
    cout << "  algorithms - List available algorithms\n";
    cout << "  nodes      - List nodes in network\n";

    cout << "set          - Set simulation parameters\n";
    cout << "  population - Set population size\n";
    cout << "  iteration  - Set number of iterations\n";
    cout << "  c_latency  - Set latency weight\n";
    cout << "  c_energy   - Set energy weight\n";

    cout << "simulate     - Run simulation with given count\n";

    cout << "export       - Export statistics to a file\n";

    cout << "plot         - Plot simulation data\n";
    cout << "  topology   - Plot network topology\n";
    cout << "  network    - Plot network data\n";
    cout << "  convergence- Plot convergence graph\n";

    cout << "generate     - Generate population or topology\n";
    cout << "  population - Generate new population\n";
    cout << "  topology   - Generate new topology\n";

    cout << "compare      - Compare two configuration files\n";

    cout << "help         - Show this help message\n";
    cout << "route        - Show route optimization info\n";
    cout << "exit         - Exit the program\n";
    cout << "clear        - Clear the screen\n\n";
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

string build_key(const vector<string>& tokens, size_t end) {
    if (tokens.empty() || end == 0) return "";  // safe
    string key = tokens[0];
    for (size_t i = 1; i < end; i++) key += "." + tokens[i];
    return key;
}


// -------------------------
// Readline completion
// -------------------------

static vector<string> last_matches;

inline char* generator(const char* text, int state) {
    return ((size_t)state < last_matches.size()) ? dupstr(last_matches[state]) : nullptr;
}

inline char** completer(const char* text, int start, int end) {
    rl_attempted_completion_over = 1;

    string buf(rl_line_buffer ? rl_line_buffer : "");
    string trimmed = trim(buf);
    vector<string> tokens = split(trimmed);

    // Determine token index at cursor
    size_t token_index = 0;
    size_t pos = 0;
    for (; token_index < tokens.size() && pos + tokens[token_index].size() < (size_t)start; ++token_index)
        pos += tokens[token_index].size() + 1;

    string key = (token_index == 0) ? "" : build_key(tokens, token_index);

    // Select options
    vector<string> options;
    if (key.empty())
        options = command_tree.at("");
    else if (command_tree.count(key))
        options = command_tree.at(key);
    else if (!key.empty() && key == "load.config") {
        string typed = text ? text : "";
        options = list_dir(typed, ".toml");
    }
    else if (!key.empty() && key == "load.topology") {
        string typed = text ? text : "";
        options = list_dir(typed, ".yaml");
    }
    else if (!key.empty() && (key.find("compare.") != string::npos || key == "compare") && tokens.size() < 4) {
        string typed = text ? text : "";
        options = list_dir(typed, ".toml"); // dynamic listing for compare subcommands
    }

    last_matches = generate_matches(options, text);

    return last_matches.empty() ? nullptr : rl_completion_matches(text, generator);
}

void execute_command(char* input, Topology& network, SimulationData& simulator){
    string trimmed_input = trim(input);
    free(input);

    // Split into tokens
    vector<string> tokens = split(trimmed_input);

    // Build the key
    string cmd_key = build_key(tokens, tokens.size());

    // Handle top-level or multi-level commands

    if (cmd_key == "clear") clear_screen();
    else if (cmd_key == "exit") exit(0);
    else if (cmd_key == "help") show_help();
    else if (cmd_key == "route") show_start();
    else if (cmd_key == "list.nodes") {
        if (network.node_list.empty()) {
            cout << "Please load topology before using this command" << endl;
            return;
        }
        network.print_network();
    }
    else if (cmd_key == "plot.topology"){
        if (network.node_list.empty()) {
            cout << "Please load topology before using this command" << endl;
            return;
        }
        plotNodes(network);
    }

    else if (cmd_key == "show.population") cout << "Population Size: " << simulator.population_size << endl;
    else if (cmd_key == "show.iteration") cout  << "Iteration Count: " << simulator.iteration_count << endl;
    else if (cmd_key == "show.c_latency") cout  << "c_latency value: " << simulator.c_latency << endl;
    else if (cmd_key == "show.c_energy") cout   << "c_energy value: " <<  simulator.c_energy << endl;

    else if (cmd_key.find("set") != string::npos && tokens.size() < 3) {
        if (find(command_tree.at("set").begin(), command_tree.at("set").end(), tokens[1]) == command_tree.at("set").end()){
            cout << "Error: Invalid set command key." << endl;
        } else {
            cout << "Usage: set " << tokens[1] << " <value>" << endl;
        }
    }
    else if (cmd_key.find("set.population") != string::npos) {
        try {
            int val = stoi(tokens[2]);
            simulator.population_size = val;
            cout << "Population size set to " << val << endl;
        } catch (...) {
            cerr << "Error: Invalid integer for population." << endl;
        }
    } 
    else if (cmd_key.find("set.iteration") != string::npos) {
        try {
            int val = stoi(tokens[2]);
            simulator.iteration_count = val;
            cout << "Iteration count set to " << val << endl;
        } catch (...) {
            cerr << "Error: Invalid integer for iteration." << endl;
        }
    } 
    else if (cmd_key.find("set.c_latency") != string::npos) {
        try {
            double val = stod(tokens[2]);
            simulator.c_latency = val;
            cout << "c_latency value set to " << val << endl;
        } catch (...) {
            cerr << "Error: Invalid double for c_latency." << endl;
        }
    } 
    else if (cmd_key.find("set.c_energy") != string::npos) {
        try {
            double val = stod(tokens[2]);
            simulator.c_energy = val;
            cout << "c_energy value set to " << val << endl;
        } catch (...) {
            cerr << "Error: Invalid double for c_energy." << endl;
        }
    }

    else if (cmd_key.find("load") != string::npos && tokens.size() < 3) {
        if (find(command_tree.at("load").begin(), command_tree.at("load").end(), tokens[1]) == command_tree.at("load").end()){
            cout << "Error: Invalid load command key." << endl;
        } else {
            cout << "Usage: set " << tokens[1] << " <path>" << endl;
        }
    }
    else if (cmd_key.find("load.topology") != string::npos){
        cout << "Loading topology from " << tokens[2] << endl;
        network = loadNodesFromYAML(tokens[2]);
    }

    else if (cmd_key.find("generate") != string::npos && tokens.size() < 2) {
        if (find(command_tree.at("generate").begin(), command_tree.at("generate").end(), tokens[1]) == command_tree.at("generate").end()){
            cout << "Error: Invalid generate command key." << endl;
        }
    }
    else if (cmd_key == "generate.population"){
        simulator.population = initial_population(simulator, network);
        cout << "Successfully generated new initial population" << endl;
    }
    else if (cmd_key.find("generate.topology") != string::npos) {
        try {
            if(tokens.size() < 5 || trim(tokens[2]) == "" || trim(tokens[3]) == "" || trim(tokens[4]) == ""){
                throw std::runtime_error("Invalid arguments");
            }
            int node_count = stoi(tokens[2]);
            int sink_count = stoi(tokens[3]);
            double area_size = stod(tokens[4]);
            network = generate_random_topology(node_count, sink_count, area_size);
        } catch (...) {
            cerr << "Usage: generate topology <node_count> <sink_count> <area_size>" << endl;
        }
    }
    else if (cmd_key.find("save.topology") != string::npos){
        if (network.node_list.empty()) {
            cout << "Please load topology before using this command" << endl;
            return;
        }
        if(tokens.size() < 3 || trim(tokens[2]) == ""){
            cout << "Usage: save topology <path>" << endl;
            return;
        }
        network.save_network(tokens[2]);
    }
    else if (cmd_key == "simulate"){
        if (network.node_list.empty()) {
            cout << "Please load topology before using this command" << endl;
            return;
        }
        if(simulator.population_size <= 0 || simulator.iteration_count <= 0){
            cout << "Please set valid population and iteration counts before simulating" << endl;
            return;
        }
        if(simulator.population.size() != simulator.population_size){
            cout << "Population size mismatch, regenerating population" << endl;
            simulator.population = initial_population(simulator, network);
        }
        GeneticAlgorithm ga(simulator, network);
        ga.plot_convergence();
    }
    
    else if (cmd_key == "about") {
        cout << "Route Optimization Using Tunable Evolution (ROUTE)" << endl;
        cout << "Version 0.1" << endl;
        cout << "Developed by Rhiddhi Prasad Das" << endl;
        cout << "For research and educational purposes." << endl;
    }

    else if (cmd_key != "") cout << "Error: Invalid command, please type help to see valid commands." << endl;
}

#endif // INTERFACE_UTILS
