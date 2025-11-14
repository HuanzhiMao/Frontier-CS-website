#include <iostream>
#include <vector>
#include <numeric>
#include <complex>
#include <algorithm>
#include <cmath>

using namespace std;

const double PI = acos(-1.0);

using Complex = complex<double>;

void fft(vector<Complex>& a, bool invert) {
    int n = a.size();
    if (n <= 1) return;

    vector<int> rev(n);
    for (int i = 0; i < n; i++) {
        rev[i] = rev[i >> 1] >> 1;
        if (i & 1) {
            rev[i] |= n >> 1;
        }
        if (i < rev[i]) {
            swap(a[i], a[rev[i]]);
        }
    }

    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        Complex wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            Complex w(1);
            for (int j = 0; j < len / 2; j++) {
                Complex u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (auto& x : a) {
            x /= n;
        }
    }
}


int query(int r, int d) {
    cout << "? " << r << " " << d << endl;
    int res;
    cin >> res;
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    long long m;
    cin >> n >> m;

    int total_sections = n * m;

    // Get a baseline reading at the state after one rotation on ring 0
    // This makes our reference point s_0^{init}+1
    int unblocked_after_one_rot = query(0, 1);
    
    vector<long long> b(total_sections);
    b[0] = total_sections - unblocked_after_one_rot;

    for (int k = 1; k < total_sections; ++k) {
        int current_unblocked = query(0, 1);
        b[k] = total_sections - current_unblocked;
    }
    // After loop, ring 0 is rotated total_sections times from start, so it's back to initial state
    // But our sequence b corresponds to states s_0+1, s_0+2, ..., s_0+nm.

    long long sum_b = 0;
    for (long long val : b) {
        sum_b += val;
    }
    
    double U_not_0_size = 0;
    if (total_sections - m > 0) {
        U_not_0_size = (double)(sum_b - (long long)total_sections * m) / (total_sections - m);
    }
    
    vector<Complex> g(total_sections);
    for (int k = 0; k < total_sections; ++k) {
        g[k] = Complex(m + U_not_0_size - b[k], 0);
    }

    vector<Complex> I_C0(total_sections, {0, 0});
    for (int i = 0; i < m; ++i) {
        I_C0[i] = {1, 0};
    }
    
    int N = 1;
    while (N < total_sections) N <<= 1;
    g.resize(N, {0,0});
    I_C0.resize(N, {0,0});

    fft(g, false);
    fft(I_C0, false);

    vector<Complex> H(N);
    for (int i = 0; i < N; ++i) {
        Complex denom = conj(I_C0[i]) * I_C0[i];
        if (abs(denom) < 1e-9) {
            H[i] = {0, 0};
        } else {
            H[i] = g[i] / denom;
        }
    }

    fft(H, true);
    
    vector<pair<double, int>> p_candidates;
    for (int i = 0; i < total_sections; ++i) {
        p_candidates.push_back({H[i].real(), i});
    }

    sort(p_candidates.rbegin(), p_candidates.rend());

    vector<int> p;
    for (int i = 0; i < total_sections && p.size() < n - 1; ++i) {
        if (p_candidates[i].first > 0.5) {
            // The found peak is at index p_found.
            // p_found = s_i^{init} - s_0^{ref} where s_0^{ref} = s_0^{init} + 1.
            // So p_found = s_i^{init} - (s_0^{init} + 1) = P_i - 1.
            // P_i = p_found + 1.
            int P_i = (p_candidates[i].second + 1) % total_sections;
            
            // Final positions are s_0^{final} = s_0^{init} + total_sections, s_i^{final} = s_i^{init}.
            // So s_0^{final} = s_0^{init}.
            // We need p_i s.t. s_0^{final} + p_i = s_i^{final}.
            // s_0^{init} + p_i = s_i^{init} => p_i = s_i^{init} - s_0^{init} = P_i.
            p.push_back(P_i);
        }
    }
    
    cout << "! ";
    for (int i = 0; i < n - 1; ++i) {
        cout << p[i] << (i == n - 2 ? "" : " ");
    }
    cout << endl;

    return 0;
}