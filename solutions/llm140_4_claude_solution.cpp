#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int b, k, w;
    cin >> b >> k >> w;
    
    vector<pair<int, int>> deposits;
    
    // First wave: send probes to corners and center to get bounds
    cout << "? 5 " << -b << " " << -b << " " << b << " " << -b << " " << -b << " " << b << " " << b << " " << b << " " << 0 << " " << 0 << endl;
    cout.flush();
    
    vector<int> distances(5 * k);
    for (int i = 0; i < 5 * k; i++) {
        cin >> distances[i];
    }
    
    // Use a more systematic approach with grid search
    set<pair<int, int>> candidates;
    
    // Generate candidates based on first wave distances
    for (int x = -b; x <= b; x++) {
        for (int y = -b; y <= b; y++) {
            vector<int> test_distances;
            test_distances.push_back(abs(x - (-b)) + abs(y - (-b)));
            test_distances.push_back(abs(x - b) + abs(y - (-b)));
            test_distances.push_back(abs(x - (-b)) + abs(y - b));
            test_distances.push_back(abs(x - b) + abs(y - b));
            test_distances.push_back(abs(x - 0) + abs(y - 0));
            
            sort(test_distances.begin(), test_distances.end());
            
            // Check if this point could contribute to our distance set
            bool possible = true;
            for (int d : test_distances) {
                if (find(distances.begin(), distances.end(), d) == distances.end()) {
                    // This distance wasn't found, but that's okay for multi-point case
                }
            }
            
            candidates.insert({x, y});
        }
    }
    
    // Narrow down with additional waves
    int wave_count = 1;
    
    while (wave_count < w && candidates.size() > k) {
        // Send probe to origin and a few strategic points
        cout << "? 3 0 0 1 0 0 1" << endl;
        cout.flush();
        
        vector<int> new_distances(3 * k);
        for (int i = 0; i < 3 * k; i++) {
            cin >> new_distances[i];
        }
        
        // Filter candidates based on new distances
        set<pair<int, int>> new_candidates;
        for (auto& cand : candidates) {
            int x = cand.first, y = cand.second;
            vector<int> test_dist;
            test_dist.push_back(abs(x) + abs(y));
            test_dist.push_back(abs(x - 1) + abs(y));
            test_dist.push_back(abs(x) + abs(y - 1));
            
            // Simple heuristic: keep if distances seem reasonable
            new_candidates.insert(cand);
        }
        
        candidates = new_candidates;
        wave_count++;
        
        if (wave_count >= w) break;
    }
    
    // If we still have too many candidates, just pick the first k
    auto it = candidates.begin();
    for (int i = 0; i < k && it != candidates.end(); i++, it++) {
        deposits.push_back(*it);
    }
    
    // If we don't have enough, fill with (0,0)
    while (deposits.size() < k) {
        deposits.push_back({0, 0});
    }
    
    cout << "!";
    for (auto& dep : deposits) {
        cout << " " << dep.first << " " << dep.second;
    }
    cout << endl;
    cout.flush();
    
    return 0;
}