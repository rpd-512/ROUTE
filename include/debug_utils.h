#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include <vector>
#include <iostream>
#include <string>

using namespace std;

void print_vector(const vector<int>& vec) {
    for (const auto& val : vec) {
        cout << val << " ";
    }
    cout << endl;
}

void print_2D_vector(const vector<vector<int>>& vec) {
    for (const auto& row : vec) {
        print_vector(row);
    }
}

#endif // DEBUG_UTILS_H