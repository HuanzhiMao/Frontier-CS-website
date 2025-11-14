#include <bits/stdc++.h>
using namespace std;

struct Vehicle {
    bool horiz;
    int len;
};

struct Move {
    int id;
    char dir;
};

static inline uint64_t encode(const vector<int>& rr, const vector<int>& cc) {
    uint64_t code = 0;
    int N = rr.size();
    for (int i = 0; i < N; ++i) {
        uint64_t chunk = (uint64_t)((rr[i] & 7) | ((cc[i] & 7) << 3));
        code |= (chunk << (6 * i));
    }
    return code;
}

static inline void decode_into(uint64_t code, int N, int rr[], int cc[]) {
    for (int i = 0; i < N; ++i) {
        uint64_t chunk = (code >> (6 * i)) & 63ULL;
        rr[i] = (int)(chunk & 7ULL);
        cc[i] = (int)((chunk >> 3) & 7ULL);
    }
}

static inline uint64_t set_chunk(uint64_t code, int idx, int r, int c) {
    uint64_t mask = 63ULL << (6 * idx);
    uint64_t chunk = (uint64_t)((r & 7) | ((c & 7) << 3));
    code &= ~mask;
    code |= (chunk << (6 * idx));
    return code;
}

int min_steps_to_solve(uint64_t startCode, const vector<Vehicle>& vs) {
    const int N = (int)vs.size();
    unordered_set<uint64_t> visited;
    visited.reserve(100000);
    queue<uint64_t> q;
    q.push(startCode);
    visited.insert(startCode);

    int rr[16], cc[16];
    int grid[6][6];

    int best = INT_MAX;
    int d = 0;

    while (!q.empty()) {
        int qs = (int)q.size();
        for (int qi = 0; qi < qs; ++qi) {
            uint64_t code = q.front(); q.pop();
            // decode and build occupancy
            decode_into(code, N, rr, cc);
            for (int i = 0; i < 6; ++i) for (int j = 0; j < 6; ++j) grid[i][j] = 0;
            for (int i = 0; i < N; ++i) {
                int r = rr[i], c = cc[i], L = vs[i].len;
                if (vs[i].horiz) {
                    for (int k = 0; k < L; ++k) grid[r][c + k] = i + 1;
                } else {
                    for (int k = 0; k < L; ++k) grid[r + k][c] = i + 1;
                }
            }

            // check exit path clear for red car (id 1 -> index 0)
            int r1 = rr[0], c1 = cc[0], L1 = vs[0].len;
            int right_end = c1 + L1 - 1;
            bool pathClear = true;
            for (int c = right_end + 1; c < 6; ++c) {
                if (grid[r1][c] != 0) { pathClear = false; break; }
            }
            if (pathClear) {
                int extra = (5 - right_end) + L1;
                int cost = d + extra;
                if (cost < best) best = cost;
            }

            if (d + 2 >= best) continue; // pruning

            // generate moves
            for (int i = 0; i < N; ++i) {
                int r = rr[i], c = cc[i], L = vs[i].len;
                if (vs[i].horiz) {
                    // left
                    if (c - 1 >= 0 && grid[r][c - 1] == 0) {
                        uint64_t ncode = set_chunk(code, i, r, c - 1);
                        if (visited.insert(ncode).second) q.push(ncode);
                    }
                    // right
                    if (c + L < 6 && grid[r][c + L] == 0) {
                        uint64_t ncode = set_chunk(code, i, r, c + 1);
                        if (visited.insert(ncode).second) q.push(ncode);
                    }
                } else {
                    // up
                    if (r - 1 >= 0 && grid[r - 1][c] == 0) {
                        uint64_t ncode = set_chunk(code, i, r - 1, c);
                        if (visited.insert(ncode).second) q.push(ncode);
                    }
                    // down
                    if (r + L < 6 && grid[r + L][c] == 0) {
                        uint64_t ncode = set_chunk(code, i, r + 1, c);
                        if (visited.insert(ncode).second) q.push(ncode);
                    }
                }
            }
        }
        if (d + 2 >= best) break;
        ++d;
    }

    return best == INT_MAX ? -1 : best;
}

struct LegalMove {
    int id; // 1-based
    char dir;
    uint64_t code;
};

static inline bool isOpposite(char a, char b) {
    return (a == 'L' && b == 'R') || (a == 'R' && b == 'L') || (a == 'U' && b == 'D') || (a == 'D' && b == 'U');
}

