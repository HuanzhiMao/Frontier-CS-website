#include <bits/stdc++.h>
using namespace std;

int n;
vector<int> parent;

int query(vector<int> nodes) {
    cout << "? " << nodes.size();
    for (int x : nodes) {
        cout << " " << x;
    }
    cout << "\n";
    cout.flush();
    
    int result;
    cin >> result;
    return result;
}

bool isAncestor(int a, int b) {
    // Check if a is ancestor of b by querying them in order
    vector<int> q = {a, b};
    int result = query(q);
    return result == 1; // If result is 1, then b was skipped, so a is ancestor of b
}

int main() {
    int ty;
    cin >> n >> ty;
    
    parent.assign(n + 1, -1);
    
    // Find root - query all nodes, the one that gets selected first in natural order is likely root
    // Or use the fact that root has no parent
    
    // Strategy: For each node, find its parent by testing against other nodes
    vector<int> nodes;
    for (int i = 1; i <= n; i++) {
        nodes.push_back(i);
    }
    
    // First find which node is root by testing pairs
    int root = 1;
    for (int i = 1; i <= n; i++) {
        bool hasParent = false;
        for (int j = 1; j <= n; j++) {
            if (i != j) {
                if (isAncestor(j, i)) {
                    hasParent = true;
                    break;
                }
            }
        }
        if (!hasParent) {
            root = i;
            break;
        }
    }
    
    parent[root] = 0;
    
    // For each non-root node, find its parent
    for (int i = 1; i <= n; i++) {
        if (i == root) continue;
        
        for (int j = 1; j <= n; j++) {
            if (i != j && isAncestor(j, i)) {
                // j is an ancestor of i
                // Check if j is the direct parent by seeing if there's any node between them
                bool isDirect = true;
                for (int k = 1; k <= n; k++) {
                    if (k != i && k != j && isAncestor(j, k) && isAncestor(k, i)) {
                        isDirect = false;
                        break;
                    }
                }
                if (isDirect) {
                    parent[i] = j;
                    break;
                }
            }
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