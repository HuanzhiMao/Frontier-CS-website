#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;

const int INF = 1e9;
int N;
int si, sj;
vector<string> C;
vector<vector<int>> cost;
vector<vector<bool>> is_road;

int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

struct State {
    int time;
    int r, c;

    bool operator>(const State& other) const {
        return time > other.time;
    }
};

void dijkstra(int start_r, int start_c, vector<vector<int>>& dist, vector<vector<pair<int, int>>>& parent) {
    dist.assign(N, vector<int>(N, INF));
    parent.assign(N, vector<pair<int, int>>(N, {-1, -1}));
    dist[start_r][start_c] = 0;

    priority_queue<State, vector<State>, greater<State>> pq;
    pq.push({0, start_r, start_c});

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        int r = current.r;
        int c = current.c;
        int time = current.time;

        if (time > dist[r][c]) {
            continue;
        }

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr >= 0 && nr < N && nc >= 0 && nc < N && is_road[nr][nc]) {
                if (dist[r][c] + cost[nr][nc] < dist[nr][nc]) {
                    dist[nr][nc] = dist[r][c] + cost[nr][nc];
                    parent[nr][nc] = {r, c};
                    pq.push({dist[nr][nc], nr, nc});
                }
            }
        }
    }
}

string get_path_str(int r1, int c1, int r2, int c2, const vector<vector<pair<int, int>>>& parent) {
    if (r1 == r2 && c1 == c2) return "";
    string path_rev = "";
    int cr = r2, cc = c2;
    while (cr != r1 || cc != c1) {
        pair<int, int> p = parent[cr][cc];
        int pr = p.first;
        int pc = p.second;
        if (pr == cr - 1) path_rev += 'D';
        else if (pr == cr + 1) path_rev += 'U';
        else if (pc == cc - 1) path_rev += 'R';
        else path_rev += 'L';
        cr = pr;
        cc = pc;
    }
    reverse(path_rev.begin(), path_rev.end());
    return path_rev;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> si >> sj;
    C.resize(N);
    cost.assign(N, vector<int>(N, 0));
    is_road.assign(N, vector<bool>(N, false));
    
    for (int i = 0; i < N; ++i) {
        cin >> C[i];
        for (int j = 0; j < N; ++j) {
            if (C[i][j] != '#') {
                is_road[i][j] = true;
                cost[i][j] = C[i][j] - '0';
            }
        }
    }

    vector<vector<int>> horiz_seg_id(N, vector<int>(N, -1));
    vector<vector<int>> vert_seg_id(N, vector<int>(N, -1));
    vector<vector<pair<int, int>>> h_seg_squares;
    vector<vector<pair<int, int>>> v_seg_squares;
    int h_seg_count = 0, v_seg_count = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (is_road[i][j] && (j == 0 || !is_road[i][j - 1])) {
                h_seg_squares.emplace_back();
                int k = j;
                while (k < N && is_road[i][k]) {
                    horiz_seg_id[i][k] = h_seg_count;
                    h_seg_squares[h_seg_count].push_back({i, k});
                    k++;
                }
                h_seg_count++;
            }
        }
    }
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < N; ++i) {
            if (is_road[i][j] && (i == 0 || !is_road[i - 1][j])) {
                v_seg_squares.emplace_back();
                int k = i;
                while (k < N && is_road[k][j]) {
                    vert_seg_id[k][j] = v_seg_count;
                    v_seg_squares[v_seg_count].push_back({k, j});
                    k++;
                }
                v_seg_count++;
            }
        }
    }

    string final_path = "";
    int current_r = si, current_c = sj;

    vector<bool> h_covered(h_seg_count, false);
    vector<bool> v_covered(v_seg_count, false);
    int num_h_covered = 0, num_v_covered = 0;

    if (horiz_seg_id[si][sj] != -1 && !h_covered[horiz_seg_id[si][sj]]) {
        h_covered[horiz_seg_id[si][sj]] = true;
        num_h_covered++;
    }
    if (vert_seg_id[si][sj] != -1 && !v_covered[vert_seg_id[si][sj]]) {
        v_covered[vert_seg_id[si][sj]] = true;
        num_v_covered++;
    }

    while (num_h_covered < h_seg_count || num_v_covered < v_seg_count) {
        vector<vector<int>> dist;
        vector<vector<pair<int, int>>> parent;
        dijkstra(current_r, current_c, dist, parent);

        int best_r = -1, best_c = -1;
        double max_score = -1.0;

        map<pair<int, int>, int> p_new_segs;
        for (int i = 0; i < h_seg_count; ++i) {
            if (!h_covered[i]) {
                for (const auto& p : h_seg_squares[i]) {
                    p_new_segs[p]++;
                }
            }
        }
        for (int i = 0; i < v_seg_count; ++i) {
            if (!v_covered[i]) {
                for (const auto& p : v_seg_squares[i]) {
                    p_new_segs[p]++;
                }
            }
        }

        for (auto const& [p, num_new] : p_new_segs) {
            int r = p.first;
            int c = p.second;
            if (dist[r][c] == INF || dist[r][c] == 0) continue;

            double score = pow(num_new, 2) / dist[r][c];
            if (score > max_score) {
                max_score = score;
                best_r = r;
                best_c = c;
            }
        }

        if (best_r == -1) {
            break;
        }

        final_path += get_path_str(current_r, current_c, best_r, best_c, parent);
        current_r = best_r;
        current_c = best_c;

        if (horiz_seg_id[current_r][current_c] != -1 && !h_covered[horiz_seg_id[current_r][current_c]]) {
            h_covered[horiz_seg_id[current_r][current_c]] = true;
            num_h_covered++;
        }
        if (vert_seg_id[current_r][current_c] != -1 && !v_covered[vert_seg_id[current_r][current_c]]) {
            v_covered[vert_seg_id[current_r][current_c]] = true;
            num_v_covered++;
        }
    }

    vector<vector<int>> dist;
    vector<vector<pair<int, int>>> parent;
    dijkstra(current_r, current_c, dist, parent);
    final_path += get_path_str(current_r, current_c, si, sj, parent);

    cout << final_path << endl;

    return 0;
}