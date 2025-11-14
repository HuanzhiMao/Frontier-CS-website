#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
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

bool areRelated(int a, int b) {
    vector<int> vec = {a, b};
    return query(vec) == 1;
}

int main() {
    int ty;
    cin >> n >> ty;
    
    parent.resize(n + 1);
    children.resize(n + 1);
    
    // Find root - query all nodes, the one that when removed reduces the antichain size most is likely root related
    vector<int> all_nodes;
    for (int i = 1; i <= n; i++) {
        all_nodes.push_back(i);
    }
    int max_antichain = query(all_nodes);
    
    // Find pairs that are related (ancestor-descendant)
    vector<vector<bool>> related(n + 1, vector<bool>(n + 1, false));
    
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            if (areRelated(i, j)) {
                related[i][j] = related[j][i] = true;
            }
        }
    }
    
    // Build the tree structure
    vector<bool> is_root_candidate(n + 1, true);
    
    // For each pair of related nodes, determine parent-child relationship
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (i != j && related[i][j]) {
                // Try to determine which is ancestor by checking with other nodes
                bool i_is_ancestor = false;
                
                // Check if i is ancestor of j by testing with unrelated nodes
                for (int k = 1; k <= n; k++) {
                    if (k != i && k != j && !related[i][k] && !related[j][k]) {
                        vector<int> test1 = {i, k};
                        vector<int> test2 = {j, k};
                        vector<int> test3 = {i, j, k};
                        
                        int r1 = query(test1);
                        int r2 = query(test2);
                        int r3 = query(test3);
                        
                        if (r1 == 2 && r2 == 2 && r3 == 2) {
                            // This helps determine the structure
                        }
                        break;
                    }
                }
                
                // Use a heuristic: assume smaller numbered node is ancestor for now
                if (i < j) {
                    if (parent[j] == 0) {
                        parent[j] = i;
                        children[i].push_back(j);
                        is_root_candidate[j] = false;
                    }
                } else {
                    if (parent[i] == 0) {
                        parent[i] = j;
                        children[j].push_back(i);
                        is_root_candidate[i] = false;
                    }
                }
            }
        }
    }
    
    // Find the root
    int root = -1;
    for (int i = 1; i <= n; i++) {
        if (is_root_candidate[i]) {
            root = i;
            break;
        }
    }
    
    if (root == -1) {
        root = 1; // fallback
    }
    
    // Refine the tree structure using more queries
    vector<int> nodes_without_parent;
    for (int i = 1; i <= n; i++) {
        if (i != root && parent[i] == 0) {
            nodes_without_parent.push_back(i);
        }
    }
    
    // Assign remaining nodes as children of root if needed
    for (int node : nodes_without_parent) {
        parent[node] = root;
        children[root].push_back(node);
    }
    
    cout << "!";
    for (int i = 1; i <= n; i++) {
        cout << " " << parent[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}