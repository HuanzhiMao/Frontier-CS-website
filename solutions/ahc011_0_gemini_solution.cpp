#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <chrono>
#include <random>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <queue>
#include <map>
#include <tuple>

using namespace std;

// --- Timer ---
auto start_time = chrono::steady_clock::now();
double time_left() {
    auto now = chrono::steady_clock::now();
    return 2000.0 - chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
}

// --- Random Number Generator ---
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

// --- Constants and Type Definitions ---
int N;
const int MAX_N = 10;
using Board = array<array<int, MAX_N>, MAX_N>;
using PosArray = array<pair<int, int>, MAX_N * MAX_N>;

struct Object {
    int id;
    int type;
    pair<int, int> initial_pos;
};

struct State {
    Board board; // stores object IDs, 0 for empty
    PosArray id_to_pos;
    long long dist_sum = -1;
    int tree_size = -1;
    long long score = -1;

    bool operator<(const State& other) const {
        return score < other.score;
    }
};

vector<Object> objects;
PosArray target_pos;

// --- DSU for cycle detection and component counting ---
struct DSU {
    vector<int> parent;
    vector<int> num_edges;
    vector<int> num_vertices;
    DSU(int n) {
        parent.resize(n);
        iota(parent.begin(), parent.end(), 0);
        num_edges.assign(n, 0);
        num_vertices.assign(n, 1);
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            if (num_vertices[root_i] < num_vertices[root_j]) swap(root_i, root_j);
            parent[root_j] = root_i;
            num_vertices[root_i] += num_vertices[root_j];
            num_edges[root_i] += num_edges[root_j];
        }
        num_edges[root_i]++;
    }
};

// --- Evaluation Function for SA ---
void evaluate(State& s) {
    if (s.score != -1) return;

    DSU dsu(N * N);
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int id1 = s.board[i][j];
            if (id1 == 0) continue;
            int type1 = objects[id1].type;

            if (i + 1 < N) {
                int id2 = s.board[i + 1][j];
                if (id2 != 0) {
                    int type2 = objects[id2].type;
                    if ((type1 & 8) && (type2 & 2)) dsu.unite(id1, id2);
                }
            }
            if (j + 1 < N) {
                int id2 = s.board[i][j + 1];
                if (id2 != 0) {
                    int type2 = objects[id2].type;
                    if ((type1 & 4) && (type2 & 1)) dsu.unite(id1, id2);
                }
            }
        }
    }

    s.tree_size = 0;
    for (int i = 1; i < N * N; ++i) {
        if (dsu.parent[i] == i) {
            if (dsu.num_edges[i] == dsu.num_vertices[i] - 1) {
                s.tree_size = max(s.tree_size, dsu.num_vertices[i]);
            }
        }
    }
    
    s.dist_sum = 0;
    for (int i = 1; i < N * N; ++i) {
        s.dist_sum += abs(s.id_to_pos[i].first - objects[i].initial_pos.first) + abs(s.id_to_pos[i].second - objects[i].initial_pos.second);
    }
    
    long long W = 2LL * N * N;
    s.score = -W * s.tree_size + s.dist_sum;
}

// --- Hex to Int Conversion ---
int hex_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    return c - 'a' + 10;
}

// --- A* search implementation ---
const int dr[] = {-1, 1, 0, 0};
const int dc[] = {0, 0, -1, 1};
const char move_chars[] = {'U', 'D', 'L', 'R'};

struct AStarNode {
    Board board;
    pair<int, int> empty_pos;
    int g;
    int h;
    string path;

    bool operator>(const AStarNode& other) const {
        return g + h > other.g + other.h;
    }
};

int manhattan_dist(pair<int, int> a, pair<int, int> b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

int calculate_h(const Board& board) {
    int total_h = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int id = board[i][j];
            if (id != 0) {
                total_h += manhattan_dist({i, j}, target_pos[id]);
            }
        }
    }
    return total_h;
}

