#include <bits/stdc++.h>

using namespace std;

struct Point {
    int x, y;
};

int manh(const Point& a, const Point& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

int main() {
    vector<Point> picks(1000), dels(1000);
    for(int i=0; i<1000; i++) {
        cin >> picks[i].x >> picks[i].y >> dels[i].x >> dels[i].y;
    }
    Point O = {400, 400};
    vector<Point> path = {O, O};
    vector<int> selected;
    vector<int> used(1000, 0);
    while(selected.size() < 50) {
        long long best_delta = LLONG_MAX / 2;
        int best_i = -1;
        int best_k = -1;
        int best_m = -1;
        for(int ii=0; ii<1000; ii++) {
            if(used[ii]) continue;
            int cur_n = path.size();
            long long min_d = LLONG_MAX / 2;
            int loc_k = -1, loc_m = -1;
            for(int k=0; k<cur_n-1; k++) {
                Point A = path[k];
                Point B = path[k+1];
                Point P = picks[ii];
                Point D = dels[ii];
                long long delta_p = (long long)manh(A, P) + manh(P, B) - manh(A, B);
                for(int m=k+1; m<cur_n; m++) {
                    Point C, E;
                    if(m == k+1) {
                        C = P;
                        E = path[m];
                    } else {
                        C = path[m-1];
                        E = path[m];
                    }
                    long long old_man = manh(C, E);
                    long long new1 = manh(C, D);
                    long long new2 = manh(D, E);
                    long long delta_d = new1 + new2 - old_man;
                    long long total_d = delta_p + delta_d;
                    if(total_d < min_d) {
                        min_d = total_d;
                        loc_k = k;
                        loc_m = m;
                    }
                }
            }
            if(min_d < best_delta) {
                best_delta = min_d;
                best_i = ii;
                best_k = loc_k;
                best_m = loc_m;
            }
        }
        used[best_i] = 1;
        selected.push_back(best_i + 1);
        Point P = picks[best_i];
        Point D = dels[best_i];
        int cur_n = path.size();
        vector<Point> new_path;
        for(int j=0; j<=best_k; j++) new_path.push_back(path[j]);
        new_path.push_back(P);
        for(int j=best_k+1; j<cur_n; j++) new_path.push_back(path[j]);
        vector<Point> final_path;
        int new_n = new_path.size();
        for(int j=0; j<=best_m; j++) final_path.push_back(new_path[j]);
        final_path.push_back(D);
        for(int j=best_m+1; j<new_n; j++) final_path.push_back(new_path[j]);
        path = final_path;
        vector<int> temp_sel = selected;
        sort(temp_sel.begin(), temp_sel.end());
        cout << temp_sel.size();
        for(int r : temp_sel) cout << " " << r;
        cout << endl;
        int nn = path.size();
        cout << nn;
        for(auto p : path) cout << " " << p.x << " " << p.y;
        cout << endl;
    }
    return 0;
}