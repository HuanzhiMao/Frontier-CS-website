#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <functional>

using namespace std;

int ask_query(const vector<int>& indices) {
    if (indices.empty()) {
        return 0;
    }
    cout << "0 " << indices.size();
    for (int idx : indices) {
        cout << " " << idx;
    }
    cout << endl;
    int response;
    cin >> response;
    if (response == -1) {
        // Error from judge, or end of test cases
        exit(0);
    }
    return response;
}

void answer(const string& s) {
    cout << "1 " << s << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    string s(n, ' ');
    vector<int> s1_query_results(n + 1, 0);
    bool s1_is_open = false;

    for (int i = 2; i <= n; ++i) {
        s1_query_results[i] = ask_query({1, i});
        if (s1_query_results[i] > 0) {
            s1_is_open = true;
        }
    }

    if (s1_is_open) {
        s[0] = '(';
        int open_idx = 1;
        for (int i = 2; i <= n; ++i) {
            if (s1_query_results[i] > 0) {
                s[i - 1] = ')';
            } else {
                s[i - 1] = '(';
            }
        }
    } else {
        s[0] = ')';
        int close_idx = 1;
        int open_idx = -1;
        for (int i = 2; i <= n; ++i) {
            if (ask_query({i, close_idx}) > 0) {
                s[i - 1] = '(';
                open_idx = i;
                break;
            }
        }
        
        // At this point, we must have found an open_idx because at least one '(' exists.
        
        for (int i = 2; i <= n; ++i) {
            if (s[i - 1] == ' ') {
                if (ask_query({open_idx, i}) > 0) {
                    s[i - 1] = ')';
                } else {
                    s[i - 1] = '(';
                }
            }
        }
    }

    answer(s);

    return 0;
}