#include <bits/stdc++.h>
using namespace std;

struct Piece {
    bool horizontal;
    int len;
    int fixed; // row if horizontal, col if vertical
};

static const int NMAX = 16;
static const int GRID = 6;

int n; // number of vehicles
Piece P[NMAX]; // 1..n
int initPos[NMAX];

// Encode/decode positions into 64-bit
inline uint64_t encodePos(const int pos[]) {
    uint64_t code = 0;
    for (int i = n; i >= 1; --i) {
        code <<= 4;
        code |= (uint64_t)(pos[i] & 0xF);
    }
    return code;
}
inline void decodePos(uint64_t code, int pos[]) {
    for (int i = 1; i <= n; ++i) {
        pos[n - i + 1] = (int)(code & 0xF);
        code >>= 4;
    }
}

// Build occupancy grid given positions
inline void buildGrid(const int pos[], int grid[GRID][GRID]) {
    for (int r = 0; r < GRID; ++r)
        for (int c = 0; c < GRID; ++c)
            grid[r][c] = 0;
    for (int i = 1; i <= n; ++i) {
        if (P[i].horizontal) {
            int r = P[i].fixed;
            int c0 = pos[i];
            for (int k = 0; k < P[i].len; ++k) {
                int c = c0 + k;
                if (c >= 0 && c < GRID) grid[r][c] = i;
            }
        } else {
            int c = P[i].fixed;
            int r0 = pos[i];
            for (int k = 0; k < P[i].len; ++k) {
                int r = r0 + k;
                if (r >= 0 && r < GRID) grid[r][c] = i;
            }
        }
    }
}

// Heuristic: blockers + secondary blockers + red distance
int heuristicScore(const int pos[]) {
    int grid[GRID][GRID];
    buildGrid(pos, grid);
    int r = 2; // row index of red car
    int c = pos[1];
    int rightmost = c + P[1].len - 1; // = c+1
    bool used[NMAX] = {false};
    vector<int> blockers;
    for (int cc = rightmost + 1; cc < GRID; ++cc) {
        int id = grid[r][cc];
        if (id != 0 && !used[id]) {
            used[id] = true;
            blockers.push_back(id);
        }
    }
    int b1 = (int)blockers.size();
    int b2 = 0;
    for (int id : blockers) {
        if (!P[id].horizontal) {
            int col = P[id].fixed;
            int top = pos[id];
            int L = P[id].len;

            const int R = 2;
            int upSteps = INT_MAX;
            if (R - L >= 0) {
                upSteps = max(0, top - (R - L));
            }
            int downSteps = max(0, (R + 1) - top);
            if (top + L - 1 + downSteps >= GRID) {
                // impossible, but in 6x6 with L<=3 and R=2 this shouldn't occur
                downSteps = INT_MAX;
            }

            auto countUp = 0;
            if (upSteps!=INT_MAX && upSteps>0) {
                bool seen[NMAX] = {false};
                for (int k = 1; k <= upSteps; ++k) {
                    int rr = top - k;
                    if (rr >= 0) {
                        int occ = grid[rr][col];
                        if (occ != 0 && occ != id && !seen[occ]) { seen[occ] = true; ++countUp; }
                    }
                }
            } else if (upSteps == 0) {
                countUp = 0;
            } else {
                countUp = INT_MAX/4;
            }

            auto countDown = 0;
            if (downSteps!=INT_MAX && downSteps>0) {
                bool seen[NMAX] = {false};
                for (int k = 0; k < downSteps; ++k) {
                    int rr = top + L + k;
                    if (rr < GRID) {
                        int occ = grid[rr][col];
                        if (occ != 0 && occ != id && !seen[occ]) { seen[occ] = true; ++countDown; }
                    }
                }
            } else if (downSteps == 0) {
                countDown = 0;
            } else {
                countDown = INT_MAX/4;
            }

            int add = min(countUp, countDown);
            if (add >= INT_MAX/8) add = 5; // fallback
            b2 += add;
        } else {
            // Horizontal piece in the red row to the right - rare.
            // Penalize as it likely must leave the board.
            b2 += 3;
        }
    }
    int redDist = 6 - pos[1]; // e + len = 6 - c
    // Weight blockers strongly
    int score = b1 * 1000 + b2 * 10 + redDist;
    return score;
}

