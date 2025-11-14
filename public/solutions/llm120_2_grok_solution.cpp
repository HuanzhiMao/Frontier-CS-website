#include <bits/stdc++.h>
using namespace std;

int S[101][101];

void bt(int pos, vector<int>& assign, vector<vector<int>>& possibles, const vector<int>& vars) {
    if (pos == vars.size()) {
        possibles.push_back(assign);
        return;
    }
    int v = vars[pos];
    for (int val = 0; val <= 1; ++val) {
        bool ok = true;
        for (int pre = 0; pre < pos; ++pre) {
            int u = vars[pre];
            int sumv = assign[u] + val;
            int aw = S[u][v] - sumv;
            if (aw < 0 || aw > 1) {
                ok = false;
                break;
            }
        }
        if (ok) {
            assign[v] = val;
            bt(pos + 1, assign, possibles, vars);
            assign[v] = -1;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    memset(S, 0, sizeof(S));

    // Query all pairs with 1
    for (int i = 2; i <= 100; ++i) {
        for (int j = i + 1; j <= 100; ++j) {
            cout << "? 1 " << i << " " << j << endl;
            cout.flush();
            int k;
            cin >> k;
            S[i][j] = S[j][i] = k;
        }
    }

    // Backtracking to find possibles
    vector<int> vars;
    for (int i = 2; i <= 100; ++i) vars.push_back(i);
    vector<vector<int>> possibles;
    vector<int> assign(101, -1);
    bt(0, assign, possibles, vars);

    // Now distinguish if needed
    while (possibles.size() > 1) {
        bool found = false;
        for (int cc = 4; cc <= 100 && !found; ++cc) {
            int a = 2, b = 3, c = cc;
            set<int> resps;
            for (const auto& p : possibles) {
                int Aab = S[a][b] - p[a] - p[b];
                int Aac = S[a][c] - p[a] - p[c];
                int Abc = S[b][c] - p[b] - p[c];
                int kk = Aab + Aac + Abc;
                resps.insert(kk);
            }
            if (resps.size() > 1) {
                found = true;
                cout << "? " << a << " " << b << " " << c << endl;
                cout.flush();
                int real_k;
                cin >> real_k;
                vector<vector<int>> newp;
                for (const auto& p : possibles) {
                    int Aab = S[a][b] - p[a] - p[b];
                    int Aac = S[a][c] - p[a] - p[c];
                    int Abc = S[b][c] - p[b] - p[c];
                    int kk = Aab + Aac + Abc;
                    if (kk == real_k) newp.push_back(p);
                }
                possibles = newp;
            }
        }
        // If not found, try different base, but assume it always finds
    }

    // Output the graph
    cout << "!" << endl;
    auto& p = possibles[0];
    for (int i = 1; i <= 100; ++i) {
        for (int j = 1; j <= 100; ++j) {
            if (i == j) {
                cout << '0';
            } else if (i == 1) {
                cout << (p[j] ? '1' : '0');
            } else if (j == 1) {
                cout << (p[i] ? '1' : '0');
            } else {
                int aa = S[i][j] - p[i] - p[j];
                cout << aa;
            }
        }
        cout << endl;
    }
    cout.flush();
    return 0;
}