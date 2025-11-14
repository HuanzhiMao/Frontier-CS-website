#include <bits/stdc++.h>
using namespace std;

#define sz(a) (int) (a.size())
#define fi first
#define se second
#define ll long long

double eps = 1e-6;
int Round(double x) {
    int tmp = (int) x;
    for(int y = tmp - 2; y <= tmp + 2; ++y) {
        if(abs(y - x) <= eps) return y;
    }
    return tmp;
}

int main()
{
    ios_base :: sync_with_stdio (0); cin.tie (0); cout.tie (0);

    int val = 1, pos;
    while(val <= 100000) {
        cout << "query " << val << " " << 0 << " " << val << " " << 100000 << endl;
        double len; cin >> len;
        if(len > eps) {
            pos = val;
            break;
        }
        val += 199;
    }

    int l = 1, r = 100000, y = 0;
    while(l <= r) {
        int mid = (l + r) / 2;
        cout << "query " << pos << " " << 0 << " " << pos << " " << mid << endl;
        double len; cin >> len;
        if(len > eps) {
            y = mid;
            r = mid - 1;
        }
        else l = mid + 1;
    }

    int x = val;
    double a1, a2, b1, b2;

    cout << "query " << x << " " << 0 << " " << x << " " << y << endl;
    cin >> b2;

    cout << "query " << x << " " << y << " " << x << " " << 100000 << endl;
    cin >> b1;

    cout << "query " << 0 << " " << y << " " << x << " " << y << endl;
    cin >> a1;

    cout << "query " << x << " " << y << " " << 100000 << " " << y << endl;
    cin >> a2;

    int radius = Round(sqrt(a1 * a1 + a2 * a2 + b1 * b1 + b2 * b2) / 2);
    int resx = x + Round((a1 + a2) / 2 - a1);
    int resy = y + Round((b1 + b2) / 2 - b2);

    cout << "answer " << resx << " " << resy << " " << radius << '\n';

    return 0;
}

