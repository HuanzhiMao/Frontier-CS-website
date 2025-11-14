#include <bits/stdc++.h>

using namespace std;

int main() {
    int n;
    scanf("%d", &n);
    bitset<1024> P[1024];
    P[1].set(1);
    for (int v = 2; v <= n; v++) {
        bitset<1024> &bs = P[v];
        for (int w = 1; w <= n; w++) {
            printf("? 2 %d %d %d\n", w, 1, v);
            fflush(stdout);
            int res;
            scanf("%d", &res);
            if (res == -1) exit(0);
            if (res) bs.set(w);
        }
    }
    vector<pair<int, int>> edges;
    for (int v = 2; v <= n; v++) {
        bitset<1024> target = P[v];
        target.reset(v);
        int parent = -1;
        for (int u = 1; u <= n; u++) {
            if (P[u] == target) {
                parent = u;
                break; // unique
            }
        }
        edges.push_back({parent, v});
    }
    printf("!\n");
    for (auto [u, v] : edges) {
        printf("%d %d\n", u, v);
    }
    fflush(stdout);
    return 0;
}