string a_star(const State& initial_state, const State& target_state) {
    for (int i = 1; i < N * N; ++i) {
        target_pos[i] = target_state.id_to_pos[i];
    }

    priority_queue<AStarNode, vector<AStarNode>, greater<AStarNode>> pq;
    map<Board, int> min_g;

    AStarNode initial_node = {initial_state.board, initial_state.id_to_pos[0], 0, calculate_h(initial_state.board), ""};
    pq.push(initial_node);
    min_g[initial_state.board] = 0;
    
    int T_limit = 2 * N * N * N;

    while (!pq.empty() && time_left() > 100) {
        AStarNode current = pq.top();
        pq.pop();

        if (current.g > min_g[current.board]) {
            continue;
        }

        if (current.h == 0) {
            return current.path;
        }

        if ((int)current.path.length() >= T_limit) {
            continue;
        }
        
        int r0 = current.empty_pos.first;
        int c0 = current.empty_pos.second;

        for (int i = 0; i < 4; ++i) {
            int r1 = r0 + dr[i];
            int c1 = c0 + dc[i];

            if (r1 >= 0 && r1 < N && c1 >= 0 && c1 < N) {
                Board next_board = current.board;
                swap(next_board[r0][c0], next_board[r1][c1]);
                
                int next_g = current.g + 1;

                if (min_g.find(next_board) == min_g.end() || next_g < min_g[next_board]) {
                    min_g[next_board] = next_g;
                    int next_h = calculate_h(next_board);
                    AStarNode next_node = {next_board, {r1, c1}, next_g, next_h, current.path + move_chars[i]};
                    pq.push(next_node);
                }
            }
        }
    }
    return "";
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T_max;
    cin >> N >> T_max;

    State initial_state;
    int current_id = 1;
    for (int i = 0; i < N; ++i) {
        string row;
        cin >> row;
        for (int j = 0; j < N; ++j) {
            int type = hex_to_int(row[j]);
            if (type == 0) {
                initial_state.board[i][j] = 0;
                initial_state.id_to_pos[0] = {i, j};
            } else {
                initial_state.board[i][j] = current_id;
                initial_state.id_to_pos[current_id] = {i, j};
                objects.emplace_back(Object{current_id, type, {i, j}});
                current_id++;
            }
        }
    }
    objects.insert(objects.begin(), Object{0, 0, {-1,-1}}); // dummy for id 0

    State current_s = initial_state;
    evaluate(current_s);
    State best_s = current_s;

    double sa_time_limit_ms = 1850.0;
    if (N <= 7) sa_time_limit_ms = 1800.0;
    if (N > 8) sa_time_limit_ms = 1900.0;
    
    double start_temp = 50, end_temp = 0.1;
    uniform_int_distribution<int> dist(0, N - 1);
    
    while (time_left() > 2000.0 - sa_time_limit_ms) {
        double elapsed_ratio = (2000.0 - time_left()) / sa_time_limit_ms;
        double temp = start_temp * pow(end_temp / start_temp, elapsed_ratio);

        State next_s = current_s;
        
        int r1, c1, r2, c2, r3, c3, r4, c4;
        r1 = dist(rng); c1 = dist(rng);
        do { r2 = dist(rng); c2 = dist(rng); } while (r1 == r2 && c1 == c2);
        
        do { r3 = dist(rng); c3 = dist(rng); } while ((r3==r1 && c3==c1) || (r3==r2 && c3==c2));
        do { r4 = dist(rng); c4 = dist(rng); } while ((r4==r1 && c4==c1) || (r4==r2 && c4==c2) || (r4==r3 && c4==c3));

        auto& b = next_s.board;
        auto& p = next_s.id_to_pos;
        int id1 = b[r1][c1], id2 = b[r2][c2];
        swap(b[r1][c1], b[r2][c2]); swap(p[id1], p[id2]);
        int id3 = b[r3][c3], id4 = b[r4][c4];
        swap(b[r3][c3], b[r4][c4]); swap(p[id3], p[id4]);

        next_s.score = -1;
        evaluate(next_s);

        if (next_s.score < current_s.score || uniform_real_distribution<double>(0.0, 1.0)(rng) < exp(-(double)(next_s.score - current_s.score) / temp)) {
            current_s = next_s;
        }

        if (current_s.score < best_s.score) {
            best_s = current_s;
        }
    }
    
    string path = a_star(initial_state, best_s);
    cout << path << endl;

    return 0;
}