// Generate neighbors (one-unit steps)
inline void genNeighbors(const int pos[], const int grid[GRID][GRID], vector<pair<uint64_t, pair<int,char>>>& out) {
    out.clear();
    int tmp[NMAX];
    for (int i = 1; i <= n; ++i) tmp[i] = pos[i];
    for (int i = 1; i <= n; ++i) {
        if (P[i].horizontal) {
            int r = P[i].fixed;
            int c0 = pos[i];
            int L = P[i].len;
            if (c0 - 1 >= 0 && grid[r][c0 - 1] == 0) {
                tmp[i] = c0 - 1;
                out.push_back({encodePos(tmp), {i,'L'}});
                tmp[i] = c0;
            }
            if (c0 + L <= GRID - 1 && grid[r][c0 + L] == 0) {
                tmp[i] = c0 + 1;
                out.push_back({encodePos(tmp), {i,'R'}});
                tmp[i] = c0;
            }
        } else {
            int c = P[i].fixed;
            int r0 = pos[i];
            int L = P[i].len;
            if (r0 - 1 >= 0 && grid[r0 - 1][c] == 0) {
                tmp[i] = r0 - 1;
                out.push_back({encodePos(tmp), {i,'U'}});
                tmp[i] = r0;
            }
            if (r0 + L <= GRID - 1 && grid[r0 + L][c] == 0) {
                tmp[i] = r0 + 1;
                out.push_back({encodePos(tmp), {i,'D'}});
                tmp[i] = r0;
            }
        }
    }
}

// BFS to exact minimal steps to solve from a given state:
// Goal when red car pos == 4 (occupies columns 4 and 5). Return d + 2.
int solveExact(uint64_t startCode) {
    int startPos[NMAX];
    decodePos(startCode, startPos);
    if (startPos[1] == 4) return 2; // only need to slide off board
    const size_t RESERVE = 200000; // expected states
    unordered_set<uint64_t> vis;
    vis.reserve(RESERVE);
    vis.max_load_factor(0.7f);

    vector<uint64_t> q;
    vector<int> qd;
    q.reserve(RESERVE);
    qd.reserve(RESERVE);
    q.push_back(startCode);
    qd.push_back(0);
    vis.insert(startCode);

    int head = 0;
    vector<pair<uint64_t, pair<int,char>>> neigh;
    neigh.reserve(64);

    int pos[NMAX];
    int grid[GRID][GRID];

    while (head < (int)q.size()) {
        uint64_t code = q[head];
        int dist = qd[head];
        ++head;
        decodePos(code, pos);
        if (pos[1] == 4) {
            return dist + 2;
        }
        buildGrid(pos, grid);
        genNeighbors(pos, grid, neigh);
        for (auto &nb : neigh) {
            uint64_t nc = nb.first;
            if (vis.insert(nc).second) {
                q.push_back(nc);
                qd.push_back(dist + 1);
            }
        }
    }
    // Guaranteed solvable
    return -1;
}

struct Node {
    uint64_t code;
    int parent;
    int depth;
    int score;
    int moveId;
    char moveDir;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // Read board
    int board[GRID][GRID];
    int maxId = 0;
    for (int r = 0; r < GRID; ++r) {
        for (int c = 0; c < GRID; ++c) {
            int x;
            if (!(cin >> x)) return 0;
            board[r][c] = x;
            if (x > maxId) maxId = x;
        }
    }
    n = maxId;
    // Collect cells for each id
    vector<pair<int,int>> cells[NMAX];
    for (int r = 0; r < GRID; ++r) {
        for (int c = 0; c < GRID; ++c) {
            int id = board[r][c];
            if (id >= 1 && id <= n) cells[id].push_back({r,c});
        }
    }
    // Determine piece properties and initial positions
    for (int i = 1; i <= n; ++i) {
        auto &v = cells[i];
        if (v.empty()) continue;
        // Determine orientation and length
        if ((int)v.size() == 2) {
            // Either horizontal or vertical
            if (v[0].first == v[1].first) {
                P[i].horizontal = true;
                P[i].len = 2;
                P[i].fixed = v[0].first;
                int cmin = min(v[0].second, v[1].second);
                initPos[i] = cmin;
            } else {
                P[i].horizontal = false;
                P[i].len = 2;
                P[i].fixed = v[0].second;
                int rmin = min(v[0].first, v[1].first);
                initPos[i] = rmin;
            }
        } else {
            // length 3
            // Determine if horizontal (same row) or vertical (same col)
            bool sameRow = (v[0].first == v[1].first) || (v[0].first == v[2].first);
            if (sameRow) {
                P[i].horizontal = true;
                P[i].len = 3;
                // fixed row is the row of any cell
                int row = v[0].first;
                if (v[1].first == row || v[2].first == row) P[i].fixed = row;
                else P[i].fixed = v[1].first;
                int cmin = min(v[0].second, min(v[1].second, v[2].second));
                initPos[i] = cmin;
            } else {
                P[i].horizontal = false;
                P[i].len = 3;
                int col = v[0].second;
                if (v[1].second == col || v[2].second == col) P[i].fixed = col;
                else P[i].fixed = v[1].second;
                int rmin = min(v[0].first, min(v[1].first, v[2].first));
                initPos[i] = rmin;
            }
        }
    }
    // Ensure red car assumptions
    if (!(P[1].horizontal && P[1].len == 2 && P[1].fixed == 2)) {
        // Adjust if input violates assumptions (shouldn't happen per problem)
        P[1].horizontal = true;
        P[1].len = 2;
        P[1].fixed = 2;
    }

