#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

bool ask(int v, int s1, int s2) {
    cout << "? 2 " << v << " " << s1 << " " << s2 << endl;
    int response;
    cin >> response;
    if (response == -1) {
        exit(0);
    }
    return response == 1;
}

vector<vector<int>> adj;
vector<int> path;
bool found_path;

void find_path_dfs(int u, int p, int target, vector<int>& current_path) {
    current_path.push_back(u);
    if (u == target) {
        path = current_path;
        found_path = true;
        return;
    }
    for (int v : adj[u]) {
        if (v == p) continue;
        find_path_dfs(v, u, target, current_path);
        if (found_path) return;
    }
    current_path.pop_back();
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    if (n <= 1) {
        cout << "!" << endl;
        cout << flush;
        return 0;
    }
    
    adj.resize(n + 1);

    adj[1].push_back(2);
    adj[2].push_back(1);
    
    for (int i = 3; i <= n; ++i) {
        // Find path between 1 and i-1 in the current tree on {1, ..., i-1}
        path.clear();
        vector<int> p_temp;
        found_path = false;
        find_path_dfs(1, 0, i - 1, p_temp);
        
        int low = 0, high = path.size() - 1;
        int parent_node = -1;

        while(low <= high) {
            if (low == high) {
                parent_node = path[low];
                break;
            }
            int mid_idx = low + (high - low) / 2;
            int mid_v = path[mid_idx];

            if(ask(mid_v, i, 1)) {
                // If mid_v is on path i-1, parent is on path mid_v...1
                // In our path vector, 1 is at index 0. So parent is in path[0..mid_idx].
                high = mid_idx;
            } else {
                // Parent is on path mid_v's neighbor...i-1
                low = mid_idx + 1;
            }
        }
        
        adj[i].push_back(parent_node);
        adj[parent_node].push_back(i);
    }

    cout << "!" << endl;
    for (int i = 1; i <= n; ++i) {
        for (int neighbor : adj[i]) {
            if (i < neighbor) {
                cout << i << " " << neighbor << endl;
            }
        }
    }
    cout << flush;

    return 0;
}