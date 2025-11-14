#include <bits/stdc++.h>
using namespace std;

const int N = 50;
const int BEAM_WIDTH = 1000;

struct Dir {
    char ch;
    int di, dj;
};

Dir dirs[4] = {{'U', -1, 0}, {'D', 1, 0}, {'L', 0, -1}, {'R', 0, 1}};

#define IDX(i, j) ((i)*N + (j))

struct State {
    int i, j;
    int score;
    int prev;
    char move;
    bitset<2500> visited;
};

struct Temp {
    int from_idx;
    int d;
    int ni, nj;
    int new_score;
};

int main() {
    int si, sj;
    cin >> si >> sj;
    vector<vector<int>> T(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> T[i][j];
        }
    }
    vector<vector<int>> P(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> P[i][j];
        }
    }
    int max_t = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            max_t = max(max_t, T[i][j]);
        }
    }
    vector<vector<pair<int, int>>> ts(max_t + 1);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            ts[T[i][j]].push_back({i, j});
        }
    }
    vector<vector<pair<int, int>>> conflict(N, vector<pair<int, int>>(N, {-1, -1}));
    for (int m = 0; m <= max_t; m++) {
        if (ts[m].size() == 2) {
            auto [i1, j1] = ts[m][0];
            auto [i2, j2] = ts[m][1];
            conflict[i1][j1] = {i2, j2};
            conflict[i2][j2] = {i1, j1};
        }
    }
    vector<State> states;
    bitset<2500> init_vis;
    int start_k = IDX(si, sj);
    init_vis.set(start_k);
    states.push_back({si, sj, P[si][sj], -1, '\0', init_vis});
    vector<int> current_beam = {0};
    int best_score = P[si][sj];
    int best_state = 0;
    for (int step = 0; step < 2500; step++) {
        vector<Temp> candidates;
        for (auto idx : current_beam) {
            State& s = states[idx];
            for (int d = 0; d < 4; d++) {
                int ni = s.i + dirs[d].di;
                int nj = s.j + dirs[d].dj;
                if (ni < 0 || ni >= N || nj < 0 || nj >= N) continue;
                int nk = IDX(ni, nj);
                if (s.visited[nk]) continue;
                auto [oi, oj] = conflict[ni][nj];
                bool can = true;
                if (oi != -1) {
                    int ok = IDX(oi, oj);
                    if (s.visited[ok]) can = false;
                }
                if (!can) continue;
                int new_sc = s.score + P[ni][nj];
                candidates.push_back({idx, d, ni, nj, new_sc});
            }
        }
        sort(candidates.begin(), candidates.end(), [](const Temp& a, const Temp& b) {
            return a.new_score > b.new_score;
        });
        vector<int> new_beam;
        for (int k = 0; k < candidates.size() && new_beam.size() < BEAM_WIDTH; k++) {
            Temp& tp = candidates[k];
            bitset<2500> new_vis = states[tp.from_idx].visited;
            int nk = IDX(tp.ni, tp.nj);
            new_vis.set(nk);
            int new_idx = states.size();
            states.push_back({tp.ni, tp.nj, tp.new_score, tp.from_idx, dirs[tp.d].ch, new_vis});
            new_beam.push_back(new_idx);
            if (tp.new_score > best_score) {
                best_score = tp.new_score;
                best_state = new_idx;
            }
        }
        current_beam = new_beam;
        if (current_beam.empty()) break;
    }
    vector<char> moves;
    int cur = best_state;
    while (states[cur].prev != -1) {
        moves.push_back(states[cur].move);
        cur = states[cur].prev;
    }
    reverse(moves.begin(), moves.end());
    string path(moves.begin(), moves.end());
    cout << path << endl;
    return 0;
}