#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    int total_sections = n * m;
    vector<int> ring_pos(n, 0);
    
    // Get initial reading
    cout << "? 0 1" << endl;
    cout.flush();
    int initial;
    cin >> initial;
    
    cout << "? 0 -1" << endl;
    cout.flush();
    int back;
    cin >> back;
    
    ring_pos[0] = 0; // Reset ring 0 position
    
    vector<int> result(n-1);
    
    // For each ring from 1 to n-1, find its relative position to ring 0
    for (int ring = 1; ring < n; ring++) {
        vector<int> readings;
        
        // Rotate this ring through all possible positions and record readings
        for (int pos = 0; pos < total_sections; pos++) {
            cout << "? " << ring << " 1" << endl;
            cout.flush();
            int reading;
            cin >> reading;
            readings.push_back(reading);
            ring_pos[ring] = (ring_pos[ring] + 1) % total_sections;
        }
        
        // Now rotate ring 0 through all positions and find where readings match
        vector<int> ring0_readings;
        for (int pos = 0; pos < total_sections; pos++) {
            cout << "? 0 1" << endl;
            cout.flush();
            int reading;
            cin >> reading;
            ring0_readings.push_back(reading);
            ring_pos[0] = (ring_pos[0] + 1) % total_sections;
        }
        
        // Find the offset where the patterns match best
        int best_offset = 0;
        int max_matches = 0;
        
        for (int offset = 0; offset < total_sections; offset++) {
            int matches = 0;
            for (int i = 0; i < total_sections; i++) {
                if (readings[i] == ring0_readings[(i + offset) % total_sections]) {
                    matches++;
                }
            }
            if (matches > max_matches) {
                max_matches = matches;
                best_offset = offset;
            }
        }
        
        result[ring-1] = best_offset;
    }
    
    cout << "!";
    for (int i = 0; i < n-1; i++) {
        cout << " " << result[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}