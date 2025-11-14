#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
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

    int result;
    cin >> result;
    if (result == -1) {
        exit(0); // Error from judge
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    string s(n, ' ');

    int p1_idx = 1;
    int p2_idx = -1;

    // Find the first character that is different from s[1]
    for (int i = 2; i <= n; ++i) {
        vector<int> q1 = {p1_idx, i};
        vector<int> q2 = {i, p1_idx};
        if (ask_query(q1) > 0 || ask_query(q2) > 0) {
            p2_idx = i;
            break;
        }
    }

    // Now we have two indices with different characters: p1_idx and p2_idx
    // We can determine their types.
    char type1_char, type2_char;
    vector<int> q_check_type = {p1_idx, p2_idx};
    if (ask_query(q_check_type) > 0) {
        type1_char = '(';
        type2_char = ')';
    } else {
        type1_char = ')';
        type2_char = '(';
    }

    s[p1_idx - 1] = type1_char;
    s[p2_idx - 1] = type2_char;

    int p_open_idx;
    if (type1_char == '(') {
        p_open_idx = p1_idx;
    } else {
        p_open_idx = p2_idx;
    }

    // All characters before p2_idx must be the same as s[p1_idx]
    for (int i = 2; i < p2_idx; ++i) {
        s[i - 1] = type1_char;
    }

    // For all other characters, we can determine their type
    // by pairing them with a known open bracket.
    for (int i = 2; i <= n; ++i) {
        if (i == p2_idx) continue;
        if (s[i-1] != ' ') continue; // already determined

        vector<int> q = {p_open_idx, i};
        if (ask_query(q) > 0) {
            s[i - 1] = ')';
        } else {
            s[i - 1] = '(';
        }
    }

    cout << "1 " << s << endl;

    return 0;
}