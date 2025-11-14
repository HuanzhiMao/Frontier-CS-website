#include <bits/stdc++.h>
using namespace std;

int n;
vector<int> parent;
vector<vector<int>> children;

int query(vector<int> vec) {
    cout << "? " << vec.size();
    for (int x : vec) {
        cout << " " << x;
    }
    cout << "\n";
    cout.flush();
    int result;
    cin >> result;
    return result;
}

bool isAncestor(int u, int v) {
    if (parent[v] == 0) return false;
    if (parent[v] == u) return true;
    return isAncestor(u, parent[v]);
}

int main() {
    int ty;
    cin >> n >> ty;
    
    parent.resize(n + 1, -1);
    children.resize(n + 1);
    
    // Find root - node that can be taken with all others
    int root = 1;
    for (int i = 1; i <= n; i++) {
        vector<int> all_others;
        for (int j = 1; j <= n; j++) {
            if (j != i) all_others.push_back(j);
        }
        if (query(all_others) == n - 1) {
            root = i;
            break;
        }
    }
    
    parent[root] = 0;
    vector<bool> processed(n + 1, false);
    processed[root] = true;
    
    queue<int> q;
    q.push(root);
    
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        
        vector<int> candidates;
        for (int i = 1; i <= n; i++) {
            if (!processed[i]) {
                candidates.push_back(i);
            }
        }
        
        if (candidates.empty()) continue;
        
        // Find children of curr
        vector<int> curr_children;
        
        for (int cand : candidates) {
            // Check if cand is a child of curr
            vector<int> test_vec = {curr, cand};
            if (query(test_vec) == 1) {
                // cand is descendant of curr, check if direct child
                bool is_direct_child = true;
                for (int other : curr_children) {
                    vector<int> test = {cand, other};
                    if (query(test) == 1) {
                        is_direct_child = false;
                        break;
                    }
                }
                if (is_direct_child) {
                    curr_children.push_back(cand);
                }
            }
        }
        
        // If no direct method works, use elimination
        if (curr_children.empty()) {
            for (int cand : candidates) {
                bool is_child = true;
                // Check against all processed nodes except curr
                for (int proc = 1; proc <= n; proc++) {
                    if (processed[proc] && proc != curr) {
                        vector<int> test = {proc, cand};
                        if (query(test) == 1) {
                            is_child = false;
                            break;
                        }
                    }
                }
                if (is_child) {
                    vector<int> test = {curr, cand};
                    if (query(test) == 1) {
                        curr_children.push_back(cand);
                    }
                }
            }
        }
        
        // Set parents and add to queue
        for (int child : curr_children) {
            parent[child] = curr;
            children[curr].push_back(child);
            processed[child] = true;
            q.push(child);
        }
    }
    
    cout << "!";
    for (int i = 1; i <= n; i++) {
        cout << " " << parent[i];
    }
    cout << "\n";
    cout.flush();
    
    return 0;
}