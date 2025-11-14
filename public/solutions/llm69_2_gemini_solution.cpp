#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;

// Suffix Automaton to count distinct substrings
namespace SuffixAutomaton {
    struct State {
        int len, link;
        map<char, int> next;
    };

    vector<State> sa;
    int last;

    void sa_init() {
        sa.assign(1, State());
        sa[0].len = 0;
        sa[0].link = -1;
        last = 0;
    }

    void sa_extend(char c) {
        int cur = sa.size();
        sa.emplace_back();
        sa[cur].len = sa[last].len + 1;
        int p = last;
        while (p != -1 && sa[p].next.find(c) == sa[p].next.end()) {
            sa[p].next[c] = cur;
            p = sa[p].link;
        }
        if (p == -1) {
            sa[cur].link = 0;
        } else {
            int q = sa[p].next[c];
            if (sa[q].len == sa[p].len + 1) {
                sa[cur].link = q;
            } else {
                int clone = sa.size();
                sa.emplace_back();
                sa[clone].len = sa[p].len + 1;
                sa[clone].next = sa[q].next;
                sa[clone].link = sa[q].link;
                while (p != -1 && sa[p].next.find(c) != sa[p].next.end() && sa[p].next[c] == q) {
                    sa[p].next[c] = clone;
                    p = sa[p].link;
                }
                sa[q].link = clone;
                sa[cur].link = clone;
            }
        }
        last = cur;
    }

    long long count_distinct() {
        long long total = 0;
        for (size_t i = 1; i < sa.size(); ++i) {
            total += sa[i].len - sa[sa[i].link].len;
        }
        return total;
    }
}

long long calculate_power(const string& s) {
    SuffixAutomaton::sa_init();
    for (char c : s) {
        SuffixAutomaton::sa_extend(c);
    }
    return SuffixAutomaton::count_distinct();
}

string to_binary_word(int val, int len) {
    string bin_str = "";
    if (val == 0) {
        bin_str = "O";
    }
    while (val > 0) {
        bin_str += ((val % 2 == 0) ? 'O' : 'X');
        val /= 2;
    }
    while (bin_str.length() < (size_t)len) {
        bin_str += 'O';
    }
    reverse(bin_str.begin(), bin_str.end());
    return bin_str;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    vector<string> words(n + 1);
    int len = 1;
    if (n > 0) {
      len = floor(log2(2 * n)) + 1;
    }

    for (int i = 1; i <= n; ++i) {
        words[i] = to_binary_word(n + i, len);
    }

    for (int i = 1; i <= n; ++i) {
        cout << words[i] << "\n";
    }
    cout.flush();

    map<long long, pair<int, int>> power_to_indices;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            string combined = words[i] + words[j];
            long long power = calculate_power(combined);
            power_to_indices[power] = {i, j};
        }
    }
    
    int q;
    cin >> q;
    for (int k = 0; k < q; ++k) {
        long long p;
        cin >> p;
        pair<int, int> res = power_to_indices[p];
        cout << res.first << " " << res.second << "\n";
        cout.flush();
    }

    return 0;
}