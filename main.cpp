#include "airplane.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    string line;
    
    while (getline(cin, line)) {
        if (line.empty()) continue;
        processCommand(line);
    }
    
    return 0;
}
