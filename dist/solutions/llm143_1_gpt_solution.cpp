#include <bits/stdc++.h>
using namespace std;

struct Card { int suit, val; };

static const long long RATE_BUDGET_MAX = 2900000; // leave some safety margin
long long rate_used = 0;

bool readToken(string &tok) {
    if (!(cin >> tok)) return false;
    if (tok == "-1") exit(0);
    return true;
}

pair<double,double> askRate(int t) {
    if (t <= 0) return {-1.0, 0.0};
    long long can = RATE_BUDGET_MAX - rate_used;
    if (can <= 0) return {-1.0, 0.0};
    if (t > can) t = (int)can;
    rate_used += t;
    cout << "RATE " << t << endl;
    cout.flush();
    string tok;
    if (!readToken(tok)) exit(0);
    if (tok != "RATES") {
        // Unexpected token; try to recover minimally
        // Attempt to read until we find RATES; if not found, exit
        while (tok != "RATES") {
            if (!readToken(tok)) exit(0);
        }
    }
    double w = 0.0, d = 0.0;
    if (!(cin >> w >> d)) exit(0);
    return {w, d};
}

double clampd(double x, double lo, double hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

// Approximate preflop equity vs random hand (coarse heuristic)
double preflopApproxEquity(int v1, int v2, bool suited) {
    int r1 = v1, r2 = v2;
    if (r1 < r2) swap(r1, r2);
    double s = 0.5;
    if (r1 == r2) {
        // Pair: from ~0.55 (22) to ~0.85 (AA)
        s = 0.5 + 0.32 * (r1 / 13.0) + 0.03;
    } else {
        // High card weights + suited + gap penalty
        double base = 0.33 + 0.17 * (r1 / 13.0) + 0.14 * (r2 / 13.0);
        int gap = (r1 - r2 - 1);
        s = base - 0.018 * max(0, gap);
        if (suited) s += 0.02;
        if (r1 >= 12 && r2 >= 10) s += 0.02;
        if (r1 <= 5 && r2 <= 5) s -= 0.01;
    }
    s = clampd(s, 0.35, 0.85);
    return s;
}

struct State {
    int h, r, a, b, P, k;
    Card c1, c2;
    vector<Card> board;
};

pair<string,int> decideAction(const State &st) {
    int a = st.a;
    int P = st.P;
    int r = st.r;
    long long invested = 100 - a;
    if (a <= 0) return {"CHECK", 0};

    // Decide whether to query RATE based on stage and pot/investment
    double e = -1.0, d = 0.0, p = -1.0;
    bool haveRates = false;

    auto computeRaise = [&](double p_est)->pair<string,int> {
        // Use p = e + d/2
        double pLoc = p_est;
        pLoc = clampd(pLoc, 0.0, 1.0);
        double denom = 2.0 * pLoc - 1.0; // = 1 - 2t
        if (denom <= 0.0 + 1e-9) {
            // p <= 0.5 -> do not raise (calling always bad)
            return {"CHECK", 0};
        }
        // x_thresh = (1 - p) * P / (2p - 1)
        double xth = (1.0 - pLoc) * (double)P / denom;
        if (xth < 0) xth = 0.0;
        // Potential gains
        double callGain = min((double)a, xth) * denom;      // with x <= xth
        double foldGain = (1.0 - pLoc) * (double)P;         // with x > xth

        bool preferCall = (callGain >= foldGain);
        int xWanted = 0;

        // Special: River with very strong hand -> shove for value
        if (r == 4 && pLoc >= 0.99) {
            xWanted = a;
            if (xWanted < 1) return {"CHECK", 0};
            return {"RAISE", xWanted};
        }

        if (preferCall) {
            double safeCallX = min((double)a, xth * 0.90);
            xWanted = (int)floor(safeCallX);
            if (xWanted < 1) {
                // cannot make a safe calling raise; fall back to fold-line small
                preferCall = false;
            }
        }
        if (!preferCall) {
            // Ensure fold by going just above threshold
            double xf = floor(xth) + 1.0;
            if (xf < 1.0) xf = 1.0;
            if (xf > a) {
                // Cannot force fold; try maximum call-raise allowed
                double safeCallX = min((double)a, xth * 0.90);
                xWanted = (int)floor(safeCallX);
            } else {
                xWanted = (int)xf;
            }
        }

        if (xWanted < 1 || xWanted > a) return {"CHECK", 0};

        // Additional conservative thresholds by round
        double thr = 0.5;
        if (r == 1) thr = 0.60;
        else if (r == 2) thr = invested > 0 ? 0.55 : 0.58;
        else if (r == 3) thr = invested > 0 ? 0.54 : 0.56;
        else if (r == 4) thr = invested > 0 ? 0.51 : 0.52;

        if (pLoc < thr) return {"CHECK", 0};
        return {"RAISE", xWanted};
    };

    if (st.r == 1) {
        // Preflop: use heuristic; avoid RATE to save budget
        bool suited = (st.c1.suit == st.c2.suit);
        double papprox = preflopApproxEquity(st.c1.val, st.c2.val, suited);
        // Decide raising using approximate p
        auto act = computeRaise(papprox);
        if (act.first == "RAISE") return act;
        return {"CHECK", 0};
    } else {
        // Postflop: consider RATE only if pot is meaningful or we've invested
        bool shouldQuery = (invested > 0) || (P >= 30);
        int base_t = 0;
        if (shouldQuery) {
            if (st.r == 2) base_t = 90;
            else if (st.r == 3) base_t = 120;
            else base_t = 150;
        }
        pair<double,double> rd = { -1.0, 0.0 };
        if (base_t > 0) {
            rd = askRate(base_t);
            if (rd.first >= 0.0) {
                e = rd.first; d = rd.second; haveRates = true;
            }
        }
        if (haveRates) {
            p = e + d * 0.5;
            auto act = computeRaise(p);
            if (act.first == "RAISE") return act;
            return {"CHECK", 0};
        } else {
            // Without rates, be conservative: check unless we've invested and have river (attempt small value)
            if (st.r == 4 && invested > 0) {
                // Small value attempt: raise 1 to possibly get called by worse per opponent policy (rare)
                if (a >= 1) return {"RAISE", 1};
            }
            return {"CHECK", 0};
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int G;
    if (!(cin >> G)) return 0;
    if (G == -1) return 0;

    string tok;
    while (readToken(tok)) {
        if (tok == "STATE") {
            State st;
            cin >> st.h >> st.r >> st.a >> st.b >> st.P >> st.k;
            string t2; readToken(t2); // ALICE
            if (t2 != "ALICE") {
                // Unexpected; try to continue
            }
            cin >> st.c1.suit >> st.c1.val >> st.c2.suit >> st.c2.val;
            string t3; readToken(t3); // BOARD
            if (t3 != "BOARD") {
                // Unexpected; try to continue
            }
            st.board.clear();
            for (int i = 0; i < st.k; ++i) {
                Card c; cin >> c.suit >> c.val;
                st.board.push_back(c);
            }

            auto act = decideAction(st);
            if (act.first == "RAISE") {
                cout << "ACTION RAISE " << act.second << endl;
            } else if (act.first == "FOLD") {
                cout << "ACTION FOLD" << endl;
            } else {
                cout << "ACTION CHECK" << endl;
            }
            cout.flush();
        } else if (tok == "RATES") {
            // Should be handled within askRate; if appears here, consume it
            double w, d; cin >> w >> d;
        } else if (tok == "OPP") {
            string what; readToken(what);
            if (what == "CALL") {
                int x; cin >> x;
            } else if (what == "FOLD") {
                // nothing
            } else if (what == "CHECK") {
                // nothing
            }
        } else if (tok == "RESULT") {
            int delta; cin >> delta;
        } else if (tok == "SCORE") {
            double W; cin >> W;
            break;
        } else if (tok == "ALICE") {
            // consume misplaced ALICE line defensively
            Card c1, c2;
            cin >> c1.suit >> c1.val >> c2.suit >> c2.val;
        } else if (tok == "BOARD") {
            // consume potential board line defensively; unknown k -> skip line? cannot without k
            // Do nothing special; rely on protocol correctness
        } else {
            // Unknown token; ignore or attempt to continue
        }
    }
    return 0;
}