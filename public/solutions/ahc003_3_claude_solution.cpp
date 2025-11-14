#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to, cost;
};

vector<vector<Edge>> graph;
vector<vector<int>> estimated_h, estimated_v;
int queries = 0;

void init_graph() {
    graph.resize(900);
    estimated_h.assign(30, vector<int>(29, 5000));
    estimated_v.assign(29, vector<int>(30, 5000));
}

int get_vertex(int i, int j) {
    return i * 30 + j;
}

pair<int, int> get_coords(int v) {
    return {v / 30, v % 30};
}

void update_graph() {
    for (int i = 0; i < 900; i++) {
        graph[i].clear();
    }
    
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 29; j++) {
            int v1 = get_vertex(i, j);
            int v2 = get_vertex(i, j + 1);
            graph[v1].push_back({v2, estimated_h[i][j]});
            graph[v2].push_back({v1, estimated_h[i][j]});
        }
    }
    
    for (int i = 0; i < 29; i++) {
        for (int j = 0; j < 30; j++) {
            int v1 = get_vertex(i, j);
            int v2 = get_vertex(i + 1, j);
            graph[v1].push_back({v2, estimated_v[i][j]});
            graph[v2].push_back({v1, estimated_v[i][j]});
        }
    }
}

string dijkstra(int si, int sj, int ti, int tj) {
    int start = get_vertex(si, sj);
    int target = get_vertex(ti, tj);
    
    vector<int> dist(900, INT_MAX);
    vector<int> parent(900, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    dist[start] = 0;
    pq.push({0, start});
    
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();
        
        if (d > dist[v]) continue;
        if (v == target) break;
        
        for (auto& edge : graph[v]) {
            int next_dist = dist[v] + edge.cost;
            if (next_dist < dist[edge.to]) {
                dist[edge.to] = next_dist;
                parent[edge.to] = v;
                pq.push({next_dist, edge.to});
            }
        }
    }
    
    vector<int> path;
    int curr = target;
    while (curr != -1) {
        path.push_back(curr);
        curr = parent[curr];
    }
    reverse(path.begin(), path.end());
    
    string result = "";
    for (int i = 1; i < path.size(); i++) {
        auto [pi, pj] = get_coords(path[i-1]);
        auto [ci, cj] = get_coords(path[i]);
        
        if (ci == pi + 1) result += "D";
        else if (ci == pi - 1) result += "U";
        else if (cj == pj + 1) result += "R";
        else if (cj == pj - 1) result += "L";
    }
    
    return result;
}

void update_estimates(int si, int sj, int ti, int tj, const string& path, int actual_cost) {
    int path_length = path.length();
    if (path_length == 0) return;
    
    double avg_cost = (double)actual_cost / path_length;
    
    int ci = si, cj = sj;
    for (char c : path) {
        int ni = ci, nj = cj;
        if (c == 'D') ni++;
        else if (c == 'U') ni--;
        else if (c == 'R') nj++;
        else if (c == 'L') nj--;
        
        if (ci == ni) { // horizontal edge
            int j = min(cj, nj);
            estimated_h[ci][j] = (estimated_h[ci][j] * 9 + avg_cost) / 10;
        } else { // vertical edge
            int i = min(ci, ni);
            estimated_v[i][cj] = (estimated_v[i][cj] * 9 + avg_cost) / 10;
        }
        
        ci = ni;
        cj = nj;
    }
}

int main() {
    init_graph();
    
    for (int q = 0; q < 1000; q++) {
        int si, sj, ti, tj;
        cin >> si >> sj >> ti >> tj;
        
        update_graph();
        string path = dijkstra(si, sj, ti, tj);
        
        cout << path << endl;
        cout.flush();
        
        int result;
        cin >> result;
        
        update_estimates(si, sj, ti, tj, path, result);
        queries++;
    }
    
    return 0;
}