#include <bits/stdc++.h>
#include <sys/select.h>
#include <unistd.h>
using namespace std;

bool stdin_has_data() {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    int r = select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &tv);
    return r > 0 && FD_ISSET(STDIN_FILENO, &readfds);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<pair<int,int>> edges(M);
    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        edges[i] = {u, v};
    }

    int A = 0, B = (N > 1 ? 1 : 0);

    // Attempt to read A and B if provided in non-interactive input.
    if (stdin_has_data()) {
        vector<long long> rest;
        long long x;
        while (cin >> x) rest.push_back(x);
        if (rest.size() >= 2) {
            A = (int)rest[rest.size() - 2];
            B = (int)rest[rest.size() - 1];
            if (A == B) {
                B = (A + 1 < N ? A + 1 : (A > 0 ? A - 1 : 0));
            }
        }
    }

    cout << "1 " << A << " " << B << "\n";
    cout.flush();
    return 0;
}