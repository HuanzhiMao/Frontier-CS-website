#include <bits/stdc++.h>
using namespace std;

struct Solver {
    int t;
    mt19937 rng;

    Solver() {
        ios::sync_with_stdio(false);
        cin.tie(nullptr);
        rng.seed(chrono::steady_clock::now().time_since_epoch().count());
    }

    int ask(int l, int r) {
        cout << "? " << l << " " << r << "\n";
        cout.flush();
        int x;
        if(!(cin >> x)) exit(0);
        int e = x - (r - l);
        if (e < 0) e = 0;
        if (e > 1) e = 1;
        return e;
    }

    int guess(int a) {
        cout << "! " << a << "\n";
        cout.flush();
        int y;
        if(!(cin >> y)) exit(0);
        return y;
    }

    void end_case() {
        cout << "#\n";
        cout.flush();
    }

    pair<int,int> choose_interval(const vector<int>& alive, int step) {
        int m = (int)alive.size();
        if (m <= 2) {
            int l = alive.front();
            int r = alive.back();
            return {l, r};
        }
        int inside = 0;
        int start = 0;
        if (m >= 6) {
            if (step % 3 == 0) {
                inside = max(1, min(m - 1, m / 2));
                start = (m - inside) / 2;
            } else if (step % 3 == 1) {
                inside = max(1, min(m - 1, m / 3));
                start = 0;
            } else {
                inside = max(1, min(m - 1, (2 * m) / 3));
                start = m - inside;
            }
        } else {
            // For small m, just split in half centered
            inside = max(1, min(m - 1, m / 2));
            start = (m - inside) / 2;
        }
        int l = alive[start];
        int r = alive[start + inside - 1];
        return {l, r};
    }

    void solve_case() {
        int n;
        if(!(cin >> n)) return;

        // Query limit
        long double base = 1.116L;
        long double lg = log((long double)n) / log(base);
        int maxQ = (int)ceil(2.0L * lg);

        vector<char> run_len(n + 1, 0);
        vector<char> last_bit(n + 1, -1);
        vector<char> aliveFlag(n + 1, 1);
        vector<int> alive;
        alive.reserve(n);
        for (int i = 1; i <= n; ++i) alive.push_back(i);

        int queries = 0;
        int step = 0;

        while ((int)alive.size() > 2 && queries < maxQ) {
            auto [l, r] = choose_interval(alive, step);
            step++;
            int e = ask(l, r);
            queries++;

            vector<int> newAlive;
            newAlive.reserve(alive.size());
            for (int a : alive) {
                int b = (a < l || a > r) ? 1 : 0; // outside -> 1
                int d = e ^ b;
                if (last_bit[a] == -1) {
                    last_bit[a] = (char)d;
                    run_len[a] = 1;
                    newAlive.push_back(a);
                } else {
                    if (last_bit[a] == d) {
                        run_len[a]++;
                        if (run_len[a] >= 3) {
                            aliveFlag[a] = 0; // eliminate
                        } else {
                            newAlive.push_back(a);
                        }
                    } else {
                        last_bit[a] = (char)d;
                        run_len[a] = 1;
                        newAlive.push_back(a);
                    }
                }
            }
            alive.swap(newAlive);
        }

        // Make up to 2 guesses
        if (alive.empty()) {
            // Fallback: just guess 1 and 2
            int y = guess(1);
            if (y == 0) {
                guess(min(2, n));
            }
            end_case();
            return;
        }

        if ((int)alive.size() == 1) {
            guess(alive[0]);
            end_case();
            return;
        }

        // Two guesses
        int y = guess(alive[0]);
        if (y == 0) {
            guess(alive[1]);
        }
        end_case();
    }

    void run() {
        if(!(cin >> t)) return;
        for (int _ = 0; _ < t; ++_) {
            solve_case();
        }
    }
};

int main() {
    Solver s;
    s.run();
    return 0;
}