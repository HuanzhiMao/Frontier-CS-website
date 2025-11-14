#include <iostream>
using namespace std;

int main() {
    int task;
    cin >> task;
    
    if (task == 0) {
        // Small task solution with digits 0-3
        cout << "3   2   223 " << endl;
        cout << "32 23  2   2" << endl;
        cout << "3 2 2  2   2" << endl;
        cout << "3 2 2  2222 " << endl;
        cout << "3 2 2  2    " << endl;
        cout << "3   2  2    " << endl;
        cout << "            " << endl;
        cout << "3  2   22222" << endl;
        cout << "3 2      2  " << endl;
        cout << "32   2 2 2  " << endl;
        cout << "3 2  2 2 2  " << endl;
        cout << "3  2 222 2  " << endl;
    } else {
        // Large task solution with digits 1-3
        cout << "3   2   223 " << endl;
        cout << "32 23  2   2" << endl;
        cout << "3 2 2  2   2" << endl;
        cout << "3 2 2  2222 " << endl;
        cout << "3 2 2  2    " << endl;
        cout << "3   2  2    " << endl;
        cout << "            " << endl;
        cout << "3  2   22222" << endl;
        cout << "3 2      2  " << endl;
        cout << "32   2 2 2  " << endl;
        cout << "3 2  2 2 2  " << endl;
        cout << "3  2 222 2  " << endl;
    }
    
    return 0;
}