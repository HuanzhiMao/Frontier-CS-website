#include <bits/stdc++.h>
using namespace std;

void add_moves(int r1, int c1, int r2, int c2, vector<string>& ops) {
    int dr = r2 - r1;
    int dc = c2 - c1;
    string vdir = dr > 0 ? "D" : "U";
    string hdir = dc > 0 ? "R" : "L";
    for(int k = 0; k < abs(dr); k++) ops.push_back(vdir);
    for(int k = 0; k < abs(dc); k++) ops.push_back(hdir);
}

int main() {
    int N;
    cin >> N;
    vector<vector<int>> h(N, vector<int>(N));
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            cin >> h[i][j];
        }
    }
    vector<string> ops;
    int depot_r = 9, depot_c = 9;
    add_moves(0, 0, depot_r, depot_c, ops);
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(i == depot_r && j == depot_c) continue;
            if(h[i][j] > 0) {
                add_moves(depot_r, depot_c, i, j, ops);
                ops.push_back("+" + to_string(h[i][j]));
                add_moves(i, j, depot_r, depot_c, ops);
                ops.push_back("-" + to_string(h[i][j]));
            }
        }
    }
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(i == depot_r && j == depot_c) continue;
            if(h[i][j] < 0) {
                int d = -h[i][j];
                ops.push_back("+" + to_string(d));
                add_moves(depot_r, depot_c, i, j, ops);
                ops.push_back("-" + to_string(d));
                add_moves(i, j, depot_r, depot_c, ops);
            }
        }
    }
    for(auto& s : ops) {
        cout << s << endl;
    }
    return 0;
}