#include <iostream>

void ask_and_flush(long long x, long long y) {
    std::cout << x << " " << y << std::endl;
}

int read_response() {
    int response;
    std::cin >> response;
    if (std::cin.fail() || response == -1) {
        exit(0);
    }
    return response;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long n;
    std::cin >> n;

    long long la = 1, ra = n;
    long long lb = 1, rb = n;

    while (true) {
        if (la == ra && lb == rb) {
            ask_and_flush(la, lb);
            read_response();
            break;
        }

        long long ma = la + (ra - la) / 2;
        long long mb = lb + (rb - lb) / 2;

        ask_and_flush(ma, mb);
        int res = read_response();

        if (res == 0) {
            break;
        } else if (res == 1) {
            la = ma + 1;
        } else if (res == 2) {
            lb = mb + 1;
        } else { // res == 3
            ra = ma;
            rb = mb;
        }
    }

    return 0;
}