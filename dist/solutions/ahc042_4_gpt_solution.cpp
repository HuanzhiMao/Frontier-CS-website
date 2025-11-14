#include <bits/stdc++.h>
using namespace std;

struct Choice {
    int type; // 0: UpDown (U then D), 1: DownUp (D then U), 2: LeftRight (L then R), 3: RightLeft (R then L)
    int idx;  // row or column index
    int k;    // number of shifts each way
};

struct Op {
    int type;
    int idx;
    int k;
    int cost; // 2*k
    vector<unsigned long long> cover; // dynamic bitset using 64-bit blocks
};

static inline void set_bit(vector<unsigned long long>& bs, int pos) {
    bs[pos >> 6] |= (1ULL << (pos & 63));
}
static inline int count_bits(const vector<unsigned long long>& bs) {
    int cnt = 0;
    for (auto v : bs) cnt += __builtin_popcountll(v);
    return cnt;
}
static inline int count_and_bits(const vector<unsigned long long>& a, const vector<unsigned long long>& b) {
    int cnt = 0;
    for (size_t i = 0; i < a.size(); ++i) cnt += __builtin_popcountll(a[i] & b[i]);
    return cnt;
}
static inline void and_not_inplace(vector<unsigned long long>& a, const vector<unsigned long long>& b) {
    for (size_t i = 0; i < a.size(); ++i) a[i] &= ~b[i];
}

vector<pair<char,int>> expandMoves(const vector<Choice>& choices) {
    vector<pair<char,int>> moves;
    for (const auto& c : choices) {
        if (c.k <= 0) continue;
        if (c.type == 0) { // Up then Down on column idx
            for (int t = 0; t < c.k; ++t) moves.emplace_back('U', c.idx);
            for (int t = 0; t < c.k; ++t) moves.emplace_back('D', c.idx);
        } else if (c.type == 1) { // Down then Up on column idx
            for (int t = 0; t < c.k; ++t) moves.emplace_back('D', c.idx);
            for (int t = 0; t < c.k; ++t) moves.emplace_back('U', c.idx);
        } else if (c.type == 2) { // Left then Right on row idx
            for (int t = 0; t < c.k; ++t) moves.emplace_back('L', c.idx);
            for (int t = 0; t < c.k; ++t) moves.emplace_back('R', c.idx);
        } else if (c.type == 3) { // Right then Left on row idx
            for (int t = 0; t < c.k; ++t) moves.emplace_back('R', c.idx);
            for (int t = 0; t < c.k; ++t) moves.emplace_back('L', c.idx);
        }
    }
    return moves;
}

