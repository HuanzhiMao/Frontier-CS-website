#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    int N;
    cin >> N;
    
    string result(N, '0');
    
    for (int i = 0; i < N; i++) {
        cout << "1\n";
        cout << "2\n";
        cout << "1 1\n";
        cout << "0 1\n";
        cout.flush();
        
        int response;
        cin >> response;
        
        if (response == 1) {
            result[i] = '1';
        } else {
            result[i] = '0';
        }
    }
    
    cout << "0\n";
    cout << result << "\n";
    cout.flush();
    
    return 0;
}