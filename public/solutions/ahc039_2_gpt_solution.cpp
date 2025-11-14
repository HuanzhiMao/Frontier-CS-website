#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    const int MAXC = 100000;
    vector<bool> usedX(MAXC+1, false), usedY(MAXC+1, false);
    vector<pair<int,int>> pts(2*N);
    for (int i = 0; i < 2*N; ++i) {
        int x, y;
        cin >> x >> y;
        pts[i] = {x, y};
        usedX[x] = true;
        usedY[y] = true;
    }
    
    auto findConsecutive = [&](const vector<bool>& used)->pair<int,bool>{
        for (int i = 0; i < MAXC; ++i) {
            if (!used[i] && !used[i+1]) return {i, true};
        }
        // Fallback: find any two unused positions
        int first = -1, second = -1;
        for (int i = 0; i <= MAXC; ++i) {
            if (!used[i]) {
                if (first == -1) first = i;
                else { second = i; break; }
            }
        }
        if (first == -1 || second == -1) {
            // Extremely unlikely given constraints; but ensure valid values
            // Choose default small safe positions
            first = 0; second = 1;
            if (used[first]) first = 2;
            if (used[second] || second == first) second = first + 1 <= MAXC ? first + 1 : first - 1;
        }
        return {first, false};
    };
    
    auto getSecondFromFirst = [&](const vector<bool>& used, int first, bool consecutive)->int{
        if (consecutive) return first + 1;
        // find next unused different from first
        for (int i = first + 1; i <= MAXC; ++i) if (!used[i]) return i;
        for (int i = first - 1; i >= 0; --i) if (!used[i]) return i;
        // fallback
        return (first + 1 <= MAXC ? first + 1 : first - 1);
    };
    
    auto [x1cand, xConsec] = findConsecutive(usedX);
    int x2cand = getSecondFromFirst(usedX, x1cand, xConsec);
    auto [y1cand, yConsec] = findConsecutive(usedY);
    int y2cand = getSecondFromFirst(usedY, y1cand, yConsec);
    
    // Ensure at least one dimension has width 1 to guarantee empty interior
    if (!xConsec && yConsec) {
        // make y consecutive and x any two
        // already satisfied
        ;
    } else if (xConsec && !yConsec) {
        // already satisfied
        ;
    } else if (!xConsec && !yConsec) {
        // Force one dimension to be consecutive by adjusting if possible
        // Try to find consecutive in X again (should exist in practice)
        bool fixed = false;
        for (int i = 0; i < MAXC; ++i) {
            if (!usedX[i] && !usedX[i+1]) {
                x1cand = i; x2cand = i+1; fixed = true; break;
            }
        }
        if (!fixed) {
            for (int i = 0; i < MAXC; ++i) {
                if (!usedY[i] && !usedY[i+1]) {
                    y1cand = i; y2cand = i+1; fixed = true; break;
                }
            }
        }
        // If still not fixed (practically impossible), keep as is.
    } else {
        // both consecutive, fine
        ;
    }
    
    // Make sure coordinates are within bounds and distinct
    int x1 = x1cand, x2 = x2cand, y1 = y1cand, y2 = y2cand;
    if (x1 == x2) {
        if (x2 + 1 <= MAXC && !usedX[x2+1]) x2 = x2 + 1;
        else if (x1 - 1 >= 0 && !usedX[x1-1]) x1 = x1 - 1;
        else if (x2 + 1 <= MAXC) x2 = x2 + 1;
        else if (x1 - 1 >= 0) x1 = x1 - 1;
    }
    if (y1 == y2) {
        if (y2 + 1 <= MAXC && !usedY[y2+1]) y2 = y2 + 1;
        else if (y1 - 1 >= 0 && !usedY[y1-1]) y1 = y1 - 1;
        else if (y2 + 1 <= MAXC) y2 = y2 + 1;
        else if (y1 - 1 >= 0) y1 = y1 - 1;
    }
    if (x1 > x2) swap(x1, x2);
    if (y1 > y2) swap(y1, y2);
    // If still not consecutive in either dimension, try enforce at least one dim delta=1
    if (x2 - x1 > 1 && y2 - y1 > 1) {
        // Try to adjust x to be consecutive
        bool done = false;
        for (int i = 0; i < MAXC; ++i) {
            if (!usedX[i] && !usedX[i+1]) {
                x1 = i; x2 = i+1; done = true; break;
            }
        }
        if (!done) {
            for (int i = 0; i < MAXC; ++i) {
                if (!usedY[i] && !usedY[i+1]) {
                    y1 = i; y2 = i+1; break;
                }
            }
        }
    }
    
    cout << 4 << "\n";
    cout << x1 << " " << y1 << "\n";
    cout << x2 << " " << y1 << "\n";
    cout << x2 << " " << y2 << "\n";
    cout << x1 << " " << y2 << "\n";
    return 0;
}