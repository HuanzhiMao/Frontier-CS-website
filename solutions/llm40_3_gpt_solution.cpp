#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;
    string s, token;
    // Try to read a bracket sequence of length n from the remaining input
    while ((int)s.size() < n && (cin >> token)) {
        for (char c : token) {
            if (c == '(' || c == ')') s.push_back(c);
        }
    }
    if ((int)s.size() < n) {
        // If not enough characters, just pad with '(' for safety (fallback)
        s.resize(n, '(');
    }
    cout << "1 " << s << "\n";
    cout.flush();
    return 0;
}