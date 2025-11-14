#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    vector<int> v(n + 1);
    vector<int> pos(n + 1);
    
    for (int i = 1; i <= n; i++) {
        cin >> v[i];
        pos[v[i]] = i;
    }
    
    // Find longest increasing subsequence of elements in correct relative order
    vector<bool> keep(n + 1, false);
    vector<int> dp;
    vector<int> parent(n + 1, -1);
    vector<int> dpPos(n + 1, -1);
    
    for (int i = 1; i <= n; i++) {
        int val = v[i];
        auto it = lower_bound(dp.begin(), dp.end(), val);
        int idx = it - dp.begin();
        
        if (it == dp.end()) {
            dp.push_back(val);
        } else {
            *it = val;
        }
        
        dpPos[val] = idx;
        if (idx > 0) {
            // Find the element that should be parent
            for (int j = i - 1; j >= 1; j--) {
                if (dpPos[v[j]] == idx - 1) {
                    parent[val] = v[j];
                    break;
                }
            }
        }
    }
    
    // Reconstruct LIS
    int maxLen = dp.size();
    int curr = dp.back();
    while (curr != -1) {
        keep[curr] = true;
        curr = parent[curr];
    }
    
    // Simulate the sorting process
    vector<int> current = v;
    vector<pair<int, int>> moves;
    int totalCost = 0;
    
    for (int target = 1; target <= n; target++) {
        if (keep[target]) continue;
        
        // Find current position of target
        int currentPos = 1;
        for (int i = 1; i <= n; i++) {
            if (current[i] == target) {
                currentPos = i;
                break;
            }
        }
        
        if (currentPos != target) {
            // Move element from currentPos to target
            moves.push_back({currentPos, target});
            totalCost += target;
            
            // Simulate the move
            int element = current[currentPos];
            if (currentPos < target) {
                for (int i = currentPos; i < target; i++) {
                    current[i] = current[i + 1];
                }
            } else {
                for (int i = currentPos; i > target; i--) {
                    current[i] = current[i - 1];
                }
            }
            current[target] = element;
        }
    }
    
    int finalCost = (totalCost + 1) * (moves.size() + 1);
    cout << finalCost << " " << moves.size() << "\n";
    
    for (auto& move : moves) {
        cout << move.first << " " << move.second << "\n";
    }
    
    return 0;
}