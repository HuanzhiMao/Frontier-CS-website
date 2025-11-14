#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <set>

using namespace std;

int N;

// Helper to ask a query and parse the response
set<pair<int, int>> ask(int x, int y, int z) {
    cout << "? " << x << " " << y << " " << z << endl;
    int r;
    cin >> r;
    set<pair<int, int>> result;
    for (int i = 0; i < r; ++i) {
        int u, v;
        cin >> u >> v;
        if (u > v) swap(u, v);
        result.insert({u, v});
    }
    return result;
}

// Checks if c is on the shorter arc between a and b
bool is_between(int c, int a, int b) {
    set<pair<int, int>> closest_pairs = ask(a, b, c);
    if (a > b) swap(a, b);
    return closest_pairs.find({a, b}) == closest_pairs.end();
}

void solve() {
    vector<int> L;

    if (N <= 2) {
        L.resize(N);
        iota(L.begin(), L.end(), 0);
    } else {
        if (is_between(1, 0, 2)) {
            L = {0, 1, 2};
        } else if (is_between(0, 1, 2)) {
            L = {1, 0, 2};
        } else {
            L = {0, 2, 1};
        }
    }
    
    for (int i = 3; i < N; ++i) {
        int door = i;
        int head = L[0];
        int tail = L.back();
        
        bool door_on_shorter_arc = is_between(door, head, tail);
        bool chain_on_shorter_arc = is_between(L[1], head, tail);

        if (door_on_shorter_arc == chain_on_shorter_arc) {
            // Door is outside the chain's arc, extend the chain
            if (is_between(head, door, L[1])) {
                L.insert(L.begin(), door);
            } else {
                L.push_back(door);
            }
        } else {
            // Door is inside the chain's arc, binary search for position
            int low = 0, high = L.size() - 1;
            while (high - low > 1) {
                int mid = low + (high - low) / 2;
                if (is_between(door, L[low], L[mid])) {
                    high = mid;
                } else {
                    low = mid;
                }
            }
            L.insert(L.begin() + high, door);
        }
    }

    cout << "! ";
    for (int i = 0; i < N; ++i) {
        cout << L[i] << (i == N - 1 ? "" : " ");
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int k;
    cin >> k >> N;
    solve();

    return 0;
}