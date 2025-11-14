#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int b, k, w;
    cin >> b >> k >> w;
    
    vector<pair<int, int>> deposits;
    
    // First wave: probe at origin and 4 cardinal directions
    cout << "? 5 0 0 " << b << " 0 " << -b << " 0 0 " << b << " 0 " << -b << endl;
    vector<int> distances1(5 * k);
    for (int i = 0; i < 5 * k; i++) {
        cin >> distances1[i];
    }
    
    // Second wave: probe at diagonal directions
    cout << "? 4 " << b << " " << b << " " << b << " " << -b << " " << -b << " " << b << " " << -b << " " << -b << endl;
    vector<int> distances2(4 * k);
    for (int i = 0; i < 4 * k; i++) {
        cin >> distances2[i];
    }
    
    // Try to find deposits by brute force within reasonable bounds
    set<pair<int, int>> found;
    
    for (int x = -b; x <= b && found.size() < k; x++) {
        for (int y = -b; y <= b && found.size() < k; y++) {
            vector<int> test_distances;
            
            // Calculate distances for first wave
            test_distances.push_back(abs(x) + abs(y)); // (0,0)
            test_distances.push_back(abs(x - b) + abs(y)); // (b,0)
            test_distances.push_back(abs(x + b) + abs(y)); // (-b,0)
            test_distances.push_back(abs(x) + abs(y - b)); // (0,b)
            test_distances.push_back(abs(x) + abs(y + b)); // (0,-b)
            
            // Calculate distances for second wave
            test_distances.push_back(abs(x - b) + abs(y - b)); // (b,b)
            test_distances.push_back(abs(x - b) + abs(y + b)); // (b,-b)
            test_distances.push_back(abs(x + b) + abs(y - b)); // (-b,b)
            test_distances.push_back(abs(x + b) + abs(y + b)); // (-b,-b)
            
            sort(test_distances.begin(), test_distances.end());
            
            // Check if this point could be one of the deposits
            bool valid = true;
            vector<int> combined_distances;
            combined_distances.insert(combined_distances.end(), distances1.begin(), distances1.end());
            combined_distances.insert(combined_distances.end(), distances2.begin(), distances2.end());
            sort(combined_distances.begin(), combined_distances.end());
            
            // Check if all test distances appear in the combined distances
            for (int dist : test_distances) {
                auto it = find(combined_distances.begin(), combined_distances.end(), dist);
                if (it != combined_distances.end()) {
                    combined_distances.erase(it);
                } else {
                    valid = false;
                    break;
                }
            }
            
            if (valid && found.find({x, y}) == found.end()) {
                found.insert({x, y});
                deposits.push_back({x, y});
            }
        }
    }
    
    // If we don't have enough deposits, do a more targeted search
    while (deposits.size() < k && deposits.size() < 20) {
        // Add some reasonable guesses
        for (int x = -min(b, 100); x <= min(b, 100) && deposits.size() < k; x += 10) {
            for (int y = -min(b, 100); y <= min(b, 100) && deposits.size() < k; y += 10) {
                if (found.find({x, y}) == found.end()) {
                    deposits.push_back({x, y});
                    found.insert({x, y});
                }
            }
        }
    }
    
    // Ensure we have exactly k deposits
    while (deposits.size() < k) {
        deposits.push_back({0, 0});
    }
    if (deposits.size() > k) {
        deposits.resize(k);
    }
    
    cout << "!";
    for (auto& dep : deposits) {
        cout << " " << dep.first << " " << dep.second;
    }
    cout << endl;
    
    return 0;
}