#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <tuple>
#include <map>

using namespace std;

const long long INF = 4e18; 

struct Point {
    long long x, y;
};

struct Edge {
    int to;
    int weight;
    int id;
};

long long dist_sq(Point p1, Point p2) {
    long long dx = p1.x - p2.x;
    long long dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M, K;
    cin >> N >> M >> K;

    vector<Point> stations(N);
    for (int i = 0; i < N; ++i) {
        cin >> stations[i].x >> stations[i].y;
    }

    vector<vector<Edge>> adj(N);
    for (int i = 0; i < M; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        --u; --v;
        adj[u].push_back({v, w, i});
        adj[v].push_back({u, w, i});
    }

    vector<Point> residents(K);
    for (int i = 0; i < K; ++i) {
        cin >> residents[i].x >> residents[i].y;
    }

    vector<vector<long long>> min_P(N, vector<long long>(K));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < K; ++j) {
            min_P[i][j] = static_cast<long long>(ceil(sqrt(static_cast<long double>(dist_sq(stations[i], residents[j])))));
        }
    }

    vector<bool> V_active(N, false);
    V_active[0] = true;

    vector<int> assigned_v(K, 0);
    vector<long long> assigned_dist(K);
    for (int k = 0; k < K; ++k) {
        assigned_dist[k] = min_P[0][k];
    }

    long long current_mst_cost = 0;
    vector<pair<long long, int>> min_edge_to_active(N, {INF, -1});
    vector<int> mst_edges_from(N, -1);

    for (const auto& edge : adj[0]) {
        min_edge_to_active[edge.to] = {edge.weight, 0};
    }

    for (int iter = 0; iter < N - 1; ++iter) {
        vector<long long> current_P(N, 0);
        for (int k = 0; k < K; ++k) {
            current_P[assigned_v[k]] = max(current_P[assigned_v[k]], assigned_dist[k]);
        }
        long long current_power_cost = 0;
        for (int i = 0; i < N; ++i) {
            current_power_cost += current_P[i] * current_P[i];
        }
        long long current_total_cost = current_mst_cost + current_power_cost;

        long long best_next_cost = current_total_cost;
        int best_v_to_add = -1;

        for (int v = 0; v < N; ++v) {
            if (V_active[v]) continue;

            long long min_w = min_edge_to_active[v].first;
            if (min_w >= INF) continue;

            long long cand_mst_cost = current_mst_cost + min_w;
            
            vector<long long> cand_P(N, 0);
            for (int k = 0; k < K; ++k) {
                if (min_P[v][k] < assigned_dist[k]) {
                    cand_P[v] = max(cand_P[v], min_P[v][k]);
                } else {
                    cand_P[assigned_v[k]] = max(cand_P[assigned_v[k]], assigned_dist[k]);
                }
            }

            long long cand_power_cost = 0;
            for (int i = 0; i < N; ++i) {
                cand_power_cost += cand_P[i] * cand_P[i];
            }

            long long cand_total_cost = cand_mst_cost + cand_power_cost;

            if (cand_total_cost < best_next_cost) {
                best_next_cost = cand_total_cost;
                best_v_to_add = v;
            }
        }

        if (best_v_to_add != -1) {
            int v_added = best_v_to_add;
            V_active[v_added] = true;
            current_mst_cost += min_edge_to_active[v_added].first;
            mst_edges_from[v_added] = min_edge_to_active[v_added].second;

            for (int k = 0; k < K; ++k) {
                if (min_P[v_added][k] < assigned_dist[k]) {
                    assigned_v[k] = v_added;
                    assigned_dist[k] = min_P[v_added][k];
                }
            }

            for (const auto& edge : adj[v_added]) {
                if (!V_active[edge.to] && (long long)edge.weight < min_edge_to_active[edge.to].first) {
                    min_edge_to_active[edge.to] = {edge.weight, v_added};
                }
            }
        } else {
            break;
        }
    }

    vector<long long> final_P(N, 0);
    for (int k = 0; k < K; ++k) {
        final_P[assigned_v[k]] = max(final_P[assigned_v[k]], assigned_dist[k]);
    }
    for(int i = 0; i < N; ++i) {
        if (!V_active[i]) final_P[i] = 0;
    }

    vector<int> final_B(M, 0);
    for(int i = 0; i < N; ++i){
        if (V_active[i] && i != 0) {
            int u = i;
            int v = mst_edges_from[i];
            
            for(const auto& edge : adj[u]){
                if(edge.to == v){
                    final_B[edge.id] = 1;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        cout << final_P[i] << (i == N - 1 ? "" : " ");
    }
    cout << "\n";
    for (int i = 0; i < M; ++i) {
        cout << final_B[i] << (i == M - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}