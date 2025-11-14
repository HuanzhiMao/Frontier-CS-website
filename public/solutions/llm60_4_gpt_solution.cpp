#include <bits/stdc++.h>
using namespace std;

const int BOX = 100000;
const double EPS = 5e-7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto ask = [&](int x1, int y1, int x2, int y2) -> double {
        cout << "query " << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
        cout.flush();
        double res;
        if (!(cin >> res)) {
            exit(0);
        }
        return res;
    };

    unordered_map<int, double> vMemo, hMemo;
    vMemo.reserve(2048);
    hMemo.reserve(2048);

    auto vert = [&](int x) -> double {
        auto it = vMemo.find(x);
        if (it != vMemo.end()) return it->second;
        double res = ask(x, 0, x, BOX);
        vMemo.emplace(x, res);
        return res;
    };
    auto horiz = [&](int y) -> double {
        auto it = hMemo.find(y);
        if (it != hMemo.end()) return it->second;
        double res = ask(0, y, BOX, y);
        hMemo.emplace(y, res);
        return res;
    };

    auto posX = [&](int x) -> bool {
        return vert(x) > EPS;
    };
    auto posY = [&](int y) -> bool {
        return horiz(y) > EPS;
    };

    // Find x-span
    int lo = 0, hi = BOX;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (posX(mid)) hi = mid;
        else lo = mid + 1;
    }
    int firstPosX = lo;
    int leftX = firstPosX - 1;

    lo = firstPosX; hi = BOX;
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        if (posX(mid)) lo = mid;
        else hi = mid - 1;
    }
    int lastPosX = lo;
    int rightX = lastPosX + 1;

    int r_from_bounds_x = (rightX - leftX) / 2;
    int cx = leftX + r_from_bounds_x;

    // Find y-span
    lo = 0; hi = BOX;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (posY(mid)) hi = mid;
        else lo = mid + 1;
    }
    int firstPosY = lo;
    int bottomY = firstPosY - 1;

    lo = firstPosY; hi = BOX;
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        if (posY(mid)) lo = mid;
        else hi = mid - 1;
    }
    int lastPosY = lo;
    int topY = lastPosY + 1;

    int r_from_bounds_y = (topY - bottomY) / 2;
    int cy = bottomY + r_from_bounds_y;

    // Refine radius using chord through center (should be 2*r)
    int r_v = (int)llround(vert(cx) / 2.0);
    int r_h = (int)llround(horiz(cy) / 2.0);
    int r = r_v;
    if (r != r_h) r = min({r_v, r_h, r_from_bounds_x, r_from_bounds_y});

    cout << "answer " << cx << " " << cy << " " << r << endl;
    cout.flush();
    return 0;
}