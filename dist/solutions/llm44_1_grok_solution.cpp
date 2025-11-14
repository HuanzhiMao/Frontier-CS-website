#include <bits/stdc++.h>
using namespace std;

struct City {
    int id;
    double angle;
    long long dist_sq;
};

int main() {
    int N;
    cin >> N;
    vector<long long> X(N), Y(N);
    for (int i = 0; i < N; i++) {
        cin >> X[i] >> Y[i];
    }
    vector<City> cities;
    for (int i = 1; i < N; i++) {
        long long dx = X[i] - X[0];
        long long dy = Y[i] - Y[0];
        double ang = atan2(dy, dx);
        long long dsq = dx * dx + dy * dy;
        cities.push_back({i, ang, dsq});
    }
    sort(cities.begin(), cities.end(), [](const City& a, const City& b) {
        if (a.angle != b.angle) return a.angle < b.angle;
        return a.dist_sq < b.dist_sq;
    });
    cout << N + 1 << endl;
    cout << 0 << endl;
    for (auto& c : cities) {
        cout << c.id << endl;
    }
    cout << 0 << endl;
    return 0;
}