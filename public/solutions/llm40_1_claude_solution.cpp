#include <iostream>
#include <vector>
#include <string>
using namespace std;

int query(vector<int>& indices) {
    cout << "0 " << indices.size();
    for (int idx : indices) {
        cout << " " << idx;
    }
    cout << endl;
    cout.flush();
    
    int result;
    cin >> result;
    return result;
}

int main() {
    int n;
    cin >> n;
    
    string s(n, '?');
    
    // First, determine which positions are '(' by checking each position individually
    for (int i = 1; i <= n; i++) {
        vector<int> q = {i, i};
        int result = query(q);
        
        if (result == 1) {
            s[i-1] = '(';
        } else {
            s[i-1] = ')';
        }
    }
    
    cout << "1 " << s << endl;
    cout.flush();
    
    return 0;
}