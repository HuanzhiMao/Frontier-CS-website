#include <bits/stdc++.h>
using namespace std;

static const long long RATE_BUDGET_MAX = 3000000LL;

struct State {
    int h, r;
    int a, b;
    int P;
    int k;
    int s1, v1, s2, v2;
    vector<pair<int,int>> board;
};

inline int clampInt(int x, int lo, int hi){
    if(x < lo) return lo;
    if(x > hi) return hi;
    return x;
}
inline double clampDouble(double x, double lo, double hi){
    if(x < lo) return lo;
    if(x > hi) return hi;
    return x;
}

int computeT(int r, int P) {
    // Base samples per street with mild pot sensitivity
    int base[5] = {0, 60, 65, 70, 80};
    int extra = min(60, P / 25 * 8); // +8 per 25 chips in pot (capped)
    return base[r] + extra;
}

int computeValueRaiseSize(double p, double d, int r, int P, int a) {
    double A = 2.0*p + d - 1.0;
    // Fractions tuned per street
    double baseF[5] = {0, 0.8, 0.7, 0.8, 1.0};
    double multF[5] = {0, 4.0, 3.0, 3.0, 3.5};
    double minF[5]  = {0, 0.5, 0.6, 0.7, 0.9};
    double maxF[5]  = {0, 2.0, 1.7, 2.0, 2.5};
    double frac = clampDouble(baseF[r] + multF[r] * A, minF[r], maxF[r]);

    // Slight boost for very strong hands
    if (A > 0.35) frac = max(frac, maxF[r]);

    long long x = (long long)floor(frac * (double)P + 0.5);
    x = clampInt((int)x, 1, a);
    return (int)x;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long usedBudget = 0;
    int G = -1;
    int resultsSeen = 0;

    auto readInt = [&](int &x) -> bool {
        string t;
        if(!(cin >> t)) return false;
        if (t == "-1") exit(0);
        x = stoi(t);
        return true;
    };
    auto readDouble = [&](double &x) -> bool {
        string t;
        if(!(cin >> t)) return false;
        if (t == "-1") exit(0);
        x = stod(t);
        return true;
    };

    string tok;
    while (cin >> tok) {
        if (tok == "-1") {
            return 0;
        }
        if (G == -1) {
            // First token should be number of hands
            // If not numeric, ignore (robustness), but per spec it is numeric
            bool isNum = !tok.empty() && (isdigit(tok[0]) || tok[0]=='-');
            if (isNum) {
                G = stoi(tok);
                continue;
            }
        }
        if (tok == "STATE") {
            State st;
            // STATE h r a b P k
            if (!readInt(st.h)) return 0;
            if (!readInt(st.r)) return 0;
            if (!readInt(st.a)) return 0;
            if (!readInt(st.b)) return 0;
            if (!readInt(st.P)) return 0;
            if (!readInt(st.k)) return 0;

            // ALICE c1 v1 c2 v2
            string t2;
            if(!(cin >> t2)) return 0;
            if (t2 == "-1") return 0;
            // should be "ALICE"
            // Read four ints
            if (!readInt(st.s1)) return 0;
            if (!readInt(st.v1)) return 0;
            if (!readInt(st.s2)) return 0;
            if (!readInt(st.v2)) return 0;

            // BOARD ...
            string t3;
            if(!(cin >> t3)) return 0;
            if (t3 == "-1") return 0;
            // should be "BOARD"
            st.board.clear();
            for (int i = 0; i < st.k; i++) {
                int cs, cv;
                if (!readInt(cs)) return 0;
                if (!readInt(cv)) return 0;
                st.board.emplace_back(cs, cv);
            }

            // Decide whether to query RATE
            double w = 0.5, d = 0.0;
            long long remain = RATE_BUDGET_MAX - usedBudget;
            if (remain > 0) {
                int t = computeT(st.r, st.P);
                if (remain < t) t = (int)remain;
                t = max(t, 1);
                usedBudget += t;
                cout << "RATE " << t << endl;
                // Expect "RATES w d"
                string rTok;
                if(!(cin >> rTok)) return 0;
                if (rTok == "-1") return 0;
                // Robust: if not "RATES", attempt to synchronize (but per spec it will be)
                // Read doubles
                if (!readDouble(w)) return 0;
                if (!readDouble(d)) return 0;
            }

            // Compute decision
            double A = 2.0*w + d - 1.0;
            double vt[5] = {0, 0.07, 0.045, 0.035, 0.03};
            bool doRaise = (A >= vt[st.r]);

            int x = 0;
            if (doRaise && st.a >= 1) {
                x = computeValueRaiseSize(w, d, st.r, st.P, st.a);
                x = clampInt(x, 1, st.a);
            }

            if (doRaise && x >= 1) {
                cout << "ACTION RAISE " << x << endl;
            } else {
                cout << "ACTION CHECK" << endl;
            }
            // After this, the judge will respond with OPP ... and either STATE ... or RESULT
        } else if (tok == "OPP") {
            string act;
            if(!(cin >> act)) return 0;
            if (act == "-1") return 0;
            if (act == "CALL") {
                int xx;
                if (!readInt(xx)) return 0; // ignore
            } else if (act == "FOLD") {
                // nothing more on this line
            } else if (act == "CHECK") {
                // nothing more
            } else {
                // unknown, ignore
            }
        } else if (tok == "RESULT") {
            int delta;
            if (!readInt(delta)) return 0;
            resultsSeen++;
        } else if (tok == "RATES") {
            // Should not occur here because we read RATES inside STATE handling
            // Consume two doubles to keep stream in sync
            double ww, dd;
            if (!readDouble(ww)) return 0;
            if (!readDouble(dd)) return 0;
        } else if (tok == "SCORE") {
            double W;
            if (!readDouble(W)) return 0;
            // Match over
            break;
        } else {
            // Unknown token; try to ignore. Could be the initial G already captured.
            // Do nothing.
        }
    }
    return 0;
}