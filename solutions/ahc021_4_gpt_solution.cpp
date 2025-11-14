#include <bits/stdc++.h>
using namespace std;

struct Move {
    int xa, ya, xb, yb;
};

struct Edge {
    int parent;
    int child;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int N = 30;
    const int NODE_CNT = N*(N+1)/2;

    // Map (x,y) -> id and back
    vector<vector<int>> id(N, vector<int>(N, -1));
    vector<int> X, Y;
    X.reserve(NODE_CNT);
    Y.reserve(NODE_CNT);
    int cur = 0;
    for (int x = 0; x < N; ++x) {
        for (int y = 0; y <= x; ++y) {
            id[x][y] = cur++;
            X.push_back(x);
            Y.push_back(y);
        }
    }

    // Read input values
    vector<int> val(NODE_CNT);
    for (int x = 0; x < N; ++x) {
        for (int y = 0; y <= x; ++y) {
            int b;
            if (!(cin >> b)) b = 0;
            val[id[x][y]] = b;
        }
    }

    // Build vertical edges (parent -> child)
    vector<Edge> edges;
    edges.reserve(N * (N - 1));
    vector<vector<int>> incEdges(NODE_CNT);
    for (int x = 0; x < N - 1; ++x) {
        for (int y = 0; y <= x; ++y) {
            int p = id[x][y];
            int c1 = id[x+1][y];
            int c2 = id[x+1][y+1];
            int eid1 = (int)edges.size();
            edges.push_back({p, c1});
            incEdges[p].push_back(eid1);
            incEdges[c1].push_back(eid1);
            int eid2 = (int)edges.size();
            edges.push_back({p, c2});
            incEdges[p].push_back(eid2);
            incEdges[c2].push_back(eid2);
        }
    }
    const int EDGE_CNT = (int)edges.size(); // should be 870

    // 6-direction neighbors for swap
    vector<vector<int>> neighbors(NODE_CNT);
    auto add_neighbor = [&](int a, int b){
        neighbors[a].push_back(b);
        neighbors[b].push_back(a);
    };
    for (int x = 0; x < N; ++x) {
        for (int y = 0; y <= x; ++y) {
            int a = id[x][y];
            // left/right
            if (y - 1 >= 0) add_neighbor(a, id[x][y-1]);
            if (y + 1 <= x) add_neighbor(a, id[x][y+1]);
            // up-left, up-right
            if (x - 1 >= 0 && y - 1 >= 0) add_neighbor(a, id[x-1][y-1]);
            if (x - 1 >= 0 && y <= x - 1) add_neighbor(a, id[x-1][y]);
            // down-left, down-right
            if (x + 1 < N) {
                add_neighbor(a, id[x+1][y]);
                add_neighbor(a, id[x+1][y+1]);
            }
        }
    }

    // Operations recorder
    vector<Move> ops;
    ops.reserve(10000);

    auto do_swap_record = [&](int a, int b){
        ops.push_back({X[a], Y[a], X[b], Y[b]});
        swap(val[a], val[b]);
    };

    // Stage A: Sift-down passes (heapify-like) to roughly enforce heap property
    const int K_MAX = 10000;
    int K_quotaA = 6000; // budget for Stage A
    bool changed = true;
    while ((int)ops.size() < K_quotaA && changed) {
        changed = false;
        for (int x = 0; x < N - 1 && (int)ops.size() < K_quotaA; ++x) {
            for (int y = 0; y <= x && (int)ops.size() < K_quotaA; ++y) {
                int cx = x, cy = y;
                while (cx < N - 1 && (int)ops.size() < K_quotaA) {
                    int p = id[cx][cy];
                    int c1 = id[cx+1][cy];
                    int c2 = id[cx+1][cy+1];
                    int mc = (val[c1] < val[c2] ? c1 : c2);
                    if (val[p] > val[mc]) {
                        do_swap_record(p, mc);
                        changed = true;
                        // move down along path of chosen child
                        if (mc == c1) {
                            cx = cx + 1;
                            cy = cy; // y stays
                        } else {
                            cx = cx + 1;
                            cy = cy + 1;
                        }
                    } else {
                        break;
                    }
                }
            }
        }
    }

    // Stage B: Local improvement via greedy swaps minimizing E
    // Build initial violation set
    vector<char> isViol(EDGE_CNT, 0);
    vector<int> where(EDGE_CNT, -1);
    vector<int> violList;
    violList.reserve(EDGE_CNT);
    auto edge_is_violation = [&](int e)->bool{
        const Edge &ed = edges[e];
        return val[ed.parent] > val[ed.child];
    };
    auto push_viol = [&](int e){
        if (!isViol[e]) {
            isViol[e] = 1;
            where[e] = (int)violList.size();
            violList.push_back(e);
        }
    };
    auto remove_viol = [&](int e){
        if (isViol[e]) {
            int idx = where[e];
            int last = violList.back();
            violList[idx] = last;
            where[last] = idx;
            violList.pop_back();
            isViol[e] = 0;
            where[e] = -1;
        }
    };
    for (int e = 0; e < EDGE_CNT; ++e) {
        if (edge_is_violation(e)) push_viol(e);
    }

