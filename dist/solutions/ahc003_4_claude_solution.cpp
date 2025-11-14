#include <bits/stdc++.h>
using namespace std;

const int N = 30;
double h[N][N-1], v[N-1][N];
int query_count = 0;

string solve(int si, int sj, int ti, int tj) {
    vector<vector<double>> dist(N, vector<double>(N, 1e18));
    vector<vector<pair<int,int>>> parent(N, vector<pair<int,int>>(N, {-1, -1}));
    
    priority_queue<tuple<double, int, int>, vector<tuple<double, int, int>>, greater<tuple<double, int, int>>> pq;
    
    dist[si][sj] = 0;
    pq.push({0, si, sj});
    
    while (!pq.empty()) {
        auto [d, i, j] = pq.top();
        pq.pop();
        
        if (d > dist[i][j]) continue;
        
        // Try all 4 directions
        vector<pair<int,int>> dirs = {{-1,0}, {1,0}, {0,-1}, {0,1}};
        for (auto [di, dj] : dirs) {
            int ni = i + di, nj = j + dj;
            if (ni < 0 || ni >= N || nj < 0 || nj >= N) continue;
            
            double edge_cost;
            if (di == -1) edge_cost = v[ni][j]; // Up
            else if (di == 1) edge_cost = v[i][j]; // Down
            else if (dj == -1) edge_cost = h[i][nj]; // Left
            else edge_cost = h[i][j]; // Right
            
            double new_dist = dist[i][j] + edge_cost;
            if (new_dist < dist[ni][nj]) {
                dist[ni][nj] = new_dist;
                parent[ni][nj] = {i, j};
                pq.push({new_dist, ni, nj});
            }
        }
    }
    
    // Reconstruct path
    string path = "";
    int ci = ti, cj = tj;
    vector<pair<int,int>> route;
    
    while (ci != si || cj != sj) {
        route.push_back({ci, cj});
        auto [pi, pj] = parent[ci][cj];
        ci = pi;
        cj = pj;
    }
    route.push_back({si, sj});
    reverse(route.begin(), route.end());
    
    for (int i = 1; i < route.size(); i++) {
        int pi = route[i-1].first, pj = route[i-1].second;
        int ci = route[i].first, cj = route[i].second;
        
        if (ci == pi - 1) path += 'U';
        else if (ci == pi + 1) path += 'D';
        else if (cj == pj - 1) path += 'L';
        else path += 'R';
    }
    
    return path;
}

void update_edge_costs(const string& path, int si, int sj, int result) {
    if (path.empty()) return;
    
    double estimated_cost = (double)result / path.length();
    double learning_rate = 0.1;
    
    int ci = si, cj = sj;
    for (char c : path) {
        if (c == 'U') {
            if (ci > 0) {
                v[ci-1][cj] = v[ci-1][cj] * (1 - learning_rate) + estimated_cost * learning_rate;
                ci--;
            }
        } else if (c == 'D') {
            if (ci < N-1) {
                v[ci][cj] = v[ci][cj] * (1 - learning_rate) + estimated_cost * learning_rate;
                ci++;
            }
        } else if (c == 'L') {
            if (cj > 0) {
                h[ci][cj-1] = h[ci][cj-1] * (1 - learning_rate) + estimated_cost * learning_rate;
                cj--;
            }
        } else if (c == 'R') {
            if (cj < N-1) {
                h[ci][cj] = h[ci][cj] * (1 - learning_rate) + estimated_cost * learning_rate;
                cj++;
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // Initialize edge costs with uniform values
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N-1; j++) {
            h[i][j] = 5000;
        }
    }
    for (int i = 0; i < N-1; i++) {
        for (int j = 0; j < N; j++) {
            v[i][j] = 5000;
        }
    }
    
    string last_path;
    int last_si, last_sj;
    
    for (int q = 0; q < 1000; q++) {
        int si, sj, ti, tj;
        cin >> si >> sj >> ti >> tj;
        
        if (q > 0) {
            int result;
            cin >> result;
            update_edge_costs(last_path, last_si, last_sj, result);
        }
        
        string path = solve(si, sj, ti, tj);
        cout << path << "\n";
        cout.flush();
        
        last_path = path;
        last_si = si;
        last_sj = sj;
    }
    
    // Read final result
    int final_result;
    cin >> final_result;
    
    return 0;
}