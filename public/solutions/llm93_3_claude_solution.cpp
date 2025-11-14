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

bool isAncestor(int a, int b) {
    while (b != 0 && b != a) {
        b = parent[b];
    }
    return b == a;
}

bool areIndependent(int a, int b) {
    return !isAncestor(a, b) && !isAncestor(b, a);
}

int main() {
    int ty;
    cin >> n >> ty;
    
    parent.resize(n + 1, -1);
    children.resize(n + 1);
    
    // Find root - node that when queried alone gives result 1
    int root = 1;
    for (int i = 1; i <= n; i++) {
        if (query({i}) == 1) {
            root = i;
            break;
        }
    }
    parent[root] = 0;
    
    vector<int> unprocessed;
    for (int i = 1; i <= n; i++) {
        if (i != root) {
            unprocessed.push_back(i);
        }
    }
    
    vector<int> queue = {root};
    
    while (!queue.empty()) {
        int curr = queue.back();
        queue.pop_back();
        
        if (unprocessed.empty()) break;
        
        vector<int> candidates;
        vector<int> remaining;
        
        // Find direct children of curr
        for (int node : unprocessed) {
            vector<int> test_vec = {curr, node};
            if (query(test_vec) == 1) {
                // node is descendant of curr
                candidates.push_back(node);
            } else {
                remaining.push_back(node);
            }
        }
        
        unprocessed = remaining;
        
        if (candidates.empty()) continue;
        
        // Group candidates into siblings
        vector<vector<int>> sibling_groups;
        
        for (int candidate : candidates) {
            bool found_group = false;
            
            for (auto& group : sibling_groups) {
                // Check if candidate can be in this group
                vector<int> test_vec = group;
                test_vec.push_back(candidate);
                
                if (query(test_vec) == test_vec.size()) {
                    group.push_back(candidate);
                    found_group = true;
                    break;
                }
            }
            
            if (!found_group) {
                sibling_groups.push_back({candidate});
            }
        }
        
        // Assign parents and add to queue
        for (auto& group : sibling_groups) {
            for (int child : group) {
                parent[child] = curr;
                children[curr].push_back(child);
                queue.push_back(child);
            }
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