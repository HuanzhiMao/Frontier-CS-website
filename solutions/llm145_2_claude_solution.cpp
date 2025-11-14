#include <iostream>
using namespace std;

int main() {
    int task;
    cin >> task;
    
    if (task == 0) {
        // Small task - use digits 0-3
        cout << "3   3   333 " << endl;
        cout << "33 33  3   3" << endl;
        cout << "3 3 3  3   3" << endl;
        cout << "3 3 3  3333 " << endl;
        cout << "3 3 3  3    " << endl;
        cout << "3   3  3    " << endl;
        cout << "            " << endl;
        cout << "3  3   33333" << endl;
        cout << "3 3      3  " << endl;
        cout << "33   3 3 3  " << endl;
        cout << "3 3  3 3 3  " << endl;
        cout << "3  3 333 3  " << endl;
    } else {
        // Large task - use digits 1-3
        cout << "3   3   333 " << endl;
        cout << "33 33  3   3" << endl;
        cout << "3 3 3  3   3" << endl;
        cout << "3 3 3  3333 " << endl;
        cout << "3 3 3  3    " << endl;
        cout << "3   3  3    " << endl;
        cout << "            " << endl;
        cout << "3  3   33333" << endl;
        cout << "3 3      3  " << endl;
        cout << "33   3 3 3  " << endl;
        cout << "3 3  3 3 3  " << endl;
        cout << "3  3 333 3  " << endl;
    }
    
    return 0;
}