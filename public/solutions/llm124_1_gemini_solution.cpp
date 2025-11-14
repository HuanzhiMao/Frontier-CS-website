#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// Helper to ask a query
int query(const std::vector<int>& indices) {
    std::cout << "? " << indices.size();
    for (int i : indices) {
        std::cout << " " << i;
    }
    std::cout << std::endl;
    int result;
    std::cin >> result;
    return result;
}

// Helper to output the answer
void answer(const std::vector<int>& p) {
    std::cout << "!";
    for (size_t i = 0; i < p.size(); ++i) {
        std::cout << " " << p[i];
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    // Step 1: Find pos(1) and pos(n)
    // The query "? n-1 {all indices except i}" is 1 iff p[i] is 1 or n.
    // This is because sum(p) = n(n+1)/2. For n even, n(n+1)/2 = (n/2)(n+1).
    // Let S = n(n+1)/2. The sum in query is S-p[i].
    // S mod (n-1) = 1.
    // So (S-p[i]) mod (n-1) == 0 iff (1-p[i]) mod (n-1) == 0.
    // This holds for p[i]=1 and p[i]=n.
    int pos1 = -1, posn = -1;
    std::vector<int> all_indices(n);
    std::iota(all_indices.begin(), all_indices.end(), 1);

    for (int i = 1; i <= n; ++i) {
        std::vector<int> query_indices;
        for (int j = 1; j <= n; ++j) {
            if (i == j) continue;
            query_indices.push_back(j);
        }
        if (query(query_indices) == 1) {
            if (pos1 == -1) pos1 = i;
            else posn = i;
        }
    }

    std::vector<int> p(n);
    // If pos1 is 1, p[0] must be 1 because p[0] <= n/2.
    // Otherwise, p[0] could be anything.
    if (pos1 == 1) {
        p[0] = 1;
        p[posn - 1] = n;
    } else if (posn == 1) {
        p[0] = 1;
        p[pos1 - 1] = n;
    } else {
        // We found {pos(1), pos(n)}, let them be pos1, posn.
        // We need to determine which is which.
        // p[pos1] and p[posn] have different parities.
        // Query(? 2 1 pos1) tells us if p[0] and p[pos1] have same parity.
        // If they do, p[0] and 1 are both odd, or p[0] and n are both even.
        // As p[0] <= n/2 and n is even, n must be p[posn] if p[pos1] is even.
        // A single query {1, pos1} is enough to check parities.
        // p[0] and p[pos1] have same parity iff (p[0]+p[pos1]) is even.
        if (query({1, pos1}) == 1) { // Same parity
            p[pos1-1] = 1; // 1 is odd. p[0] must be odd too.
            p[posn-1] = n;
        } else { // Different parity
            p[pos1-1] = n; // n is even. p[0] must be odd.
            p[posn-1] = 1;
        }
    }
    
    // Step 2: Find all other values
    // We have found p[pos(1)-1] = 1 and p[pos(n)-1]=n.
    // For any other index i, query "? 2 pos(1) i".
    // This tells if p[i] has same parity as p[pos(1)], which is 1 (odd).
    // So we know the parity of p[i] for all i.
    int real_pos1 = -1;
    if (p[pos1 - 1] == 1) real_pos1 = pos1;
    else real_pos1 = posn;

    for (int i = 1; i <= n; ++i) {
        if (p[i - 1] == 0) { // If value not yet known
            if (query({real_pos1, i}) == 1) { // p[i] is odd
                // Binary search for the odd value
                int low = 1, high = n / 2, val = -1;
                while (low <= high) {
                    int mid_k = (low + high) / 2;
                    int mid_val = 2 * mid_k - 1;
                    if (p[mid_val-1] != 0) { // already assigned
                        low = mid_k + 1;
                        continue;
                    }
                    if (query({i, mid_val}) == 1) {
                        val = mid_val;
                        p[i - 1] = val;
                        high = mid_k - 1;
                    } else {
                        low = mid_k + 1;
                    }
                }
            } else { // p[i] is even
                // Binary search for the even value
                int low = 1, high = n / 2, val = -1;
                 while (low <= high) {
                    int mid_k = (low + high) / 2;
                    int mid_val = 2 * mid_k;
                     if (p[mid_val-1] != 0) {
                        low = mid_k + 1;
                        continue;
                    }
                    if (query({i, mid_val}) == 1) {
                        val = mid_val;
                        p[i - 1] = val;
                        high = mid_k - 1;
                    } else {
                        low = mid_k + 1;
                    }
                }
            }
        }
    }

    // Due to ambiguity, p and n+1-p are indistinguishable.
    // The problem guarantees p[0] <= n/2.
    // If our resulting p[0] > n/2, we must flip the permutation.
    if (p[0] > n / 2) {
        for (int i = 0; i < n; ++i) {
            p[i] = n + 1 - p[i];
        }
    }

    answer(p);

    return 0;
}