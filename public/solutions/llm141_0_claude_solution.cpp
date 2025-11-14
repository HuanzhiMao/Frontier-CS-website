#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, k;
    cin >> n >> k;
    
    set<int> distinct_types;
    queue<int> memory;
    map<int, bool> in_memory;
    
    // First pass: query all bakeries to get initial information
    for (int i = 1; i <= n; i++) {
        cout << "? " << i << endl;
        cout.flush();
        
        char response;
        cin >> response;
        
        if (response == 'N') {
            // New cake type found
            distinct_types.insert(i);
            memory.push(i);
            in_memory[i] = true;
            
            // If memory exceeds capacity, remove oldest
            if (memory.size() > k) {
                int oldest = memory.front();
                memory.pop();
                in_memory[oldest] = false;
            }
        }
    }
    
    // If we found fewer than k distinct types, we're done
    if (distinct_types.size() <= k) {
        cout << "! " << distinct_types.size() << endl;
        cout.flush();
        return 0;
    }
    
    // Reset memory for systematic checking
    cout << "R" << endl;
    cout.flush();
    
    while (!memory.empty()) memory.pop();
    in_memory.clear();
    
    // Second pass: verify and count distinct types more carefully
    vector<int> bakeries_to_check;
    for (int type : distinct_types) {
        bakeries_to_check.push_back(type);
    }
    
    set<int> verified_types;
    
    for (int bakery : bakeries_to_check) {
        cout << "? " << bakery << endl;
        cout.flush();
        
        char response;
        cin >> response;
        
        if (response == 'N') {
            verified_types.insert(bakery);
        }
    }
    
    cout << "! " << verified_types.size() << endl;
    cout.flush();
    
    return 0;
}