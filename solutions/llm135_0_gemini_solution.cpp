#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <numeric>
#include <map>

using namespace std;

// Forward declaration for the recursive sort function
vector<int> sort_arc(int a, int b, vector<int>& doors);

// Function to ask a query and parse the response.
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

// Checks if y is on the shorter arc between x and z.
// This is true if the distance between x and z is not minimal among the three pairs.
// {x, z} not being a minimal pair implies y "breaks" the arc x-z.
bool is_between(int x, int y, int z) {
    auto res = ask(x, y, z);
    int u = min(x, z), v = max(x, z);
    for (const auto& p : res) {
        if (p.first == u && p.second == v) {
            return false;
        }
    }
    return true;
}

// Recursively sorts the doors on the shorter arc between a and b.
vector<int> sort_arc(int a, int b, vector<int>& doors) {
    if (doors.empty()) {
        return {};
    }

    // Pick a pivot (here, the first element for simplicity)
    int pivot = doors[0];
    vector<int> left_arc_doors, right_arc_doors;

    // Partition remaining doors based on which side of the pivot they lie on.
    for (size_t i = 1; i < doors.size(); ++i) {
        if (is_between(a, doors[i], pivot)) {
            left_arc_doors.push_back(doors[i]);
        } else {
            right_arc_doors.push_back(doors[i]);
        }
    }

    // Recursively sort the two partitions.
    vector<int> sorted_left = sort_arc(a, pivot, left_arc_doors);
    vector<int> sorted_right = sort_arc(pivot, b, right_arc_doors);

    // Combine the results.
    vector<int> result;
    result.insert(result.end(), sorted_left.begin(), sorted_left.end());
    result.push_back(pivot);
    result.insert(result.end(), sorted_right.begin(), sorted_right.end());
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int k, n;
    cin >> k >> n;

    if (n <= 2) {
        cout << "!";
        for (int i = 0; i < n; ++i) cout << " " << i;
        cout << endl;
        return 0;
    }

    // Anchor points for partitioning the circle.
    int p1 = 0, p2 = 1;

    // Partition all other doors based on which arc between p1 and p2 they lie on.
    vector<int> s_short, s_long;
    for (int i = 2; i < n; ++i) {
        if (is_between(p1, i, p2)) {
            s_short.push_back(i);
        } else {
            s_long.push_back(i);
        }
    }

    // Sort doors on each arc.
    vector<int> sorted_short = sort_arc(p1, p2, s_short);
    vector<int> sorted_long = sort_arc(p2, p1, s_long);

    // Output the final circular order.
    cout << "! " << p1;
    for (int node : sorted_short) cout << " " << node;
    cout << " " << p2;
    for (int node : sorted_long) cout << " " << node;
    cout << endl;

    return 0;
}