    uint64_t initCode = encodePos(initPos);

    // Beam search to find a "hard" reachable state
    vector<Node> nodes;
    nodes.reserve(300000);
    unordered_map<uint64_t,int> idxOf;
    idxOf.reserve(300000);
    idxOf.max_load_factor(0.7f);

    Node root;
    root.code = initCode;
    root.parent = -1;
    root.depth = 0;
    root.moveId = 0;
    root.moveDir = 0;
    int rootPos[NMAX];
    decodePos(initCode, rootPos);
    root.score = heuristicScore(rootPos);
    nodes.push_back(root);
    idxOf[initCode] = 0;

    vector<int> beam;
    beam.reserve(2000);
    beam.push_back(0);

    int bestIdx = 0;
    int bestScore = nodes[0].score;
    int bestDepth = 0;

    auto tstart = chrono::steady_clock::now();
    const double totalLimit = 1.9; // seconds
    const double beamLimit = 0.7;  // time reserved for beam
    const int BEAM_WIDTH = 500;
    const int VISIT_LIMIT = 200000;

    vector<pair<int,int>> nextPool; nextPool.reserve(BEAM_WIDTH*40);
    vector<pair<uint64_t, pair<int,char>>> neigh;
    neigh.reserve(64);

    int posTmp[NMAX];
    int grid[GRID][GRID];

    while (!beam.empty()) {
        auto now = chrono::steady_clock::now();
        double elapsed = chrono::duration<double>(now - tstart).count();
        if (elapsed > beamLimit || (int)nodes.size() >= VISIT_LIMIT) break;

        nextPool.clear();
        for (int idx : beam) {
            decodePos(nodes[idx].code, posTmp);
            buildGrid(posTmp, grid);
            genNeighbors(posTmp, grid, neigh);
            for (auto &nb : neigh) {
                uint64_t code = nb.first;
                if (idxOf.find(code) != idxOf.end()) continue;
                Node nd;
                nd.code = code;
                nd.parent = idx;
                nd.depth = nodes[idx].depth + 1;
                nd.moveId = nb.second.first;
                nd.moveDir = nb.second.second;
                int p[NMAX];
                decodePos(code, p);
                nd.score = heuristicScore(p);
                int nid = (int)nodes.size();
                nodes.push_back(nd);
                idxOf[code] = nid;
                nextPool.emplace_back(nd.score, nid);
                if (nd.score > bestScore || (nd.score == bestScore && nd.depth > bestDepth)) {
                    bestScore = nd.score;
                    bestDepth = nd.depth;
                    bestIdx = nid;
                }
            }
        }
        if (nextPool.empty()) break;
        // Select top BEAM_WIDTH
        int k = min((int)nextPool.size(), BEAM_WIDTH);
        nth_element(nextPool.begin(), nextPool.begin() + k, nextPool.end(),
                    [](const pair<int,int>& a, const pair<int,int>& b){
                        if (a.first != b.first) return a.first > b.first;
                        return a.second > b.second;
                    });
        sort(nextPool.begin(), nextPool.begin() + k,
             [](const pair<int,int>& a, const pair<int,int>& b){
                 if (a.first != b.first) return a.first > b.first;
                 return a.second > b.second;
             });
        beam.clear();
        beam.reserve(k);
        for (int i = 0; i < k; ++i) beam.push_back(nextPool[i].second);
    }

    // Compute exact minimal steps to solve from best found state
    int minSteps = solveExact(nodes[bestIdx].code);
    if (minSteps < 0) minSteps = 0; // fallback, though problem guarantees solvable

    // Reconstruct formation moves
    vector<pair<int,char>> moves;
    int cur = bestIdx;
    while (cur != 0) {
        moves.push_back({nodes[cur].moveId, nodes[cur].moveDir});
        cur = nodes[cur].parent;
    }
    reverse(moves.begin(), moves.end());

    cout << minSteps << " " << moves.size() << "\n";
    for (auto &mv : moves) {
        cout << mv.first << " " << mv.second << "\n";
    }
    return 0;
}