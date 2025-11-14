#include <bits/stdc++.h>
using namespace std;

using u64 = unsigned long long;
using i64 = long long;
using u128 = __uint128_t;

struct Solver {
    u64 n;
    u64 AL, BL; // lower bounds for a and b
    vector<pair<u64,u64>> forb; // minimal set of (x,y) where response was 3; sorted by x asc, y strictly desc

    Solver(u64 n_): n(n_), AL(1), BL(1) {}

    // Maintain minimal set: remove dominated, ignore if dominated by existing
    void add_forbid(u64 x, u64 y) {
        // If dominated by existing (x' <= x and y' <= y), ignore
        for (auto &p : forb) {
            if (p.first <= x && p.second <= y) return;
        }
        // Remove all pairs dominated by new (x' >= x and y' >= y)
        vector<pair<u64,u64>> nf;
        nf.reserve(forb.size()+1);
        for (auto &p : forb) {
            if (!(p.first >= x && p.second >= y)) nf.push_back(p);
        }
        nf.push_back({x,y});
        sort(nf.begin(), nf.end());
        // Enforce minimal: y strictly decreasing with x increasing
        vector<pair<u64,u64>> nf2;
        for (auto &p : nf) {
            while (!nf2.empty() && nf2.back().second <= p.second) {
                // If previous y <= current y, then previous is dominated (since x_prev < x and y_prev <= y)
                nf2.pop_back();
            }
            nf2.push_back(p);
        }
        forb.swap(nf2);
    }

    // Count total feasible pairs in current shape
    u128 count_total() const {
        u128 total = 0;
        u64 prev_x = AL;
        u64 cap = n; // current b upper cap
        for (auto &p : forb) {
            u64 x = p.first, y = p.second;
            if (prev_x > n) break;
            u64 end = x - 1;
            if (end > n) end = n;
            if (end >= prev_x) {
                u64 bcap = cap > n ? n : cap;
                if (bcap >= BL) {
                    u64 w = end - prev_x + 1;
                    u64 h = bcap - BL + 1;
                    total += (u128)w * (u128)h;
                }
            }
            if (x < prev_x) {
                // still update cap; prev_x unchanged
                cap = min(cap, y - 1);
            } else {
                prev_x = x;
                cap = min(cap, y - 1);
            }
        }
        if (prev_x <= n) {
            u64 bcap = cap > n ? n : cap;
            if (bcap >= BL) {
                u64 w = n - prev_x + 1;
                u64 h = bcap - BL + 1;
                total += (u128)w * (u128)h;
            }
        }
        return total;
    }

    // Count # of pairs with a <= x
    u128 count_left(u64 x) const {
        if (x < AL) return 0;
        if (x > n) x = n;
        u128 total = 0;
        u64 prev_x = AL;
        u64 cap = n;
        for (auto &p : forb) {
            u64 xi = p.first, yi = p.second;
            if (prev_x > x) break;
            u64 end = xi - 1;
            if (end > x) end = x;
            if (end >= prev_x) {
                u64 bcap = cap > n ? n : cap;
                if (bcap >= BL) {
                    u64 w = end - prev_x + 1;
                    u64 h = bcap - BL + 1;
                    total += (u128)w * (u128)h;
                }
            }
            if (xi <= x) {
                if (xi < prev_x) {
                    cap = min(cap, yi - 1);
                } else {
                    prev_x = xi;
                    cap = min(cap, yi - 1);
                }
            } else {
                break;
            }
        }
        if (prev_x <= x) {
            u64 bcap = cap > n ? n : cap;
            if (bcap >= BL) {
                u64 w = x - prev_x + 1;
                u64 h = bcap - BL + 1;
                total += (u128)w * (u128)h;
            }
        }
        return total;
    }

    // Count # of pairs with b <= y
    u128 count_below(u64 y) const {
        if (y < BL) return 0;
        if (y > n) y = n;
        u128 total = 0;
        u64 prev_x = AL;
        u64 cap = n;
        for (auto &p : forb) {
            u64 xi = p.first, yi = p.second;
            if (prev_x > n) break;
            u64 end = xi - 1;
            if (end > n) end = n;
            if (end >= prev_x) {
                u64 bcap = cap > n ? n : cap;
                if (bcap >= BL) {
                    u64 w = end - prev_x + 1;
                    u64 h = (y < bcap ? y : bcap) - BL + 1;
                    if ((i64)h > 0) total += (u128)w * (u128)h;
                }
            }
            prev_x = max(prev_x, xi);
            cap = min(cap, yi - 1);
        }
        if (prev_x <= n) {
            u64 bcap = cap > n ? n : cap;
            if (bcap >= BL) {
                u64 w = n - prev_x + 1;
                u64 h = (y < bcap ? y : bcap) - BL + 1;
                if ((i64)h > 0) total += (u128)w * (u128)h;
            }
        }
        return total;
    }

