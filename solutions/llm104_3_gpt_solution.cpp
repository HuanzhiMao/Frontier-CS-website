#include <bits/stdc++.h>
using namespace std;

static inline void flush_out() {
    cout.flush();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    if (!(cin >> t)) return 0;
    std::mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    
    while (t--) {
        int n;
        cin >> n;
        
        // States for each candidate: last two statuses (0=Honest,1=Dishonest) and length
        vector<unsigned char> last1(n+1, 0), last2(n+1, 0), len(n+1, 0);
        vector<char> alive(n+1, 1);
        int alive_cnt = n;
        
        auto ask = [&](int l, int r)->int{
            cout << "? " << l << " " << r << "\n";
            flush_out();
            int x; 
            if (!(cin >> x)) exit(0);
            return x;
        };
        auto control = [&]()->int{
            int x = ask(1, n);
            int delt = x - (n - 1); // 0 if honest, 1 if dishonest
            return delt;
        };
        auto apply_update_all = [&](int h_control){
            for (int i = 1; i <= n; ++i) if (alive[i]) {
                if (len[i] >= 2 && last1[i] == h_control && last2[i] == h_control) {
                    alive[i] = 0;
                    --alive_cnt;
                } else {
                    last2[i] = last1[i];
                    last1[i] = (unsigned char)h_control;
                    if (len[i] < 2) ++len[i];
                }
            }
        };
        auto apply_update_test = [&](int l, int r, int delta){
            for (int i = 1; i <= n; ++i) if (alive[i]) {
                int inside = (l <= i && i <= r) ? 1 : 0;
                int h = inside ? delta : (1 - delta);
                if (len[i] >= 2 && last1[i] == h && last2[i] == h) {
                    alive[i] = 0;
                    --alive_cnt;
                } else {
                    last2[i] = last1[i];
                    last1[i] = (unsigned char)h;
                    if (len[i] < 2) ++len[i];
                }
            }
        };
        
        auto guess = [&](int a)->int{
            cout << "! " << a << "\n";
            flush_out();
            int y;
            if (!(cin >> y)) exit(0);
            return y;
        };
        
        auto end_case = [&](){
            cout << "#\n";
            flush_out();
        };
        
        // Query limit
        double base = 1.116;
        int qlim = (int)(2.0 * ceil(log((double)n) / log(base)));
        if (qlim < 1) qlim = 1;
        int used = 0;
        
        // Strategy: alternate control and random half-split tests, maintaining per-candidate status sequences
        while (alive_cnt > 2 && used < qlim) {
            // Control
            int hc = control();
            ++used;
            apply_update_all(hc);
            if (alive_cnt <= 2 || used >= qlim) break;
            
            // Choose a random pivot for test [1, p]
            int p = (int)(uniform_int_distribution<int>(1, n)(rng));
            int l = 1, r = p;
            if (l > r) swap(l, r);
            int x = ask(l, r);
            ++used;
            int lenq = r - l + 1;
            int delta = x - (lenq - 1); // 0 or 1
            if (delta < 0) delta = 0;
            if (delta > 1) delta = 1;
            apply_update_test(l, r, delta);
        }
        
        // Prepare guesses
        vector<int> cand;
        cand.reserve(2);
        for (int i = 1; i <= n && (int)cand.size() < 2; ++i) {
            if (alive[i]) cand.push_back(i);
        }
        if (cand.empty()) {
            // Fallback: guess 1 and 2
            cand.push_back(1);
            if (n >= 2) cand.push_back(2);
        } else if (cand.size() == 1) {
            // Add any other
            int other = (cand[0] == 1 ? 2 : 1);
            cand.push_back(other);
        }
        
        // Make up to two guesses
        int y1 = guess(cand[0]);
        if (y1 != 1) {
            if ((int)cand.size() >= 2) {
                guess(cand[1]);
            } else {
                // If only one candidate, make a second arbitrary guess if needed
                int other = (cand[0] == 1 ? 2 : 1);
                guess(other);
            }
        }
        
        end_case();
    }
    return 0;
}