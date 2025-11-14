#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

using namespace std;

int n, m;
vector<int> poles[53];
vector<pair<int, int>> moves;

map<int, int> counts[53];
bool is_sorted[51];

void make_move(int from, int to) {
    int ball = poles[from].back();
    poles[from].pop_back();
    poles[to].push_back(ball);
    moves.push_back({from, to});

    counts[from][ball]--;
    if (counts[from][ball] == 0) {
        counts[from].erase(ball);
    }
    counts[to][ball]++;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;
    for (int i = 1; i <= n; ++i) {
        poles[i].resize(m);
        for (int j = 0; j < m; ++j) {
            cin >> poles[i][j];
            counts[i][poles[i][j]]++;
        }
    }

    int num_sorted = 0;
    while (num_sorted < n) {
        // Update sorted status
        num_sorted = 0;
        for (int i = 1; i <= n; ++i) {
            if (!is_sorted[i]) {
                if (poles[i].size() == m && counts[i].size() == 1 && counts[i].begin()->first == i) {
                    is_sorted[i] = true;
                }
            }
            if (is_sorted[i]) {
                num_sorted++;
            }
        }
        if (num_sorted == n) break;

        bool moved = false;

        // Priority 1: Move a ball to its correct, clean pole ("Homing Move")
        for (int p = 1; p <= n + 1; ++p) {
            if (poles[p].empty() || (p <= n && is_sorted[p])) continue;
            
            int c = poles[p].back();
            if (p == c) continue;

            if (!is_sorted[c] && poles[c].size() < m) {
                bool c_is_clean_for_c = (counts[c].empty() || (counts[c].size() == 1 && counts[c].count(c)));
                if (c_is_clean_for_c) {
                    make_move(p, c);
                    moved = true;
                    break;
                }
            }
        }
        if (moved) continue;

        // Priority 2: Unblock a "dirty" or "wrong color" pole to the buffer
        if (poles[n + 1].size() < m) {
            int p_to_unblock = -1;
            for (int p = 1; p <= n; ++p) {
                if (!is_sorted[p] && !poles[p].empty()) {
                    bool p_is_clean_for_p = (counts[p].size() == 1 && counts[p].count(p));
                    if (!p_is_clean_for_p) {
                        p_to_unblock = p;
                        break;
                    }
                }
            }
            if (p_to_unblock != -1) {
                make_move(p_to_unblock, n + 1);
                moved = true;
                continue;
            }
        }

        // Priority 3: All non-sorted poles are clean. Move balls from buffer to home.
        if (!poles[n + 1].empty()) {
            int c = poles[n+1].back();
            if(!is_sorted[c] && poles[c].size() < m){
                 bool c_is_clean_for_c = (counts[c].empty() || (counts[c].size() == 1 && counts[c].count(c)));
                 if(c_is_clean_for_c){
                    make_move(n + 1, c);
                    moved = true;
                    continue;
                 }
            }
        }
        
        // Priority 4: Buffer is full or has unmovable balls. Move one to any available spot to prevent deadlock.
        if (!poles[n+1].empty()) {
            int k_to_dump = -1;
            for(int k=1; k<=n; ++k) {
                if (!is_sorted[k] && poles[k].size() < m) {
                    k_to_dump = k;
                    break;
                }
            }
            if (k_to_dump != -1) {
                make_move(n+1, k_to_dump);
                moved = true;
                continue;
            }
        }
    }

    cout << moves.size() << "\n";
    for (const auto& move : moves) {
        cout << move.first << " " << move.second << "\n";
    }

    return 0;
}