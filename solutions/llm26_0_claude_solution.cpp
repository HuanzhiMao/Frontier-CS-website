#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    vector<int> v(n);
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }
    
    vector<pair<int, int>> moves;
    int total_cost = 0;
    
    for (int target = 1; target <= n; target++) {
        // Find current position of target
        int pos = -1;
        for (int i = 0; i < n; i++) {
            if (v[i] == target) {
                pos = i;
                break;
            }
        }
        
        // If already in correct position, continue
        if (pos == target - 1) {
            continue;
        }
        
        // Move element from pos+1 to target position
        moves.push_back({pos + 1, target});
        total_cost += target;
        
        // Update the array
        int element = v[pos];
        v.erase(v.begin() + pos);
        v.insert(v.begin() + target - 1, element);
    }
    
    int final_cost = (total_cost + 1) * (moves.size() + 1);
    
    cout << final_cost << " " << moves.size() << "\n";
    for (auto move : moves) {
        cout << move.first << " " << move.second << "\n";
    }
    
    return 0;
}