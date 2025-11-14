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
    
    // First pass: query all bakeries to get initial data
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
    
    // Reset memory
    cout << "R" << endl;
    cout.flush();
    memory_queue.clear();
    
    // Second pass: verify distinct types by querying them again
    vector<int> candidates(distinct_types.begin(), distinct_types.end());
    set<int> verified_types;
    
    for (int bakery : candidates) {
        cout << "? " << bakery << endl;
        cout.flush();
        
        char response;
        cin >> response;
        
        if (response == 'N') {
            verified_types.insert(bakery);
        }
        
        memory_queue.push_back(bakery);
        if (memory_queue.size() > k) {
            memory_queue.erase(memory_queue.begin());
        }
    }
    
    cout << "! " << verified_types.size() << endl;
    cout.flush();
    
    return 0;
}