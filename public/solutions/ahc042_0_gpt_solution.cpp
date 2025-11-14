#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    if (!(cin >> N)) return 0;
    vector<string> C(N);
    for (int i = 0; i < N; ++i) cin >> C[i];

    // Record positions
    vector<int> firstOCol(N, N), lastOCol(N, -1);
    vector<int> firstORow(N, N), lastORow(N, -1);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (C[i][j] == 'o') {
                firstOCol[j] = min(firstOCol[j], i);
                lastOCol[j] = max(lastOCol[j], i);
                firstORow[i] = min(firstORow[i], j);
                lastORow[i] = max(lastORow[i], j);
            }
        }
    }

    vector<vector<char>> x0(N, vector<char>(N, 0));
    int cntX = 0;
    for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) if (C[i][j] == 'x') { x0[i][j] = 1; cntX++; }

    auto add_ops = [&](vector<pair<char,int>>& ops, char d, int p, int times){
        for (int t = 0; t < times; ++t) ops.emplace_back(d, p);
    };

    // Try bundled strategy
    vector<vector<char>> x = x0; // working copy
    vector<pair<char,int>> moves;

    // Column-wise up-safe
    for (int j = 0; j < N; ++j) {
        int bound = firstOCol[j]; // rows < bound are up-safe
        int maxi = -1;
        for (int i = 0; i < bound; ++i) if (x[i][j]) maxi = i;
        if (maxi >= 0) {
            int s = maxi + 1;
            add_ops(moves, 'U', j, s);
            add_ops(moves, 'D', j, s);
            for (int i = 0; i < s; ++i) x[i][j] = 0;
        }
    }
    // Column-wise down-safe
    for (int j = 0; j < N; ++j) {
        int bound = lastOCol[j]; // rows > bound are down-safe
        int mini = N;
        for (int i = bound + 1; i < N; ++i) if (x[i][j]) { mini = i; break; }
        if (mini < N) {
            int s = N - mini;
            add_ops(moves, 'D', j, s);
            add_ops(moves, 'U', j, s);
            for (int i = mini; i < N; ++i) x[i][j] = 0;
        }
    }
    // Row-wise left-safe
    for (int i = 0; i < N; ++i) {
        int bound = firstORow[i]; // cols < bound are left-safe
        int maxj = -1;
        for (int j = 0; j < bound; ++j) if (x[i][j]) maxj = j;
        if (maxj >= 0) {
            int s = maxj + 1;
            add_ops(moves, 'L', i, s);
            add_ops(moves, 'R', i, s);
            for (int j = 0; j < s; ++j) x[i][j] = 0;
        }
    }
    // Row-wise right-safe
    for (int i = 0; i < N; ++i) {
        int bound = lastORow[i]; // cols > bound are right-safe
        int minj = N;
        for (int j = bound + 1; j < N; ++j) if (x[i][j]) { minj = j; break; }
        if (minj < N) {
            int s = N - minj;
            add_ops(moves, 'R', i, s);
            add_ops(moves, 'L', i, s);
            for (int j = minj; j < N; ++j) x[i][j] = 0;
        }
    }
    // Fallback for any remaining x
    auto opposite = [](char d)->char{
        if (d == 'U') return 'D';
        if (d == 'D') return 'U';
        if (d == 'L') return 'R';
        return 'L';
    };
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) if (x[i][j]) {
            vector<pair<int,char>> cand;
            if (firstOCol[j] >= i) cand.emplace_back(i+1, 'U');
            if (lastOCol[j] <= i)  cand.emplace_back(N - i, 'D');
            if (firstORow[i] >= j) cand.emplace_back(j+1, 'L');
            if (lastORow[i] <= j)  cand.emplace_back(N - j, 'R');
            // choose minimal
            pair<int,char> best = cand[0];
            for (auto &p : cand) if (p.first < best.first) best = p;
            add_ops(moves, best.second, (best.second=='U'||best.second=='D') ? j : i, best.first);
            add_ops(moves, opposite(best.second), (best.second=='U'||best.second=='D') ? j : i, best.first);
            x[i][j] = 0;
        }
    }

    // If moves exceed limit (shouldn't, but just in case), fall back to per-oni minimal plan
    if ((int)moves.size() > 4*N*N) {
        moves.clear();
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) if (x0[i][j]) {
                vector<pair<int,char>> cand;
                if (firstOCol[j] >= i) cand.emplace_back(i+1, 'U');
                if (lastOCol[j] <= i)  cand.emplace_back(N - i, 'D');
                if (firstORow[i] >= j) cand.emplace_back(j+1, 'L');
                if (lastORow[i] <= j)  cand.emplace_back(N - j, 'R');
                pair<int,char> best = cand[0];
                for (auto &p : cand) if (p.first < best.first) best = p;
                add_ops(moves, best.second, (best.second=='U'||best.second=='D') ? j : i, best.first);
                add_ops(moves, opposite(best.second), (best.second=='U'||best.second=='D') ? j : i, best.first);
            }
        }
        if ((int)moves.size() > 4*N*N) {
            // As a last resort, truncate (though theoretically unnecessary)
            moves.resize(4*N*N);
        }
    }

    // Output
    for (auto &op : moves) {
        cout << op.first << ' ' << op.second << '\n';
    }
    return 0;
}