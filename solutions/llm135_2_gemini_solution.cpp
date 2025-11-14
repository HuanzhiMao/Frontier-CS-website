#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <set>

using namespace std;

// Function to ask a query
// Returns a vector of pairs with the smallest distance
vector<pair<int, int>> ask(int x, int y, int z) {
    cout << "? " << x << " " << y << " " << z << endl;
    int r;
    cin >> r;
    vector<pair<int, int>> result;
    for (int i = 0; i < r; ++i) {
        int u, v;
        cin >> u >> v;
        if (u > v) swap(u, v);
        result.push_back({u, v});
    }
    return result;
}

// Checks if a is on the shorter arc between p and b.
// This is equivalent to checking if the arc distance d(p, a) < d(p, b).
// On a circle, if a is on the shorter arc p-b, then the triangle p-a-b has
// its longest side being p-b. Thus, {p,b} will not be among the pairs with
// the smallest distance.
bool is_between(int p, int a, int b) {
    auto pairs = ask(p, a, b);
    int p_sorted = p, b_sorted = b;
    if (p_sorted > b_sorted) swap(p_sorted, b_sorted);
    
    for (const auto& pa : pairs) {
        if (pa.first == p_sorted && pa.second == b_sorted) {
            return false;
        }
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int k, N;
    cin >> k >> N;

    if (N <= 2) {
        cout << "! ";
        for (int i = 0; i < N; ++i) cout << i << (i == N - 1 ? "" : " ");
        cout << endl;
        return 0;
    }

    int p0 = 0;
    
    // Find n1, a neighbor of p0, by finding the door closest to p0.
    int n1 = 1;
    for (int i = 2; i < N; ++i) {
        if (is_between(p0, i, n1)) {
            n1 = i;
        }
    }

    // Find n2, the other neighbor of p0, by finding the closest door among the rest.
    vector<int> others_for_n2;
    for (int i = 1; i < N; ++i) {
        if (i != n1) {
            others_for_n2.push_back(i);
        }
    }

    int n2 = others_for_n2[0];
    for (size_t i = 1; i < others_for_n2.size(); ++i) {
        if (is_between(p0, others_for_n2[i], n2)) {
            n2 = others_for_n2[i];
        }
    }
    
    vector<int> s1_doors, s2_doors;
    int p_opp = -1;

    // Partition remaining N-3 doors.
    // A door i is on n1's side of the diameter through p0 if d(i,n1) < d(i,n2).
    // This is tested by querying {i, n1, n2}. Because p0 is equidistant from its
    // neighbors n1 and n2, the bisector of segment n1-n2 is the diameter through p0.
    for (int i = 1; i < N; ++i) {
        if (i == n1 || i == n2) continue;
        
        auto pairs = ask(i, n1, n2);
        bool n1_closer = false, n2_closer = false;
        
        int sorted_i_n1_f = min(i, n1), sorted_i_n1_s = max(i, n1);
        int sorted_i_n2_f = min(i, n2), sorted_i_n2_s = max(i, n2);

        for (const auto& p : pairs) {
            if (p.first == sorted_i_n1_f && p.second == sorted_i_n1_s) n1_closer = true;
            if (p.first == sorted_i_n2_f && p.second == sorted_i_n2_s) n2_closer = true;
        }

        if (n1_closer && n2_closer) { // Equidistant: must be opposite to p0
            p_opp = i;
        } else if (n1_closer) { // Closer to n1
            s1_doors.push_back(i);
        } else { // Closer to n2
            s2_doors.push_back(i);
        }
    }

    auto sort_comp = [&](int a, int b) {
        return is_between(p0, a, b);
    };

    sort(s1_doors.begin(), s1_doors.end(), sort_comp);
    sort(s2_doors.begin(), s2_doors.end(), sort_comp);

    cout << "! " << p0;
    cout << " " << n1;
    for (int door : s1_doors) {
        cout << " " << door;
    }
    if (p_opp != -1) {
        cout << " " << p_opp;
    }
    reverse(s2_doors.begin(), s2_doors.end());
    for (int door : s2_doors) {
        cout << " " << door;
    }
    cout << " " << n2 << endl;

    return 0;
}