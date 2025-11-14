#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

bool ask(int v, int s_size, const vector<int>& s) {
    cout << "? " << s_size << " " << v;
    for (int i = 0; i < s_size; ++i) {
        cout << " " << s[i];
    }
    cout << endl;
    int response;
    cin >> response;
    if (response == -1) {
        // Error or limit exceeded, exit gracefully
        exit(0);
    }
    return response == 1;
}

bool is_on_path(int v, int u1, int u2) {
    vector<int> s = {u1, u2};
    return ask(v, 2, s);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    if (n == 1) {
        cout << "!" << endl;
        return 0;
    }

    vector<pair<int, int>> edges;
    vector<int> parent(n + 1, 0);

    // parent[1] is 0 (root)
    for (int i = 2; i <= n; ++i) {
        vector<int> candidates;
        for (int j = 1; j < i; ++j) {
            if (is_on_path(j, 1, i)) {
                candidates.push_back(j);
            }
        }

        int p_i = -1;
        int max_depth = -1;

        for (int cand : candidates) {
            int depth = 0;
            int curr = cand;
            while (curr != 0) {
                curr = parent[curr];
                depth++;
            }
            if (depth > max_depth) {
                max_depth = depth;
                p_i = cand;
            }
        }
        parent[i] = p_i;
        edges.push_back({i, p_i});
    }

    cout << "!" << endl;
    for (const auto& edge : edges) {
        cout << edge.first << " " << edge.second << endl;
    }

    return 0;
}