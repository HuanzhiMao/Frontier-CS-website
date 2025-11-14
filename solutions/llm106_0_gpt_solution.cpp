#include <bits/stdc++.h>
using namespace std;

static const int MAXN = 605;

int n;
vector<int> treeAdj[MAXN];
int parentArr[MAXN], depthArr[MAXN], colorArr[MAXN];

int ask(const vector<int>& S) {
    if (S.empty()) return 0; // cannot query empty set; logically 0 edges
    cout << "? " << S.size() << "\n";
    for (size_t i = 0; i < S.size(); ++i) {
        if (i) cout << " ";
        cout << S[i];
    }
    cout << "\n";
    cout.flush();
    int ans;
    if (!(cin >> ans)) exit(0);
    if (ans == -1) exit(0);
    return ans;
}

int edgesBetween_v_T(int v, const vector<int>& T) {
    if (T.empty()) return 0;
    vector<int> U = T;
    U.push_back(v);
    int a = ask(U);
    int b = ask(T);
    return a - b;
}

int findNeighborInSet(int v, vector<int> T) {
    if (T.empty()) return -1;
    if (edgesBetween_v_T(v, T) == 0) return -1;
    while (T.size() > 1) {
        size_t mid = T.size() / 2;
        vector<int> L(T.begin(), T.begin() + mid);
        vector<int> R(T.begin() + mid, T.end());
        if (edgesBetween_v_T(v, L) > 0) {
            T.swap(L);
        } else {
            T.swap(R);
        }
    }
    return T[0];
}

vector<int> getPath(int a, int b) {
    vector<int> upA, upB;
    int x = a, y = b;
    while (depthArr[x] > depthArr[y]) {
        upA.push_back(x);
        x = parentArr[x];
    }
    while (depthArr[y] > depthArr[x]) {
        upB.push_back(y);
        y = parentArr[y];
    }
    while (x != y) {
        upA.push_back(x);
        upB.push_back(y);
        x = parentArr[x];
        y = parentArr[y];
    }
    vector<int> path = upA;
    path.push_back(x); // LCA
    reverse(upB.begin(), upB.end());
    path.insert(path.end(), upB.begin(), upB.end());
    return path;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n)) return 0;

    if (n == 0) {
        cout << "Y 0\n\n";
        cout.flush();
        return 0;
    }

    vector<int> processed;
    processed.reserve(n);

    // Initialize with vertex 1
    processed.push_back(1);
    parentArr[1] = 0;
    depthArr[1] = 0;
    colorArr[1] = 0;

    for (int v = 2; v <= n; ++v) {
        int u = findNeighborInSet(v, processed);
        if (u == -1) {
            // Should not happen for connected graph, but handle gracefully
            parentArr[v] = 0;
            depthArr[v] = 0;
            colorArr[v] = 0;
        } else {
            parentArr[v] = u;
            depthArr[v] = depthArr[u] + 1;
            colorArr[v] = colorArr[u] ^ 1;
            treeAdj[u].push_back(v);
            treeAdj[v].push_back(u);
        }
        processed.push_back(v);
    }

    vector<int> part0, part1;
    for (int i = 1; i <= n; ++i) {
        if (colorArr[i] == 0) part0.push_back(i);
        else part1.push_back(i);
    }

    int e0 = ask(part0);
    int e1 = ask(part1);

    if (e0 == 0 && e1 == 0) {
        cout << "Y " << part0.size() << "\n";
        for (size_t i = 0; i < part0.size(); ++i) {
            if (i) cout << " ";
            cout << part0[i];
        }
        cout << "\n";
        cout.flush();
        return 0;
    } else {
        vector<int> bad = (e0 > 0 ? part0 : part1);
        int totalEdgesInBad = ask(bad);
        int v = -1;
        vector<int> badWithoutV;
        for (size_t i = 0; i < bad.size(); ++i) {
            vector<int> T;
            T.reserve(bad.size() - 1);
            for (size_t j = 0; j < bad.size(); ++j) {
                if (j != i) T.push_back(bad[j]);
            }
            int edgesT = ask(T);
            if (edgesT < totalEdgesInBad) {
                v = bad[i];
                badWithoutV = move(T);
                break;
            }
        }
        if (v == -1) {
            // Fallback, should not happen
            cout << "Y " << part0.size() << "\n";
            for (size_t i = 0; i < part0.size(); ++i) {
                if (i) cout << " ";
                cout << part0[i];
            }
            cout << "\n";
            cout.flush();
            return 0;
        }
        int u = findNeighborInSet(v, badWithoutV);
        if (u == -1) {
            // Fallback
            cout << "Y " << part0.size() << "\n";
            for (size_t i = 0; i < part0.size(); ++i) {
                if (i) cout << " ";
                cout << part0[i];
            }
            cout << "\n";
            cout.flush();
            return 0;
        }
        vector<int> cyclePath = getPath(v, u);
        cout << "N " << cyclePath.size() << "\n";
        for (size_t i = 0; i < cyclePath.size(); ++i) {
            if (i) cout << " ";
            cout << cyclePath[i];
        }
        cout << "\n";
        cout.flush();
    }

    return 0;
}