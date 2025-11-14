#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    const int H = 8, W = 14, N = H * W;
    vector<vector<int>> adj(N);
    auto id = [&](int r, int c){ return r * W + c; };
    // Precompute 8-neighbor adjacency
    for (int r = 0; r < H; ++r) {
        for (int c = 0; c < W; ++c) {
            int u = id(r, c);
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue; // cannot stay in place
                    int nr = r + dr, nc = c + dc;
                    if (0 <= nr && nr < H && 0 <= nc && nc < W) {
                        adj[u].push_back(id(nr, nc));
                    }
                }
            }
        }
    }

    // RNG
    std::mt19937 rng((uint32_t)chrono::high_resolution_clock::now().time_since_epoch().count());

    // Initialize grid with random digits
    vector<int> grid(N);
    for (int i = 0; i < N; ++i) grid[i] = rng() % 10;

    // Ensure at least one of each digit 1..9 exists
    array<int, 10> cntDigit{};
    for (int i = 0; i < N; ++i) cntDigit[grid[i]]++;
    for (int d = 1; d <= 9; ++d) {
        if (cntDigit[d] == 0) {
            int pos = rng() % N;
            cntDigit[grid[pos]]--;
            grid[pos] = d;
            cntDigit[d]++;
        }
    }

    // Pair counts for oriented edges: counts[a][b] = number of oriented adj edges u->v with grid[u]=a, grid[v]=b
    int counts[10][10];
    memset(counts, 0, sizeof(counts));
    for (int u = 0; u < N; ++u) {
        int a = grid[u];
        for (int v : adj[u]) {
            int b = grid[v];
            counts[a][b]++;
        }
    }

    auto computeScoreComponents = [&](){
        int pairCov = 0; // for a in 1..9, b in 0..9
        for (int a = 1; a <= 9; ++a) {
            for (int b = 0; b <= 9; ++b) {
                if (counts[a][b] > 0) pairCov++;
            }
        }
        int pres = 0;
        for (int d = 1; d <= 9; ++d) if (cntDigit[d] > 0) pres++;
        int dd = 0; // adjacency for repeated digits 1..9
        for (int d = 1; d <= 9; ++d) if (counts[d][d] > 0) dd++;
        int zerozero = (counts[0][0] > 0) ? 1 : 0;
        return array<int,4>{pairCov, pres, dd, zerozero};
    };

    auto comp = computeScoreComponents();
    int pairCov = comp[0], pres = comp[1], dd = comp[2], zerozero = comp[3];

    // Weights for objective
    const int W_PAIR = 1000;
    const int W_PRES = 100;
    const int W_DD = 20;
    const int W_ZZ = 10;
    long long objective = 1LL * W_PAIR * pairCov + 1LL * W_PRES * pres + 1LL * W_DD * dd + 1LL * W_ZZ * zerozero;

    vector<int> bestGrid = grid;
    long long bestObjective = objective;

    // Simulated annealing parameters
    const double TIME_LIMIT = 0.9; // seconds
    auto timeStart = chrono::high_resolution_clock::now();

    double T0 = 50.0, T1 = 0.1;
    long long iter = 0;

    uniform_int_distribution<int> distCell(0, N - 1);
    uniform_int_distribution<int> distDigit(0, 9);
    uniform_real_distribution<double> dist01(0.0, 1.0);

    // Pre-allocate some buffers for speed
    int delta[10][10];

    while (true) {
        iter++;
        auto now = chrono::high_resolution_clock::now();
        double elapsed = chrono::duration<double>(now - timeStart).count();
        if (elapsed > TIME_LIMIT) break;
        double t = elapsed / TIME_LIMIT;
        double T = T0 * pow(T1 / T0, t);

        int u = distCell(rng);
        int oldd = grid[u];
        int newd = distDigit(rng);
        if (newd == oldd) continue;

        // Build delta for counts changes due to changing u from oldd to newd
        memset(delta, 0, sizeof(delta));
        for (int v : adj[u]) {
            int b = grid[v];
            delta[oldd][b] -= 1;
            delta[b][oldd] -= 1;
            delta[newd][b] += 1;
            delta[b][newd] += 1;
        }

        // Compute score deltas
        int dPair = 0, dDD = 0, dZZ = 0, dPres = 0;

        // presence delta
        if (oldd >= 1 && cntDigit[oldd] == 1) dPres -= 1;
        if (newd >= 1 && cntDigit[newd] == 0) dPres += 1;

        // pair and dd and zz deltas
        for (int a = 0; a <= 9; ++a) {
            for (int b = 0; b <= 9; ++b) {
                int dAB = delta[a][b];
                if (dAB == 0) continue;
                int before = counts[a][b];
                int after = before + dAB;
                bool was = before > 0;
                bool nowb = after > 0;
                if (a >= 1) dPair += (nowb - was);
                if (a == b && a >= 1) dDD += (nowb - was);
                if (a == 0 && b == 0) dZZ += (nowb - was);
            }
        }

        long long dObj = 1LL * W_PAIR * dPair + 1LL * W_PRES * dPres + 1LL * W_DD * dDD + 1LL * W_ZZ * dZZ;

        bool accept = false;
        if (dObj >= 0) accept = true;
        else {
            double prob = exp((double)dObj / max(1e-9, T));
            if (dist01(rng) < prob) accept = true;
        }
        if (!accept) continue;

        // Apply mutation
        for (int a = 0; a <= 9; ++a) for (int b = 0; b <= 9; ++b) {
            int dAB = delta[a][b];
            if (dAB) counts[a][b] += dAB;
        }
        cntDigit[oldd]--;
        cntDigit[newd]++;
        grid[u] = newd;

        pairCov += dPair;
        pres += dPres;
        dd += dDD;
        zerozero += dZZ;
        objective += dObj;

        if (objective > bestObjective) {
            bestObjective = objective;
            bestGrid = grid;
        }
    }

    grid = bestGrid;

    // Function to check if a number string can be read as a path
    auto canRead = [&](const string &s)->bool {
        int L = (int)s.size();
        vector<char> curr(N, 0), next(N, 0);
        int d0 = s[0] - '0';
        bool any = false;
        for (int i = 0; i < N; ++i) if (grid[i] == d0) { curr[i] = 1; any = true; }
        if (!any) return false;
        for (int i = 1; i < L; ++i) {
            int di = s[i] - '0';
            bool anyNext = false;
            fill(next.begin(), next.end(), 0);
            for (int j = 0; j < N; ++j) {
                if (grid[j] != di) continue;
                // check neighbors of j for previous step
                const auto &nb = adj[j];
                for (int k : nb) {
                    if (curr[k]) { next[j] = 1; anyNext = true; break; }
                }
            }
            if (!anyNext) return false;
            curr.swap(next);
        }
        return true;
    };

    // Evaluate prefix X
    auto evaluateX = [&](int maxN)->int {
        for (int n = 1; n <= maxN; ++n) {
            string s = to_string(n);
            if (!canRead(s)) return n - 1;
        }
        return maxN;
    };

    // We don't need to print X, but we could use it to optionally tweak more if there was more time.
    // Compute once for decent bound; using moderate maxN.
    int X = evaluateX(5000);
    (void)X; // suppress unused warning; judge will compute their own score.

    // Output the grid
    for (int r = 0; r < H; ++r) {
        for (int c = 0; c < W; ++c) cout << grid[id(r,c)];
        cout << '\n';
    }
    return 0;
}