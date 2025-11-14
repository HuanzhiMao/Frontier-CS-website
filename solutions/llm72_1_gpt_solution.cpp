#include <bits/stdc++.h>
using namespace std;

struct MoveRec {
    int vid;
    char dir;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    const int NROWS = 6, NCOLS = 6;
    vector<vector<int>> A(NROWS, vector<int>(NCOLS));
    int maxId = 0;
    for (int i = 0; i < NROWS; ++i) {
        for (int j = 0; j < NCOLS; ++j) {
            cin >> A[i][j];
            maxId = max(maxId, A[i][j]);
        }
    }
    int n = maxId; // number of vehicles (ids 1..n)
    if (n == 0) {
        // No vehicles: trivial
        cout << 0 << " " << 0 << "\n";
        return 0;
    }
    
    vector<vector<pair<int,int>>> cells(n+1);
    for (int i = 0; i < NROWS; ++i) {
        for (int j = 0; j < NCOLS; ++j) {
            int id = A[i][j];
            if (id >= 1) cells[id].push_back({i,j});
        }
    }
    
    vector<bool> hor(n+1,false);
    vector<int> len(n+1,0);
    vector<int> fix(n+1,0); // for horizontal: row; for vertical: col
    vector<int> initPosVar(n+1,0); // for horizontal: leftmost col; for vertical: topmost row
    
    for (int id = 1; id <= n; ++id) {
        auto &v = cells[id];
        if (v.empty()) continue; // Shouldn't happen
        len[id] = (int)v.size();
        if (len[id] < 2) {
            // invalid, but proceed
            len[id] = max(len[id], 2);
        }
        // Determine orientation
        // If any two cells in same row => horizontal, else vertical
        bool isHor = false;
        if (v.size() >= 2) {
            if (v[0].first == v[1].first) isHor = true;
            else isHor = false;
        } else {
            // Fallback: check neighbors
            isHor = true;
        }
        hor[id] = isHor;
        if (isHor) {
            int r = v[0].first;
            for (auto &p : v) r = p.first; // all same
            int cmin = 6;
            for (auto &p : v) cmin = min(cmin, p.second);
            fix[id] = r;
            initPosVar[id] = cmin;
        } else {
            int c = v[0].second;
            for (auto &p : v) c = p.second; // all same
            int rmin = 6;
            for (auto &p : v) rmin = min(rmin, p.first);
            fix[id] = c;
            initPosVar[id] = rmin;
        }
    }
    
    // Encode state: 4 bits per vehicle, storing variable coordinate (col for horizontal, row for vertical)
    auto encode = [&](const vector<int> &posVar)->uint64_t{
        uint64_t code = 0;
        for (int id = 1; id <= n; ++id) {
            uint64_t v = (uint64_t)(posVar[id] & 0xF);
            code |= (v << (4*(id-1)));
        }
        return code;
    };
    auto setNibble = [&](uint64_t code, int id, int value)->uint64_t{
        int shift = 4*(id-1);
        uint64_t mask = (uint64_t)0xFULL << shift;
        code &= ~mask;
        code |= ((uint64_t)(value & 0xF) << shift);
        return code;
    };
    auto decodeToArray = [&](uint64_t code, int *posVar){
        for (int id = 1; id <= n; ++id) {
            int shift = 4*(id-1);
            posVar[id] = (int)((code >> shift) & 0xF);
        }
    };
    
    vector<int> pos0 = initPosVar;
    uint64_t startCode = encode(pos0);
    
    // BFS over state-space from initial to enumerate component, build adjacency, parents, and info
    unordered_map<uint64_t,int> idOf;
    idOf.reserve(200000);
    idOf.max_load_factor(0.7);
    
    vector<uint64_t> codes;
    vector<int> parent;
    vector<MoveRec> parentMove;
    vector<vector<int>> adj;
    vector<int> distInit; // formation steps distance
    vector<int> pos1_left; // leftmost col of red car
    vector<char> isClear; // whether path to right is clear
    
    queue<int> q;
    
    auto add_state = [&](uint64_t code, int par, int vid, char dir, int d)->int{
        int newId = (int)codes.size();
        codes.push_back(code);
        parent.push_back(par);
        parentMove.push_back({vid, dir});
        adj.emplace_back();
        distInit.push_back(d);
        pos1_left.push_back(0);
        isClear.push_back(0);
        idOf[code] = newId;
        q.push(newId);
        return newId;
    };
    
    codes.reserve(200000);
    parent.reserve(200000);
    parentMove.reserve(200000);
    adj.reserve(200000);
    distInit.reserve(200000);
    pos1_left.reserve(200000);
    isClear.reserve(200000);
    
    add_state(startCode, -1, -1, 'X', 0);
    
    int posVarArr[16]; // enough for up to 15 vehicles, here <=10
    char grid[6][6];
    
