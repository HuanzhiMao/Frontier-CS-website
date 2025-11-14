#include <bits/stdc++.h>
using namespace std;

static const long long RATE_BUDGET_TOTAL = 3000000;

struct State {
    int h, r, a, b, P, k;
    int as1, av1, as2, av2;
    vector<pair<int,int>> board; // (suit, value)
};

long long budgetSpent = 0;

bool getline_valid(string &s) {
    if (!std::getline(cin, s)) return false;
    if (s == "-1") exit(0);
    return true;
}

vector<string> split(const string& s) {
    vector<string> t;
    string cur;
    for (char c : s) {
        if (isspace((unsigned char)c)) {
            if (!cur.empty()) { t.push_back(cur); cur.clear(); }
        } else cur.push_back(c);
    }
    if (!cur.empty()) t.push_back(cur);
    return t;
}

pair<double,double> askRates(int t) {
    if (t <= 0) return {0.5, 0.0};
    cout << "RATE " << t << endl;
    cout.flush();
    string line;
    while (true) {
        if (!getline_valid(line)) return {0.5, 0.0};
        auto tokens = split(line);
        if (tokens.empty()) continue;
        if (tokens[0] == "RATES" && tokens.size() >= 3) {
            double w = atof(tokens[1].c_str());
            double d = atof(tokens[2].c_str());
            return {w, d};
        }
        // ignore other lines until RATES (shouldn't happen, but be robust)
        if (tokens[0] == "-1") exit(0);
    }
}

int t_for_k(int k) {
    // Conservative budget: total ~2.7M if all 4 streets per hand across 10k hands (rare).
    if (k == 0) return 20;
    if (k == 3) return 50;
    if (k == 4) return 80;
    return 120; // k==5
}

double threshold_for_k(int k) {
    // Heuristic thresholds
    if (k == 0) return 0.545; // preflop: need stronger edge
    if (k == 3) return 0.56;  // flop
    if (k == 4) return 0.585; // turn
    return 0.52;              // river: closer to 0.5
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    if (!getline_valid(line)) return 0;
    long long G = 0;
    {
        auto tokens = split(line);
        if (!tokens.empty()) {
            G = atoll(tokens[0].c_str());
        }
    }

    while (getline_valid(line)) {
        auto tokens = split(line);
        if (tokens.empty()) continue;

        if (tokens[0] == "STATE") {
            State st{};
            if ((int)tokens.size() < 7) continue;
            st.h = atoi(tokens[1].c_str());
            st.r = atoi(tokens[2].c_str());
            st.a = atoi(tokens[3].c_str());
            st.b = atoi(tokens[4].c_str());
            st.P = atoi(tokens[5].c_str());
            st.k = atoi(tokens[6].c_str());

            // Read ALICE
            string la;
            if (!getline_valid(la)) return 0;
            auto ta = split(la);
            if (ta.size() >= 5 && ta[0] == "ALICE") {
                st.as1 = atoi(ta[1].c_str());
                st.av1 = atoi(ta[2].c_str());
                st.as2 = atoi(ta[3].c_str());
                st.av2 = atoi(ta[4].c_str());
            } else {
                // unexpected, continue
                continue;
            }

            // Read BOARD
            string lb;
            if (!getline_valid(lb)) return 0;
            auto tb = split(lb);
            st.board.clear();
            if (!tb.empty() && tb[0] == "BOARD") {
                int need = 2 * st.k;
                if ((int)tb.size() >= 1 + need) {
                    for (int i = 0; i < st.k; ++i) {
                        int s = atoi(tb[1 + 2*i].c_str());
                        int v = atoi(tb[1 + 2*i + 1].c_str());
                        st.board.emplace_back(s, v);
                    }
                }
            } else {
                // unexpected, continue
                continue;
            }

            // Decide action
            string action;

            if (st.a <= 0) {
                action = "ACTION CHECK";
            } else {
                int t = t_for_k(st.k);
                // Ensure budget not exceeded
                if (budgetSpent + t > RATE_BUDGET_TOTAL) {
                    t = max(0LL, RATE_BUDGET_TOTAL - budgetSpent);
                }
                pair<double,double> wd = {0.5, 0.0};
                if (t > 0) {
                    wd = askRates(t);
                    budgetSpent += t;
                }
                double w = wd.first, d = wd.second;
                double e = w + 0.5 * d;

                double thr = threshold_for_k(st.k);

                if (e > thr) {
                    // Aggressive: shove
                    int x = st.a;
                    if (x < 1) x = 1;
                    action = "ACTION RAISE " + to_string(x);
                } else {
                    action = "ACTION CHECK";
                }
            }

            cout << action << endl;
            cout.flush();
        } else if (tokens[0] == "RATES") {
            // Normally handled immediately after RATE; ignore here.
            continue;
        } else if (tokens[0] == "OPP") {
            // OPP FOLD / OPP CALL x / OPP CHECK -> ignore
            continue;
        } else if (tokens[0] == "RESULT") {
            // RESULT delta -> ignore
            continue;
        } else if (tokens[0] == "SCORE") {
            // End of match
            break;
        }
    }
    return 0;
}