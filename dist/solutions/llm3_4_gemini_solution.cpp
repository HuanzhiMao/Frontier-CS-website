#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

int n;
vector<int> adj[100005];

// Function to ask a query
// It sends a list of operations to the interactor and receives the results.
vector<int> ask_query(const vector<int>& q) {
    if (q.empty()) {
        return {};
    }
    cout << q.size();
    for (int x : q) {
        cout << " " << x;
    }
    cout << endl;
    cout.flush();

    vector<int> res(q.size());
    for (size_t i = 0; i < q.size(); ++i) {
        cin >> res[i];
    }
    return res;
}

// Function to submit the final answer
void submit_answer(const vector<int>& p) {
    cout << -1;
    for (int x : p) {
        cout << " " << x;
    }
    cout << endl;
    cout.flush();
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int subtask;
    cin >> subtask >> n;

    // Handle small cases
    if (n <= 2) {
        vector<int> p;
        for (int i = 1; i <= n; ++i) p.push_back(i);
        submit_answer(p);
        return 0;
    }

    // Path-growing O(N^2) solution.
    // This approach finds the cycle by starting with a small path and extending it one node at a time.
    
    // Step 1: Find the neighbors of node 1 to start the path.
    // We light up node 1, then individually test every other node v by toggling it on and off.
    // The response tells us if {1, v} has an adjacent pair.
    ask_query({1});
    vector<int> query_vec_1;
    for (int v = 2; v <= n; ++v) {
        query_vec_1.push_back(v);
        query_vec_1.push_back(v);
    }
    vector<int> results_1 = ask_query(query_vec_1);
    for (size_t i = 0; i < results_1.size(); i += 2) {
        if (results_1[i] == 1) {
            int v = query_vec_1[i];
            adj[1].push_back(v);
            adj[v].push_back(1);
        }
    }
    ask_query({1}); // Turn off node 1 to clean up.

    // Step 2: Build the initial path using node 1 and its two neighbors.
    vector<int> p;
    p.push_back(adj[1][0]);
    p.push_back(1);
    p.push_back(adj[1][1]);
    
    vector<bool> visited(n + 1, false);
    visited[p[0]] = true;
    visited[p[1]] = true;
    visited[p[2]] = true;

    // Step 3: Grow the path by repeatedly finding the next node in the cycle.
    while (p.size() < n) {
        int last = p.back();
        int prev = p[p.size() - 2];
        
        // If we don't know both neighbors of 'last' yet, find them.
        if (adj[last].size() < 2) {
            ask_query({last});
            vector<int> q_find_neighbor;
            for (int v = 1; v <= n; ++v) {
                if (!visited[v]) {
                    q_find_neighbor.push_back(v);
                    q_find_neighbor.push_back(v);
                }
            }
            if (!q_find_neighbor.empty()){
                vector<int> res = ask_query(q_find_neighbor);
                 for (size_t i = 0; i < res.size(); i += 2) {
                    if (res[i] == 1) {
                        int v = q_find_neighbor[i];
                        adj[last].push_back(v);
                        adj[v].push_back(last);
                    }
                }
            }
            ask_query({last});
        }
        
        // The other neighbor of 'last' (not 'prev') is the next node in the path.
        int next_node = -1;
        for (int neighbor : adj[last]) {
            if (neighbor != prev) {
                next_node = neighbor;
                break;
            }
        }
        p.push_back(next_node);
        visited[next_node] = true;
    }

    submit_answer(p);

    return 0;
}