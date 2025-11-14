#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>
#include <unordered_set>
#include <iomanip>
#include <queue>

using namespace std;

const long long INF = 1e18;

int N, M, K;

struct Point {
    long long x, y;
};

struct Edge {
    int to;
    long long cost;
};

struct EdgeInfo {
    int u, v, w;
};

Point stations[100];
Point residents[5000];
vector<Edge> adj[100];
EdgeInfo edges[300];
long long dist[100][100];
int pred[100][100];
long long eucl_dist_sq[100][5000];

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

long long dist_sq(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

void dijkstra(int start) {
    for (int i = 0; i < N; ++i) {
        dist[start][i] = INF;
        pred[start][i] = -1;
    }
    dist[start][start] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[start][u]) {
            continue;
        }

        for (const auto& edge : adj[u]) {
            if (dist[start][u] + edge.cost < dist[start][edge.to]) {
                dist[start][edge.to] = dist[start][u] + edge.cost;
                pred[start][edge.to] = u;
                pq.push({dist[start][edge.to], edge.to});
            }
        }
    }
}

long long calculate_cable_cost(const vector<int>& active_nodes) {
    if (active_nodes.size() <= 1) {
        return 0;
    }
    long long total_w = 0;
    vector<long long> min_cost(active_nodes.size(), INF);
    vector<bool> visited(active_nodes.size(), false);
    min_cost[0] = 0;

    for (size_t i = 0; i < active_nodes.size(); ++i) {
        int u_idx = -1;
        for (size_t j = 0; j < active_nodes.size(); ++j) {
            if (!visited[j] && (u_idx == -1 || min_cost[j] < min_cost[u_idx])) {
                u_idx = j;
            }
        }
        if (u_idx == -1) break;

        visited[u_idx] = true;
        total_w += min_cost[u_idx];
        int u_node = active_nodes[u_idx];

        for (size_t v_idx = 0; v_idx < active_nodes.size(); ++v_idx) {
            if (!visited[v_idx]) {
                int v_node = active_nodes[v_idx];
                min_cost[v_idx] = min(min_cost[v_idx], dist[u_node][v_node]);
            }
        }
    }
    return total_w;
}

