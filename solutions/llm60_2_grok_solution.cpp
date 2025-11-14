#include <bits/stdc++.h>
using namespace std;

int main() {
    const int MAX = 100000;
    const double EPS = 1e-5;
    int min_y, max_y, min_x, max_x;

    // Find min_y
    int low = 0, high = MAX;
    while (low < high) {
        int mid = (low + high) / 2;
        cout << "query 0 " << mid << " 100000 " << mid << endl;
        double l;
        cin >> l;
        if (l > EPS) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    min_y = low;

    // Find max_y
    low = 0, high = MAX;
    while (low < high) {
        int mid = (low + high + 1) / 2;
        cout << "query 0 " << mid << " 100000 " << mid << endl;
        double l;
        cin >> l;
        if (l > EPS) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    max_y = low;

    // Find min_x
    low = 0, high = MAX;
    while (low < high) {
        int mid = (low + high) / 2;
        cout << "query " << mid << " 0 " << mid << " 100000" << endl;
        double l;
        cin >> l;
        if (l > EPS) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    min_x = low;

    // Find max_x
    low = 0, high = MAX;
    while (low < high) {
        int mid = (low + high + 1) / 2;
        cout << "query " << mid << " 0 " << mid << " 100000" << endl;
        double l;
        cin >> l;
        if (l > EPS) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    max_x = low;

    int yc = (min_y + max_y) / 2;
    int r = (max_y - yc) + 1;
    int xc = (min_x + max_x) / 2;

    cout << "answer " << xc << " " << yc << " " << r << endl;
    return 0;
}