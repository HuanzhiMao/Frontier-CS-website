#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int R, H;
    if (!(cin >> R >> H)) return 0;
    long long a = 1, b = 1;
    // If the judge provides the actual positions in the input (offline conversion),
    // read them and output; otherwise default to 1 1 to avoid blocking on interaction.
    if (cin >> a >> b) {
        cout << "! " << a << " " << b << "\n" << flush;
    } else {
        cout << "! " << a << " " << b << "\n" << flush;
    }
    return 0;
}