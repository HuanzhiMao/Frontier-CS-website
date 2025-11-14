#include <bits/stdc++.h>
using namespace std;

struct EdgeRef {
    bool hor; // true: horizontal h[i][j], false: vertical v[i][j]
    int a;    // if hor: row i (0..29), else: row i (0..28)
    int b;    // if hor: col j (0..28), else: col j (0..29)
};

static inline int idxH(int i, int j) { return i * 29 + j; }
static inline int idxV(int i, int j) { return i * 30 + j; }
static inline int nodeId(int i, int j) { return i * 30 + j; }
static inline void nodeCoord(int id, int &i, int &j) { i = id / 30; j = id % 30; }

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int N = 30;
    const int Q = 1000;

    // Initialize edge weights and usage counts
    const double INIT_W = 6000.0;
    vector<double> h(N * 29, INIT_W), v((N - 1) * N, INIT_W);
    vector<int> cntH(N * 29, 0), cntV((N - 1) * N, 0);

    auto getBaseWeight = [&](const EdgeRef &e) -> double {
        if (e.hor) return h[idxH(e.a, e.b)];
        else return v[idxV(e.a, e.b)];
    };

    auto addPenaltyWeight = [&](const EdgeRef &e, double gamma) -> double {
        double base = getBaseWeight(e);
        int c = e.hor ? cntH[idxH(e.a, e.b)] : cntV[idxV(e.a, e.b)];
        // Encourage exploration: penalize frequently used edges
        double pen = gamma * sqrt((double)c);
        return base + pen;
    };

    auto edgeFromStep = [&](int pi, int pj, int ci, int cj) -> EdgeRef {
        if (ci == pi) {
            if (cj == pj + 1) return {true, pi, pj};       // Right: h[pi][pj]
            else /* cj == pj - 1 */ return {true, pi, pj-1}; // Left: h[pi][pj-1]
        } else {
            if (ci == pi + 1) return {false, pi, pj};       // Down: v[pi][pj]
            else /* ci == pi - 1 */ return {false, pi-1, pj}; // Up: v[pi-1][pj]
        }
    };

    auto clampW = [](double x) -> double {
        if (x < 100.0) return 100.0;
        if (x > 10000.0) return 10000.0;
        return x;
    };

    for (int k = 0; k < Q; k++) {
        int si, sj, ti, tj;
        if (!(cin >> si >> sj >> ti >> tj)) {
            return 0; // in case of unexpected EOF
        }

        // Schedule for exploration penalty and learning rate
        double progress = (double)k / (double)Q;
        double gammaStart = 800.0, gammaEnd = 0.0;
        double gamma = gammaStart * (1.0 - progress) + gammaEnd * progress;

        // Dijkstra
        const int NN = N * N;
        const double INF = 1e100;
        vector<double> dist(NN, INF);
        vector<int> prevNode(NN, -1);
        vector<char> prevMove(NN, 0);
        vector<char> visited(NN, 0);

        int sId = nodeId(si, sj), tId = nodeId(ti, tj);
        dist[sId] = 0.0;
        using PDI = pair<double, int>;
        priority_queue<PDI, vector<PDI>, greater<PDI>> pq;
        pq.emplace(0.0, sId);

        auto relax = [&](int uId, int vi, int vj, char mv, double w) {
            int vId = nodeId(vi, vj);
            if (dist[vId] > dist[uId] + w) {
                dist[vId] = dist[uId] + w;
                prevNode[vId] = uId;
                prevMove[vId] = mv;
                pq.emplace(dist[vId], vId);
            }
        };

        while (!pq.empty()) {
            auto [du, uId] = pq.top(); pq.pop();
            if (visited[uId]) continue;
            visited[uId] = 1;
            if (uId == tId) break;
            int ui, uj;
            nodeCoord(uId, ui, uj);

            // Up
            if (ui > 0) {
                EdgeRef e = {false, ui - 1, uj};
                double w = addPenaltyWeight(e, gamma);
                relax(uId, ui - 1, uj, 'U', w);
            }
            // Down
            if (ui < N - 1) {
                EdgeRef e = {false, ui, uj};
                double w = addPenaltyWeight(e, gamma);
                relax(uId, ui + 1, uj, 'D', w);
            }
            // Left
            if (uj > 0) {
                EdgeRef e = {true, ui, uj - 1};
                double w = addPenaltyWeight(e, gamma);
                relax(uId, ui, uj - 1, 'L', w);
            }
            // Right
            if (uj < N - 1) {
                EdgeRef e = {true, ui, uj};
                double w = addPenaltyWeight(e, gamma);
                relax(uId, ui, uj + 1, 'R', w);
            }
        }

        // Reconstruct path
        string path;
        vector<EdgeRef> usedEdges;
        long double predictedSum = 0.0L;

        if (prevNode[tId] == -1 && sId != tId) {
            // Fallback: straight Manhattan path (shouldn't happen on grid)
            int ci = si, cj = sj;
            while (ci != ti) {
                if (ti < ci) {
                    EdgeRef e = {false, ci - 1, cj};
                    usedEdges.push_back(e);
                    predictedSum += getBaseWeight(e);
                    path.push_back('U');
                    ci--;
                } else {
                    EdgeRef e = {false, ci, cj};
                    usedEdges.push_back(e);
                    predictedSum += getBaseWeight(e);
                    path.push_back('D');
                    ci++;
                }
            }
            while (cj != tj) {
                if (tj < cj) {
                    EdgeRef e = {true, ci, cj - 1};
                    usedEdges.push_back(e);
                    predictedSum += getBaseWeight(e);
                    path.push_back('L');
                    cj--;
                } else {
                    EdgeRef e = {true, ci, cj};
                    usedEdges.push_back(e);
                    predictedSum += getBaseWeight(e);
                    path.push_back('R');
                    cj++;
                }
            }
        } else {
            // Trace back
            vector<char> revMoves;
            int cur = tId;
            while (cur != sId) {
                revMoves.push_back(prevMove[cur]);
                cur = prevNode[cur];
            }
            reverse(revMoves.begin(), revMoves.end());
            // Build path and collect edges
            int ci = si, cj = sj;
            for (char c : revMoves) {
                int ni = ci, nj = cj;
                if (c == 'U') ni--;
                else if (c == 'D') ni++;
                else if (c == 'L') nj--;
                else if (c == 'R') nj++;
                EdgeRef e = edgeFromStep(ci, cj, ni, nj);
                usedEdges.push_back(e);
                predictedSum += getBaseWeight(e);
                path.push_back(c);
                ci = ni; cj = nj;
            }
        }

        // Output and flush
        cout << path << "\n" << flush;

        // Read feedback
        long long observed;
        if (!(cin >> observed)) return 0;

        // Learning/update
        if (!usedEdges.empty()) {
            long double error = (long double)observed - predictedSum;

            // Learning rate schedule
            double lrStart = 0.22, lrEnd = 0.05;
            double lr = lrStart * (1.0 - progress) + lrEnd * progress;

            // Weight edges inversely to their usage to learn new edges faster
            double baseC0 = 3.0;
            long double sumG = 0.0L;
            vector<long double> ge(usedEdges.size());
            for (size_t i = 0; i < usedEdges.size(); i++) {
                const EdgeRef &e = usedEdges[i];
                int c = e.hor ? cntH[idxH(e.a, e.b)] : cntV[idxV(e.a, e.b)];
                long double g = 1.0L / (baseC0 + (long double)c);
                ge[i] = g;
                sumG += g;
            }
            if (sumG < 1e-12L) sumG = 1.0L;

            for (size_t i = 0; i < usedEdges.size(); i++) {
                const EdgeRef &e = usedEdges[i];
                long double share = ge[i] / sumG;
                double delta = (double)(lr * error * share);
                if (e.hor) {
                    int id = idxH(e.a, e.b);
                    h[id] = clampW(h[id] + delta);
                    cntH[id]++;
                } else {
                    int id = idxV(e.a, e.b);
                    v[id] = clampW(v[id] + delta);
                    cntV[id]++;
                }
            }
        }
    }

    return 0;
}