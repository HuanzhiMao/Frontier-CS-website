#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int b, k, w;
    cin >> b >> k >> w;
    
    vector<pair<int, int>> deposits;
    
    // First wave: probe at origin to get distances
    cout << "? 1 0 0" << endl;
    cout.flush();
    
    vector<int> dist_origin(k);
    for (int i = 0; i < k; i++) {
        cin >> dist_origin[i];
    }
    
    // Second wave: probe at (1, 0) and (0, 1) to get more constraints
    cout << "? 2 1 0 0 1" << endl;
    cout.flush();
    
    vector<int> dist_wave2(2 * k);
    for (int i = 0; i < 2 * k; i++) {
        cin >> dist_wave2[i];
    }
    
    // Third wave: probe at (-1, 0) and (0, -1)
    cout << "? 2 -1 0 0 -1" << endl;
    cout.flush();
    
    vector<int> dist_wave3(2 * k);
    for (int i = 0; i < 2 * k; i++) {
        cin >> dist_wave3[i];
    }
    
    // Generate candidate points within reasonable bounds
    set<pair<int, int>> candidates;
    int limit = min(b, 1000);
    
    for (int x = -limit; x <= limit; x++) {
        for (int y = -limit; y <= limit; y++) {
            int d0 = abs(x) + abs(y);
            int d1 = abs(x - 1) + abs(y);
            int d2 = abs(x) + abs(y - 1);
            int d3 = abs(x + 1) + abs(y);
            int d4 = abs(x) + abs(y + 1);
            
            // Check if this point could be a deposit
            bool valid = false;
            for (int i = 0; i < k; i++) {
                if (d0 == dist_origin[i]) {
                    // Check consistency with other waves
                    bool found1 = false, found2 = false;
                    for (int j = 0; j < 2 * k; j++) {
                        if (dist_wave2[j] == d1 || dist_wave2[j] == d2) found1 = true;
                        if (dist_wave3[j] == d3 || dist_wave3[j] == d4) found2 = true;
                    }
                    if (found1 && found2) {
                        valid = true;
                        break;
                    }
                }
            }
            if (valid) {
                candidates.insert({x, y});
            }
        }
    }
    
    // If we have too many candidates or need more precision, send additional probes
    if (candidates.size() > k && w > 3) {
        vector<pair<int, int>> cand_vec(candidates.begin(), candidates.end());
        
        // Probe at a different location to disambiguate
        cout << "? 1 5 5" << endl;
        cout.flush();
        
        vector<int> dist_wave4(k);
        for (int i = 0; i < k; i++) {
            cin >> dist_wave4[i];
        }
        
        // Filter candidates based on new distances
        set<pair<int, int>> filtered;
        for (auto& p : cand_vec) {
            int dist = abs(p.first - 5) + abs(p.second - 5);
            for (int i = 0; i < k; i++) {
                if (dist == dist_wave4[i]) {
                    filtered.insert(p);
                    break;
                }
            }
        }
        candidates = filtered;
    }
    
    // Select the k most likely candidates
    vector<pair<int, int>> result(candidates.begin(), candidates.end());
    
    // If we have more candidates than needed, take first k
    if (result.size() > k) {
        result.resize(k);
    }
    
    // If we have fewer candidates than needed, pad with (0,0)
    while (result.size() < k) {
        result.push_back({0, 0});
    }
    
    cout << "!";
    for (int i = 0; i < k; i++) {
        cout << " " << result[i].first << " " << result[i].second;
    }
    cout << endl;
    cout.flush();
    
    return 0;
}