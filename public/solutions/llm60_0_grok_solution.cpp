#include <bits/stdc++.h>
using namespace std;

int main() {
    const int L = 100000;
    const int STEP = 199;
    const int P = 502;
    vector<int> probes;
    for(int k=1; k<=P; k++){
        long long x = (long long)STEP * k - 1;
        if(x > L) break;
        probes.push_back(x);
    }
    int x1 = -1;
    double l1 = 0;
    for(int px : probes){
        cout << "query " << px << " 0 " << px << " " << L << endl;
        cout.flush();
        double len;
        cin >> len;
        if(len > 0.001){
            x1 = px;
            l1 = len;
            break;
        }
    }
    int x2 = -1;
    double l2 = 0;
    int cand = x1 + 1;
    if(cand <= L){
        cout << "query " << cand << " 0 " << cand << " " << L << endl;
        cout.flush();
        cin >> l2;
        if(l2 > 0.001){
            x2 = cand;
        }
    }
    if(x2 == -1){
        cand = x1 - 1;
        if(cand >= 0){
            cout << "query " << cand << " 0 " << cand << " " << L << endl;
            cout.flush();
            cin >> l2;
            if(l2 > 0.001){
                x2 = cand;
            }
        }
    }
    double diff = (l2*l2 - l1*l1) / (4.0 * (x1 - x2));
    double cx_d = (x1 + x2 - diff) / 2.0;
    int cx = round(cx_d);
    cout << "query " << cx << " 0 " << cx << " " << L << endl;
    cout.flush();
    double l_max;
    cin >> l_max;
    int r = round(l_max / 2.0);
    int m = 50000;
    cout << "query " << cx << " 0 " << cx << " " << m << endl;
    cout.flush();
    double l;
    cin >> l;
    double cy_comp = m + r - l;
    if(cy_comp <= m - r + 0.001){
        int m2 = m - 1;
        cout << "query " << cx << " 0 " << cx << " " << m2 << endl;
        cout.flush();
        double l2_;
        cin >> l2_;
        int cy = round(m2 + r - l2_);
        cout << "answer " << cx << " " << cy << " " << r << endl;
        cout.flush();
    }else{
        int cy = round(cy_comp);
        cout << "answer " << cx << " " << cy << " " << r << endl;
        cout.flush();
    }
    return 0;
}