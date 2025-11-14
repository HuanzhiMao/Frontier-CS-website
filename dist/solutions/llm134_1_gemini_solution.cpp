#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>

using namespace std;

typedef long long ll;
typedef __int128_t i128;

ll n;
ll la, lb;
map<ll, ll> p;

// Add (qx, qy) to staircase boundary, maintaining minimality.
void update_p(ll qx, ll qy) {
    auto it = p.lower_bound(qx);
    if (it != p.end() && it->first == qx) {
        it->second = min(it->second, qy);
    } else {
        it = p.insert({qx, qy}).first;
    }

    if (it != p.begin()) {
        auto prev_it = prev(it);
        if (prev_it->second <= qy) {
            p.erase(it);
            return;
        }
    }

    auto next_it = next(it);
    while (next_it != p.end() && next_it->second >= qy) {
        next_it = p.erase(next_it);
    }
}

// Get max valid y for a given x.
ll get_uy(ll x) {
    if (x > n) return lb - 1;
    auto it = p.upper_bound(x);
    ll max_y = n;
    if (it != p.begin()) {
        max_y = prev(it)->second - 1;
    }
    return max_y;
}

i128 calculate_area() {
    i128 area = 0;
    ll current_x = la;
    ll max_y_for_segment = n;

    for (auto const& [px, py] : p) {
        if (px <= current_x) {
            max_y_for_segment = min(max_y_for_segment, py - 1);
            continue;
        }
        
        if (max_y_for_segment >= lb) {
            area += (i128)(px - current_x) * (max_y_for_segment - lb + 1);
        }
        current_x = px;
        max_y_for_segment = min(max_y_for_segment, py - 1);
    }

    if (max_y_for_segment >= lb) {
        if (n >= current_x) {
            area += (i128)(n - current_x + 1) * (max_y_for_segment - lb + 1);
        }
    }
    return area;
}

ll find_median_x(i128 total_area) {
    if (total_area <= 0) return la;

    i128 target_area = (total_area + 1) / 2;
    i128 current_area = 0;
    ll current_x = la;
    ll max_y_for_segment = n;
    
    for (auto const& [px, py] : p) {
        if (px <= current_x) {
            max_y_for_segment = min(max_y_for_segment, py - 1);
            continue;
        }

        if (max_y_for_segment >= lb) {
            i128 segment_area = (i128)(px - current_x) * (max_y_for_segment - lb + 1);
            if (current_area + segment_area >= target_area) {
                ll height = max_y_for_segment - lb + 1;
                if (height <= 0) return current_x;
                ll offset = (target_area - current_area - 1) / height;
                return current_x + offset;
            }
            current_area += segment_area;
        }
        current_x = px;
        max_y_for_segment = min(max_y_for_segment, py - 1);
    }
    
    if (max_y_for_segment >= lb) {
        if (n >= current_x) {
            ll height = max_y_for_segment - lb + 1;
            if (height <= 0) return current_x;
            ll offset = (target_area - current_area - 1) / height;
            return current_x + offset;
        }
    }

    return la;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n;

    la = 1;
    lb = 1;

    while (true) {
        auto it = p.begin();
        while (it != p.end() && it->first <= la) {
            it = p.erase(it);
        }
        
        i128 total_area = calculate_area();
        
        ll qx, qy;
        if (total_area <= 0) {
            qx = la > n ? n : la;
            qy = lb > n ? n : lb;
        } else {
            qx = find_median_x(total_area);
            ll uy = get_uy(qx);

            if (uy < lb) {
                qy = lb;
            } else {
                qy = lb + (uy - lb) / 2;
            }
        }
        
        cout << qx << " " << qy << endl;

        int resp;
        cin >> resp;

        if (resp == 0) {
            return 0;
        } else if (resp == 1) {
            la = max(la, qx + 1);
        } else if (resp == 2) {
            lb = max(lb, qy + 1);
        } else { // resp == 3
            update_p(qx, qy);
        }
    }

    return 0;
}