#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int b, k, w;
    cin >> b >> k >> w;
    
    vector<pair<int, int>> deposits;
    
    // First wave: send probes at corners and center to get rough positions
    cout << "? 5 " << -b << " " << -b << " " << b << " " << -b << " " 
         << -b << " " << b << " " << b << " " << b << " 0 0" << endl;
    cout.flush();
    
    vector<int> distances(5 * k);
    for (int i = 0; i < 5 * k; i++) {
        cin >> distances[i];
    }
    
    // Try to narrow down locations using binary search approach
    set<pair<int, int>> candidates;
    
    // Generate initial candidate set based on distances
    for (int x = -b; x <= b; x++) {
        for (int y = -b; y <= b; y++) {
            vector<int> test_distances;
            test_distances.push_back(abs(x - (-b)) + abs(y - (-b)));
            test_distances.push_back(abs(x - b) + abs(y - (-b)));
            test_distances.push_back(abs(x - (-b)) + abs(y - b));
            test_distances.push_back(abs(x - b) + abs(y - b));
            test_distances.push_back(abs(x - 0) + abs(y - 0));
            sort(test_distances.begin(), test_distances.end());
            
            // Check if this point could be one of the deposits
            bool possible = false;
            for (int i = 0; i <= 5 * k - 5; i++) {
                bool match = true;
                for (int j = 0; j < 5; j++) {
                    if (distances[i + j] != test_distances[j]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    possible = true;
                    break;
                }
            }
            if (possible) {
                candidates.insert({x, y});
            }
        }
    }
    
    // If we have exactly k candidates, we're done
    if (candidates.size() == k) {
        cout << "!";
        for (auto& p : candidates) {
            cout << " " << p.first << " " << p.second;
        }
        cout << endl;
        return 0;
    }
    
    // Otherwise, use additional waves to narrow down
    vector<pair<int, int>> cand_vec(candidates.begin(), candidates.end());
    
    for (int wave = 1; wave < w && cand_vec.size() > k; wave++) {
        // Send probes to discriminate between candidates
        vector<pair<int, int>> probes;
        
        // Choose probe locations that maximize discrimination
        for (int i = 0; i < min(10, (int)cand_vec.size()) && probes.size() < 100; i++) {
            probes.push_back(cand_vec[i]);
        }
        
        if (probes.empty()) break;
        
        cout << "? " << probes.size();
        for (auto& p : probes) {
            cout << " " << p.first << " " << p.second;
        }
        cout << endl;
        cout.flush();
        
        vector<int> new_distances(probes.size() * k);
        for (int i = 0; i < probes.size() * k; i++) {
            cin >> new_distances[i];
        }
        
        // Filter candidates based on new distances
        vector<pair<int, int>> new_candidates;
        for (auto& cand : cand_vec) {
            vector<int> test_distances;
            for (auto& probe : probes) {
                test_distances.push_back(abs(cand.first - probe.first) + abs(cand.second - probe.second));
            }
            sort(test_distances.begin(), test_distances.end());
            
            bool possible = false;
            for (int i = 0; i <= (int)new_distances.size() - (int)test_distances.size(); i++) {
                bool match = true;
                for (int j = 0; j < test_distances.size(); j++) {
                    if (new_distances[i + j] != test_distances[j]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    possible = true;
                    break;
                }
            }
            if (possible) {
                new_candidates.push_back(cand);
            }
        }
        
        cand_vec = new_candidates;
        if (cand_vec.size() <= k) break;
    }
    
    // Output the first k candidates
    cout << "!";
    for (int i = 0; i < min(k, (int)cand_vec.size()); i++) {
        cout << " " << cand_vec[i].first << " " << cand_vec[i].second;
    }
    cout << endl;
    
    return 0;
}