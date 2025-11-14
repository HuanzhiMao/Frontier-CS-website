#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <random>
#include <set>

using namespace std;

// --- Timer ---
auto start_time = chrono::high_resolution_clock::now();
double TIME_LIMIT = 1.95; // seconds

bool is_time_up() {
    auto current_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = current_time - start_time;
    return elapsed.count() > TIME_LIMIT;
}

// --- Random Number Generator ---
mt19937 rng(0);

// --- Data Structures ---
struct Point {
    int r, c;
    bool operator<(const Point& other) const {
        if (r != other.r) return r < other.r;
        return c < other.c;
    }
    bool operator==(const Point& other) const {
        return r == other.r && c == other.c;
    }
};

struct State {
    Point p;
    int cost;
    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

// --- Global Variables & Constants ---
int N;
Point start_pos;
vector<string> grid;
vector<vector<int>> costs;
vector<Point> road_squares;
vector<vector<vector<Point>>> visible_from;
int total_road_squares = 0;

int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

// --- Helper Functions ---
bool is_valid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N && grid[r][c] != '#';
}

string get_path_str(Point from, Point to, const vector<vector<Point>>& parent) {
    if (from.r == to.r && from.c == to.c) return "";
    string path_str = "";
    Point curr = to;
    while (!(curr.r == from.r && curr.c == from.c)) {
        Point p = parent[curr.r][curr.c];
        if (p.r == -1) break;
        if (p.r == curr.r - 1) path_str += 'D';
        else if (p.r == curr.r + 1) path_str += 'U';
        else if (p.c == curr.c - 1) path_str += 'R';
        else path_str += 'L';
        curr = p;
    }
    reverse(path_str.begin(), path_str.end());
    return path_str;
}

pair<vector<vector<int>>, vector<vector<Point>>> dijkstra(Point start) {
    vector<vector<int>> dist(N, vector<int>(N, 1e9));
    vector<vector<Point>> parent(N, vector<Point>(N, {-1, -1}));
    priority_queue<State, vector<State>, greater<State>> pq;

    dist[start.r][start.c] = 0;
    pq.push({start, 0});

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        if (current.cost > dist[current.p.r][current.p.c]) {
            continue;
        }

        for (int i = 0; i < 4; ++i) {
            int nr = current.p.r + dr[i];
            int nc = current.p.c + dc[i];

            if (is_valid(nr, nc)) {
                int new_cost = current.cost + costs[nr][nc];
                if (new_cost < dist[nr][nc]) {
                    dist[nr][nc] = new_cost;
                    parent[nr][nc] = current.p;
                    pq.push({{nr, nc}, new_cost});
                }
            }
        }
    }
    return {dist, parent};
}

// --- Main Logic ---
void read_input() {
    cin >> N >> start_pos.r >> start_pos.c;
    grid.resize(N);
    costs.assign(N, vector<int>(N, 0));
    for (int i = 0; i < N; ++i) {
        cin >> grid[i];
        for (int j = 0; j < N; ++j) {
            if (grid[i][j] != '#') {
                costs[i][j] = grid[i][j] - '0';
                road_squares.push_back({i, j});
            }
        }
    }
    total_road_squares = road_squares.size();
}

void precompute_visibility() {
    visible_from.assign(N, vector<vector<Point>>(N));
    for (const auto& p : road_squares) {
        // Horizontal
        for (int c = p.c; c >= 0 && grid[p.r][c] != '#'; --c) visible_from[p.r][p.c].push_back({p.r, c});
        for (int c = p.c + 1; c < N && grid[p.r][c] != '#'; ++c) visible_from[p.r][p.c].push_back({p.r, c});
        // Vertical
        for (int r = p.r - 1; r >= 0 && grid[r][p.c] != '#'; --r) visible_from[p.r][p.c].push_back({r, p.c});
        for (int r = p.r + 1; r < N && grid[r][p.c] != '#'; ++r) visible_from[p.r][p.c].push_back({r, p.c});
    }
}

