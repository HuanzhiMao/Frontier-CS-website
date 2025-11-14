#include <bits/stdc++.h>
using namespace std;

struct Vehicle {
    bool horiz;
    int len;
    int fixedCoord; // row for horiz (1-based), col for vert (1-based)
};

static const int H = 6, W = 6;
static const int MAXV = 16;

struct StateData {
    // positions are 1-based leftmost/topmost for each vehicle
    // store as uint8_t to save memory
    vector<uint8_t> pos;
};

uint64_t encode(const vector<uint8_t>& pos) {
    // pack up to 16 vehicles * 4 bits each = 64 bits
    uint64_t code = 0;
    for (size_t i = 0; i < pos.size(); ++i) {
        code |= (uint64_t)(pos[i] & 0xF) << (i * 4);
    }
    return code;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // Read board
    int board[H][W];
    int maxId = 0;
    for (int r = 0; r < H; ++r) {
        for (int c = 0; c < W; ++c) {
            int x;
            if (!(cin >> x)) return 0;
            board[r][c] = x;
            if (x > maxId) maxId = x;
        }
    }
    int n = maxId; // number of vehicles
    if (n <= 0) {
        // No vehicles; trivial
        cout << "0 0\n";
        return 0;
    }

    // Determine vehicles properties
    vector<Vehicle> veh(n);
    for (int id = 1; id <= n; ++id) {
        int rmin = 10, rmax = -1, cmin = 10, cmax = -1, cnt = 0;
        for (int r = 0; r < H; ++r) {
            for (int c = 0; c < W; ++c) {
                if (board[r][c] == id) {
                    rmin = min(rmin, r);
                    rmax = max(rmax, r);
                    cmin = min(cmin, c);
                    cmax = max(cmax, c);
                    cnt++;
                }
            }
        }
        Vehicle v{};
        if (rmin == rmax) {
            v.horiz = true;
            v.len = cmax - cmin + 1;
            v.fixedCoord = rmin + 1; // 1-based row
        } else {
            v.horiz = false;
            v.len = rmax - rmin + 1;
            v.fixedCoord = cmin + 1; // 1-based col
        }
        veh[id - 1] = v;
    }

    // Initial positions
    vector<uint8_t> initPos(n);
    for (int id = 1; id <= n; ++id) {
        Vehicle &v = veh[id - 1];
        if (v.horiz) {
            // leftmost column containing id
            int r = v.fixedCoord - 1;
            int cmin = 6;
            for (int c = 0; c < W; ++c) if (board[r][c] == id) { cmin = min(cmin, c + 0); }
            initPos[id - 1] = (uint8_t)(cmin + 1);
        } else {
            // topmost row containing id
            int c = v.fixedCoord - 1;
            int rmin = 6;
            for (int r = 0; r < H; ++r) if (board[r][c] == id) { rmin = min(rmin, r + 0); }
            initPos[id - 1] = (uint8_t)(rmin + 1);
        }
    }

    // BFS from initial to enumerate reachable states
    unordered_map<uint64_t, int> idOf;
    idOf.reserve(1<<20);
    idOf.max_load_factor(0.7f);

    vector<StateData> states;
    vector<int> distForm;
    vector<int> pred;
    vector<uint8_t> predVeh;
    vector<char> predDir;
    vector<int> clearExtra; // if >=0, in T with extra steps; else -1

    auto add_state = [&](const vector<uint8_t>& pos, int parent, uint8_t mvVeh, char mvDir, int parentDist)->int{
        uint64_t code = encode(pos);
        auto it = idOf.find(code);
        if (it != idOf.end()) return it->second;
        int idx = (int)states.size();
        idOf.emplace(code, idx);
        StateData sd; sd.pos = pos;
        states.emplace_back(std::move(sd));
        distForm.push_back(parentDist + 1);
        pred.push_back(parent);
        predVeh.push_back(mvVeh);
        predDir.push_back(mvDir);
        clearExtra.push_back(-1);
        return idx;
    };

    vector<int> grid(36, 0);
    auto build_grid = [&](const vector<uint8_t>& pos) {
        fill(grid.begin(), grid.end(), 0);
        for (int i = 0; i < n; ++i) {
            const Vehicle &v = veh[i];
            if (v.horiz) {
                int r = v.fixedCoord - 1;
                int c0 = pos[i] - 1;
                for (int k = 0; k < v.len; ++k) {
                    grid[r*6 + (c0 + k)] = i + 1;
                }
            } else {
                int c = v.fixedCoord - 1;
                int r0 = pos[i] - 1;
                for (int k = 0; k < v.len; ++k) {
                    grid[(r0 + k)*6 + c] = i + 1;
                }
            }
        }
    };

    deque<int> q;
    {
        uint64_t code0 = encode(initPos);
        idOf.emplace(code0, 0);
        StateData sd; sd.pos = initPos;
        states.push_back(std::move(sd));
        distForm.push_back(0);
        pred.push_back(-1);
        predVeh.push_back(0);
        predDir.push_back(0);
        clearExtra.push_back(-1);
        q.push_back(0);
    }

    // Limits
    const size_t MAX_STATES = 600000; // to keep under time/memory
    auto t_start = chrono::steady_clock::now();
    const double TIME_LIMIT_BFS = 1.5; // seconds for BFS phase

    while (!q.empty()) {
        int u = q.front(); q.pop_front();
        const vector<uint8_t>& pos = states[u].pos;

        build_grid(pos);

        // Check clear path to exit for red car (id 1 -> index 0), row 3
        {
            int rowRed = veh[0].fixedCoord - 1;
            int L = pos[0];
            int R = L + veh[0].len - 1;
            bool clear = true;
            for (int c = R; c < W; ++c) { // cells to the right: col = R..5, but grid index uses 0-based; we need c=R to W-1 but excluding R itself
                // Correction: we need cells strictly to the right: R..W-1 corresponds to (R+1..W) 1-based
            }
            // Fix strict right check
            clear = true;
            for (int c = R; c < W; ++c) {
                // This loop starts at R (1-based), but for 0-based R-1
                // Let's recalc properly:
                ;
            }
            clear = true;
            int R0 = (int)(pos[0] + veh[0].len - 1) - 1; // 0-based rightmost col
            for (int c = R0 + 1; c < W; ++c) {
                if (grid[rowRed*6 + c] != 0) { clear = false; break; }
            }
            if (clear) {
                int R1 = pos[0] + veh[0].len - 1; // 1-based rightmost col in-board
                int extra = (6 - R1) + 2;
                clearExtra[u] = extra;
            }
        }

        // Generate neighbors
        for (int i = 0; i < n; ++i) {
            const Vehicle &v = veh[i];
            if (v.horiz) {
                int r = v.fixedCoord - 1;
                int c0 = pos[i] - 1;
                // left
                if (c0 - 1 >= 0 && grid[r*6 + (c0 - 1)] == 0) {
                    vector<uint8_t> np = pos;
                    np[i] = (uint8_t)(np[i] - 1);
                    int idx = add_state(np, u, (uint8_t)(i + 1), 'L', distForm[u]);
                    if ((int)idx == (int)states.size() - 1) q.push_back(idx);
                }
                // right
                if (c0 + v.len < W && grid[r*6 + (c0 + v.len)] == 0) {
                    vector<uint8_t> np = pos;
                    np[i] = (uint8_t)(np[i] + 1);
                    int idx = add_state(np, u, (uint8_t)(i + 1), 'R', distForm[u]);
                    if ((int)idx == (int)states.size() - 1) q.push_back(idx);
                }
            } else {
                int c = v.fixedCoord - 1;
                int r0 = pos[i] - 1;
                // up
                if (r0 - 1 >= 0 && grid[(r0 - 1)*6 + c] == 0) {
                    vector<uint8_t> np = pos;
                    np[i] = (uint8_t)(np[i] - 1);
                    int idx = add_state(np, u, (uint8_t)(i + 1), 'U', distForm[u]);
                    if ((int)idx == (int)states.size() - 1) q.push_back(idx);
                }
                // down
                if (r0 + v.len < H && grid[(r0 + v.len)*6 + c] == 0) {
                    vector<uint8_t> np = pos;
                    np[i] = (uint8_t)(np[i] + 1);
                    int idx = add_state(np, u, (uint8_t)(i + 1), 'D', distForm[u]);
                    if ((int)idx == (int)states.size() - 1) q.push_back(idx);
                }
            }
        }

        if (states.size() >= MAX_STATES) break;
        if (((int)states.size() & 4095) == 0) {
            auto t_now = chrono::steady_clock::now();
            double elapsed = chrono::duration<double>(t_now - t_start).count();
            if (elapsed > TIME_LIMIT_BFS) break;
        }
    }

    int S = (int)states.size();

    // Multi-source Dijkstra from clear states with initial distances = extra
    const int INF = 1e9;
    vector<int> distSolve(S, INF);

    struct Node { int d, u; };
    struct Cmp { bool operator()(const Node& a, const Node& b) const { return a.d > b.d; } };
    priority_queue<Node, vector<Node>, Cmp> pq;

    int seeded = 0;
    for (int i = 0; i < S; ++i) {
        if (clearExtra[i] >= 0) {
            distSolve[i] = clearExtra[i];
            pq.push({distSolve[i], i});
            seeded++;
        }
    }

    // If we somehow didn't see any clear state (due to truncation), we must force at least explore a bit more
    // As a fallback, if no seeds, we consider shortest path to any state where red car can move right to column 6 with no blockers ahead.
    // But to keep things simple and safe within time, if no seeds, we treat states with no blockers to the right as seeds (already done),
    // otherwise, we can't compute distances; in this rare case, just set best as initial and steps as 0 + a conservative estimate.
    if (seeded == 0) {
        // ensure output is valid: no formation moves, minimal steps computed by a forward greedy:
        // We'll attempt to compute minimal solution from initial via a BFS-to-clear to ensure at least one seed.
        // Small BFS from initial-only to reach a clear state.
        unordered_map<uint64_t, int> seen2;
        queue<pair<vector<uint8_t>, int>> qq;
        seen2.reserve(1<<16);
        seen2[encode(states[0].pos)] = 0;
        qq.push({states[0].pos, 0});
        int minExtra = INF;
        int minDist = INF;
        auto get_clear_extra = [&](const vector<uint8_t>& pos)->int{
            fill(grid.begin(), grid.end(), 0);
            for (int i = 0; i < n; ++i) {
                const Vehicle &v = veh[i];
                if (v.horiz) {
                    int r = v.fixedCoord - 1, c0 = pos[i] - 1;
                    for (int k = 0; k < v.len; ++k) grid[r*6 + (c0 + k)] = i + 1;
                } else {
                    int c = v.fixedCoord - 1, r0 = pos[i] - 1;
                    for (int k = 0; k < v.len; ++k) grid[(r0 + k)*6 + c] = i + 1;
                }
            }
            int rowRed = veh[0].fixedCoord - 1;
            int R0 = (int)(pos[0] + veh[0].len - 1) - 1; // 0-based rightmost col
            for (int c = R0 + 1; c < W; ++c) {
                if (grid[rowRed*6 + c] != 0) return -1;
            }
            int R1 = pos[0] + veh[0].len - 1;
            return (6 - R1) + 2;
        };
        bool found = false;
        while (!qq.empty()) {
            auto cur = qq.front(); qq.pop();
            auto &pos = cur.first;
            int d = cur.second;
            int ex = get_clear_extra(pos);
            if (ex >= 0) {
                minDist = d + ex;
                found = true;
                break;
            }
            // expand a bit
            if (d > 200) continue; // limit
            fill(grid.begin(), grid.end(), 0);
            for (int i = 0; i < n; ++i) {
                const Vehicle &v = veh[i];
                if (v.horiz) {
                    int r = v.fixedCoord - 1, c0 = pos[i] - 1;
                    for (int k = 0; k < v.len; ++k) grid[r*6 + (c0 + k)] = i + 1;
                } else {
                    int c = v.fixedCoord - 1, r0 = pos[i] - 1;
                    for (int k = 0; k < v.len; ++k) grid[(r0 + k)*6 + c] = i + 1;
                }
            }
            for (int i = 0; i < n; ++i) {
                const Vehicle &v = veh[i];
                if (v.horiz) {
                    int r = v.fixedCoord - 1, c0 = pos[i] - 1;
                    if (c0 - 1 >= 0 && grid[r*6 + (c0 - 1)] == 0) {
                        auto np = pos; np[i]--;
                        uint64_t cd = encode(np);
                        if (!seen2.count(cd)) { seen2[cd] = d + 1; qq.push({np, d + 1}); }
                    }
                    if (c0 + v.len < W && grid[r*6 + (c0 + v.len)] == 0) {
                        auto np = pos; np[i]++;
                        uint64_t cd = encode(np);
                        if (!seen2.count(cd)) { seen2[cd] = d + 1; qq.push({np, d + 1}); }
                    }
                } else {
                    int c = v.fixedCoord - 1, r0 = pos[i] - 1;
                    if (r0 - 1 >= 0 && grid[(r0 - 1)*6 + c] == 0) {
                        auto np = pos; np[i]--;
                        uint64_t cd = encode(np);
                        if (!seen2.count(cd)) { seen2[cd] = d + 1; qq.push({np, d + 1}); }
                    }
                    if (r0 + v.len < H && grid[(r0 + v.len)*6 + c] == 0) {
                        auto np = pos; np[i]++;
                        uint64_t cd = encode(np);
                        if (!seen2.count(cd)) { seen2[cd] = d + 1; qq.push({np, d + 1}); }
                    }
                }
            }
        }
        // Use initial state as best
        int your_steps = found ? minDist : 0;
        cout << your_steps << " " << 0 << "\n";
        return 0;
    }

    // Dijkstra over the undirected graph (neighbors generated on the fly)
    auto t_dij_start = chrono::steady_clock::now();
    const double TIME_LIMIT_TOTAL = 1.95; // total time budget

    while (!pq.empty()) {
        Node cur = pq.top(); pq.pop();
        int u = cur.u, d = cur.d;
        if (d != distSolve[u]) continue;
        // Small time guard
        if (((u + 1) & 8191) == 0) {
            auto t_now = chrono::steady_clock::now();
            double elapsed = chrono::duration<double>(t_now - t_start).count();
            if (elapsed > TIME_LIMIT_TOTAL) break;
        }
        const vector<uint8_t>& pos = states[u].pos;
        build_grid(pos);
        // Generate neighbors
        for (int i = 0; i < n; ++i) {
            const Vehicle &v = veh[i];
            if (v.horiz) {
                int r = v.fixedCoord - 1;
                int c0 = pos[i] - 1;
                // left
                if (c0 - 1 >= 0 && grid[r*6 + (c0 - 1)] == 0) {
                    vector<uint8_t> np = pos;
                    np[i] = (uint8_t)(np[i] - 1);
                    uint64_t cd = encode(np);
                    auto it = idOf.find(cd);
                    if (it != idOf.end()) {
                        int vid = it->second;
                        if (distSolve[vid] > d + 1) {
                            distSolve[vid] = d + 1;
                            pq.push({d + 1, vid});
                        }
                    }
                }
                // right
                if (c0 + v.len < W && grid[r*6 + (c0 + v.len)] == 0) {
                    vector<uint8_t> np = pos;
                    np[i] = (uint8_t)(np[i] + 1);
                    uint64_t cd = encode(np);
                    auto it = idOf.find(cd);
                    if (it != idOf.end()) {
                        int vid = it->second;
                        if (distSolve[vid] > d + 1) {
                            distSolve[vid] = d + 1;
                            pq.push({d + 1, vid});
                        }
                    }
                }
            } else {
                int c = v.fixedCoord - 1;
                int r0 = pos[i] - 1;
                // up
                if (r0 - 1 >= 0 && grid[(r0 - 1)*6 + c] == 0) {
                    vector<uint8_t> np = pos;
                    np[i] = (uint8_t)(np[i] - 1);
                    uint64_t cd = encode(np);
                    auto it = idOf.find(cd);
                    if (it != idOf.end()) {
                        int vid = it->second;
                        if (distSolve[vid] > d + 1) {
                            distSolve[vid] = d + 1;
                            pq.push({d + 1, vid});
                        }
                    }
                }
                // down
                if (r0 + v.len < H && grid[(r0 + v.len)*6 + c] == 0) {
                    vector<uint8_t> np = pos;
                    np[i] = (uint8_t)(np[i] + 1);
                    uint64_t cd = encode(np);
                    auto it = idOf.find(cd);
                    if (it != idOf.end()) {
                        int vid = it->second;
                        if (distSolve[vid] > d + 1) {
                            distSolve[vid] = d + 1;
                            pq.push({d + 1, vid});
                        }
                    }
                }
            }
        }
    }

    // Choose best state maximizing distSolve
    int bestIdx = 0;
    int bestSolve = distSolve[0];
    for (int i = 1; i < S; ++i) {
        if (distSolve[i] > bestSolve) {
            bestSolve = distSolve[i];
            bestIdx = i;
        }
    }
    if (bestSolve >= INF/2) {
        // Fallback: choose initial with trivial estimate if something went wrong
        // Estimate steps to exit if clear else 0
        int est = 0;
        {
            const vector<uint8_t>& pos = states[0].pos;
            build_grid(pos);
            int rowRed = veh[0].fixedCoord - 1;
            int R0 = (int)(pos[0] + veh[0].len - 1) - 1;
            bool clear = true;
            for (int c = R0 + 1; c < W; ++c) if (grid[rowRed*6 + c] != 0) { clear = false; break; }
            if (clear) {
                int R1 = pos[0] + veh[0].len - 1;
                est = (6 - R1) + 2;
            } else est = 0;
        }
        cout << est << " " << 0 << "\n";
        return 0;
    }

    // Reconstruct path from initial (0) to bestIdx
    vector<pair<int,char>> moves;
    int cur = bestIdx;
    while (cur != 0 && cur != -1) {
        moves.push_back({(int)predVeh[cur], predDir[cur]});
        cur = pred[cur];
    }
    reverse(moves.begin(), moves.end());

    cout << bestSolve << " " << moves.size() << "\n";
    for (auto &mv : moves) {
        cout << mv.first << " " << mv.second << "\n";
    }

    return 0;
}