#include <bits/stdc++.h>
using namespace std;

static const long long MAX_RATE_BUDGET = 3000000;

struct State {
    int h, r, a, b, P, k;
    int as1, av1, as2, av2;
    vector<pair<int,int>> board;
};

vector<string> split(const string& s) {
    vector<string> tok;
    string t;
    istringstream iss(s);
    while (iss >> t) tok.push_back(t);
    return tok;
}

struct RateEstimator {
    long long used = 0;

    pair<double,double> askOnce(int t) {
        if (t <= 0) return {0.5, 0.0};
        if (used + t > MAX_RATE_BUDGET) {
            t = (int)max(0LL, MAX_RATE_BUDGET - used);
        }
        if (t <= 0) return {0.5, 0.0};
        used += t;
        cout << "RATE " << t << '\n' << flush;
        string line;
        while (true) {
            if (!getline(cin, line)) exit(0);
            if (line == "-1") exit(0);
            auto tok = split(line);
            if (tok.size() >= 3 && tok[0] == "RATES") {
                double w = stod(tok[1]);
                double d = stod(tok[2]);
                return {w, d};
            }
            // Unexpected line here would violate protocol; ignore just in case.
        }
    }

    // Returns equity estimate e = w + 0.5*d using possibly multiple queries.
    double estimateE(int k) {
        // Base and target samples per street
        int baseT, targetT;
        if (k == 0)      { baseT = 40; targetT = 160; }
        else if (k == 3) { baseT = 35; targetT = 120; }
        else if (k == 4) { baseT = 45; targetT = 160; }
        else             { baseT = 60; targetT = 220; } // k == 5

        // if near budget, reduce
        long long remain = max(0LL, MAX_RATE_BUDGET - used);
        if (remain < baseT) baseT = (int)remain;

        if (baseT <= 0) return 0.5;

        auto r1 = askOnce(baseT);
        double wSum = r1.first * baseT;
        double dSum = r1.second * baseT;
        int tSum = baseT;
        double e = (wSum + 0.5 * dSum) / tSum;

        // Margins per street to decide if we need refine sampling
        double margin;
        if (k == 0)      margin = 0.020;
        else if (k == 3) margin = 0.020;
        else if (k == 4) margin = 0.015;
        else             margin = 0.010;

        // Refine only if close to 0.5 and budget allows
        if (fabs(e - 0.5) < 1.5 * margin && tSum < targetT) {
            int add = targetT - tSum;
            remain = max(0LL, MAX_RATE_BUDGET - used);
            if (add > (int)remain) add = (int)remain;
            if (add > 0) {
                auto r2 = askOnce(add);
                wSum += r2.first * add;
                dSum += r2.second * add;
                tSum += add;
                e = (wSum + 0.5 * dSum) / tSum;
            }
        }
        return e;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    if (!getline(cin, line)) return 0;
    if (line == "-1") return 0;
    int G = 0;
    {
        auto tok = split(line);
        if (tok.empty()) return 0;
        G = stoi(tok[0]);
    }

    RateEstimator estimator;
    bool locked = false; // If our previous raise in this hand was called, we will CHECK for the rest of the hand
    int currentHand = 0;

    while (getline(cin, line)) {
        if (line == "-1") return 0;
        if (line.empty()) continue;
        auto tok = split(line);
        if (tok.empty()) continue;

        if (tok[0] == "STATE") {
            // Parse state
            State st;
            st.h = stoi(tok[1]);
            st.r = stoi(tok[2]);
            st.a = stoi(tok[3]);
            st.b = stoi(tok[4]);
            st.P = stoi(tok[5]);
            st.k = stoi(tok[6]);

            // New hand resets lock
            if (st.h != currentHand) {
                currentHand = st.h;
                locked = false;
            }

            // Read ALICE line
            string la;
            if (!getline(cin, la)) return 0;
            if (la == "-1") return 0;
            auto ta = split(la);
            // Expected: ALICE c1 v1 c2 v2
            if (ta.size() >= 5 && ta[0] == "ALICE") {
                st.as1 = stoi(ta[1]);
                st.av1 = stoi(ta[2]);
                st.as2 = stoi(ta[3]);
                st.av2 = stoi(ta[4]);
            } else {
                // Protocol violation; but attempt to continue
                st.as1 = st.av1 = st.as2 = st.av2 = 0;
            }

            // Read BOARD line
            string lb;
            if (!getline(cin, lb)) return 0;
            if (lb == "-1") return 0;
            auto tb = split(lb);
            st.board.clear();
            if (!tb.empty() && tb[0] == "BOARD") {
                for (size_t i = 1; i + 1 < tb.size(); i += 2) {
                    int s = stoi(tb[i]);
                    int v = stoi(tb[i + 1]);
                    st.board.emplace_back(s, v);
                }
            }

            // Decide action
            string action;
            if (locked || st.a <= 0) {
                action = "CHECK";
            } else {
                double e = estimator.estimateE(st.k);

                // Margins per street
                double margin;
                if (st.k == 0)      margin = 0.020;
                else if (st.k == 3) margin = 0.020;
                else if (st.k == 4) margin = 0.015;
                else                margin = 0.010;

                if (e > 0.5 + margin) {
                    int x = st.a;
                    if (x < 1) action = "CHECK";
                    else action = string("RAISE ") + to_string(x);
                } else {
                    action = "CHECK";
                }
            }

            cout << "ACTION " << action << '\n' << flush;
        } else if (tok[0] == "OPP") {
            // Update lock state on call
            if (tok.size() >= 2) {
                if (tok[1] == "CALL") {
                    locked = true;
                }
                // FOLD and CHECK do not change lock
            }
        } else if (tok[0] == "RESULT") {
            // Hand ends, reset lock for safety (though next STATE changes hand anyway)
            locked = false;
        } else if (tok[0] == "SCORE") {
            // Match ended
            break;
        } else if (tok[0] == "RATES") {
            // Normally handled within askOnce; ignore any stray
            continue;
        } else {
            // Ignore any other lines
            continue;
        }
    }

    return 0;
}