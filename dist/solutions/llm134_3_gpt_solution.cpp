#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;
using i128 = __int128_t;

struct Step {
    ull a;      // starting a where cap applies
    ull bmax;   // max b allowed (y-1)
};
struct Seg {
    ull l, r;   // a in [l, r]
    ull bmax;   // max b allowed on this a-interval
};

static inline i128 mul128(ull a, ull b){
    return (i128)a * (i128)b;
}

// Rebuild step function f(a) = min over all constraints (y_i - 1) for x_i <= a, else n
// constraints: vector of pairs (x, bmax=y-1)
static vector<Step> build_steps(const vector<pair<ull, ull>>& constraints, ull n){
    if(constraints.empty()){
        return {};
    }
    vector<pair<ull, ull>> v = constraints;
    sort(v.begin(), v.end(), [](const auto& L, const auto& R){
        if(L.first != R.first) return L.first < R.first;
        return L.second < R.second;
    });
    // merge by same x keeping minimal bmax
    vector<pair<ull, ull>> merged;
    for(size_t i=0;i<v.size();){
        ull x = v[i].first;
        ull b = v[i].second > n ? n : v[i].second;
        size_t j=i+1;
        while(j<v.size() && v[j].first==x){
            ull tb = v[j].second > n ? n : v[j].second;
            if(tb < b) b = tb;
            j++;
        }
        merged.emplace_back(x, b);
        i = j;
    }
    vector<Step> steps;
    ull curMin = ULLONG_MAX; // effectively +inf
    for(auto &p : merged){
        ull x = p.first, b = p.second;
        if(b < curMin){
            curMin = b;
            steps.push_back(Step{x, curMin});
            if(curMin == 0) break; // can't go below 0
        }
    }
    return steps;
}

// Build segments starting from A_low to n based on steps
static vector<Seg> build_segments(ull A_low, const vector<Step>& steps, ull n){
    vector<Seg> segs;
    if(A_low > n) return segs;
    // find value at A_low
    ull val = n;
    if(!steps.empty()){
        // find last step with a <= A_low
        auto it = upper_bound(steps.begin(), steps.end(), A_low, [](ull key, const Step& s){
            return key < s.a;
        });
        if(it != steps.begin()){
            --it;
            val = min(val, it->bmax);
        }
    }
    ull curL = A_low;
    ull curVal = val;
    for(const auto& s : steps){
        if(s.a <= curL) {
            curVal = min(curVal, s.bmax);
            continue;
        }
        if(curL <= n){
            ull r = min(n, s.a - 1);
            if(curL <= r){
                segs.push_back(Seg{curL, r, curVal});
                curL = s.a;
                curVal = min(curVal, s.bmax);
            } else {
                curVal = min(curVal, s.bmax);
            }
        }
    }
    if(curL <= n){
        segs.push_back(Seg{curL, n, curVal});
    }
    return segs;
}

// Compute total area S given segments and B_low
static i128 total_area(const vector<Seg>& segs, ull B_low){
    i128 S = 0;
    for(const auto& sg : segs){
        if(sg.bmax + 1 <= B_low) continue; // bmax < B_low
        ull lenA = (sg.r - sg.l + 1);
        ull countB = sg.bmax - B_low + 1;
        S += mul128(lenA, countB);
    }
    return S;
}

// Area after answer 1 (a >= x+1): compute sum over a>=T of (bmax - B_low + 1)+
static i128 area_after_ans1(const vector<Seg>& segs, ull B_low, ull T){
    i128 S = 0;
    for(const auto& sg : segs){
        if(sg.r < T) continue;
        ull l1 = max(sg.l, T);
        if(l1 > sg.r) continue;
        if(sg.bmax + 1 <= B_low) continue;
        ull lenA = (sg.r - l1 + 1);
        ull countB = sg.bmax - B_low + 1;
        S += mul128(lenA, countB);
    }
    return S;
}

// Area after answer 2 (b >= y+1): compute with new B_low' = max(B_low, y+1)
static i128 area_after_ans2(const vector<Seg>& segs, ull B_low, ull y){
    ull B_low2 = max(B_low, y + 1);
    i128 S = 0;
    for(const auto& sg : segs){
        if(sg.bmax + 1 <= B_low2) continue;
        ull lenA = (sg.r - sg.l + 1);
        ull countB = sg.bmax - B_low2 + 1;
        S += mul128(lenA, countB);
    }
    return S;
}

