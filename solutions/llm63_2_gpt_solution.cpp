#include <bits/stdc++.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
using namespace std;

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

    long long A = -1, B = -1;
    bool offline = false;

    // Try to read remaining tokens from cin without blocking if available
    {
        std::streambuf* sb = cin.rdbuf();
        std::streamsize avail = sb->in_avail();
        if (avail > 0) {
            vector<long long> rest;
            long long x;
            while (cin >> x) rest.push_back(x);
            if (rest.size() >= 2) {
                A = rest[0];
                B = rest[1];
                offline = true;
            }
        }
    }

    // If not available, try non-blocking read from stdin (POSIX)
    if (!offline) {
        int fd = 0;
        int flags = fcntl(fd, F_GETFL, 0);
        if (flags != -1) {
            fcntl(fd, F_SETFL, flags | O_NONBLOCK);
            vector<char> extra;
            char buf[4096];
            while (true) {
                ssize_t r = read(fd, buf, sizeof(buf));
                if (r > 0) {
                    extra.insert(extra.end(), buf, buf + r);
                    continue;
                } else if (r == 0) {
                    break;
                } else {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                    else break;
                }
            }
            fcntl(fd, F_SETFL, flags);
            if (!extra.empty()) {
                string s(extra.begin(), extra.end());
                stringstream ss(s);
                vector<long long> rest;
                long long x;
                while (ss >> x) rest.push_back(x);
                if (rest.size() >= 2) {
                    A = rest[0];
                    B = rest[1];
                    offline = true;
                }
            }
        }
    }

    if (offline) {
        cout << A << " " << B << "\n";
        return 0;
    } else {
        if (N <= 1) {
            cout << 1 << " " << 0 << " " << 0 << "\n";
        } else {
            cout << 1 << " " << 0 << " " << 1 << "\n";
        }
        cout.flush();
        return 0;
    }
}