    // Helpers to compute delta E for swapping two adjacent nodes
    auto gather_union_edges = [&](int a, int b, vector<int>& out){
        out.clear();
        for (int e : incEdges[a]) {
            bool found = false;
            for (int u : out) if (u == e) { found = true; break; }
            if (!found) out.push_back(e);
        }
        for (int e : incEdges[b]) {
            bool found = false;
            for (int u : out) if (u == e) { found = true; break; }
            if (!found) out.push_back(e);
        }
    };
    auto delta_swap = [&](int a, int b, const vector<int>& unionEdges)->int{
        int before = 0, after = 0;
        for (int e : unionEdges) {
            before += isViol[e] ? 1 : 0;
            // compute after swap
            const Edge &ed = edges[e];
            int pa = ed.parent, ch = ed.child;
            int vp = val[pa], vc = val[ch];
            if (pa == a) vp = val[b];
            else if (pa == b) vp = val[a];
            if (ch == a) vc = val[b];
            else if (ch == b) vc = val[a];
            after += (vp > vc) ? 1 : 0;
        }
        return after - before;
    };

    auto update_viol_after_swap = [&](int a, int b, const vector<int>& unionEdges){
        // Values have been swapped already
        for (int e : unionEdges) {
            bool now = edge_is_violation(e);
            if (now) push_viol(e);
            else remove_viol(e);
        }
    };

    // Random utilities
    mt19937 rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> distNode(0, NODE_CNT - 1);

    vector<int> unionEdges;
    int stuck = 0;
    const int SAMPLE_VIOL = 64;
    const int SAMPLE_NEI = 32;

    while ((int)ops.size() < K_MAX) {
        if (violList.empty()) break; // E == 0

        // Sample from violating vertical edges
        int bestA = -1, bestB = -1;
        int bestDelta = 0;
        vector<int> bestUnion;
        int bestA_pos = -1;

        int sampleCount = min((int)violList.size(), SAMPLE_VIOL);
        for (int t = 0; t < sampleCount; ++t) {
            int idx = rng() % violList.size();
            int e = violList[idx];
            int a = edges[e].parent;
            int b = edges[e].child;
            gather_union_edges(a, b, unionEdges);
            int d = delta_swap(a, b, unionEdges);
            if (d < bestDelta || bestA == -1) {
                bestDelta = d;
                bestA = a;
                bestB = b;
                bestUnion = unionEdges;
                bestA_pos = 0;
            }
        }

        if (bestA != -1 && bestDelta < 0) {
            // Accept improving swap
            do_swap_record(bestA, bestB);
            update_viol_after_swap(bestA, bestB, bestUnion);
            stuck = 0;
            continue;
        }

        // Try random neighbor pairs (including horizontal)
        int bestA2 = -1, bestB2 = -1;
        int bestDelta2 = 0;
        vector<int> bestUnion2;
        for (int t = 0; t < SAMPLE_NEI; ++t) {
            int a = distNode(rng);
            if (neighbors[a].empty()) continue;
            int b = neighbors[a][rng() % neighbors[a].size()];
            if (a == b) continue;
            gather_union_edges(a, b, unionEdges);
            if (unionEdges.empty()) continue;
            int d = delta_swap(a, b, unionEdges);
            if (d < bestDelta2 || bestA2 == -1) {
                bestDelta2 = d;
                bestA2 = a;
                bestB2 = b;
                bestUnion2 = unionEdges;
            }
        }

        if (bestA2 != -1 && bestDelta2 < 0) {
            do_swap_record(bestA2, bestB2);
            update_viol_after_swap(bestA2, bestB2, bestUnion2);
            stuck = 0;
            continue;
        }

        // If no improvement found, occasionally accept the least bad move to escape local minima
        ++stuck;
        if (stuck >= 8 && (int)ops.size() < K_MAX) {
            // Choose the least positive delta among the two best candidates if available
            int selA = -1, selB = -1;
            vector<int> selUnion;
            int selDelta = 1e9;
            if (bestA != -1) {
                selA = bestA;
                selB = bestB;
                selUnion = bestUnion;
                selDelta = bestDelta;
            }
            if (bestA2 != -1 && bestDelta2 < selDelta) {
                selA = bestA2;
                selB = bestB2;
                selUnion = bestUnion2;
                selDelta = bestDelta2;
            }
            if (selA != -1) {
                do_swap_record(selA, selB);
                update_viol_after_swap(selA, selB, selUnion);
            }
            stuck = 0;
        }

        if ((int)ops.size() >= K_MAX) break;
    }

    // Output
    int K = (int)ops.size();
    cout << K << "\n";
    for (int i = 0; i < K; ++i) {
        cout << ops[i].xa << " " << ops[i].ya << " " << ops[i].xb << " " << ops[i].yb << "\n";
    }

    return 0;
}