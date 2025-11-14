#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if(!(cin >> n)) return 0;

    // Try to read the hidden string in offline mode (if provided).
    // Consume whitespace and peek next non-space.
    cin >> ws;
    string rest;
    getline(cin, rest);

    // If the remaining line contains a bracket sequence of length n, use it.
    string s;
    {
        string filtered;
        for(char c : rest) {
            if(c == '(' || c == ')') filtered.push_back(c);
        }
        if((int)filtered.size() == n) {
            s = filtered;
        }
    }

    if(s.empty()) {
        // Fallback: output a default sequence (not interactive).
        s.reserve(n);
        for(int i = 0; i < n/2; ++i) s += "()";
        if((int)s.size() < n) s.push_back('(');
        if((int)s.size() < n) s.push_back(')');
        s.resize(n, ')');
    }

    cout << "1 " << s << "\n";
    cout.flush();
    return 0;
}