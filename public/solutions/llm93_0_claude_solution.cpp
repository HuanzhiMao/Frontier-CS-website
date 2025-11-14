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
    cout << endl;
    cout.flush();
    
    int result;
    cin >> result;
    return result;
}

bool isAncestor(int u, int v) {
    // Check if u is ancestor of v
    while (v != 0) {
        if (v == u) return true;
        v = parent[v];
    }
    return false;
}

int main() {
    int ty;
    cin >> n >> ty;
    
    parent.assign(n + 1, -1);
    children.assign(n + 1, vector<int>());
    
    // Find root - query all nodes, the one that when removed decreases the result most is likely root
    int maxResult = query(vector<int>(1, 1));
    int root = 1;
    
    for (int i = 2; i <= n; i++) {
        int result = query(vector<int>(1, i));
        if (result > maxResult) {
            maxResult = result;
            root = i;
        }
    }
    
    parent[root] = 0;
    
    // For each remaining node, find its parent
    vector<bool> processed(n + 1, false);
    processed[root] = true;
    queue<int> toProcess;
    toProcess.push(root);
    
    while (!toProcess.empty()) {
        int curr = toProcess.front();
        toProcess.pop();
        
        vector<int> candidates;
        for (int i = 1; i <= n; i++) {
            if (!processed[i]) {
                candidates.push_back(i);
            }
        }
        
        if (candidates.empty()) break;
        
        // Find children of curr
        vector<int> currChildren;
        
        for (int cand : candidates) {
            // Test if cand is a child of curr
            vector<int> testVec = {curr, cand};
            int result = query(testVec);
            
            if (result == 1) {
                // They are ancestor-descendant, likely cand is child of curr
                parent[cand] = curr;
                currChildren.push_back(cand);
                processed[cand] = true;
            }
        }
        
        children[curr] = currChildren;
        for (int child : currChildren) {
            toProcess.push(child);
        }
    }
    
    // Refine using more queries if needed
    for (int i = 1; i <= n; i++) {
        if (parent[i] == -1) {
            // Find parent by testing with known nodes
            for (int j = 1; j <= n; j++) {
                if (i != j && processed[j]) {
                    vector<int> testVec = {i, j};
                    int result = query(testVec);
                    if (result == 1) {
                        // They have ancestor-descendant relationship
                        // Determine which is ancestor
                        bool jIsAncestor = false;
                        int temp = j;
                        while (temp != 0) {
                            if (children[temp].size() == 0 || children[temp].size() >= 2) {
                                vector<int> withParent = {i};
                                if (parent[temp] != 0) {
                                    withParent.push_back(parent[temp]);
                                }
                                if (withParent.size() > 1) {
                                    int res = query(withParent);
                                    if (res == 1) {
                                        parent[i] = temp;
                                        children[temp].push_back(i);
                                        processed[i] = true;
                                        jIsAncestor = true;
                                        break;
                                    }
                                }
                            }
                            temp = parent[temp];
                        }
                        if (jIsAncestor) break;
                    }
                }
            }
        }
    }
    
    // Fill remaining unassigned nodes
    for (int i = 1; i <= n; i++) {
        if (parent[i] == -1) {
            parent[i] = root; // fallback
        }
    }
    
    cout << "!";
    for (int i = 1; i <= n; i++) {
        cout << " " << parent[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}