vector<Point> select_viewpoints() {
    vector<Point> viewpoints;
    vector<vector<bool>> is_covered(N, vector<bool>(N, false));
    int uncovered_count = total_road_squares;

    for (const auto& visible_p : visible_from[start_pos.r][start_pos.c]) {
        if (!is_covered[visible_p.r][visible_p.c]) {
            is_covered[visible_p.r][visible_p.c] = true;
            uncovered_count--;
        }
    }

    while (uncovered_count > 0) {
        if (is_time_up()) break;
        Point best_p = {-1, -1};
        int max_newly_covered = 0;

        for (const auto& p : road_squares) {
            int newly_covered = 0;
            for (const auto& visible_p : visible_from[p.r][p.c]) {
                if (!is_covered[visible_p.r][visible_p.c]) {
                    newly_covered++;
                }
            }
            if (newly_covered > max_newly_covered) {
                max_newly_covered = newly_covered;
                best_p = p;
            }
        }

        if (max_newly_covered == 0) break;

        viewpoints.push_back(best_p);
        for (const auto& visible_p : visible_from[best_p.r][best_p.c]) {
            if (!is_covered[visible_p.r][visible_p.c]) {
                is_covered[visible_p.r][visible_p.c] = true;
                uncovered_count--;
            }
        }
    }
    
    if (uncovered_count > 0) {
        for (const auto& p : road_squares) {
            if (!is_covered[p.r][p.c]) {
                viewpoints.push_back(p);
                 for (const auto& visible_p : visible_from[p.r][p.c]) {
                    if (!is_covered[visible_p.r][visible_p.c]) {
                        is_covered[visible_p.r][visible_p.c] = true;
                        uncovered_count--;
                    }
                }
            }
        }
    }
    return viewpoints;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read_input();
    precompute_visibility();

    vector<Point> initial_viewpoints = select_viewpoints();
    
    set<Point> tsp_nodes_set;
    tsp_nodes_set.insert(start_pos);
    for(const auto& p : initial_viewpoints) {
        tsp_nodes_set.insert(p);
    }
    vector<Point> tsp_nodes(tsp_nodes_set.begin(), tsp_nodes_set.end());
    int k = tsp_nodes.size();
    int start_node_idx = -1;
    for(int i=0; i<k; ++i) {
        if(tsp_nodes[i] == start_pos) {
            start_node_idx = i;
            break;
        }
    }

    vector<vector<int>> dist_matrix(k, vector<int>(k));
    vector<vector<vector<Point>>> path_parents(k);

    for (int i = 0; i < k; ++i) {
        auto [dist_from_i, parent_from_i] = dijkstra(tsp_nodes[i]);
        path_parents[i] = parent_from_i;
        for (int j = 0; j < k; ++j) {
            dist_matrix[i][j] = dist_from_i[tsp_nodes[j].r][tsp_nodes[j].c];
        }
    }

    vector<int> current_tour;
    for(int i=0; i<k; ++i) {
        if(i != start_node_idx) current_tour.push_back(i);
    }
    shuffle(current_tour.begin(), current_tour.end(), rng);

    auto calculate_cost = [&](const vector<int>& tour) {
        long long current_cost = 0;
        if (tour.empty()) return (long long)0;
        current_cost += dist_matrix[start_node_idx][tour[0]];
        for (size_t i = 0; i < tour.size() - 1; ++i) {
            current_cost += dist_matrix[tour[i]][tour[i+1]];
        }
        current_cost += dist_matrix[tour.back()][start_node_idx];
        return current_cost;
    };
    
    long long best_cost = calculate_cost(current_tour);
    vector<int> best_tour = current_tour;

    double T = 5000;
    double T_end = 1e-2;
    double alpha = 0.99998;
    
    while (!is_time_up() && T > T_end) {
        if (current_tour.size() < 2) break;

        int i = uniform_int_distribution<int>(0, current_tour.size() - 1)(rng);
        int j = uniform_int_distribution<int>(0, current_tour.size() - 1)(rng);
        if (i == j) continue;
        if (i > j) swap(i, j);

        vector<int> next_tour = current_tour;
        reverse(next_tour.begin() + i, next_tour.begin() + j + 1);

        long long current_c = calculate_cost(current_tour);
        long long next_c = calculate_cost(next_tour);

        if (next_c < current_c) {
            current_tour = next_tour;
            if (next_c < best_cost) {
                best_cost = next_c;
                best_tour = next_tour;
            }
        } else {
            double prob = exp((double)(current_c - next_c) / T);
            if (uniform_real_distribution<double>(0.0, 1.0)(rng) < prob) {
                current_tour = next_tour;
            }
        }
        T *= alpha;
    }

    string final_path = "";
    int current_node_idx = start_node_idx;
    for (int next_node_idx : best_tour) {
        final_path += get_path_str(tsp_nodes[current_node_idx], tsp_nodes[next_node_idx], path_parents[current_node_idx]);
        current_node_idx = next_node_idx;
    }
    if(k > 1) {
        final_path += get_path_str(tsp_nodes[current_node_idx], tsp_nodes[start_node_idx], path_parents[current_node_idx]);
    }

    cout << final_path << endl;

    return 0;
}