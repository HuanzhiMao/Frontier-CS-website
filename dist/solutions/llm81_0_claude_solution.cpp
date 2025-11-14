#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    cin >> N;
    
    string result = "";
    
    for (int i = 0; i < N; i++) {
        cout << "1" << endl;
        cout << "2" << endl;
        cout << "0 1" << endl;
        cout << "1 0" << endl;
        cout.flush();
        
        int response;
        cin >> response;
        
        if (response == 0) {
            result += "0";
        } else {
            result += "1";
        }
    }
    
    cout << "0" << endl;
    cout << result << endl;
    cout.flush();
    
    return 0;
}