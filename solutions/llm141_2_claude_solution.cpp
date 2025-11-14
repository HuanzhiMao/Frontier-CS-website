#include <iostream>
#include <vector>
#include <set>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, k;
    cin >> n >> k;
    
    set<int> distinct_types;
    vector<int> memory_queue;
    
    // First pass: go through all bakeries once
    for (int i = 1; i <= n; i++) {
        cout << "? " << i << endl;
        cout.flush();
        
        char response;
        cin >> response;
        
        if (response == 'N') {
            // New cake type found
            distinct_types.insert(i);
        }
        
        memory_queue.push_back(i);
        if (memory_queue.size() > k) {
            memory_queue.erase(memory_queue.begin());
        }
    }
    
    // If we have room in memory for all distinct types, we're done
    if (distinct_types.size() <= k) {
        cout << "! " << distinct_types.size() << endl;
        cout.flush();
        return 0;
    }
    
    // Reset and verify our count
    cout << "R" << endl;
    cout.flush();
    
    int confirmed_distinct = 0;
    vector<int> candidates(distinct_types.begin(), distinct_types.end());
    
    // Query each candidate to confirm it's distinct
    for (int bakery : candidates) {
        cout << "? " << bakery << endl;
        cout.flush();
        
        char response;
        cin >> response;
        
        if (response == 'N') {
            confirmed_distinct++;
        }
        
        if (confirmed_distinct == k) {
            break;
        }
    }
    
    // If we confirmed k distinct types and there were more candidates,
    // we need to account for the remaining ones
    if (confirmed_distinct == k && candidates.size() > k) {
        cout << "! " << candidates.size() << endl;
    } else {
        cout << "! " << confirmed_distinct << endl;
    }
    cout.flush();
    
    return 0;
}