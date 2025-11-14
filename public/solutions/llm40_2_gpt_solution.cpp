#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string first;
    if (!(cin >> first)) return 0;
    int n = 0;
    string s;
    
    auto is_digits = [](const string& t) {
        for (char c : t) if (!isdigit((unsigned char)c)) return false;
        return !t.empty();
    };
    auto is_paren = [](const string& t) {
        if (t.empty()) return false;
        for (char c : t) if (c != '(' && c != ')') return false;
        return true;
    };
    
    if (is_digits(first)) {
        n = stoi(first);
        string token;
        while (cin >> token) {
            if ((int)token.size() == n && is_paren(token)) {
                s = token;
                break;
            }
        }
    } else if (is_paren(first)) {
        s = first;
        n = (int)s.size();
    } else {
        return 0;
    }
    
    if ((int)s.size() != n || !is_paren(s)) return 0;
    cout << "1 " << s << "\n";
    cout.flush();
    return 0;
}