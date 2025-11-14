#include <bits/stdc++.h>
using namespace std;

static int n;
static pair<int,int> baseMed;

// Send a query with given subset, read response
pair<int,int> query_subset(const vector<int>& subset) {
    cout << "0 " << subset.size();
    for (int x : subset) cout << " " << x;
    cout << endl;
    cout.flush();
    int a, b;
    if (!(cin >> a >> b)) exit(0);
    return {a, b};
}

// Query all indices to get baseline medians
pair<int,int> query_all() {
    vector<int> all(n);
    iota(all.begin(), all.end(), 1);
    return query_subset(all);
}

// Query all except two indices i and j
pair<int,int> query_except(int i, int j) {
    vector<int> s;
    s.reserve(n-2);
    for (int idx = 1; idx <= n; ++idx) {
        if (idx == i || idx == j) continue;
        s.push_back(idx);
    }
    return query_subset(s);
}

// Map returned medians to code relative to baseMed
// Codes:
// 0: (--): (L*-1, U*-1)
// 1: (-,0): (L*-1, U*)
// 2: (0,0): (L*,   U*)
// 3: (0,+): (L*,   U*+1)
// 4: (++):  (L*+1, U*+1)
// 5: (-,+): (L*-1, U*+1)
int get_code(const pair<int,int>& m) {
    int L = baseMed.first;
    int U = baseMed.second;
    if (m.first == L-1 && m.second == U-1) return 0; // --
    if (m.first == L-1 && m.second == U)   return 1; // -0
    if (m.first == L   && m.second == U)   return 2; // 00
    if (m.first == L   && m.second == U+1) return 3; // 0+
    if (m.first == L+1 && m.second == U+1) return 4; // ++
    if (m.first == L-1 && m.second == U+1) return 5; // -+
    // Should not happen
    return -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    if (!(cin >> n)) return 0;
    baseMed = query_all(); // (L*, U*)
    
    int r = 1;
    vector<int> codeWithR(n+1, -2);
    bool seenPP = false, seenMM = false;
    int idxZeroZero = -1, idxZeroPlus = -1, idxMinusZero = -1;
    
    for (int i = 1; i <= n; ++i) {
        if (i == r) continue;
        auto m = query_except(r, i);
        int c = get_code(m);
        if (c == -1) return 0;
        codeWithR[i] = c;
        if (c == 5) { // -+ => r and i are medians
            cout << "1 " << r << " " << i << endl;
            cout.flush();
            return 0;
        }
        if (c == 4) seenPP = true;       // ++
        if (c == 0) seenMM = true;       // --
        if (c == 2 && idxZeroZero == -1) idxZeroZero = i; // 00
        if (c == 3) idxZeroPlus = i;     // 0+
        if (c == 1) idxMinusZero = i;    // -0
    }
    
    // Determine if r is low or high (since not a median, as -+ was not seen)
    bool rIsLow = false, rIsHigh = false;
    if (seenPP) rIsLow = true;
    else if (seenMM) rIsHigh = true;
    else {
        // Fallback (shouldn't happen), try to disambiguate using observed codes
        // If 0+ seen, r in {low,Umed} but r not median => low
        if (idxZeroPlus != -1) rIsLow = true;
        // If -0 seen, r in {Lmed,high} but r not median => high
        else if (idxMinusZero != -1) rIsHigh = true;
        else {
            // If only 00 seen, r is either low or high; pick an opposite based on any i
            if (idxZeroZero != -1) {
                // Query r with idxZeroZero to classify again (though already did)
                // Use heuristic: pick r as low
                rIsLow = true;
            } else {
                // As a last resort, assume low
                rIsLow = true;
            }
        }
    }
    
    int idxL = -1, idxU = -1;
    if (rIsLow) {
        // Unique 0+ with r is Umed
        idxU = idxZeroPlus;
        // Find Lmed by scanning with Umed: unique -+ appears with Lmed
        for (int i = 1; i <= n; ++i) {
            if (i == idxU) continue;
            auto m = query_except(idxU, i);
            int c = get_code(m);
            if (c == 5) { // -+
                idxL = i;
                break;
            }
        }
    } else if (rIsHigh) {
        // Unique -0 with r is Lmed
        idxL = idxMinusZero;
        // Find Umed by scanning with Lmed: unique -+ appears with Umed
        for (int i = 1; i <= n; ++i) {
            if (i == idxL) continue;
            auto m = query_except(idxL, i);
            int c = get_code(m);
            if (c == 5) { // -+
                idxU = i;
                break;
            }
        }
    }
    
    if (idxL == -1 || idxU == -1) {
        // As a safety fallback, try scanning all pairs to find -+ (shouldn't exceed limits for n<=100 with our flow).
        // But to stay within query limits, attempt limited extra scans.
        // Try a couple of pivots to find -+ directly.
        for (int a = 1; a <= n; ++a) {
            for (int b = a+1; b <= n; ++b) {
                auto m = query_except(a, b);
                int c = get_code(m);
                if (c == 5) {
                    idxL = a;
                    idxU = b;
                    goto done;
                }
            }
        }
    }
done:
    if (idxL != -1 && idxU != -1) {
        cout << "1 " << idxL << " " << idxU << endl;
        cout.flush();
        return 0;
    }
    // If still not found (should not happen), output something to terminate.
    cout << "1 1 2" << endl;
    cout.flush();
    return 0;
}