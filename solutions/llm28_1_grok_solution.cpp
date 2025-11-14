#include <bits/stdc++.h>
using namespace std;

int main() {
    int T;
    cin >> T;
    for(int t = 0; t < T; t++) {
        int N;
        cin >> N;
        cin.ignore(numeric_limits<streamsize>::max, '\n');
        set<string> allwords;
        for(char ch = 'a'; ch <= 'z'; ch++) {
            string S(1, ch);
            int f = 0;
            int additional = 1;
            while(true) {
                int remaining = N - allwords.size();
                if(remaining == 0) break;
                int K = f + additional;
                K = min(K, N);
                cout << "query " << S << " " << K << endl;
                cout.flush();
                string line;
                getline(cin, line);
                if(line.empty()) {
                    getline(cin, line);
                }
                istringstream iss(line);
                int k;
                iss >> k;
                vector<string> neww(k);
                for(int i = 0; i < k; i++) iss >> neww[i];
                size_t previous_size = allwords.size();
                for(auto& w : neww) {
                    allwords.insert(w);
                }
                if(allwords.size() == static_cast<size_t>(N)) break;
                if(k < K) {
                    break;
                }
                f = k;
                additional *= 2;
            }
            if(allwords.size() == static_cast<size_t>(N)) break;
        }
        cout << "answer";
        for(auto& w : allwords) {
            cout << " " << w;
        }
        cout << endl;
        cout.flush();
    }
    return 0;
}