#include <bits/stdc++.h>
using namespace std;

struct Vehicle {
    bool horizontal;
    int fixed; // row for horizontal, col for vertical
    int len;
};

struct MoveRec {
    int vid;
    char dir;
};

using u64 = unsigned long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // Read input
    vector<vector<int>> boardInput(6, vector<int>(6));
    for (int r = 0; r < 6; ++r) {
        for (int c = 0; c < 6; ++c) {
            cin >> boardInput[r][c];
        }
    }
    // Determine number of vehicles
    int n = 0;
    for (int r = 0; r < 6; ++r) {
        for (int c = 0; c < 6; ++c) {
            n = max(n, boardInput[r][c]);
        }
    }
    if (n <= 0) {
        // No vehicles: trivial
        cout << 0 << " " << 0 << "\n";
        return 0;
    }
    vector<vector<pair<int,int>>> cells(n+1);
    for (int r = 0; r < 6; ++r) {
        for (int c = 0; c < 6; ++c) {
            int id = boardInput[r][c];
            if (id >= 1) cells[id].push_back({r,c});
        }
    }
    vector<Vehicle> veh(n+1);
    vector<int> initPos(n+1, 0);
    for (int id = 1; id <= n; ++id) {
        auto &v = cells[id];
        int rmin = 10, rmax = -10, cmin = 10, cmax = -10;
        for (auto &p : v) {
            rmin = min(rmin, p.first);
            rmax = max(rmax, p.first);
            cmin = min(cmin, p.second);
            cmax = max(cmax, p.second);
        }
        Vehicle vi;
        if (rmin == rmax) {
            vi.horizontal = true;
            vi.fixed = rmin;
            vi.len = (int)v.size();
            initPos[id] = cmin;
        } else {
            vi.horizontal = false;
            vi.fixed = cmin;
            vi.len = (int)v.size();
            initPos[id] = rmin;
        }
        veh[id] = vi;
    }
    // Pack/unpack helpers
    vector<int> shifts(n+1, 0);
    for (int i = 1; i <= n; ++i) shifts[i] = 3*(i-1);
    auto pack = [&](const vector<int>& pos)->u64 {
        u64 key = 0;
        for (int i = 1; i <= n; ++i) {
            key |= (u64(pos[i]) << shifts[i]);
        }
        return key;
    };
    auto unpack = [&](u64 key, int *pos) {
        for (int i = 1; i <= n; ++i) {
            pos[i] = int((key >> shifts[i]) & 7ULL);
        }
    };
    auto buildBoard = [&](int *pos, unsigned char board[6][6]) {
        memset(board, 0, 36);
        for (int i = 1; i <= n; ++i) {
            auto &vi = veh[i];
            if (vi.horizontal) {
                int r = vi.fixed;
                int c = pos[i];
                for (int k = 0; k < vi.len; ++k) board[r][c+k] = (unsigned char)i;
            } else {
                int c = vi.fixed;
                int r = pos[i];
                for (int k = 0; k < vi.len; ++k) board[r+k][c] = (unsigned char)i;
            }
        }
    };
    auto exitReady = [&](int *pos, unsigned char board[6][6])->bool {
        // Red car id 1, horizontal, row 2 (0-based)
        int L = pos[1];
        int R = L + veh[1].len - 1;
        int r = veh[1].fixed;
        for (int c = R + 1; c < 6; ++c) {
            if (board[r][c] != 0) return false;
        }
        return true;
    };
    // Time management
    auto startTime = chrono::steady_clock::now();
    auto elapsedMs = [&](){
        return (int)chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - startTime).count();
    };
    const int TOTAL_TIME_MS = 1900;
    const int BFS_TIME_MS = 1000; // time budget for first BFS
    const size_t MAX_STATES = 300000; // cap

    // BFS from initial
    u64 initKey = pack(initPos);
    unordered_map<u64,int> idMap;
    idMap.reserve(1<<20);
    vector<u64> states;
    states.reserve(200000);
    vector<int> parent;
    parent.reserve(200000);
    vector<MoveRec> moveRec;
    moveRec.reserve(200000);
    vector<int> distStart;
    distStart.reserve(200000);

    idMap[initKey] = 0;
    states.push_back(initKey);
    parent.push_back(-1);
    moveRec.push_back({0, 'L'});
    distStart.push_back(0);

    vector<int> q;
    q.reserve(200000);
    size_t qhead = 0;
    q.push_back(0);

    int posArrCap = n+1; // for unpack
    int *posArr = (int*)alloca((n+1)*sizeof(int));
    unsigned char board[6][6];

    while (qhead < q.size()) {
        if ((int)elapsedMs() > BFS_TIME_MS || states.size() >= MAX_STATES) break;
        int uidx = q[qhead++];
        u64 key = states[uidx];
        unpack(key, posArr);
        buildBoard(posArr, board);
        // neighbors: move any vehicle by 1 step if possible
        for (int i = 1; i <= n; ++i) {
            auto &vi = veh[i];
            int shift = shifts[i];
            int p = posArr[i];
            if (vi.horizontal) {
                int r = vi.fixed;
                // left
                if (p > 0 && board[r][p-1] == 0) {
                    u64 nkey = key - (1ULL << shift);
                    auto it = idMap.find(nkey);
                    if (it == idMap.end()) {
                        int idx = (int)states.size();
                        idMap.emplace(nkey, idx);
                        states.push_back(nkey);
                        parent.push_back(uidx);
                        moveRec.push_back({i, 'L'});
                        distStart.push_back(distStart[uidx] + 1);
                        q.push_back(idx);
                    }
                }
                // right
                if (p + vi.len < 6 && board[r][p + vi.len] == 0) {
                    u64 nkey = key + (1ULL << shift);
                    auto it = idMap.find(nkey);
                    if (it == idMap.end()) {
                        int idx = (int)states.size();
                        idMap.emplace(nkey, idx);
                        states.push_back(nkey);
                        parent.push_back(uidx);
                        moveRec.push_back({i, 'R'});
                        distStart.push_back(distStart[uidx] + 1);
                        q.push_back(idx);
                    }
                }
            } else {
                int c = vi.fixed;
                // up
                if (p > 0 && board[p-1][c] == 0) {
                    u64 nkey = key - (1ULL << shift);
                    auto it = idMap.find(nkey);
                    if (it == idMap.end()) {
                        int idx = (int)states.size();
                        idMap.emplace(nkey, idx);
                        states.push_back(nkey);
                        parent.push_back(uidx);
                        moveRec.push_back({i, 'U'});
                        distStart.push_back(distStart[uidx] + 1);
                        q.push_back(idx);
                    }
                }
                // down
                if (p + vi.len < 6 && board[p + vi.len][c] == 0) {
                    u64 nkey = key + (1ULL << shift);
                    auto it = idMap.find(nkey);
                    if (it == idMap.end()) {
                        int idx = (int)states.size();
                        idMap.emplace(nkey, idx);
                        states.push_back(nkey);
                        parent.push_back(uidx);
                        moveRec.push_back({i, 'D'});
                        distStart.push_back(distStart[uidx] + 1);
                        q.push_back(idx);
                    }
                }
            }
        }
    }

    int V = (int)states.size();

    // Compute approximate distances to goal using Dijkstra restricted to visited states
    vector<int> approxGoal(V, INT_MAX/4);

    // Multi-source initial states: exit-ready with base cost (6 - L)
    struct Node { int d; int idx; };
    struct Cmp { bool operator()(const Node& a, const Node& b) const { return a.d > b.d; } };
    priority_queue<Node, vector<Node>, Cmp> pq;

    // Scan all visited states for exit-ready
    for (int idx = 0; idx < V; ++idx) {
        u64 key = states[idx];
        unpack(key, posArr);
        buildBoard(posArr, board);
        if (exitReady(posArr, board)) {
            int L = posArr[1];
            int base = 6 - L;
            if (base < approxGoal[idx]) {
                approxGoal[idx] = base;
                pq.push({base, idx});
            }
        }
    }

    // Dijkstra
    while (!pq.empty()) {
        if ((int)elapsedMs() > TOTAL_TIME_MS - 200) break; // leave time to print
        auto cur = pq.top(); pq.pop();
        int uidx = cur.idx;
        int du = cur.d;
        if (du != approxGoal[uidx]) continue;
        u64 key = states[uidx];
        unpack(key, posArr);
        buildBoard(posArr, board);
        for (int i = 1; i <= n; ++i) {
            auto &vi = veh[i];
            int shift = shifts[i];
            int p = posArr[i];
            if (vi.horizontal) {
                int r = vi.fixed;
                // left
                if (p > 0 && board[r][p-1] == 0) {
                    u64 nkey = key - (1ULL << shift);
                    auto it = idMap.find(nkey);
                    if (it != idMap.end()) {
                        int vidx = it->second;
                        if (du + 1 < approxGoal[vidx]) {
                            approxGoal[vidx] = du + 1;
                            pq.push({du + 1, vidx});
                        }
                    }
                }
                // right
                if (p + vi.len < 6 && board[r][p + vi.len] == 0) {
                    u64 nkey = key + (1ULL << shift);
                    auto it = idMap.find(nkey);
                    if (it != idMap.end()) {
                        int vidx = it->second;
                        if (du + 1 < approxGoal[vidx]) {
                            approxGoal[vidx] = du + 1;
                            pq.push({du + 1, vidx});
                        }
                    }
                }
            } else {
                int c = vi.fixed;
                // up
                if (p > 0 && board[p-1][c] == 0) {
                    u64 nkey = key - (1ULL << shift);
                    auto it = idMap.find(nkey);
                    if (it != idMap.end()) {
                        int vidx = it->second;
                        if (du + 1 < approxGoal[vidx]) {
                            approxGoal[vidx] = du + 1;
                            pq.push({du + 1, vidx});
                        }
                    }
                }
                // down
                if (p + vi.len < 6 && board[p + vi.len][c] == 0) {
                    u64 nkey = key + (1ULL << shift);
                    auto it = idMap.find(nkey);
                    if (it != idMap.end()) {
                        int vidx = it->second;
                        if (du + 1 < approxGoal[vidx]) {
                            approxGoal[vidx] = du + 1;
                            pq.push({du + 1, vidx});
                        }
                    }
                }
            }
        }
    }

    // Select best state
    int bestIdx = 0;
    int bestGoal = -1;
    for (int i = 0; i < V; ++i) {
        if (approxGoal[i] < INT_MAX/8) {
            if (approxGoal[i] > bestGoal) {
                bestGoal = approxGoal[i];
                bestIdx = i;
            }
        }
    }
    if (bestGoal < 0) {
        // Fallback to initial
        bestIdx = 0;
        // compute trivial goal from initial (if exit-ready)
        unpack(states[0], posArr);
        buildBoard(posArr, board);
        if (exitReady(posArr, board)) bestGoal = 6 - posArr[1];
        else bestGoal = 0;
    }

    // Reconstruct path to bestIdx
    vector<MoveRec> moves;
    int cur = bestIdx;
    while (parent[cur] != -1) {
        moves.push_back(moveRec[cur]);
        cur = parent[cur];
    }
    reverse(moves.begin(), moves.end());

    // Output
    cout << bestGoal << " " << moves.size() << "\n";
    for (auto &m : moves) {
        cout << m.vid << " " << m.dir << "\n";
    }
    return 0;
}