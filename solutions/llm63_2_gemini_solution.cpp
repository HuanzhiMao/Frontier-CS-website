#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <queue>

using namespace std;

int N, M;
vector<pair<int, int>> adj[10000];
vector<pair<int, int>> edges;

enum class DirType { AWAY, TOWARDS };
enum class CutDir { S0_TO_S1, S1_TO_S0 };

bool query(const vector<int>& orientations) {
    cout << 0;
    for (int i = 0; i < M; ++i) {
        cout << " " << orientations[i];
    }
    cout << endl;
    int result;
    cin >> result;
    return result == 1;
}

void answer(int a, int b) {
    cout << 1 << " " << a << " " << b << endl;
}

vector<int> build_orient(int k, DirType s0_type, DirType s1_type, CutDir cut_dir) {
    vector<int> orientations(M, -1);
    
    vector<bool> s0_nodes(N, false);
    for (int i = 0; i < N; ++i) {
        if (!((i >> k) & 1)) {
            s0_nodes[i] = true;
        }
    }

    auto process_set = [&](const vector<bool>& in_set, DirType type) {
        vector<bool> visited(N, false);
        for (int i = 0; i < N; ++i) {
            if (in_set[i] && !visited[i]) {
                vector<int> component_q;
                component_q.push_back(i);
                visited[i] = true;

                vector<pair<int,int>> parent(N, {-1,-1});

                int head = 0;
                while(head < component_q.size()){
                    int u = component_q[head++];
                    for(auto& edge : adj[u]){
                        int v = edge.first;
                        int edge_idx = edge.second;
                        if(in_set[v] && !visited[v]){
                            visited[v] = true;
                            parent[v] = {u, edge_idx};
                            component_q.push_back(v);
                        }
                    }
                }
                
                for(int node : component_q){
                    if(node == i) continue;
                    int p = parent[node].first;
                    int edge_idx = parent[node].second;

                    if (type == DirType::AWAY) { // p -> node
                        if (edges[edge_idx].first == p) {
                            orientations[edge_idx] = 0;
                        } else {
                            orientations[edge_idx] = 1;
                        }
                    } else { // TOWARDS, node -> p
                        if (edges[edge_idx].first == node) {
                            orientations[edge_idx] = 0;
                        } else {
                            orientations[edge_idx] = 1;
                        }
                    }
                }
            }
        }
    };

    vector<bool> s1_nodes(N, false);
    for (int i = 0; i < N; ++i) s1_nodes[i] = !s0_nodes[i];

    process_set(s0_nodes, s0_type);
    process_set(s1_nodes, s1_type);

    for (int i = 0; i < M; ++i) {
        if (orientations[i] == -1) {
            int u = edges[i].first;
            int v = edges[i].second;
            bool u_in_s0 = s0_nodes[u];
            bool v_in_s0 = s0_nodes[v];

            if (u_in_s0 != v_in_s0) { // cut edge
                if (cut_dir == CutDir::S0_TO_S1) {
                    if (u_in_s0) { // u -> v
                        orientations[i] = 0;
                    } else { // v -> u
                        orientations[i] = 1;
                    }
                } else { // S1_TO_S0
                    if (!u_in_s0) { // u -> v
                        orientations[i] = 0;
                    } else { // v -> u
                        orientations[i] = 1;
                    }
                }
            } else { // non-tree edge
                orientations[i] = 0; // U_i -> V_i by default
            }
        }
    }
    return orientations;
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> M;
    edges.resize(M);
    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        edges[i] = {u, v};
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }

    int final_A = 0, final_B = 0;

    for (int k = 0; k < 14; ++k) {
        if ((1 << k) >= N) break;

        auto q1_orient = build_orient(k, DirType::AWAY, DirType::AWAY, CutDir::S0_TO_S1);
        bool r1 = query(q1_orient);

        auto q2_orient = build_orient(k, DirType::AWAY, DirType::AWAY, CutDir::S1_TO_S0);
        bool r2 = query(q2_orient);

        if (r1 != r2) {
            if (r1) { // A in S0, B in S1
                final_B |= (1 << k);
            } else { // A in S1, B in S0
                final_A |= (1 << k);
            }
        } else {
            bool r_same = r1;
            auto q3_orient = build_orient(k, DirType::AWAY, DirType::TOWARDS, CutDir::S0_TO_S1);
            bool r3 = query(q3_orient);
            if (r3 != r_same) { // A, B in S1
                final_A |= (1 << k);
                final_B |= (1 << k);
            } // else A, B in S0, do nothing
        }
    }

    answer(final_A, final_B);

    return 0;
}