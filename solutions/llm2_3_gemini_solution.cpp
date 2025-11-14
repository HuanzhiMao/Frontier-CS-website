#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

int n;
vector<int> p;

int ask(const vector<int>& q) {
    cout << "0 ";
    for (int i = 0; i < n; ++i) {
        cout << q[i] << (i == n - 1 ? "" : " ");
    }
    cout << endl;
    int x;
    cin >> x;
    return x;
}

void guess() {
    cout << "1 ";
    for (int i = 0; i < n; ++i) {
        cout << p[i] << (i == n - 1 ? "" : " ");
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n;
    p.resize(n);
    vector<bool> found(n + 1, false);

    vector<int> p_inv(n + 1, 0);
    vector<int> last_unpaired;

    if (n % 2 != 0) {
        last_unpaired.push_back(n);
    }

    for (int v = 1; v + 1 <= n; v += 2) {
        int v1 = v;
        int v2 = v + 1;

        vector<int> q(n);
        int prev_ans;

        // Base case k=0
        for (int i = 0; i < n; ++i) {
            q[i] = v2;
        }
        prev_ans = ask(q);

        bool v1_found = false;
        bool v2_found = false;

        for (int k = 1; k <= n; ++k) {
            q[k - 1] = v1;
            int current_ans = ask(q);

            int diff = current_ans - prev_ans;
            // diff = [pos_v1 == k-1] - [pos_v2 == k-1]
            if (diff == 1) {
                p_inv[v1] = k;
                v1_found = true;
            } else if (diff == -1) {
                p_inv[v2] = k;
                v2_found = true;
            }
            prev_ans = current_ans;
            if (v1_found && v2_found) break;
        }
        
        if (!v1_found) {
            last_unpaired.push_back(v1);
        }
        if (!v2_found) {
            last_unpaired.push_back(v2);
        }
    }
    
    if (!last_unpaired.empty()){
        int last_v = last_unpaired[0];
        vector<bool> pos_taken(n + 1, false);
        for(int i=1; i<=n; ++i){
            if(p_inv[i] != 0){
                pos_taken[p_inv[i]] = true;
            }
        }
        for(int i=1; i<=n; ++i){
            if(!pos_taken[i]){
                p_inv[last_v] = i;
                break;
            }
        }
    }


    for (int i = 1; i <= n; ++i) {
        p[p_inv[i] - 1] = i;
    }

    guess();

    return 0;
}