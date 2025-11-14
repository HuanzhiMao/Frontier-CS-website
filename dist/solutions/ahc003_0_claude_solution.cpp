#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to, cost;
};

vector<vector<Edge>> graph;
vector<vector<int>> dist;
int N = 30;

void initGraph() {
    graph.assign(N * N, vector<Edge>());
    dist.assign(N * N, vector<int>(N * N, 1e9));
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int v = i * N + j;
            if (i > 0) {
                int u = (i - 1) * N + j;
                graph[v].push_back({u, 5000});
                graph[u].push_back({v, 5000});
            }
            if (j > 0) {
                int u = i * N + (j - 1);
                graph[v].push_back({u, 5000});
                graph[u].push_back({v, 5000});
            }
        }
    }
}

void updateEdge(int from, int to, int cost) {
    for (auto& e : graph[from]) {
        if (e.to == to) {
            e.cost = cost;
            break;
        }
    }
    for (auto& e : graph[to]) {
        if (e.to == from) {
            e.cost = cost;
            break;
        }
    }
}

string dijkstra(int si, int sj, int ti, int tj) {
    int start = si * N + sj;
    int goal = ti * N + tj;
    
    vector<int> d(N * N, 1e9);
    vector<int> parent(N * N, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    
    d[start] = 0;
    pq.push({0, start});
    
    while (!pq.empty()) {
        auto [cost, v] = pq.top();
        pq.pop();
        
        if (cost > d[v]) continue;
        if (v == goal) break;
        
        for (auto& e : graph[v]) {
            if (d[v] + e.cost < d[e.to]) {
                d[e.to] = d[v] + e.cost;
                parent[e.to] = v;
                pq.push({d[e.to], e.to});
            }
        }
    }
    
    string path = "";
    int cur = goal;
    while (parent[cur] != -1) {
        int prev = parent[cur];
        int pi = prev / N, pj = prev % N;
        int ci = cur / N, cj = cur % N;
        
        if (pi < ci) path += "D";
        else if (pi > ci) path += "U";
        else if (pj < cj) path += "R";
        else path += "L";
        
        cur = prev;
    }
    
    reverse(path.begin(), path.end());
    return path;
}

int main() {
    initGraph();
    
    for (int k = 0; k < 1000; k++) {
        int si, sj, ti, tj;
        cin >> si >> sj >> ti >> tj;
        
        string path = dijkstra(si, sj, ti, tj);
        cout << path << endl;
        cout.flush();
        
        int result;
        cin >> result;
        
        // Update edge costs based on the path taken
        int ci = si, cj = sj;
        int pathLen = path.length();
        if (pathLen > 0) {
            int avgCost = result / pathLen;
            for (char c : path) {
                int ni = ci, nj = cj;
                if (c == 'U') ni--;
                else if (c == 'D') ni++;
                else if (c == 'L') nj--;
                else if (c == 'R') nj++;
                
                int from = ci * N + cj;
                int to = ni * N + nj;
                updateEdge(from, to, avgCost);
                
                ci = ni;
                cj = nj;
            }
        }
    }
    
    return 0;
}