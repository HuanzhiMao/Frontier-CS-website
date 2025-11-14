#include <bits/stdc++.h>
using namespace std;

double dist(const vector<pair<long long, long long>>& pos, int a, int b) {
    long long dx = pos[a].first - pos[b].first;
    long long dy = pos[a].second - pos[b].second;
    return sqrt(static_cast<double>(dx * dx + dy * dy));
}

double compute_L(const vector<int>& path, const vector<pair<long long, long long>>& pos, const vector<bool>& is_prime) {
    double sum = 0.0;
    int len = path.size();
    for (int t = 1; t < len; ++t) {
        int from = path[t - 1];
        int to = path[t];
        double d = dist(pos, from, to);
        double m = 1.0;
        if (t % 10 == 0 && !is_prime[from]) m = 1.1;
        sum += m * d;
    }
    return sum;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    cin >> N;
    vector<pair<long long, long long>> pos(N);
    for (int i = 0; i < N; ++i) {
        cin >> pos[i].first >> pos[i].second;
    }
    vector<bool> is_prime(N + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (long long i = 2; i * i <= N; ++i) {
        if (is_prime[i]) {
            for (long long j = i * i; j <= N; j += i) {
                is_prime[j] = false;
            }
        }
    }
    vector<vector<int>> candidates;
    // Baseline
    {
        vector<int> path(N + 1);
        path[0] = 0;
        path[N] = 0;
        for (int i = 1; i < N; ++i) path[i] = i;
        candidates.push_back(path);
    }
    // Reverse baseline
    {
        vector<int> path(N + 1);
        path[0] = 0;
        path[N] = 0;
        for (int i = 1; i < N; ++i) path[i] = N - i;
        candidates.push_back(path);
    }
    int M = N - 1;
    vector<int> strip_sizes = {200, 450, 1000};
    for (int s : strip_sizes) {
        if (s > M || s <= 0) continue;
        int k = (M + s - 1) / s;
        for (bool flip : {false, true}) {
            vector<int> order;
            for (int j = 0; j < k; ++j) {
                int strip_idx = k - 1 - j;
                int start_city = 1 + strip_idx * s;
                int end_city = min(M, start_city + s - 1);
                vector<pair<long long, int>> group;
                for (int cid = start_city; cid <= end_city; ++cid) {
                    group.push_back({pos[cid].second, cid});
                }
                sort(group.begin(), group.end());
                bool decrease = ((j % 2 == 0) != flip);
                if (decrease) {
                    reverse(group.begin(), group.end());
                }
                for (auto& p : group) {
                    order.push_back(p.second);
                }
            }
            // Normal order
            {
                vector<int> path(N + 1);
                path[0] = 0;
                path[N] = 0;
                for (int i = 0; i < M; ++i) path[i + 1] = order[i];
                candidates.push_back(path);
            }
            // Reversed order
            reverse(order.begin(), order.end());
            {
                vector<int> path(N + 1);
                path[0] = 0;
                path[N] = 0;
                for (int i = 0; i < M; ++i) path[i + 1] = order[i];
                candidates.push_back(path);
            }
        }
    }
    double min_L = 1e100;
    vector<int> best;
    for (auto& p : candidates) {
        double l = compute_L(p, pos, is_prime);
        if (l < min_L) {
            min_L = l;
            best = p;
        }
    }
    cout << N + 1 << endl;
    for (int city : best) {
        cout << city << endl;
    }
    return 0;
}