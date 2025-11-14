#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

int query(int i, int j) {
    std::cout << "? " << i + 1 << " " << j + 1 << std::endl;
    int result;
    std::cin >> result;
    if (result == -1) exit(0);
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<int> p(n);
    int zero_cand_idx = 0;

    for (int i = 1; i < n; ++i) {
        int k = -1;
        for (int j = 0; j < n; ++j) {
            if (j != zero_cand_idx && j != i) {
                k = j;
                break;
            }
        }
        
        int q_ci = query(zero_cand_idx, i);
        int q_ck = query(zero_cand_idx, k);
        int q_ik = query(i, k);

        if ((q_ci | q_ck) != q_ik) { // p[zero_cand_idx] is not a submask of p[i]|p[k]
            // This means p[zero_cand_idx] cannot be 0
            // The true zero is among i and k, and we don't know which one.
            // But we know for sure zero_cand_idx is not it.
            // So we can update our candidate to i and continue.
            // The real zero will never be eliminated.
        } else if ((q_ci | q_ik) != q_ck) { // p[i] is not a submask of p[zero_cand_idx]|p[k]
             // p[i] cannot be 0, zero_cand_idx remains the candidate.
        } else { // both could be zero, maybe k isn't.
            // if p[k] is not submask of p[c]|p[i]
            if ((q_ck | q_ik) != q_ci) {
                 // p[k] cannot be 0. 
                 // We don't learn about c or i.
            } else {
                 // all are submasks of each other's ORs.
                 // we cannot eliminate anyone. We keep our candidate.
            }
        }

        // Simpler elimination logic:
        // Test if p[i] is a better candidate than p[zero_cand_idx].
        // p[i] | p[k] is q_ik. If p[zero_cand_idx] | q_ik == q_ik,
        // it means p[zero_cand_idx] is a submask of p[i]|p[k], so it's still a candidate.
        // Let's test p[i] against p[zero_cand_idx]|p[k] which is q_ck.
        // If p[i] | q_ck != q_ck, p[i] is not a submask, so p[i] cannot be 0.
        // zero_cand_idx stays.
        // Otherwise, p[i] might be 0. Let's switch candidate.
        if ((q_ck & q_ik) == q_ik) { // p[i] is submask of p[cand], so cand could be p[i]|p[k]
             // but p[i]|p[k] is q_ik, so p[cand] is a submask of q_ik
        }
        
        int res_cand = query(zero_cand_idx, k);
        int res_i = query(i, k);
        if (res_cand > res_i) {
            zero_cand_idx = i;
        } else if (res_cand == res_i) {
            // Tie-break, query with another witness.
            // This simple greedy choice is not robust. A tournament approach is better.
        }
    }
    
    // A robust tournament-like approach
    std::vector<int> candidates(n);
    std::iota(candidates.begin(), candidates.end(), 0);

    while (candidates.size() >= 2) {
        int c1 = candidates.back();
        candidates.pop_back();
        if (candidates.empty()) { // Should not happen with >=2 check
            candidates.push_back(c1);
            break;
        }
        int c2 = candidates.back();
        candidates.pop_back();
        
        int witness = -1;
        for (int i = 0; i < n; ++i) {
            if (i != c1 && i != c2) {
                witness = i;
                break;
            }
        }
        if (witness == -1) { // n=2 case, not possible by constraints
           witness = 0; // fallback
        }

        int q_c1_w = query(c1, witness);
        int q_c2_w = query(c2, witness);
        
        if (q_c1_w < q_c2_w) {
            candidates.push_back(c1);
        } else if (q_c2_w < q_c1_w) {
            candidates.push_back(c2);
        } else { // q_c1_w == q_c2_w
            // p[c1]|p[w] == p[c2]|p[w]
            // This implies p[c1] and p[c2] are identical on bits where p[w] is 0
            // Let's test against c1|c2
            int q_c1_c2 = query(c1, c2);
            if (q_c1_c2 < q_c1_w) { // p[c1]|p[c2] < p[c1]|p[w]
                // p[c2] must have more 0s where p[c1] has 1s than p[w] does.
                // suggests p[c2] is smaller.
                candidates.push_back(c2);
            } else {
                candidates.push_back(c1);
            }
        }
    }
    
    zero_cand_idx = candidates[0];
    p[zero_cand_idx] = 0;
    for (int i = 0; i < n; i++) {
        if (i == zero_cand_idx) continue;
        p[i] = query(zero_cand_idx, i);
    }

    std::cout << "! ";
    for (int i = 0; i < n; ++i) {
        std::cout << p[i] << (i == n - 1 ? "" : " ");
    }
    std::cout << std::endl;

    return 0;
}