    while (!q.empty()) {
        int cur = q.front(); q.pop();
        uint64_t code = codes[cur];
        
        // decode and build occupancy grid
        decodeToArray(code, posVarArr);
        for (int r = 0; r < 6; ++r) for (int c = 0; c < 6; ++c) grid[r][c] = 0;
        for (int id = 1; id <= n; ++id) {
            if (hor[id]) {
                int r = fix[id];
                int c0 = posVarArr[id];
                for (int k = 0; k < len[id]; ++k) {
                    int c = c0 + k;
                    if (r >= 0 && r < 6 && c >= 0 && c < 6) grid[r][c] = 1;
                }
            } else {
                int c = fix[id];
                int r0 = posVarArr[id];
                for (int k = 0; k < len[id]; ++k) {
                    int r = r0 + k;
                    if (r >= 0 && r < 6 && c >= 0 && c < 6) grid[r][c] = 1;
                }
            }
        }
        // compute clear path for red car (id=1)
        int redRow = fix[1];
        int redLeft = posVarArr[1];
        int redRight = redLeft + len[1] - 1; // len[1] should be 2
        bool clear = true;
        for (int c = redRight + 1; c < 6; ++c) {
            if (grid[redRow][c]) { clear = false; break; }
        }
        pos1_left[cur] = redLeft;
        isClear[cur] = (char)(clear ? 1 : 0);
        
        // generate neighbors
        for (int id = 1; id <= n; ++id) {
            if (hor[id]) {
                int r = fix[id];
                int c0 = posVarArr[id];
                // move left
                if (c0 - 1 >= 0 && grid[r][c0 - 1] == 0) {
                    uint64_t ncode = setNibble(code, id, c0 - 1);
                    auto it = idOf.find(ncode);
                    int nid;
                    if (it == idOf.end()) {
                        nid = add_state(ncode, cur, id, 'L', distInit[cur] + 1);
                    } else {
                        nid = it->second;
                    }
                    adj[cur].push_back(nid);
                }
                // move right
                if (c0 + len[id] < 6 && grid[r][c0 + len[id]] == 0) {
                    uint64_t ncode = setNibble(code, id, c0 + 1);
                    auto it = idOf.find(ncode);
                    int nid;
                    if (it == idOf.end()) {
                        nid = add_state(ncode, cur, id, 'R', distInit[cur] + 1);
                    } else {
                        nid = it->second;
                    }
                    adj[cur].push_back(nid);
                }
            } else {
                int c = fix[id];
                int r0 = posVarArr[id];
                // move up
                if (r0 - 1 >= 0 && grid[r0 - 1][c] == 0) {
                    uint64_t ncode = setNibble(code, id, r0 - 1);
                    auto it = idOf.find(ncode);
                    int nid;
                    if (it == idOf.end()) {
                        nid = add_state(ncode, cur, id, 'U', distInit[cur] + 1);
                    } else {
                        nid = it->second;
                    }
                    adj[cur].push_back(nid);
                }
                // move down
                if (r0 + len[id] < 6 && grid[r0 + len[id]][c] == 0) {
                    uint64_t ncode = setNibble(code, id, r0 + 1);
                    auto it = idOf.find(ncode);
                    int nid;
                    if (it == idOf.end()) {
                        nid = add_state(ncode, cur, id, 'D', distInit[cur] + 1);
                    } else {
                        nid = it->second;
                    }
                    adj[cur].push_back(nid);
                }
            }
        }
    }
    
    int V = (int)codes.size();
    // Dijkstra from all clear states with initial costs = 6 - redLeft
    const int INF = 1e9;
    vector<int> distExit(V, INF);
    using PII = pair<int,int>;
    priority_queue<PII, vector<PII>, greater<PII>> pq;
    for (int i = 0; i < V; ++i) {
        if (isClear[i]) {
            int extra = 6 - pos1_left[i]; // steps to get completely out
            if (extra < distExit[i]) {
                distExit[i] = extra;
                pq.emplace(distExit[i], i);
            }
        }
    }
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != distExit[u]) continue;
        for (int v : adj[u]) {
            if (distExit[v] > d + 1) {
                distExit[v] = d + 1;
                pq.emplace(distExit[v], v);
            }
        }
    }
    
    // Choose state with maximal distExit (finite). Tie-break: minimal distInit
    int bestId = 0;
    int bestSteps = -1;
    for (int i = 0; i < V; ++i) {
        if (distExit[i] >= INF) continue; // unsolvable state (shouldn't happen for at least some)
        if (distExit[i] > bestSteps) {
            bestSteps = distExit[i];
            bestId = i;
        } else if (distExit[i] == bestSteps) {
            if (distInit[i] < distInit[bestId]) {
                bestId = i;
            }
        }
    }
    if (bestSteps < 0) {
        // Fallback: use original state
        bestId = 0;
        bestSteps = distExit[0] >= INF ? 0 : distExit[0];
    }
    
    // Reconstruct formation moves from initial (0) to bestId
    vector<MoveRec> seq;
    int cur = bestId;
    while (cur != 0 && cur != -1) {
        MoveRec mv = parentMove[cur];
        seq.push_back(mv);
        cur = parent[cur];
    }
    reverse(seq.begin(), seq.end());
    
    cout << bestSteps << " " << seq.size() << "\n";
    for (auto &mv : seq) {
        cout << mv.vid << " " << mv.dir << "\n";
    }
    return 0;
}