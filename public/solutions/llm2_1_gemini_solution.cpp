#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

// Helper to ask a query
void ask_query(int n, const std::vector<int>& q) {
    std::cout << "0";
    for (int i = 0; i < n; ++i) {
        std::cout << " " << q[i];
    }
    std::cout << std::endl;
}

// Helper to submit the final answer
void submit_answer(int n, const std::vector<int>& p) {
    std::cout << "1";
    for (int i = 0; i < n; ++i) {
        std::cout << " " << p[i];
    }
    std::cout << std::endl;
}

// Helper to get the k-th bit of n
int get_bit(int n, int k) {
    return (n >> k) & 1;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    if (n == 1) {
        submit_answer(1, {1});
        return 0;
    }

    // Use 1-based indexing for permutation p for convenience
    std::vector<int> p(n + 1, 0);
    
    int k = 0;
    while ((1 << k) < n) {
        k++;
    }

    for (int m = 1; m * 2 <= n; ++m) {
        int v1 = 2 * m - 1;
        int v2 = 2 * m;

        std::vector<int> pos1_bits(k);
        std::vector<int> pos2_bits(k);
        std::vector<int> ambiguous_bits;
        int unambiguous_bit_idx = -1;
        int unambiguous_bit_ans = -1;

        for (int j = 0; j < k; ++j) {
            std::vector<int> q(n);
            for (int i = 0; i < n; ++i) {
                if (get_bit(i, j)) {
                    q[i] = v2;
                } else {
                    q[i] = v1;
                }
            }
            ask_query(n, q);
            int ans;
            std::cin >> ans;

            if (ans == 0) { // b(pos1-1,j)=1, b(pos2-1,j)=0
                pos1_bits[j] = 1;
                pos2_bits[j] = 0;
                if (unambiguous_bit_idx == -1) {
                    unambiguous_bit_idx = j;
                    unambiguous_bit_ans = 0;
                }
            } else if (ans == 2) { // b(pos1-1,j)=0, b(pos2-1,j)=1
                pos1_bits[j] = 0;
                pos2_bits[j] = 1;
                if (unambiguous_bit_idx == -1) {
                    unambiguous_bit_idx = j;
                    unambiguous_bit_ans = 2;
                }
            } else { // ans == 1, b(pos1-1,j) == b(pos2-1,j)
                ambiguous_bits.push_back(j);
            }
        }
        
        for (int j_amb : ambiguous_bits) {
            std::vector<int> q(n);
            for (int i = 0; i < n; ++i) {
                if ((get_bit(i, j_amb) ^ get_bit(i, unambiguous_bit_idx)) == 0) {
                    q[i] = v1;
                } else {
                    q[i] = v2;
                }
            }
            ask_query(n, q);
            int ans;
            std::cin >> ans;

            int common_bit;
            if (unambiguous_bit_ans == 2) { // b(pos1,unamb)=0, b(pos2,unamb)=1 -> ans = 2*[common_bit==0]
                common_bit = (ans == 2) ? 0 : 1;
            } else { // unambiguous_bit_ans == 0, b(pos1,unamb)=1, b(pos2,unamb)=0 -> ans = 2*[common_bit==1]
                common_bit = (ans == 2) ? 1 : 0;
            }
            pos1_bits[j_amb] = common_bit;
            pos2_bits[j_amb] = common_bit;
        }

        int pos1_idx = 0;
        int pos2_idx = 0;
        for (int j = 0; j < k; ++j) {
            if (pos1_bits[j]) {
                pos1_idx |= (1 << j);
            }
            if (pos2_bits[j]) {
                pos2_idx |= (1 << j);
            }
        }
        
        p[pos1_idx + 1] = v1;
        p[pos2_idx + 1] = v2;
    }

    if (n % 2 == 1) {
        for (int i = 1; i <= n; ++i) {
            if (p[i] == 0) {
                p[i] = n;
                break;
            }
        }
    }
    
    std::vector<int> final_p(n);
    for(int i=0; i<n; ++i) {
        final_p[i] = p[i+1];
    }
    submit_answer(n, final_p);

    return 0;
}