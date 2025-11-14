#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <random>
#include <tuple>
#include <map>
#include <queue>

using namespace std;

const long long INF = 1e18;

struct Point {
    long long x, y;
};

struct Edge {
    int u, v, w, id;
};

int N, M, K;
vector<Point> stations;
vector<Edge> edges;
vector<Point> residents;

vector<vector<long long>> sp_dist;
vector<vector<int>> sp_parent;
vector<vector<pair<int, int>>> adj_w; // for dijkstra with weights
vector<vector<pair<int, int>>> adj_id; // for path reconstruction with edge ids

vector<vector<int>> p_ik;
vector<vector<int>> sorted_stations_by_p;

struct DSU {
    vector<int> parent;
    DSU(int n) {
        parent.resize(n + 1);
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
        }
    }
};

long long get_dist_sq(const Point& p1, const Point& p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

int calculate_p(long long dist_sq) {
    if (dist_sq == 0) return 0;
    long long r = sqrt(dist_sq);
    while (r * r < dist_sq) r++;
    while (r > 0 && (r - 1) * (r - 1) >= dist_sq) r--;
    return r;
}

void dijkstra(int start_node) {
    sp_dist[start_node].assign(N + 1, INF);
    sp_parent[start_node].assign(N + 1, -1);
    sp_dist[start_node][start_node] = 0;

    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    pq.push({0, start_node});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > sp_dist[start_node][u]) {
            continue;
        }

        for (auto& edge : adj_w[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (sp_dist[start_node][u] + weight < sp_dist[start_node][v]) {
                sp_dist[start_node][v] = sp_dist[start_node][u] + weight;
                sp_parent[start_node][v] = u;
                pq.push({sp_dist[start_node][v], v});
            }
        }
    }
}

struct Solution {
    vector<int> P;
    vector<int> B;
    long long cost;
};

