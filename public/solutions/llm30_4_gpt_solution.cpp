#include <bits/stdc++.h>
using namespace std;

struct Solver {
    int n;
    vector<vector<int>> g;
    vector<int> parent, tin, tout, depth, subSum;
    vector<char> isCand;
    int timer = 0;
    int totalCand = 0;
    int query_count = 0;

    void dfs_build(int v, int p){
        parent[v] = p;
        tin[v] = ++timer;
        for(int to: g[v]){
            if(to == p) continue;
            depth[to] = depth[v] + 1;
            dfs_build(to, v);
        }
        tout[v] = timer;
    }

    int dfs_sum(int v, int p){
        int s = isCand[v] ? 1 : 0;
        for(int to: g[v]){
            if(to == p) continue;
            s += dfs_sum(to, v);
        }
        subSum[v] = s;
        return s;
    }

    inline bool inSub(int u, int v){
        return tin[u] >= tin[v] && tin[u] <= tout[v];
    }

    int choose_centroid(){
        subSum.assign(n+1, 0);
        dfs_sum(1, 0);
        int best = 1;
        int bestVal = n+1;
        for(int v=1; v<=n; v++){
            int in = subSum[v];
            int out = totalCand - in;
            int val = max(in, out);
            if(val < bestVal){
                bestVal = val;
                best = v;
            }
        }
        return best;
    }

    int ask(int x){
        query_count++;
        cout << "? " << x << endl;
        cout.flush();
        int ans;
        if(!(cin >> ans)){
            // If the stream is closed, terminate
            exit(0);
        }
        return ans;
    }

    void answer(int x){
        cout << "! " << x << endl;
        cout.flush();
    }

    void run_case(){
        cin >> n;
        g.assign(n+1, {});
        for(int i=0;i<n-1;i++){
            int u,v; cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        parent.assign(n+1, 0);
        tin.assign(n+1, 0);
        tout.assign(n+1, 0);
        depth.assign(n+1, 0);
        timer = 0;
        dfs_build(1, 0);

        isCand.assign(n+1, 0);
        for(int i=1;i<=n;i++) isCand[i] = 1;
        totalCand = n;

        while(totalCand > 1){
            int v = choose_centroid();
            int res = ask(v);
            if(res == 1){
                // Keep only candidates in subtree of v
                for(int u=1; u<=n; u++){
                    if(isCand[u] && !inSub(u, v)){
                        isCand[u] = 0;
                        totalCand--;
                    }
                }
            }else{
                // Map candidates outside subtree(v) to their parents (or 1 if root)
                vector<char> newCand(n+1, 0);
                int newCount = 0;
                for(int u=1; u<=n; u++){
                    if(!isCand[u]) continue;
                    if(!inSub(u, v)){
                        int p = parent[u];
                        if(p == 0) p = 1;
                        if(!newCand[p]){
                            newCand[p] = 1;
                            newCount++;
                        }
                    }
                }
                // In a correct interaction, newCount should be >= 1
                if(newCount == 0){
                    // Fallback: keep 1 as possible
                    newCand[1] = 1;
                    newCount = 1;
                }
                isCand.swap(newCand);
                totalCand = newCount;
            }
        }

        int pos = 1;
        for(int i=1;i<=n;i++) if(isCand[i]) { pos = i; break; }
        answer(pos);
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    if(!(cin >> t)) return 0;
    while(t--){
        Solver s;
        s.run_case();
    }
    return 0;
}