// Mass of top-right subregion (a >= x and b >= y)
static i128 mass_top_right(const vector<Seg>& segs, ull A_low, ull B_low, ull x, ull y){
    ull x0 = max(x, A_low);
    ull y0 = max(y, B_low);
    i128 M = 0;
    for(const auto& sg : segs){
        if(sg.r < x0) continue;
        ull l1 = max(sg.l, x0);
        if(l1 > sg.r) continue;
        if(sg.bmax + 1 <= y0) continue;
        ull lenA = (sg.r - l1 + 1);
        ull countB = sg.bmax - y0 + 1;
        M += mul128(lenA, countB);
    }
    return M;
}

// Enumerate candidate x values: x = a_threshold - 1 for each segment + (A_low - 1)
static vector<ull> enumerate_x_candidates(const vector<Seg>& segs, ull A_low){
    vector<ull> xs;
    xs.reserve(segs.size() + 1);
    if(A_low >= 1) xs.push_back(A_low - 1);
    for(const auto& sg : segs){
        if(sg.l >= 1) xs.push_back(sg.l - 1);
    }
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    return xs;
}

// Enumerate candidate y values: y = B_low - 1 and y = distinct bmax of segments
static vector<ull> enumerate_y_candidates(const vector<Seg>& segs, ull B_low){
    vector<ull> ys;
    if(B_low >= 1) ys.push_back(B_low - 1);
    for(const auto& sg : segs){
        ys.push_back(sg.bmax);
    }
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    return ys;
}