void generate_legal_moves(uint64_t code, const vector<Vehicle>& vs, vector<LegalMove>& out) {
    out.clear();
    int N = (int)vs.size();
    int rr[16], cc[16], grid[6][6];
    decode_into(code, N, rr, cc);
    for (int i = 0; i < 6; ++i) for (int j = 0; j < 6; ++j) grid[i][j] = 0;
    for (int i = 0; i < N; ++i) {
        int r = rr[i], c = cc[i], L = vs[i].len;
        if (vs[i].horiz) {
            for (int k = 0; k < L; ++k) grid[r][c + k] = i + 1;
        } else {
            for (int k = 0; k < L; ++k) grid[r + k][c] = i + 1;
        }
    }

    for (int i = 0; i < N; ++i) {
        int r = rr[i], c = cc[i], L = vs[i].len;
        if (vs[i].horiz) {
            if (c - 1 >= 0 && grid[r][c - 1] == 0) {
                out.push_back({i + 1, 'L', set_chunk(code, i, r, c - 1)});
            }
            if (c + L < 6 && grid[r][c + L] == 0) {
                out.push_back({i + 1, 'R', set_chunk(code, i, r, c + 1)});
            }
        } else {
            if (r - 1 >= 0 && grid[r - 1][c] == 0) {
                out.push_back({i + 1, 'U', set_chunk(code, i, r - 1, c)});
            }
            if (r + L < 6 && grid[r + L][c] == 0) {
                out.push_back({i + 1, 'D', set_chunk(code, i, r + 1, c)});
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read board
    int board[6][6];
    int maxId = 0;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            int x; 
            if (!(cin >> x)) x = 0;
            board[i][j] = x;
            if (x > maxId) maxId = x;
        }
    }
    int N = maxId;
    if (N <= 0) {
        // No vehicles, trivial
        cout << 0 << " " << 0 << "\n";
        return 0;
    }

    vector<Vehicle> vs(N);
    vector<int> rr(N, 0), cc(N, 0);

    // Build vehicle info
    for (int id = 1; id <= N; ++id) {
        vector<pair<int,int>> cells;
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 6; ++j) {
                if (board[i][j] == id) cells.emplace_back(i, j);
            }
        }
        if (cells.empty()) {
            // Should not happen
            vs[id - 1] = {true, 2};
            rr[id - 1] = 0;
            cc[id - 1] = 0;
            continue;
        }
        sort(cells.begin(), cells.end()); // sort by row, then col
        bool horiz = true;
        if (cells.size() >= 2) {
            if (cells[0].first == cells[1].first) horiz = true;
            else horiz = false;
        } else {
            // Single cell shouldn't happen
            horiz = true;
        }
        int len = (int)cells.size();
        int rmin = 10, cmin = 10;
        for (auto &p : cells) {
            rmin = min(rmin, p.first);
            cmin = min(cmin, p.second);
        }
        vs[id - 1] = {horiz, len};
        if (horiz) {
            rr[id - 1] = cells[0].first;
            cmin = 10;
            for (auto &p : cells) cmin = min(cmin, p.second);
            cc[id - 1] = cmin;
        } else {
            cc[id - 1] = cells[0].second;
            rmin = 10;
            for (auto &p : cells) rmin = min(rmin, p.first);
            rr[id - 1] = rmin;
        }
    }

    uint64_t initialCode = encode(rr, cc);

    // Timer
    auto t_start = chrono::steady_clock::now();
    const double TIME_LIMIT_SEC = 1.9;

    // Baseline: no transformation
    int bestSolve = min_steps_to_solve(initialCode, vs);
    vector<Move> bestPath;
    uint64_t bestCode = initialCode;

    // Random improvements
    std::mt19937_64 rng((uint64_t)chrono::high_resolution_clock::now().time_since_epoch().count());
    unordered_map<uint64_t, int> cacheSolve;
    cacheSolve.reserve(1024);
    cacheSolve[initialCode] = bestSolve;

    vector<LegalMove> moves;
    moves.reserve(64);

    int attempts = 0;
    while (true) {
        auto t_now = chrono::steady_clock::now();
        double elapsed = chrono::duration<double>(t_now - t_start).count();
        if (elapsed > TIME_LIMIT_SEC) break;
        ++attempts;

        // Random walk length
        int minLen = 15, maxLen = 120;
        int walkLen = (int)(rng() % (maxLen - minLen + 1)) + minLen;

        uint64_t curCode = initialCode;
        vector<Move> curPath; curPath.reserve(walkLen);
        int lastId = -1;
        char lastDir = '?';

        for (int step = 0; step < walkLen; ++step) {
            generate_legal_moves(curCode, vs, moves);
            if (moves.empty()) break;
            // Remove immediate reversal if possible
            vector<int> candIdx;
            candIdx.reserve(moves.size());
            for (int i = 0; i < (int)moves.size(); ++i) {
                if (lastId == moves[i].id && isOpposite(lastDir, moves[i].dir)) continue;
                candIdx.push_back(i);
            }
            int pickIdx;
            if (!candIdx.empty()) {
                pickIdx = candIdx[rng() % candIdx.size()];
            } else {
                pickIdx = (int)(rng() % moves.size());
            }
            auto &mv = moves[pickIdx];
            curCode = mv.code;
            curPath.push_back({mv.id, mv.dir});
            lastId = mv.id;
            lastDir = mv.dir;
        }

        // Evaluate candidate
        int solveSteps;
        auto it = cacheSolve.find(curCode);
        if (it != cacheSolve.end()) {
            solveSteps = it->second;
        } else {
            // Ensure time left for a solve
            t_now = chrono::steady_clock::now();
            elapsed = chrono::duration<double>(t_now - t_start).count();
            if (elapsed > TIME_LIMIT_SEC) break;
            solveSteps = min_steps_to_solve(curCode, vs);
            cacheSolve[curCode] = solveSteps;
        }

        if (solveSteps > bestSolve) {
            bestSolve = solveSteps;
            bestPath = curPath;
            bestCode = curCode;
        }
    }

    cout << bestSolve << " " << bestPath.size() << "\n";
    for (auto &m : bestPath) {
        cout << m.id << " " << m.dir << "\n";
    }
    return 0;
}