    // Count # of pairs with a >= x and b >= y
    u128 count_topright(u64 x, u64 y) const {
        if (x < AL) x = AL;
        if (y < BL) y = BL;
        if (x > n || y > n) return 0;
        u128 total = 0;
        u64 prev_x = AL;
        u64 cap = n;
        for (auto &p : forb) {
            u64 xi = p.first, yi = p.second;
            if (prev_x > n) break;
            u64 segL = prev_x;
            u64 segR = xi - 1;
            if (segR > n) segR = n;
            // intersection with a >= x
            if (segR >= segL) {
                u64 L = max(segL, x);
                if (segR >= L) {
                    u64 bcap = cap > n ? n : cap;
                    if (bcap >= y) {
                        u64 w = segR - L + 1;
                        u64 h = bcap - y + 1;
                        total += (u128)w * (u128)h;
                    }
                }
            }
            prev_x = max(prev_x, xi);
            cap = min(cap, yi - 1);
        }
        if (prev_x <= n) {
            u64 segL = prev_x, segR = n;
            u64 L = max(segL, x);
            if (segR >= L) {
                u64 bcap = cap > n ? n : cap;
                if (bcap >= y) {
                    u64 w = segR - L + 1;
                    u64 h = bcap - y + 1;
                    total += (u128)w * (u128)h;
                }
            }
        }
        return total;
    }

    // Choose x,y for next query ensuring progress
    pair<u64,u64> choose_xy() const {
        u128 S = count_total();
        // Median by a
        u64 lo = AL, hi = n;
        while (lo < hi) {
            u64 mid = lo + ((hi - lo) >> 1);
            u128 left = count_left(mid);
            if (left * 2 >= S) hi = mid;
            else lo = mid + 1;
        }
        u64 x = lo;

        // Median by b
        lo = BL; hi = n;
        while (lo < hi) {
            u64 mid = lo + ((hi - lo) >> 1);
            u128 below = count_below(mid);
            if (below * 2 >= S) hi = mid;
            else lo = mid + 1;
        }
        u64 y = lo;

        // Ensure top-right area >= S/4 by lowering x and/or y if needed
        u128 thr = S / 4;
        u128 T = count_topright(x, y);
        if (T < thr) {
            // Try lowering x
            u64 lox = AL, hix = x;
            bool okx = false;
            while (lox < hix) {
                u64 mid = lox + ((hix - lox) >> 1);
                u128 tmid = count_topright(mid, y);
                if (tmid >= thr) hix = mid, okx = true;
                else lox = mid + 1;
            }
            if (count_topright(lox, y) >= thr) {
                x = lox; okx = true;
            }
            if (!okx) {
                // Lower y (with x at AL)
                u64 loy = BL, hiy = y;
                while (loy < hiy) {
                    u64 mid = loy + ((hiy - loy) >> 1);
                    u128 tmid = count_topright(AL, mid);
                    if (tmid >= thr) hiy = mid;
                    else loy = mid + 1;
                }
                y = loy;
                x = AL; // ensure thr satisfied
            }
        }
        if (x < AL) x = AL;
        if (y < BL) y = BL;
        if (x > n) x = n;
        if (y > n) y = n;
        return {x, y};
    }

    void run() {
        while (true) {
            // If the feasible set is reduced to one point, query it
            u128 S = count_total();
            if (S == 1) {
                // Find the unique pair
                u64 prev_x = AL;
                u64 cap = n;
                bool found = false;
                u64 ansA = AL, ansB = BL;
                for (auto &p : forb) {
                    u64 xi = p.first, yi = p.second;
                    if (prev_x > n) break;
                    u64 end = xi - 1;
                    if (end > n) end = n;
                    if (end >= prev_x) {
                        u64 bcap = cap > n ? n : cap;
                        if (bcap >= BL) {
                            // This segment contributes (end - prev_x + 1) * (bcap - BL + 1) points
                            u64 w = end - prev_x + 1;
                            u64 h = bcap - BL + 1;
                            if ((u128)w * (u128)h >= 1) {
                                // The unique point is the first in this segment
                                ansA = prev_x;
                                ansB = BL;
                                found = true;
                                break;
                            }
                        }
                    }
                    prev_x = max(prev_x, xi);
                    cap = min(cap, yi - 1);
                }
                if (!found && prev_x <= n) {
                    u64 bcap = cap > n ? n : cap;
                    if (bcap >= BL) {
                        ansA = prev_x;
                        ansB = BL;
                        found = true;
                    }
                }
                // Query the unique pair
                cout << ansA << " " << ansB << endl;
                cout.flush();
                int res;
                if (!(cin >> res)) return;
                if (res == 0) return;
                if (res == 1) { AL = max(AL, ansA + 1); }
                else if (res == 2) { BL = max(BL, ansB + 1); }
                else if (res == 3) { add_forbid(ansA, ansB); }
                else return;
                continue;
            }

            // General step
            auto [x, y] = choose_xy();

            cout << x << " " << y << endl;
            cout.flush();

            int r;
            if (!(cin >> r)) return;
            if (r == 0) return;
            if (r == 1) {
                if (x == ULLONG_MAX) return;
                if (x == n) {
                    // Must be impossible, but guard
                    AL = n + 1;
                } else {
                    AL = max(AL, x + 1);
                }
            } else if (r == 2) {
                if (y == n) {
                    BL = n + 1;
                } else {
                    BL = max(BL, y + 1);
                }
            } else if (r == 3) {
                add_forbid(x, y);
            } else {
                return;
            }
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    u64 n;
    if (!(cin >> n)) return 0;
    Solver solver(n);
    solver.run();
    return 0;
}