long long calculate_power_cost(const vector<int>& active_nodes, vector<int>* assignments = nullptr) {
    if (active_nodes.empty()) {
        if (assignments) assignments->assign(K, -1);
        return 0;
    }

    if (assignments) assignments->resize(K);
    vector<long long> max_dist_sq_per_station(N, 0);

    for (int k = 0; k < K; ++k) {
        long long min_d_sq = -1;
        int best_i = -1;
        for (int i : active_nodes) {
            if (best_i == -1 || eucl_dist_sq[i][k] < min_d_sq) {
                min_d_sq = eucl_dist_sq[i][k];
                best_i = i;
            }
        }
        max_dist_sq_per_station[best_i] = max(max_dist_sq_per_station[best_i], min_d_sq);
        if (assignments) (*assignments)[k] = best_i;
    }

    long long total_power_cost = 0;
    for (int i : active_nodes) {
        if (max_dist_sq_per_station[i] > 0) {
            long long p = round(sqrt((double)max_dist_sq_per_station[i]));
            total_power_cost += p * p;
        }
    }
    return total_power_cost;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start_time = chrono::high_resolution_clock::now();

    cin >> N >> M >> K;
    for (int i = 0; i < N; ++i) cin >> stations[i].x >> stations[i].y;
    for (int i = 0; i < M; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        --u; --v;
        adj[u].push_back({v, (long long)w});
        adj[v].push_back({u, (long long)w});
        edges[i] = {u, v, w};
    }
    for (int i = 0; i < K; ++i) cin >> residents[i].x >> residents[i].y;

    for (int i = 0; i < N; ++i) {
        dijkstra(i);
    }
    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < K; ++k) {
            eucl_dist_sq[i][k] = dist_sq(stations[i], residents[k]);
        }
    }

    unordered_set<int> current_active_set = {0};
    
    vector<int> current_active_vec(current_active_set.begin(), current_active_set.end());
    long long current_power_cost = calculate_power_cost(current_active_vec);
    long long current_cable_cost = 0;
    long long current_total_cost = current_power_cost;
    
    unordered_set<int> best_active_set = current_active_set;
    long long best_total_cost = current_total_cost;

    double time_limit = 2.9;
    
    double T_start = 5e7;
    double T_end = 1e2;
    
    while(true) {
        auto current_time = chrono::high_resolution_clock::now();
        double elapsed = chrono::duration<double>(current_time - start_time).count();
        if (elapsed > time_limit) break;

        double T = T_start * pow(T_end/T_start, elapsed/time_limit);
        
        unordered_set<int> next_active_set = current_active_set;
        
        if (uniform_int_distribution<>(0,1)(rng) == 0 && next_active_set.size() < N) {
            int v;
            do {
                v = uniform_int_distribution<>(0, N-1)(rng);
            } while (next_active_set.count(v));
            next_active_set.insert(v);
        } else if (next_active_set.size() > 1) {
            int v_idx = uniform_int_distribution<>(0, (int)next_active_set.size()-1)(rng);
            auto it = next_active_set.begin();
            advance(it, v_idx);
            int v = *it;
            if (v == 0) continue;
            next_active_set.erase(v);
        } else {
            continue;
        }

        vector<int> next_active_vec(next_active_set.begin(), next_active_set.end());
        sort(next_active_vec.begin(), next_active_vec.end());

        long long next_cable_cost = calculate_cable_cost(next_active_vec);
        long long next_power_cost = calculate_power_cost(next_active_vec);
        long long next_total_cost = next_cable_cost + next_power_cost;

        long long delta = next_total_cost - current_total_cost;
        
        if (delta < 0 || (T > 1e-9 && uniform_real_distribution<>(0.0, 1.0)(rng) < exp(-(double)delta/T))) {
            current_active_set = next_active_set;
            current_total_cost = next_total_cost;
            
            if (current_total_cost < best_total_cost) {
                best_total_cost = current_total_cost;
                best_active_set = current_active_set;
            }
        }
    }

    vector<int> P(N, 0);
    vector<int> B(M, 0);

    vector<int> best_active_vec(best_active_set.begin(), best_active_set.end());
    sort(best_active_vec.begin(), best_active_vec.end());

    vector<int> assignments;
    calculate_power_cost(best_active_vec, &assignments);
    
    vector<long long> max_dist_sq(N, 0);
    for(int k=0; k<K; ++k) {
        int station_idx = assignments[k];
        max_dist_sq[station_idx] = max(max_dist_sq[station_idx], eucl_dist_sq[station_idx][k]);
    }
    for(int i=0; i<N; ++i) {
        if(max_dist_sq[i] > 0) {
            P[i] = round(sqrt((double)max_dist_sq[i]));
        }
    }

    if (best_active_vec.size() > 1) {
        vector<long long> mst_cost(best_active_vec.size(), INF);
        vector<int> parent_idx(best_active_vec.size(), -1);
        vector<bool> in_mst(best_active_vec.size(), false);

        mst_cost[0] = 0;

        for (size_t i = 0; i < best_active_vec.size(); ++i) {
            int u_idx = -1;
            for (size_t j = 0; j < best_active_vec.size(); ++j) {
                if (!in_mst[j] && (u_idx == -1 || mst_cost[j] < mst_cost[u_idx])) {
                    u_idx = j;
                }
            }

            if (u_idx == -1) break;
            in_mst[u_idx] = true;
            int u_node = best_active_vec[u_idx];

            for (size_t v_idx = 0; v_idx < best_active_vec.size(); ++v_idx) {
                if (!in_mst[v_idx]) {
                    int v_node = best_active_vec[v_idx];
                    if (dist[u_node][v_node] < mst_cost[v_idx]) {
                        mst_cost[v_idx] = dist[u_node][v_node];
                        parent_idx[v_idx] = u_idx;
                    }
                }
            }
        }
        
        unordered_map<int, int> edge_indices[N];
        for (int i = 0; i < M; ++i) {
            edge_indices[edges[i].u][edges[i].v] = i;
            edge_indices[edges[i].v][edges[i].u] = i;
        }

        for (size_t i = 0; i < best_active_vec.size(); ++i) {
            if (parent_idx[i] != -1) {
                int u_node = best_active_vec[parent_idx[i]];
                int v_node = best_active_vec[i];
                int curr = v_node;
                while (curr != u_node) {
                    int p = pred[u_node][curr];
                    B[edge_indices[p][curr]] = 1;
                    curr = p;
                }
            }
        }
    }
    
    for (int i = 0; i < N; ++i) {
        cout << P[i] << (i == N - 1 ? "" : " ");
    }
    cout << endl;
    for (int i = 0; i < M; ++i) {
        cout << B[i] << (i == M - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}