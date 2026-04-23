#include "SkipList.hpp"
#include <iostream>
#include <string>

using namespace std;

int main() {
    int n;
    cin >> n;
    
    SkipList<int> skipList;
    
    for (int i = 0; i < n; i++) {
        string operation;
        cin >> operation;
        
        if (operation == "insert") {
            int value;
            cin >> value;
            skipList.insert(value);
        } else if (operation == "search") {
            int value;
            cin >> value;
            bool found = skipList.search(value);
            cout << (found ? "true" : "false") << endl;
        } else if (operation == "delete") {
            int value;
            cin >> value;
            skipList.deleteItem(value);
        }
    }
    
    return 0;
}
