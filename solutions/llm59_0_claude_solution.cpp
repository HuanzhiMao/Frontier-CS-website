#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<vector<int>> less(n + 1);
    vector<vector<int>> greater(n + 1);
    
    auto query = [&](int i, int j) -> char {
        cout << "? " << i << " " << j << endl;
        cout.flush();
        char result;
        cin >> result;
        return result;
    };
    
    // Build comparison graph
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            char result = query(i, j);
            if (result == '<') {
                less[i].push_back(j);
                greater[j].push_back(i);
            } else {
                less[j].push_back(i);
                greater[i].push_back(j);
            }
        }
    }
    
    // Topological sort to determine the order
    vector<int> indegree(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        indegree[i] = greater[i].size();
    }
    
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int i = 1; i <= n; i++) {
        if (indegree[i] == 0) {
            pq.push(i);
        }
    }
    
    vector<int> result;
    while (!pq.empty()) {
        int curr = pq.top();
        pq.pop();
        result.push_back(curr);
        
        for (int next : less[curr]) {
            indegree[next]--;
            if (indegree[next] == 0) {
                pq.push(next);
            }
        }
    }
    
    cout << "!";
    for (int x : result) {
        cout << " " << x;
    }
    cout << endl;
    cout.flush();
    
    return 0;
}