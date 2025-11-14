#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to, cost;
};

vector<vector<Edge>> graph(900);
vector<vector<int>> edge_usage(900);
vector<vector<int>> edge_total_cost(900);

int get_node(int i, int j) {
    return i * 30 + j;
}

pair<int, int> get_pos(int node) {
    return {node / 30, node % 30};
}

vector<int> dijkstra(int start, int goal) {
    vector<int> dist(900, INT_MAX);
    vector<int> prev(900, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    dist[start] = 0;
    pq.push({0, start});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (d > dist[u]) continue;
        if (u == goal) break;
        
        for (int i = 0; i < graph[u].size(); i++) {
            int v = graph[u][i].to;
            int cost = graph[u][i].cost;
            
            if (edge_usage[u][i] > 0) {
                cost = edge_total_cost[u][i] / edge_usage[u][i];
            }
            
            if (dist[u] + cost < dist[v]) {
                dist[v] = dist[u] + cost;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    
    vector<int> path;
    int curr = goal;
    while (curr != -1) {
        path.push_back(curr);
        curr = prev[curr];
    }
    reverse(path.begin(), path.end());
    return path;
}

string path_to_string(const vector<int>& path) {
    string result = "";
    for (int i = 1; i < path.size(); i++) {
        auto [pi, pj] = get_pos(path[i-1]);
        auto [ci, cj] = get_pos(path[i]);
        
        if (ci == pi + 1) result += 'D';
        else if (ci == pi - 1) result += 'U';
        else if (cj == pj + 1) result += 'R';
        else if (cj == pj - 1) result += 'L';
    }
    return result;
}

void update_edge_costs(const vector<int>& path, int total_cost) {
    int path_length = path.size() - 1;
    if (path_length == 0) return;
    
    int cost_per_edge = total_cost / path_length;
    
    for (int i = 1; i < path.size(); i++) {
        int u = path[i-1];
        int v = path[i];
        
        for (int j = 0; j < graph[u].size(); j++) {
            if (graph[u][j].to == v) {
                edge_usage[u][j]++;
                edge_total_cost[u][j] += cost_per_edge;
                break;
            }
        }
        
        for (int j = 0; j < graph[v].size(); j++) {
            if (graph[v][j].to == u) {
                edge_usage[v][j]++;
                edge_total_cost[v][j] += cost_per_edge;
                break;
            }
        }
    }
}

int main() {
    // Initialize graph
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            int node = get_node(i, j);
            
            if (i > 0) {
                graph[node].push_back({get_node(i-1, j), 5000});
                edge_usage[node].push_back(0);
                edge_total_cost[node].push_back(0);
            }
            if (i < 29) {
                graph[node].push_back({get_node(i+1, j), 5000});
                edge_usage[node].push_back(0);
                edge_total_cost[node].push_back(0);
            }
            if (j > 0) {
                graph[node].push_back({get_node(i, j-1), 5000});
                edge_usage[node].push_back(0);
                edge_total_cost[node].push_back(0);
            }
            if (j < 29) {
                graph[node].push_back({get_node(i, j+1), 5000});
                edge_usage[node].push_back(0);
                edge_total_cost[node].push_back(0);
            }
        }
    }
    
    int prev_result = 0;
    
    for (int k = 0; k < 1000; k++) {
        int si, sj, ti, tj;
        cin >> si >> sj >> ti >> tj;
        
        int start = get_node(si, sj);
        int goal = get_node(ti, tj);
        
        vector<int> path = dijkstra(start, goal);
        string path_str = path_to_string(path);
        
        cout << path_str << endl;
        cout.flush();
        
        int result;
        cin >> result;
        
        if (k > 0) {
            update_edge_costs(path, prev_result);
        }
        
        prev_result = result;
    }
    
    return 0;
}