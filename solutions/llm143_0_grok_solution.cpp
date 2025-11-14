#include <bits/stdc++.h>
using namespace std;

int main() {
    string line;
    getline(cin, line);
    istringstream iss(line);
    int G;
    iss >> G;

    long long used = 0;
    const long long budget = 3000000;

    while (true) {
        getline(cin, line);
        if (line.empty()) continue;
        istringstream iss(line);
        string token;
        iss >> token;
        if (token == "-1") return 0;
        if (token == "SCORE") return 0;
        if (token == "OPP" || token == "RESULT") continue;
        if (token != "STATE") continue;

        int h, r, a, b, P, k;
        iss >> h >> r >> a >> b >> P >> k;

        string aline;
        getline(cin, aline);
        istringstream aiss(aline);
        string dum;
        aiss >> dum;
        int c1, v1, c2, v2;
        aiss >> c1 >> v1 >> c2 >> v2;

        string bline;
        getline(cin, bline);
        istringstream biss(bline);
        biss >> dum;
        vector<pair<int, int>> board(k);
        for (int i = 0; i < k; ++i) {
            int c, v;
            biss >> c >> v;
            board[i] = {c, v};
        }

        // Decide t
        int t = 25 * (1 << (r - 1));
        if (used + t > budget) t = budget - used;
        double w = 0.5, d = 0.0;
        if (t > 0) {
            cout << "RATE " << t << endl;
            cout.flush();
            string rline;
            getline(cin, rline);
            if (rline.find("-1") != string::npos) return 0;
            istringstream riss(rline);
            string rdum;
            riss >> rdum >> w >> d;
            used += t;
        }
        double e = w + 0.5 * d;

        int x = 0;
        if (r == 1) {
            if (e > 0.55) x = min(20, a);
            else if (e < 0.45) x = 5;
        } else if (r == 2) {
            if (e > 0.6) x = P;
            else if (e < 0.3) x = P;
        } else if (r == 3) {
            if (e > 0.55) x = P;
            else if (e < 0.35) x = 2 * P;
        } else if (r == 4) {
            if (e > 0.5) x = min(P, a);
            else if (e < 0.2) x = min(2 * P, a);
        }
        if (x > 0) {
            if (x < 1) x = 1;
            if (x > a) x = a;
            cout << "ACTION RAISE " << x << endl;
        } else {
            cout << "ACTION CHECK" << endl;
        }
        cout.flush();
    }
    return 0;
}