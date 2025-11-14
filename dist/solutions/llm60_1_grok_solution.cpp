#include <bits/stdc++.h>
using namespace std;

const int MAX = 100000;

double probe(int x1, int y1, int x2, int y2) {
    cout << "query " << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
    cout.flush();
    double len;
    cin >> len;
    return len;
}

pair<int, double> find_max_vertical() {
    int low = 0, high = MAX;
    while (high - low >= 3) {
        int diff = (high - low) / 3;
        int m1 = low + diff;
        int m2 = high - diff;
        double l1 = probe(m1, 0, m1, MAX);
        double l2 = probe(m2, 0, m2, MAX);
        if (l1 < l2) {
            low = m1;
        } else {
            high = m2;
        }
    }
    double max_len = -1;
    int max_pos = -1;
    for (int a = low; a <= high; a++) {
        double len = probe(a, 0, a, MAX);
        if (len > max_len) {
            max_len = len;
            max_pos = a;
        }
    }
    return {max_pos, max_len};
}

pair<int, double> find_max_horizontal() {
    int low = 0, high = MAX;
    while (high - low >= 3) {
        int diff = (high - low) / 3;
        int m1 = low + diff;
        int m2 = high - diff;
        double l1 = probe(0, m1, MAX, m1);
        double l2 = probe(0, m2, MAX, m2);
        if (l1 < l2) {
            low = m1;
        } else {
            high = m2;
        }
    }
    double max_len = -1;
    int max_pos = -1;
    for (int b = low; b <= high; b++) {
        double len = probe(0, b, MAX, b);
        if (len > max_len) {
            max_len = len;
            max_pos = b;
        }
    }
    return {max_pos, max_len};
}

int main() {
    auto [cx, lenx] = find_max_vertical();
    auto [cy, leny] = find_max_horizontal();
    int r = round(lenx / 2.0);
    cout << "answer " << cx << " " << cy << " " << r << endl;
    return 0;
}