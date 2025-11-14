#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <set>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;

    int N = n * m;

    auto query = [](int r, int d) {
        cout << "? " << r << " " << d << endl;
        fflush(stdout);
        int result;
        cin >> result;
        return result;
    };

    vector<pair<int, int>> readings;
    
    // Rotate ring 0 N times.
    // The k-th query (1-indexed) corresponds to a state where ring 0 has been rotated k times.
    // The value k=N is equivalent to k=0.
    for (int k = 1; k <= N; ++k) {
        int a = query(0, 1);
        readings.push_back({a, k % N});
    }

    // Sort readings by the number of unblocked lasers in descending order.
    sort(readings.rbegin(), readings.rend());

    vector<int> p_initial;
    for (const auto& p : readings) {
        if (p_initial.size() == n - 1) {
            break;
        }
        int k = p.second;
        
        bool is_neighbor_of_found = false;
        for (int pk : p_initial) {
            int dist = abs(pk - k);
            if (min(dist, N - dist) < m) {
                is_neighbor_of_found = true;
                break;
            }
        }

        if (!is_neighbor_of_found) {
            p_initial.push_back(k);
        }
    }
    
    // Fallback: if the neighborhood filtering was too aggressive.
    if (p_initial.size() < n - 1) {
        p_initial.clear();
        set<int> taken_k;
        for (const auto& p : readings) {
            if (p_initial.size() == n - 1) break;
            int k = p.second;
            if (taken_k.find(k) == taken_k.end()) {
                 p_initial.push_back(k);
                 taken_k.insert(k);
            }
        }
    }

    // After N rotations, ring 0 is back where it started. So final positions are same as initial.
    cout << "!";
    sort(p_initial.begin(), p_initial.end());
    for (int i = 0; i < n - 1; ++i) {
        cout << " " << p_initial[i];
    }
    cout << endl;
    fflush(stdout);

    return 0;
}