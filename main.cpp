#include <iostream>
#include "QuickySort.h"
#include <fstream>
#include <map>

using namespace std;

int main() {
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;
    arr a(n);

    map<string, function<void()>> commands = {
            {"binary", [&]() { a.binary(); }},
            {"Smart", [&]() { a.Smart(); }},
            {"Medium", [&]() { a.Medium(); }},
            {"radix", [&]() { a.radix(); }},
            {"dumb", [&]() { a.aBitDum(); }}
    };
    while (true) {
        string command;
        cout << "Enter sorting type ('binary', 'Smart', 'Medium', 'radix', 'dumb') or 'exit' to quit: ";
        cin >> command;

        if (command == "exit") {
            break;
        } else if (commands.find(command) != commands.end()) {
            a.fill();
            a.testSortingAlgorithms(commands[command], command);
            a.clear();
        } else {
            cout << "Invalid command. Please try again." << endl;
        }
    }

    return 0;
}
