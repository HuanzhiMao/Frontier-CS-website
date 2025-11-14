#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    vector<string> C(N);
    for (int i = 0; i < N; ++i) cin >> C[i];
    
    vector<vector<bool>> isFuku(N, vector<bool>(N, false));
    vector<pair<int,int>> oni;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (C[i][j] == 'o') isFuku[i][j] = true;
            if (C[i][j] == 'x') oni.emplace_back(i, j);
        }
    }
    
    vector<pair<char,int>> ops;
    ops.reserve(4 * N * N);
    
    auto hasFukuUp = [&](int i, int j) {
        for (int r = 0; r < i; ++r) if (isFuku[r][j]) return true;
        return false;
    };
    auto hasFukuDown = [&](int i, int j) {
        for (int r = i + 1; r < N; ++r) if (isFuku[r][j]) return true;
        return false;
    };
    auto hasFukuLeft = [&](int i, int j) {
        for (int c = 0; c < j; ++c) if (isFuku[i][c]) return true;
        return false;
    };
    auto hasFukuRight = [&](int i, int j) {
        for (int c = j + 1; c < N; ++c) if (isFuku[i][c]) return true;
        return false;
    };
    
    for (auto [i, j] : oni) {
        int bestK = INT_MAX;
        char dir = 'U'; // placeholder
        
        if (!hasFukuUp(i, j)) {
            int k = i + 1;
            if (k < bestK) { bestK = k; dir = 'U'; }
        }
        if (!hasFukuDown(i, j)) {
            int k = N - i;
            if (k < bestK) { bestK = k; dir = 'D'; }
        }
        if (!hasFukuLeft(i, j)) {
            int k = j + 1;
            if (k < bestK) { bestK = k; dir = 'L'; }
        }
        if (!hasFukuRight(i, j)) {
            int k = N - j;
            if (k < bestK) { bestK = k; dir = 'R'; }
        }
        
        // Apply the operations for this Oni
        if (dir == 'U') {
            for (int t = 0; t < bestK; ++t) ops.emplace_back('U', j);
            for (int t = 0; t < bestK; ++t) ops.emplace_back('D', j);
        } else if (dir == 'D') {
            for (int t = 0; t < bestK; ++t) ops.emplace_back('D', j);
            for (int t = 0; t < bestK; ++t) ops.emplace_back('U', j);
        } else if (dir == 'L') {
            for (int t = 0; t < bestK; ++t) ops.emplace_back('L', i);
            for (int t = 0; t < bestK; ++t) ops.emplace_back('R', i);
        } else { // 'R'
            for (int t = 0; t < bestK; ++t) ops.emplace_back('R', i);
            for (int t = 0; t < bestK; ++t) ops.emplace_back('L', i);
        }
    }
    
    // Ensure we do not exceed 4N^2 operations
    int limit = 4 * N * N;
    if ((int)ops.size() > limit) ops.resize(limit);
    
    for (auto &op : ops) {
        cout << op.first << ' ' << op.second << '\n';
    }
    return 0;
}