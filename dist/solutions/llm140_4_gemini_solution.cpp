#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

long long B;
const long long S = 100000000;
int K, W;

struct Point {
    long long x, y;
};

vector<long long> query(const vector<Point>& probes) {
    cout << "? " << probes.size();
    for (const auto& p : probes) {
        cout << " " << p.x << " " << p.y;
    }
    cout << endl;
    int n_distances = K * probes.size();
    vector<long long> distances(n_distances);
    for (int i = 0; i < n_distances; ++i) {
        cin >> distances[i];
    }
    return distances;
}

vector<pair<long long, long long>> get_pairs(const vector<long long>& dists) {
    vector<long long> sorted_dists = dists;
    sort(sorted_dists.begin(), sorted_dists.end());
    vector<pair<long long, long long>> pairs;
    vector<bool> used(sorted_dists.size(), false);
    for (size_t i = 0; i < sorted_dists.size(); ++i) {
        if (used[i]) continue;
        for (size_t j = i + 1; j < sorted_dists.size(); ++j) {
            if (used[j]) continue;
            if (sorted_dists[j] == sorted_dists[i] + 1) {
                pairs.push_back({sorted_dists[i], sorted_dists[j]});
                used[i] = true;
                used[j] = true;
                break;
            }
        }
    }
    return pairs;
}

vector<vector<int>> adj;
vector<int> match;
vector<bool> visited;

bool dfs_match(int u) {
    if (visited[u]) {
        return false;
    }
    visited[u] = true;
    for (int v : adj[u]) {
        if (match[v] < 0 || dfs_match(match[v])) {
            match[v] = u;
            return true;
        }
    }
    return false;
}

void max_bipartite_matching(int n, int m) {
    match.assign(m, -1);
    for (int u = 0; u < n; ++u) {
        visited.assign(n, false);
        dfs_match(u);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> B >> K >> W;

    vector<long long> p_dists = query({{S, 0}, {S, 1}});
    vector<pair<long long, long long>> p_pairs = get_pairs(p_dists);

    vector<long long> q_dists = query({{-S, 0}, {-S, 1}});
    vector<pair<long long, long long>> q_pairs = get_pairs(q_dists);

    adj.assign(K, vector<int>());
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            if ((p_pairs[i].first % 2) == (q_pairs[j].first % 2)) {
                adj[i].push_back(j);
            }
        }
    }

    max_bipartite_matching(K, K);
    
    vector<int> p_to_q_match(K);
    for(int i=0; i<K; ++i) {
        if(match[i] != -1) {
            p_to_q_match[match[i]] = i;
        }
    }

    vector<pair<long long, pair<long long, long long>>> candidates(K);
    for (int i = 0; i < K; ++i) {
        long long d1 = p_pairs[i].first;
        long long d2 = q_pairs[p_to_q_match[i]].first;
        
        long long x = (d2 - d1) / 2;
        
        long long V = d1 + d2 + 1 - 2 * S;
        long long y1 = (V + 1) / 2;
        long long y2 = (1 - V) / 2;

        candidates[i] = {x, {y1, y2}};
    }
    
    vector<long long> check_dists = query({{0, 0}});
    sort(check_dists.begin(), check_dists.end());

    int k1 = K / 2;
    int k2 = K - k1;

    map<vector<long long>, int> half1_sums;
    for (int i = 0; i < (1 << k1); ++i) {
        vector<long long> current_sums;
        for (int j = 0; j < k1; ++j) {
            long long x = candidates[j].first;
            long long y = ((i >> j) & 1) ? candidates[j].second.second : candidates[j].second.first;
            current_sums.push_back(abs(x) + abs(y));
        }
        sort(current_sums.begin(), current_sums.end());
        half1_sums[current_sums] = i;
    }
    
    int final_mask = -1;

    for (int i = 0; i < (1 << k2); ++i) {
        vector<long long> s2;
        for (int j = 0; j < k2; ++j) {
            long long x = candidates[k1 + j].first;
            long long y = ((i >> j) & 1) ? candidates[k1 + j].second.second : candidates[k1 + j].second.first;
            s2.push_back(abs(x) + abs(y));
        }

        multiset<long long> T(check_dists.begin(), check_dists.end());
        bool possible = true;
        for(long long val : s2) {
            auto it = T.find(val);
            if (it == T.end()) {
                possible = false;
                break;
            }
            T.erase(it);
        }

        if(possible) {
            vector<long long> s1(T.begin(), T.end());
            if (half1_sums.count(s1)) {
                final_mask = (i << k1) | half1_sums[s1];
                break;
            }
        }
    }
    
    cout << "!";
    for (int i = 0; i < K; ++i) {
        long long x = candidates[i].first;
        long long y = ((final_mask >> i) & 1) ? candidates[i].second.second : candidates[i].second.first;
        cout << " " << x << " " << y;
    }
    cout << endl;

    return 0;
}