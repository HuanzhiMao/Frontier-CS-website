#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

long long n;
long long k;

// Memoization to avoid re-querying the same cell.
// This might help, especially if the random pivot picker hits the same cell.
vector<vector<long long>> memo;

long long do_query(int r, int c) {
    if (memo[r][c] != -1) {
        return memo[r][c];
    }
    cout << "QUERY " << r << " " << c << endl;
    long long v;
    cin >> v;
    if (v == -1) exit(0); // Error case
    return memo[r][c] = v;
}

// Counts number of elements <= val
// This takes at most 2n-1 queries.
long long count_le(long long val) {
    long long count = 0;
    int c = n;
    for (int r = 1; r <= n; ++r) {
        while (c >= 1 && do_query(r, c) > val) {
            c--;
        }
        count += c;
    }
    return count;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> k;
    memo.assign(n + 1, vector<long long>(n + 1, -1));

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distrib(1, n);

    long long L_val = -1;
    long long R_val = 2e18; // A safe initial range
    long long k_target = k;
    long long count_at_L = 0;

    while (true) {
        int r = distrib(gen);
        int c = distrib(gen);
        long long p = do_query(r, c);
        
        if (p <= L_val || p >= R_val) continue;

        long long count_le_p = count_le(p);
        long long items_le_p_in_range = count_le_p - count_at_L;

        long long count_lt_p = count_le(p - 1);
        long long items_lt_p_in_range = count_lt_p - count_at_L;

        if (k_target > items_lt_p_in_range && k_target <= items_le_p_in_range) {
            cout << "DONE " << p << endl;
            return 0;
        }
        
        if (k_target <= items_lt_p_in_range) {
            R_val = p;
        } else { // k_target > items_le_p_in_range
            k_target -= items_le_p_in_range;
            L_val = p;
            count_at_L = count_le_p;
        }
    }

    return 0;
}