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
            
            // Manage memory queue
            if (memory.size() > k) {
                int front = memory.front();
                memory.pop();
                in_memory[front] = false;
            }
        } else {
            // This cake type is already in memory, so it's a duplicate
            // We need to find which previous bakery has the same type
            memory.push(i);
            if (memory.size() > k) {
                int front = memory.front();
                memory.pop();
                in_memory[front] = false;
            }
        }
    }
    
    // If we have potential duplicates, we need to identify them more carefully
    if (distinct_types.size() < n) {
        cout << "R" << endl;
        cout.flush();
        memory = queue<int>();
        in_memory.clear();
        
        vector<int> cake_types;
        map<int, int> type_mapping;
        int current_type = 1;
        
        for (int i = 1; i <= n; i++) {
            cout << "? " << i << endl;
            cout.flush();
            
            char response;
            cin >> response;
            
            if (response == 'N') {
                // New type
                cake_types.push_back(current_type);
                type_mapping[i] = current_type;
                memory.push(current_type);
                in_memory[current_type] = true;
                current_type++;
            } else {
                // Find which type this matches
                queue<int> temp_memory = memory;
                vector<int> memory_contents;
                while (!temp_memory.empty()) {
                    memory_contents.push_back(temp_memory.front());
                    temp_memory.pop();
                }
                
                // This bakery has the same type as one in memory
                // Since we process in order, we can track this
                memory.push(memory_contents.back());
            }
            
            if (memory.size() > k) {
                int front = memory.front();
                memory.pop();
                in_memory[front] = false;
            }
        }
        
        cout << "! " << (current_type - 1) << endl;
    } else {
        cout << "! " << distinct_types.size() << endl;
    }
    
    cout.flush();
    return 0;
}