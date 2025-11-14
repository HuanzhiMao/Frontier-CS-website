#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1LL << 60;
const int NN = 15;

int main() {
    int N, M;
    cin >> N >> M;
    int si, sj;
    cin >> si >> sj;
    vector<string> A(N);
    for (int i = 0; i < N; i++) {
        cin >> A[i];
    }
    vector<string> strs(M);
    for (int i = 0; i < M; i++) {
        cin >> strs[i];
    }
    vector<vector<pair<int, int>>> pos(26);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            pos[A[i][j] - 'A'].push_back({i, j});
        }
    }
    auto compute_overlap = [](const string& a, const string& b) -> int {
        int minlen = min((int)a.size(), (int)b.size());
        for (int kk = minlen; kk >= 1; --kk) {
            if (a.substr(a.size() - kk, kk) == b.substr(0, kk)) {
                return kk;
            }
        }
        return 0;
    };
    while (strs.size() > 1) {
        int max_o = -1;
        int best_i = -1, best_j = -1;
        for (size_t i = 0; i < strs.size(); i++) {
            for (size_t jj = 0; jj < strs.size(); jj++) {
                if (i == jj) continue;
                int o = compute_overlap(strs[i], strs[jj]);
                if (o > max_o) {
                    max_o = o;
                    best_i = i;
                    best_j = jj;
                }
            }
        }
        string aa = strs[best_i], bb = strs[best_j];
        string new_s = aa + bb.substr(max_o);
        vector<string> new_strs;
        for (size_t k = 0; k < strs.size(); k++) {
            if (k != (size_t)best_i && k != (size_t)best_j) new_strs.push_back(strs[k]);
        }
        new_strs.push_back(new_s);
        strs = new_strs;
    }
    string SS = strs[0];
    int Len = SS.size();
    vector<vector<vector<ll>>> dpp(Len + 1, vector<vector<ll>>(NN, vector<ll>(NN, INF)));
    vector<vector<vector<pair<int, int>>>> pree(Len + 1, vector<vector<pair<int, int>>>(NN, vector<pair<int, int>>(NN, {-1, -1})));
    char need0 = SS[0];
    for (auto& p : pos[need0 - 'A']) {
        int ni = p.first, nj = p.second;
        ll cost = abs(ni - si) + abs(nj - sj);
        dpp[1][ni][nj] = cost;
        pree[1][ni][nj] = {si, sj};
    }
    for (int kk = 2; kk <= Len; kk++) {
        char need = SS[kk - 1];
        for (int pi = 0; pi < NN; pi++) {
            for (int pj = 0; pj < NN; pj++) {
                if (dpp[kk - 1][pi][pj] == INF) continue;
                for (auto& pp : pos[need - 'A']) {
                    int ni = pp.first, nj = pp.second;
                    ll dc = dpp[kk - 1][pi][pj] + abs(ni - pi) + abs(nj - pj);
                    if (dc < dpp[kk][ni][nj]) {
                        dpp[kk][ni][nj] = dc;
                        pree[kk][ni][nj] = {pi, pj};
                    }
                }
            }
        }
    }
    ll min_move = INF;
    int best_i = -1, best_j = -1;
    for (int i = 0; i < NN; i++) {
        for (int j = 0; j < NN; j++) {
            if (dpp[Len][i][j] < min_move) {
                min_move = dpp[Len][i][j];
                best_i = i;
                best_j = j;
            }
        }
    }
    vector<pair<int, int>> sequence;
    pair<int, int> curr = {best_i, best_j};
    int current_k = Len;
    while (current_k >= 1) {
        sequence.push_back(curr);
        curr = pree[current_k][curr.first][curr.second];
        current_k--;
    }
    reverse(sequence.begin(), sequence.end());
    for (auto& p : sequence) {
        cout << p.first << " " << p.second << endl;
    }
    return 0;
}