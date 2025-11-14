#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int b, k, w;
    cin >> b >> k >> w;
    
    vector<pair<int, int>> deposits;
    
    // First wave: send probes to corners and center
    cout << "? 5 " << -b << " " << -b << " " << b << " " << -b << " " << -b << " " << b << " " << b << " " << b << " 0 0" << endl;
    cout.flush();
    
    vector<int> distances(5 * k);
    for (int i = 0; i < 5 * k; i++) {
        cin >> distances[i];
    }
    
    // Try to find deposits by brute force within the boundary
    for (int x = -b; x <= b && deposits.size() < k; x++) {
        for (int y = -b; y <= b && deposits.size() < k; y++) {
            vector<int> expected;
            expected.push_back(abs(x - (-b)) + abs(y - (-b))); // corner 1
            expected.push_back(abs(x - b) + abs(y - (-b)));    // corner 2
            expected.push_back(abs(x - (-b)) + abs(y - b));    // corner 3
            expected.push_back(abs(x - b) + abs(y - b));       // corner 4
            expected.push_back(abs(x - 0) + abs(y - 0));       // center
            
            sort(expected.begin(), expected.end());
            
            // Check if this point could be a deposit
            bool found = true;
            for (int i = 0; i < 5; i++) {
                bool match = false;
                for (int j = 0; j < distances.size(); j++) {
                    if (distances[j] == expected[i]) {
                        match = true;
                        break;
                    }
                }
                if (!match) {
                    found = false;
                    break;
                }
            }
            
            if (found) {
                // Verify this is actually a deposit by checking if we can remove these distances
                vector<int> temp_distances = distances;
                bool can_remove = true;
                for (int i = 0; i < 5; i++) {
                    bool removed = false;
                    for (int j = 0; j < temp_distances.size(); j++) {
                        if (temp_distances[j] == expected[i]) {
                            temp_distances.erase(temp_distances.begin() + j);
                            removed = true;
                            break;
                        }
                    }
                    if (!removed) {
                        can_remove = false;
                        break;
                    }
                }
                
                if (can_remove) {
                    deposits.push_back({x, y});
                    distances = temp_distances;
                }
            }
        }
    }
    
    // If we haven't found all deposits, try more waves with different probe positions
    int waves_used = 1;
    while (deposits.size() < k && waves_used < w) {
        // Send probes in a different configuration
        int offset = waves_used * 1000;
        cout << "? 4 " << -b + offset << " " << -b + offset << " " << b - offset << " " << -b + offset 
             << " " << -b + offset << " " << b - offset << " " << b - offset << " " << b - offset << endl;
        cout.flush();
        
        vector<int> new_distances(4 * k);
        for (int i = 0; i < 4 * k; i++) {
            cin >> new_distances[i];
        }
        
        // Try to find remaining deposits
        for (int x = -b; x <= b && deposits.size() < k; x++) {
            for (int y = -b; y <= b && deposits.size() < k; y++) {
                // Check if this point is already found
                bool already_found = false;
                for (auto& dep : deposits) {
                    if (dep.first == x && dep.second == y) {
                        already_found = true;
                        break;
                    }
                }
                if (already_found) continue;
                
                vector<int> expected;
                expected.push_back(abs(x - (-b + offset)) + abs(y - (-b + offset)));
                expected.push_back(abs(x - (b - offset)) + abs(y - (-b + offset)));
                expected.push_back(abs(x - (-b + offset)) + abs(y - (b - offset)));
                expected.push_back(abs(x - (b - offset)) + abs(y - (b - offset)));
                
                sort(expected.begin(), expected.end());
                
                bool found = true;
                for (int i = 0; i < 4; i++) {
                    bool match = false;
                    for (int j = 0; j < new_distances.size(); j++) {
                        if (new_distances[j] == expected[i]) {
                            match = true;
                            break;
                        }
                    }
                    if (!match) {
                        found = false;
                        break;
                    }
                }
                
                if (found) {
                    deposits.push_back({x, y});
                    // Remove these distances
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < new_distances.size(); j++) {
                            if (new_distances[j] == expected[i]) {
                                new_distances.erase(new_distances.begin() + j);
                                break;
                            }
                        }
                    }
                }
            }
        }
        waves_used++;
    }
    
    cout << "!";
    for (int i = 0; i < min((int)deposits.size(), k); i++) {
        cout << " " << deposits[i].first << " " << deposits[i].second;
    }
    // If we couldn't find all deposits, output zeros for missing ones
    for (int i = deposits.size(); i < k; i++) {
        cout << " 0 0";
    }
    cout << endl;
    cout.flush();
    
    return 0;
}