Solution calculate_solution(const vector<bool>& V_active) {
    vector<int> terminals;
    for (int i = 1; i <= N; ++i) {
        if (V_active[i]) {
            terminals.push_back(i);
        }
    }
    if (terminals.empty() || find(terminals.begin(), terminals.end(), 1) == terminals.end()) {
        terminals.push_back(1);
    }
    
    vector<tuple<long long, int, int>> terminal_edges;
    for (size_t i = 0; i < terminals.size(); ++i) {
        for (size_t j = i + 1; j < terminals.size(); ++j) {
            terminal_edges.emplace_back(sp_dist[terminals[i]][terminals[j]], terminals[i], terminals[j]);
        }
    }
    sort(terminal_edges.begin(), terminal_edges.end());

    DSU dsu(N);
    vector<pair<int,int>> mst_edges;
    for (const auto& edge : terminal_edges) {
        auto [w, u, v] = edge;
        if (dsu.find(u) != dsu.find(v)) {
            dsu.unite(u, v);
            mst_edges.push_back({u, v});
        }
    }

    vector<bool> V_network(N + 1, false);
    vector<bool> edge_on(M + 1, false);
    V_network[1] = true;

    for(auto const& p : mst_edges) {
        int u1 = p.first;
        int v1 = p.second;
        int curr = v1;
        while(curr != u1){
            int prev = sp_parent[u1][curr];
            V_network[curr] = V_network[prev] = true;
            for(auto const& edge : adj_id[curr]){
                if(edge.first == prev){
                    edge_on[edge.second] = true;
                    break;
                }
            }
            curr = prev;
        }
    }

    long long network_cost = 0;
    vector<int> B(M, 0);
    for(int i=1; i<=M; ++i){
        if(edge_on[i]){
            network_cost += edges[i-1].w;
            B[i-1] = 1;
        }
    }

    vector<int> P(N + 1, 0);
    long long power_cost = 0;
    if (K > 0) {
        vector<long long> station_max_p_sq(N + 1, 0);
        for (int k = 0; k < K; ++k) {
            int best_station = -1;
            for (int i = 0; i < N; ++i) {
                int station_idx = sorted_stations_by_p[k][i];
                if (V_network[station_idx]) {
                    best_station = station_idx;
                    break;
                }
            }
            if (best_station != -1) {
                long long p_val = p_ik[k][best_station-1];
                station_max_p_sq[best_station] = max(station_max_p_sq[best_station], p_val * p_val);
            }
        }
        for(int i = 1; i <= N; ++i) {
            if (station_max_p_sq[i] > 0) {
                P[i] = calculate_p(station_max_p_sq[i]);
            }
            power_cost += (long long)P[i] * P[i];
        }
    }
    
    return {P, B, network_cost + power_cost};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start_time = chrono::high_resolution_clock::now();

    cin >> N >> M >> K;
    stations.resize(N + 1);
    for (int i = 1; i <= N; ++i) cin >> stations[i].x >> stations[i].y;
    
    edges.resize(M);
    adj_w.resize(N + 1);
    adj_id.resize(N + 1);
    for (int i = 0; i < M; ++i) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
        edges[i].id = i + 1;
        adj_w[edges[i].u].push_back({edges[i].v, edges[i].w});
        adj_w[edges[i].v].push_back({edges[i].u, edges[i].w});
        adj_id[edges[i].u].push_back({edges[i].v, edges[i].id});
        adj_id[edges[i].v].push_back({edges[i].u, edges[i].id});
    }
    
    residents.resize(K);
    for (int i = 0; i < K; ++i) cin >> residents[i].x >> residents[i].y;
    
    p_ik.resize(K, vector<int>(N));
    sorted_stations_by_p.resize(K, vector<int>(N));
    for (int k = 0; k < K; ++k) {
        vector<pair<int, int>> station_ps;
        for (int i = 1; i <= N; ++i) {
            long long d_sq = get_dist_sq(residents[k], stations[i]);
            p_ik[k][i-1] = calculate_p(d_sq);
            station_ps.push_back({p_ik[k][i-1], i});
        }
        sort(station_ps.begin(), station_ps.end());
        for(int i=0; i<N; ++i) {
            sorted_stations_by_p[k][i] = station_ps[i].second;
        }
    }
    
    sp_dist.resize(N + 1);
    sp_parent.resize(N + 1);
    for (int i = 1; i <= N; ++i) {
        dijkstra(i);
    }
    
    vector<bool> current_v_active(N + 1, false);
    if(K > 0) {
        for (int k = 0; k < K; ++k) {
            current_v_active[sorted_stations_by_p[k][0]] = true;
        }
    }
    current_v_active[1] = true;

    Solution best_sol = calculate_solution(current_v_active);
    vector<bool> best_v_active = current_v_active;
    
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> station_dist(2, N);
    uniform_real_distribution<double> prob_dist(0.0, 1.0);
    
    double T_start = 5e6;
    double T_end = 1e3;
    double time_limit = 2800.0;

    int iter = 0;
    while(true){
        auto now = chrono::high_resolution_clock::now();
        double time_spent = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
        if (time_spent > time_limit) break;

        double T = T_start * pow(T_end/T_start, time_spent/time_limit);

        int station_to_flip = station_dist(rng);
        
        vector<bool> next_v_active = current_v_active;
        next_v_active[station_to_flip] = !next_v_active[station_to_flip];
        
        Solution next_sol = calculate_solution(next_v_active);

        if (next_sol.cost < best_sol.cost) {
            best_sol = next_sol;
            best_v_active = next_v_active;
            current_v_active = next_v_active;
        } else {
            double p = exp((double)(best_sol.cost - next_sol.cost) / T);
            if (prob_dist(rng) < p) {
                current_v_active = next_v_active;
            }
        }
        iter++;
    }

    Solution final_sol = calculate_solution(best_v_active);

    for (int i = 1; i <= N; ++i) {
        cout << final_sol.P[i] << (i == N ? "" : " ");
    }
    cout << endl;
    for (int i = 0; i < M; ++i) {
        cout << final_sol.B[i] << (i == M - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}