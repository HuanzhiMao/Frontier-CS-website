#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M;
    cin >> N >> M;
    
    vector<pair<int, int>> edges(M);
    for (int i = 0; i < M; i++) {
        cin >> edges[i].first >> edges[i].second;
    }
    
    // First, find all possible pairs using binary search approach
    vector<pair<int, int>> candidates;
    for (int a = 0; a < N; a++) {
        for (int b = 0; b < N; b++) {
            if (a != b) {
                candidates.push_back({a, b});
            }
        }
    }
    
    // Use random queries to eliminate candidates
    while (candidates.size() > 1) {
        // Create a random configuration
        vector<int> config(M);
        for (int i = 0; i < M; i++) {
            config[i] = rand() % 2;
        }
        
        // Build directed graph based on config
        vector<vector<int>> adj(N);
        for (int i = 0; i < M; i++) {
            if (config[i] == 0) {
                adj[edges[i].first].push_back(edges[i].second);
            } else {
                adj[edges[i].second].push_back(edges[i].first);
            }
        }
        
        // Ask query
        cout << "0";
        for (int i = 0; i < M; i++) {
            cout << " " << config[i];
        }
        cout << endl;
        cout.flush();
        
        int response;
        cin >> response;
        
        // Filter candidates based on response
        vector<pair<int, int>> new_candidates;
        for (auto& candidate : candidates) {
            int a = candidate.first;
            int b = candidate.second;
            
            // Check if there's a path from a to b in current directed graph
            vector<bool> visited(N, false);
            queue<int> q;
            q.push(a);
            visited[a] = true;
            bool can_reach = false;
            
            while (!q.empty()) {
                int curr = q.front();
                q.pop();
                
                if (curr == b) {
                    can_reach = true;
                    break;
                }
                
                for (int next : adj[curr]) {
                    if (!visited[next]) {
                        visited[next] = true;
                        q.push(next);
                    }
                }
            }
            
            // Keep candidate if it matches the response
            if ((can_reach && response == 1) || (!can_reach && response == 0)) {
                new_candidates.push_back(candidate);
            }
        }
        
        candidates = new_candidates;
    }
    
    cout << "1 " << candidates[0].first << " " << candidates[0].second << endl;
    cout.flush();
    
    return 0;
}