// Find a single remaining pair when total area == 1
static pair<ull, ull> find_single_pair(const vector<Seg>& segs, ull B_low){
    for(const auto& sg : segs){
        if(sg.bmax + 1 <= B_low) continue;
        ull lenA = (sg.r - sg.l + 1);
        ull countB = sg.bmax - B_low + 1;
        i128 area = mul128(lenA, countB);
        if(area == 0) continue;
        // Since total area is 1, the first non-zero segment contains it.
        // Determine the exact a,b: choose a = sg.l, b = B_low (if valid), else compute offsets.
        // Compute offset from start of this segment where the single cell lies.
        // But since total area == 1, we can pick any valid a and b; choose a = sg.l, b = B_low.
        ull a = sg.l;
        ull b = B_low;
        return {a, b};
    }
    // Fallback (should not happen)
    return {1, B_low};
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ull n;
    if(!(cin >> n)){
        return 0;
    }

    ull A_low = 1, B_low = 1;
    vector<pair<ull, ull>> constraints3; // (x, y-1)
    vector<Step> steps;

    // Main interactive loop
    while(true){
        // Rebuild steps and segments
        steps = build_steps(constraints3, n);
        vector<Seg> segs = build_segments(A_low, steps, n);
        i128 S = total_area(segs, B_low);
        if(S <= 0){
            // Shouldn't happen; pick something safe
            ull x = A_low, y = B_low;
            cout << x << " " << y << endl << flush;
            int res;
            if(!(cin >> res)) return 0;
            if(res == 0) return 0;
            if(res == 1) A_low = min(n, x + 1);
            else if(res == 2) B_low = min(n, y + 1);
            else if(res == 3) constraints3.emplace_back(x, y - 1);
            continue;
        }
        if(S == 1){
            auto p = find_single_pair(segs, B_low);
            ull x = p.first, y = p.second;
            cout << x << " " << y << endl << flush;
            int res;
            if(!(cin >> res)) return 0;
            if(res == 0) return 0;
            if(res == 1) A_low = min(n, x + 1);
            else if(res == 2) B_low = min(n, y + 1);
            else if(res == 3) constraints3.emplace_back(x, y - 1);
            continue;
        }

        // Enumerate candidate x and y values
        vector<ull> xs = enumerate_x_candidates(segs, A_low);
        vector<ull> ys = enumerate_y_candidates(segs, B_low);
        if(xs.empty()) xs.push_back(A_low - 1);
        if(ys.empty()) ys.push_back(B_low - 1);

        // Choose the pair (x,y) minimizing worst-case next area
        i128 bestMax = -1;
        ull bestX = xs[0], bestY = ys[0];
        bool first = true;

        for(ull x : xs){
            // Clamp x to [1, n]
            if(x < 1) x = 1;
            if(x > n) x = n;
            i128 a1 = area_after_ans1(segs, B_low, (x >= n ? n : x) + 1); // T = x+1
            for(ull y : ys){
                if(y < 1) y = 1;
                if(y > n) y = n;
                i128 a2 = area_after_ans2(segs, B_low, y);
                i128 M = mass_top_right(segs, A_low, B_low, x, y);
                i128 a3 = S - M;
                i128 mx = a1;
                if(a2 > mx) mx = a2;
                if(a3 > mx) mx = a3;
                if(first || mx < bestMax){
                    first = false;
                    bestMax = mx;
                    bestX = x;
                    bestY = y;
                }
            }
        }

        // As a backup, also try some heuristic midpoints (to ensure progress if candidate sets are poor)
        // Use golden ratio splits on a and b dimensions
        // Compute target fractions
        // For a-dimension
        // We try T such that area_after_ans1 <= 2S/3 via scanning
        i128 target = (S * 2) / 3;
        // Scan to find a T that meets target
        ull heuristicX = A_low;
        {
            // We'll scan segments from high a to low a
            // Build a vector of cumulative areas for convenience
            i128 acc = 0;
            // We'll gather all unique boundaries for T
            vector<ull> Ts;
            Ts.reserve(segs.size()*2+2);
            Ts.push_back(A_low);
            for(const auto& sg : segs){
                Ts.push_back(sg.l);
                if(sg.r < ULLONG_MAX) Ts.push_back(sg.r + 1);
            }
            sort(Ts.begin(), Ts.end());
            Ts.erase(remove_if(Ts.begin(), Ts.end(), [A_low](ull v){ return v < A_low; }), Ts.end());
            Ts.erase(unique(Ts.begin(), Ts.end()), Ts.end());
            // Evaluate area_after_ans1 for each T candidate and pick first <= target
            ull chosen = Ts.empty()?A_low:Ts.back();
            for(ull T : Ts){
                i128 ar = area_after_ans1(segs, B_low, T);
                if(ar <= target){
                    chosen = T; break;
                }
            }
            if(chosen == 0) chosen = 1;
            heuristicX = (chosen <= 1 ? 1 : (chosen - 1));
        }
        ull heuristicY = B_low;
        {
            // We want area_after_ans2 <= target, so choose y accordingly scanning unique bmax values
            vector<ull> Ys;
            Ys.reserve(segs.size()+2);
            Ys.push_back(B_low);
            for(const auto& sg : segs){
                Ys.push_back(sg.bmax + 1); // y+1 threshold candidates
            }
            sort(Ys.begin(), Ys.end());
            Ys.erase(remove_if(Ys.begin(), Ys.end(), [B_low](ull v){ return v < B_low; }), Ys.end());
            Ys.erase(unique(Ys.begin(), Ys.end()), Ys.end());
            ull chosenYp1 = Ys.empty()?B_low: Ys.back();
            for(ull Yp1 : Ys){
                if(Yp1 == 0) continue;
                ull yCand = (Yp1 == 0 ? 1 : (Yp1 - 1));
                if(yCand < 1) yCand = 1;
                i128 ar = area_after_ans2(segs, B_low, yCand);
                if(ar <= target){
                    chosenYp1 = Yp1;
                    break;
                }
            }
            if(chosenYp1 == 0) chosenYp1 = B_low;
            heuristicY = (chosenYp1 == 0 ? 1 : chosenYp1 - 1);
            if(heuristicY < 1) heuristicY = 1;
        }
        // Try heuristic pair too
        {
            i128 a1 = area_after_ans1(segs, B_low, (heuristicX >= n ? n : heuristicX) + 1);
            i128 a2 = area_after_ans2(segs, B_low, heuristicY);
            i128 M = mass_top_right(segs, A_low, B_low, heuristicX, heuristicY);
            i128 a3 = S - M;
            i128 mx = a1;
            if(a2 > mx) mx = a2;
            if(a3 > mx) mx = a3;
            if(mx < bestMax){
                bestMax = mx;
                bestX = heuristicX;
                bestY = heuristicY;
            }
        }

        // Output query
        ull qx = bestX;
        ull qy = bestY;
        if(qx < 1) qx = 1;
        if(qx > n) qx = n;
        if(qy < 1) qy = 1;
        if(qy > n) qy = n;

        cout << qx << " " << qy << endl << flush;

        int res;
        if(!(cin >> res)) return 0;
        if(res == 0){
            return 0;
        } else if(res == 1){
            if(qx < n) A_low = max(A_low, qx + 1);
            else A_low = n;
        } else if(res == 2){
            if(qy < n) B_low = max(B_low, qy + 1);
            else B_low = n;
        } else if(res == 3){
            // add (x, y-1) constraint
            ull bmax = (qy == 0 ? 0 : (qy - 1));
            constraints3.emplace_back(qx, bmax);
        } else {
            // Unknown response; just exit
            return 0;
        }
    }
}