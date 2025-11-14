#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <queue>
#include <set>
#include <map>
#include <functional>
#include <chrono>
#include <random>

using namespace std;

const long long INF = 4e18; 

struct Point {
    long long x, y;
};

long long dist_sq(const Point& p1, const Point& p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M, K;
    cin >> N >> M >> K;

    vector<Point> stations(N + 1);
    for (int i = 1; i <= N; ++i) {
        cin >> stations[i].x >> stations[i].y;
    }

    vector<vector<pair<int, int>>> adj(N + 1);
    map<pair<int, int>, int> edge_to_id;
    for (int i = 0; i < M; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
        edge_to_id[{min(u, v), max(u, v)}] = i;
    }

    vector<Point> residents(K);
    for (int i = 0; i < K; ++i) {
        cin >> residents[i].x >> residents[i].y;
    }

    vector<vector<long long>> shortest_paths(N + 1, vector<long long>(N + 1, INF));
    vector<vector<int>> path_parents(N + 1, vector<int>(N + 1, 0));

    for (int i = 1; i <= N; ++i) {
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        shortest_paths[i][i] = 0;
        pq.push({0, i});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d > shortest_paths[i][u]) {
                continue;
            }

            for (auto& edge : adj[u]) {
                int v = edge.first;
                int weight = edge.second;
                if (shortest_paths[i][u] != INF && shortest_paths[i][u] + weight < shortest_paths[i][v]) {
                    shortest_paths[i][v] = shortest_paths[i][u] + weight;
                    path_parents[i][v] = u;
                    pq.push({shortest_paths[i][v], v});
                }
            }
        }
    }

    vector<vector<int>> resident_station_dist(K, vector<int>(N + 1));
    for (int i = 0; i < K; ++i) {
        for (int j = 1; j <= N; ++j) {
            resident_station_dist[i][j] = ceil(sqrt(dist_sq(residents[i], stations[j])));
        }
    }

    auto calculate_cost = [&](const vector<int>& active_stations_vec) {
        if (active_stations_vec.empty()) return INF;

        vector<int> p(N + 1, 0);
        for (int i = 0; i < K; ++i) {
            int min_dist = 5001;
            int best_station = -1;
            for (int station_idx : active_stations_vec) {
                if (resident_station_dist[i][station_idx] < min_dist) {
                    min_dist = resident_station_dist[i][station_idx];
                    best_station = station_idx;
                }
            }
            if (best_station == -1) return INF;
            p[best_station] = max(p[best_station], min_dist);
        }

        long long broadcast_cost = 0;
        for (int station_idx : active_stations_vec) {
            if (p[station_idx] > 5000) return INF;
            broadcast_cost += (long long)p[station_idx] * p[station_idx];
        }
        
        long long mst_cost = 0;
        if (active_stations_vec.size() > 1) {
            vector<long long> d(N + 1, INF);
            vector<bool> visited(N + 1, false);
            d[active_stations_vec[0]] = 0;

            for (size_t i = 0; i < active_stations_vec.size(); ++i) {
                int u = -1;
                for (int node : active_stations_vec) {
                    if (!visited[node] && (u == -1 || d[node] < d[u])) {
                        u = node;
                    }
                }
                if (u == -1) break;
                if (d[u] == INF) return INF;
                
                visited[u] = true;
                mst_cost += d[u];

                for (int v : active_stations_vec) {
                    if (!visited[v] && shortest_paths[u][v] < d[v]) {
                        d[v] = shortest_paths[u][v];
                    }
                }
            }
        }

        return broadcast_cost + mst_cost;
    };
    
    set<int> active_stations_set;
    active_stations_set.insert(1);
    for (int i = 0; i < K; ++i) {
        int min_dist = 5001;
        int best_station = -1;
        for (int j = 1; j <= N; ++j) {
            if (resident_station_dist[i][j] < min_dist) {
                min_dist = resident_station_dist[i][j];
                best_station = j;
            }
        }
        if (best_station != -1) {
            active_stations_set.insert(best_station);
        }
    }

    vector<int> current_active_stations(active_stations_set.begin(), active_stations_set.end());
    long long current_best_cost = calculate_cost(current_active_stations);

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    bool changed = true;
    while (changed) {
        changed = false;
        int best_station_to_remove = -1;
        
        vector<int> stations_to_try_removing;
        for(int s : current_active_stations){
            if (s != 1) stations_to_try_removing.push_back(s);
        }
        shuffle(stations_to_try_removing.begin(), stations_to_try_removing.end(), rng);

        for (int station_to_remove : stations_to_try_removing) {
            vector<int> next_active_stations;
            for (int s : current_active_stations) {
                if (s != station_to_remove) {
                    next_active_stations.push_back(s);
                }
            }
            
            long long next_cost = calculate_cost(next_active_stations);

            if (next_cost < current_best_cost) {
                current_best_cost = next_cost;
                best_station_to_remove = station_to_remove;
                break;
            }
        }

        if (best_station_to_remove != -1) {
            vector<int> updated_stations;
            for (int s : current_active_stations) {
                if (s != best_station_to_remove) {
                    updated_stations.push_back(s);
                }
            }
            current_active_stations = updated_stations;
            changed = true;
        }
    }
    
    vector<int> final_p(N + 1, 0);
    for (int i = 0; i < K; ++i) {
        int min_dist = 5001;
        int best_station = -1;
        for (int station_idx : current_active_stations) {
            if (resident_station_dist[i][station_idx] < min_dist) {
                min_dist = resident_station_dist[i][station_idx];
                best_station = station_idx;
            }
        }
        if (best_station != -1) {
            final_p[best_station] = max(final_p[best_station], min_dist);
        }
    }

    for (int i = 1; i <= N; ++i) {
        cout << final_p[i] << (i == N ? "" : " ");
    }
    cout << endl;

    vector<int> final_b(M, 0);
    if (current_active_stations.size() > 1) {
        vector<pair<long long, pair<int, int>>> mst_edges;
        for (size_t i = 0; i < current_active_stations.size(); ++i) {
            for (size_t j = i + 1; j < current_active_stations.size(); ++j) {
                mst_edges.push_back({shortest_paths[current_active_stations[i]][current_active_stations[j]], {current_active_stations[i], current_active_stations[j]}});
            }
        }
        sort(mst_edges.begin(), mst_edges.end());

        vector<int> dsu_parent(N + 1);
        iota(dsu_parent.begin(), dsu_parent.end(), 0);
        function<int(int)> find_set = [&](int v) {
            return v == dsu_parent[v] ? v : dsu_parent[v] = find_set(dsu_parent[v]);
        };
        auto unite_sets = [&](int a, int b) {
            a = find_set(a);
            b = find_set(b);
            if (a != b) dsu_parent[b] = a;
        };

        for (const auto& edge : mst_edges) {
            int u = edge.second.first;
            int v = edge.second.second;
            if (find_set(u) != find_set(v)) {
                unite_sets(u, v);
                int curr = v;
                while (curr != u) {
                    int p = path_parents[u][curr];
                    final_b[edge_to_id.at({min(p, curr), max(p, curr)})] = 1;
                    curr = p;
                }
            }
        }
    }

    for (int i = 0; i < M; ++i) {
        cout << final_b[i] << (i == M - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}