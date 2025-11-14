#include <bits/stdc++.h>
using namespace std;

struct Operation {
    int x1, y1, x2, y2, x3, y3, x4, y4;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<vector<char>> dot(N, vector<char>(N, 0));
    for (int i = 0; i < M; i++) {
        int x, y;
        cin >> x >> y;
        if (0 <= x && x < N && 0 <= y && y < N) dot[x][y] = 1;
    }

    auto weight = [&](int x, int y) -> long long {
        long long c = (N - 1) / 2;
        long long dx = x - c, dy = y - c;
        return dx * dx + dy * dy + 1;
    };

    vector<pair<long long, Operation>> cand[2];
    for (int x = 0; x < N - 1; x++) {
        for (int y = 0; y < N - 1; y++) {
            bool c[4];
            c[0] = dot[x][y];
            c[1] = dot[x + 1][y];
            c[2] = dot[x + 1][y + 1];
            c[3] = dot[x][y + 1];
            int cnt = c[0] + c[1] + c[2] + c[3];
            if (cnt == 3) {
                int miss = -1;
                for (int i = 0; i < 4; i++) if (!c[i]) { miss = i; break; }
                int px[4] = {x, x + 1, x + 1, x};
                int py[4] = {y, y, y + 1, y + 1};
                Operation op;
                op.x1 = px[miss]; op.y1 = py[miss];
                op.x2 = px[(miss + 1) & 3]; op.y2 = py[(miss + 1) & 3];
                op.x3 = px[(miss + 2) & 3]; op.y3 = py[(miss + 2) & 3];
                op.x4 = px[(miss + 3) & 3]; op.y4 = py[(miss + 3) & 3];
                int parity = (x + y) & 1;
                cand[parity].emplace_back(weight(op.x1, op.y1), op);
            }
        }
    }

    // Choose parity with more potential (by unique p1 count)
    auto count_unique = [&](vector<pair<long long, Operation>>& v) -> int {
        vector<vector<char>> used(N, vector<char>(N, 0));
        int cnt = 0;
        for (auto &pr : v) {
            auto &op = pr.second;
            if (!used[op.x1][op.y1]) {
                used[op.x1][op.y1] = 1;
                cnt++;
            }
        }
        return cnt;
    };

    int p0u = count_unique(cand[0]);
    int p1u = count_unique(cand[1]);
    int useParity = (p1u > p0u) ? 1 : 0;

    auto &chosenCand = cand[useParity];
    sort(chosenCand.begin(), chosenCand.end(), [&](const auto& a, const auto& b){
        return a.first > b.first;
    });

    vector<vector<char>> usedP1(N, vector<char>(N, 0));
    vector<Operation> ops;

    for (auto &pr : chosenCand) {
        auto &op = pr.second;
        if (usedP1[op.x1][op.y1]) continue;
        // Ensure that the perimeter does not share edge with previously selected unit squares:
        // Selecting only one cell parity guarantees no shared edges.
        // Ensure unique p1.
        usedP1[op.x1][op.y1] = 1;
        ops.push_back(op);
    }

    cout << ops.size() << "\n";
    for (auto &op : ops) {
        cout << op.x1 << " " << op.y1 << " "
             << op.x2 << " " << op.y2 << " "
             << op.x3 << " " << op.y3 << " "
             << op.x4 << " " << op.y4 << "\n";
    }
    return 0;
}