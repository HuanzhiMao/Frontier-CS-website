#include <bits/stdc++.h>

using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<double> weights(n, 1.0);
    for(int wager = 0; wager < m; wager++) {
        string s;
        cin >> s;
        double sum0 = 0, sum1 = 0;
        for(int i = 0; i < n; i++) {
            if(s[i] == '0') sum0 += weights[i];
            else sum1 += weights[i];
        }
        char pred = (sum1 >= sum0) ? '1' : '0';
        cout << pred << endl;
        int actual;
        cin >> actual;
        for(int i = 0; i < n; i++) {
            int exp_pred = s[i] - '0';
            if(exp_pred != actual) {
                weights[i] *= 0.5;
            }
        }
    }
    return 0;
}