vector<Choice> compressChoices(const vector<Choice>& choices) {
    map<pair<int,int>, int> bestK;
    for (const auto& c : choices) {
        auto key = make_pair(c.type, c.idx);
        auto it = bestK.find(key);
        if (it == bestK.end()) bestK[key] = c.k;
        else bestK[key] = max(bestK[key], c.k);
    }
    vector<Choice> res;
    res.reserve(bestK.size());
    for (auto& kv : bestK) {
        if (kv.second > 0) res.push_back(Choice{kv.first.first, kv.first.second, kv.second});
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    if (!(cin >> N)) return 0;
    vector<string> C(N);
    for (int i = 0; i < N; ++i) cin >> C[i];

    vector<pair<int,int>> xs;
    vector<vector<int>> idOf(N, vector<int>(N, -1));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (C[i][j] == 'x') {
                idOf[i][j] = (int)xs.size();
                xs.emplace_back(i, j);
            }
        }
    }
    int M = (int)xs.size();
    int blocks = (M + 63) >> 6;

    // Precompute limits
    vector<int> leftlimit(N, N), rightlimit(N, N), uplimit(N, N), downlimit(N, N);
    for (int i = 0; i < N; ++i) {
        int firstO = N;
        for (int j = 0; j < N; ++j) {
            if (C[i][j] == 'o') { firstO = j; break; }
        }
        leftlimit[i] = firstO;
        int lastO = -1;
        for (int j = N - 1; j >= 0; --j) {
            if (C[i][j] == 'o') { lastO = j; break; }
        }
        rightlimit[i] = (lastO == -1 ? N : (N - 1 - lastO));
    }
    for (int j = 0; j < N; ++j) {
        int firstO = N;
        for (int i = 0; i < N; ++i) {
            if (C[i][j] == 'o') { firstO = i; break; }
        }
        uplimit[j] = firstO;
        int lastO = -1;
        for (int i = N - 1; i >= 0; --i) {
            if (C[i][j] == 'o') { lastO = i; break; }
        }
        downlimit[j] = (lastO == -1 ? N : (N - 1 - lastO));
    }

    // Build all allowed operations for greedy
    vector<Op> ops;
    ops.reserve(4 * N * N);
    auto make_cover_vector = [&](void)->vector<unsigned long long> { return vector<unsigned long long>(blocks, 0ULL); };

    // Row operations
    for (int i = 0; i < N; ++i) {
        // Left then Right: k = 1..leftlimit[i], cover all xs at row i with j < k
        for (int k = 1; k <= leftlimit[i]; ++k) {
            vector<unsigned long long> cov = make_cover_vector();
            bool any = false;
            for (int id = 0; id < M; ++id) {
                if (xs[id].first == i && xs[id].second < k) {
                    set_bit(cov, id);
                    any = true;
                }
            }
            if (any) ops.push_back(Op{2, i, k, 2*k, move(cov)});
        }
        // Right then Left: k = 1..rightlimit[i], cover all xs at row i with j >= N-k
        for (int k = 1; k <= rightlimit[i]; ++k) {
            vector<unsigned long long> cov = make_cover_vector();
            bool any = false;
            int thresh = N - k;
            for (int id = 0; id < M; ++id) {
                if (xs[id].first == i && xs[id].second >= thresh) {
                    set_bit(cov, id);
                    any = true;
                }
            }
            if (any) ops.push_back(Op{3, i, k, 2*k, move(cov)});
        }
    }
    // Column operations
    for (int j = 0; j < N; ++j) {
        // Up then Down: k = 1..uplimit[j], cover all xs at col j with i < k
        for (int k = 1; k <= uplimit[j]; ++k) {
            vector<unsigned long long> cov = make_cover_vector();
            bool any = false;
            for (int id = 0; id < M; ++id) {
                if (xs[id].second == j && xs[id].first < k) {
                    set_bit(cov, id);
                    any = true;
                }
            }
            if (any) ops.push_back(Op{0, j, k, 2*k, move(cov)});
        }
        // Down then Up: k = 1..downlimit[j], cover all xs at col j with i >= N-k
        for (int k = 1; k <= downlimit[j]; ++k) {
            vector<unsigned long long> cov = make_cover_vector();
            bool any = false;
            int thresh = N - k;
            for (int id = 0; id < M; ++id) {
                if (xs[id].second == j && xs[id].first >= thresh) {
                    set_bit(cov, id);
                    any = true;
                }
            }
            if (any) ops.push_back(Op{1, j, k, 2*k, move(cov)});
        }
    }

    // Naive plan: for each x choose minimal cost direction (among allowed)
    vector<Choice> naiveChoices;
    naiveChoices.reserve(M);
    for (int id = 0; id < M; ++id) {
        int i = xs[id].first, j = xs[id].second;
        int bestCost = INT_MAX;
        Choice best{0, 0, 0};

        // Up
        bool ok = true;
        for (int r = 0; r < i; ++r) if (C[r][j] == 'o') { ok = false; break; }
        if (ok) {
            int k = i + 1;
            int cost = 2 * k;
            if (cost < bestCost || (cost == bestCost && k > best.k)) { bestCost = cost; best = {0, j, k}; }
        }
        // Down
        ok = true;
        for (int r = i + 1; r < N; ++r) if (C[r][j] == 'o') { ok = false; break; }
        if (ok) {
            int k = N - i;
            int cost = 2 * k;
            if (cost < bestCost || (cost == bestCost && k > best.k)) { bestCost = cost; best = {1, j, k}; }
        }
        // Left
        ok = true;
        for (int c = 0; c < j; ++c) if (C[i][c] == 'o') { ok = false; break; }
        if (ok) {
            int k = j + 1;
            int cost = 2 * k;
            if (cost < bestCost || (cost == bestCost && k > best.k)) { bestCost = cost; best = {2, i, k}; }
        }
        // Right
        ok = true;
        for (int c = j + 1; c < N; ++c) if (C[i][c] == 'o') { ok = false; break; }
        if (ok) {
            int k = N - j;
            int cost = 2 * k;
            if (cost < bestCost || (cost == bestCost && k > best.k)) { bestCost = cost; best = {3, i, k}; }
        }
        // As per problem guarantee, at least one direction is ok
        naiveChoices.push_back(best);
    }
    // Compress naive choices (merge same type+idx by taking max k)
    vector<Choice> naiveCompressed = compressChoices(naiveChoices);
    vector<pair<char,int>> naiveMoves = expandMoves(naiveCompressed);

    // Greedy set cover plan
    vector<unsigned long long> uncovered(blocks, 0ULL);
    for (int id = 0; id < M; ++id) set_bit(uncovered, id);

    vector<int> selectedIdx;
    selectedIdx.reserve(M);
    int remaining = M;

    while (remaining > 0) {
        int bestIdx = -1;
        int bestGain = -1;
        int bestCost = 1;
        for (int oi = 0; oi < (int)ops.size(); ++oi) {
            int gain = count_and_bits(ops[oi].cover, uncovered);
            if (gain <= 0) continue;
            int cost = ops[oi].cost;
            // maximize gain/cost (avoid float with cross-multiply), tie-break by higher gain, then lower cost
            if (bestIdx == -1) {
                bestIdx = oi; bestGain = gain; bestCost = cost;
            } else {
                long long lhs = 1LL * gain * bestCost;
                long long rhs = 1LL * bestGain * cost;
                if (lhs > rhs || (lhs == rhs && (gain > bestGain || (gain == bestGain && cost < bestCost)))) {
                    bestIdx = oi; bestGain = gain; bestCost = cost;
                }
            }
        }
        if (bestIdx == -1) break; // should not happen due to guarantee
        selectedIdx.push_back(bestIdx);
        and_not_inplace(uncovered, ops[bestIdx].cover);
        remaining -= bestGain;
        if (remaining < 0) remaining = 0;
    }

    // Convert greedy selection to choices and compress duplicates
    vector<Choice> greedyChoices;
    greedyChoices.reserve(selectedIdx.size());
    for (int idx : selectedIdx) {
        greedyChoices.push_back(Choice{ops[idx].type, ops[idx].idx, ops[idx].k});
    }
    vector<Choice> greedyCompressed = compressChoices(greedyChoices);
    vector<pair<char,int>> greedyMoves = expandMoves(greedyCompressed);

    // Choose better (shorter) between naive and greedy, ensuring limit
    const int limit = 4 * N * N;
    vector<pair<char,int>> result;
    if ((int)greedyMoves.size() <= limit && (int)greedyMoves.size() <= (int)naiveMoves.size()) {
        result = move(greedyMoves);
    } else {
        if ((int)naiveMoves.size() <= limit) result = move(naiveMoves);
        else {
            // Fallback (should not happen): truncate to limit just in case
            result = move(naiveMoves);
            if ((int)result.size() > limit) result.resize(limit);
        }
    }

    for (auto &mv : result) {
        cout << mv.first << ' ' << mv.second << '\n';
    }
    return 0;
}