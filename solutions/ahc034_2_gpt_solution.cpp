#include <bits/stdc++.h>
using namespace std;

using ll = long long;

vector<pair<int,int>> makeRowPath(int N){
    vector<pair<int,int>> path;
    path.reserve(N*N);
    for(int i=0;i<N;i++){
        if(i%2==0){
            for(int j=0;j<N;j++) path.emplace_back(i,j);
        }else{
            for(int j=N-1;j>=0;j--) path.emplace_back(i,j);
        }
    }
    return path;
}

vector<pair<int,int>> makeColPath(int N){
    vector<pair<int,int>> path;
    path.reserve(N*N);
    for(int j=0;j<N;j++){
        if(j%2==0){
            for(int i=0;i<N;i++) path.emplace_back(i,j);
        }else{
            for(int i=N-1;i>=0;i--) path.emplace_back(i,j);
        }
    }
    return path;
}

pair<int,ll> evaluatePath(const vector<pair<int,int>>& path, const vector<vector<int>>& h){
    int M = (int)path.size();
    vector<int> b(M);
    for(int i=0;i<M;i++){
        b[i] = h[path[i].first][path[i].second];
    }
    ll load = 0;
    int earliest_neg = -1;
    for(int i=0;i<M;i++){
        if(b[i] > 0){
            load += b[i];
            b[i] = 0;
        }
        if(b[i] < 0){
            ll need = - (ll)b[i];
            ll d = min(load, need);
            load -= d;
            b[i] += (int)d;
            if(b[i] < 0 && earliest_neg == -1) earliest_neg = i;
        }
    }
    int dist = (earliest_neg == -1) ? 0 : (M - 1 - earliest_neg);
    return {dist, load};
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if(!(cin >> N)) return 0;
    vector<vector<int>> h(N, vector<int>(N));
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            cin >> h[i][j];
        }
    }

    auto rowPath = makeRowPath(N);
    auto colPath = makeColPath(N);

    auto evalRow = evaluatePath(rowPath, h);
    auto evalCol = evaluatePath(colPath, h);

    const vector<pair<int,int>>* bestPathPtr = &rowPath;
    if (evalCol.first < evalRow.first || (evalCol.first == evalRow.first && evalCol.second < evalRow.second)) {
        bestPathPtr = &colPath;
    }
    const auto& path = *bestPathPtr;
    int M = (int)path.size();

    vector<string> ops;
    ops.reserve(3000);

    auto moveStep = [&](pair<int,int> from, pair<int,int> to){
        int di = to.first - from.first;
        int dj = to.second - from.second;
        if(di == 1 && dj == 0) ops.emplace_back("D");
        else if(di == -1 && dj == 0) ops.emplace_back("U");
        else if(di == 0 && dj == 1) ops.emplace_back("R");
        else if(di == 0 && dj == -1) ops.emplace_back("L");
    };

    ll load = 0;

    auto processCellForward = [&](int i, int j){
        if(h[i][j] > 0){
            int d = h[i][j];
            load += d;
            h[i][j] = 0;
            ops.emplace_back("+" + to_string(d));
        }
        if(h[i][j] < 0 && load > 0){
            ll need = -(ll)h[i][j];
            ll d = min(load, need);
            if(d > 0){
                load -= d;
                h[i][j] += (int)d;
                ops.emplace_back("-" + to_string(d));
            }
        }
    };

    // Start at (0,0); both paths start at (0,0)
    for(int idx = 0; idx < M; idx++){
        auto [i,j] = path[idx];
        processCellForward(i,j);
        if(idx + 1 < M){
            moveStep(path[idx], path[idx+1]);
        }
    }

    // Reverse pass until load becomes zero
    for(int idx = M-1; load > 0 && idx >= 0; idx--){
        auto [i,j] = path[idx];
        if(h[i][j] < 0 && load > 0){
            ll need = -(ll)h[i][j];
            ll d = min(load, need);
            if(d > 0){
                load -= d;
                h[i][j] += (int)d;
                ops.emplace_back("-" + to_string(d));
            }
        }
        if(load == 0) break;
        if(idx > 0){
            moveStep(path[idx], path[idx-1]);
        }
    }

    // Output operations
    for(const auto& s : ops){
        cout << s << '\n';
    }

    return 0;
}