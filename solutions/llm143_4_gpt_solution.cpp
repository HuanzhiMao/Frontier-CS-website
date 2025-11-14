#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long G;
    if (!(cin >> G)) return 0;
    if (G == -1) return 0;

    // RATE budget management
    const long long RATE_BUDGET_TOTAL = 2900000; // keep safety margin under 3,000,000
    long long used_rate = 0;
    long long per_hand_avg = (G > 0 ? RATE_BUDGET_TOTAL / G : RATE_BUDGET_TOTAL);
    if (per_hand_avg < 0) per_hand_avg = 0;
    // Weights for rounds: preflop, flop, turn, river
    const double weights[4] = {1.0, 1.0, 2.0, 3.0};
    const double weight_sum = 7.0;

    auto ask_rates = [&](long long t, double &w, double &d) -> bool {
        if (t <= 0) return false;
        if (used_rate + t > RATE_BUDGET_TOTAL) {
            t = max(0LL, RATE_BUDGET_TOTAL - used_rate);
        }
        if (t <= 0) return false;
        cout << "RATE " << t << endl;
        used_rate += t;
        string tok;
        while (cin >> tok) {
            if (tok == "-1") return false;
            if (tok == "RATES") {
                cin >> w >> d;
                return true;
            }
            // In case of stray tokens (shouldn't happen), consume gracefully
            if (tok == "OPP") {
                string what; cin >> what;
                if (what == "CALL") { long long x; cin >> x; }
            } else if (tok == "RESULT") {
                long long delta; cin >> delta;
            } else if (tok == "SCORE") {
                double W; cin >> W;
            } else if (tok == "STATE") {
                // Unexpected STATE before RATES would be protocol violation; exit gracefully
                return false;
            } else {
                // Consume rest tokens defensively if any unexpected
            }
        }
        return false;
    };

    auto compute_raise = [&](int r, int a, int P, double p) -> long long {
        // Only called when we intend to raise and a >= 1
        // p = our win+0.5*tie equity
        double s2 = 1.0 - p;
        // Target y = x / (P + 2x)
        double eps = 0.02;
        double y = s2 - eps;
        if (y < 0.01) y = 0.01;
        if (y > 0.49) y = 0.49;
        double denom = (1.0 - 2.0 * y);
        long long x = 1;
        if (denom > 1e-12) {
            double xf = (y * P) / denom;
            if (xf < 1.0) x = 1;
            else {
                x = (long long) floor(xf + 1e-9);
                if (x < 1) x = 1;
            }
        } else {
            x = 1;
        }
        // Hard caps based on pot
        long long hardcap = P;
        if (r == 1) hardcap = 3LL * P;
        else if (r == 2) hardcap = 2LL * P;
        else if (r == 3) hardcap = 2LL * P;
        else if (r == 4) hardcap = 2LL * P;

        // Stack-based cap depending on confidence
        double stackScale = 0.25;
        if (p >= 0.70) stackScale = 1.0;
        else if (p >= 0.65) stackScale = 0.7;
        else if (p >= 0.60) stackScale = 0.5;
        else if (p >= 0.58) stackScale = 0.4;
        else if (p >= 0.56) stackScale = 0.3;
        else if (p >= 0.54) stackScale = 0.25;

        long long x_stack_limit = (long long) floor(a * stackScale);
        if (x_stack_limit < 1) x_stack_limit = 1;

        long long x_max = min<long long>(a, min(hardcap, x_stack_limit));
        if (x_max < 1) x_max = min(1, a);
        if (x > x_max) x = x_max;
        if (x < 1) x = min(1, a);
        return x;
    };

    // Equity thresholds by round for deciding to raise
    // r: 1..4
    double thr[5] = {0.0, 0.54, 0.56, 0.58, 0.62};

    string tok;
    while (cin >> tok) {
        if (tok == "-1") {
            return 0;
        } else if (tok == "STATE") {
            int h, r, a, b, P, k;
            cin >> h >> r >> a >> b >> P >> k;
            string tok2; cin >> tok2; // "ALICE"
            int as1=0, av1=0, as2=0, av2=0;
            if (tok2 == "ALICE") {
                cin >> as1 >> av1 >> as2 >> av2;
            } else {
                // Protocol violation; exit
                return 0;
            }
            string tok3; cin >> tok3; // "BOARD"
            vector<pair<int,int>> board;
            if (tok3 == "BOARD") {
                for (int i = 0; i < 2*k; i += 2) {
                    int cs, cv;
                    cin >> cs >> cv;
                    board.emplace_back(cs, cv);
                }
            } else {
                return 0;
            }

            // Determine sampling count for RATE based on round
            long long t_query = (long long) floor((per_hand_avg * weights[r - 1]) / weight_sum);
            if (t_query < 10) t_query = 10;
            if (t_query > 5000) t_query = 5000;
            // Ensure we don't exceed global budget
            long long remaining_budget = max(0LL, RATE_BUDGET_TOTAL - used_rate);
            if (t_query > remaining_budget) t_query = remaining_budget;

            double w = 0.5, d = 0.0;
            if (t_query > 0) {
                bool ok = ask_rates(t_query, w, d);
                if (!ok) {
                    return 0;
                }
            } else {
                // no RATE call; fallback heuristic (very rough)
                // Preflop simple heuristic: pairs strong, high cards decent, suited bonus
                double p_est = 0.5;
                bool pair = (av1 == av2);
                bool suited = (as1 == as2);
                int hi = max(av1, av2);
                int lo = min(av1, av2);
                if (pair) {
                    p_est = 0.52 + (hi / 13.0) * 0.25; // approx 0.52..0.77
                } else {
                    p_est = 0.48 + (hi / 13.0) * 0.12 + (lo / 13.0) * 0.05;
                    if (suited) p_est += 0.02;
                    int gap = abs(av1 - av2) - 1;
                    if (gap <= 0) p_est += 0.01;
                    else if (gap >= 3) p_est -= 0.02 * (gap - 2);
                }
                if (p_est < 0.05) p_est = 0.05;
                if (p_est > 0.95) p_est = 0.95;
                w = p_est; d = 0.0;
            }

            double p = w + 0.5 * d;
            if (a <= 0) {
                cout << "ACTION CHECK" << endl;
                continue;
            }

            // Decide action
            if (p >= thr[r]) {
                long long x = compute_raise(r, a, P, p);
                if (x < 1) x = 1;
                if (x > a) x = a;
                cout << "ACTION RAISE " << x << endl;
            } else {
                cout << "ACTION CHECK" << endl;
            }
        } else if (tok == "OPP") {
            string what; cin >> what;
            if (what == "CALL") { long long x; cin >> x; }
            // else CHECK or FOLD: nothing to read
        } else if (tok == "RESULT") {
            long long delta; cin >> delta;
        } else if (tok == "SCORE") {
            double W; cin >> W;
            // Match end
        } else if (tok == "RATES") {
            // Should only appear right after a RATE request; consume to keep stream in sync
            double w, d; cin >> w >> d;
        } else if (tok == "ALICE") {
            // Should be only after STATE; consume 4 ints to resync
            int s1,v1,s2,v2; cin >> s1 >> v1 >> s2 >> v2;
        } else if (tok == "BOARD") {
            // Without knowing k, this is unexpected; do nothing
        } else {
            // Unknown token; ignore
        }
    }

    return 0;
}