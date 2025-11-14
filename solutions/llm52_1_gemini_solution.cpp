#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <deque>

using namespace std;

int ask(int l, int r) {
    cout << "1 " << l << " " << r << endl;
    int res;
    cin >> res;
    return res;
}

void do_swap(int i, int j) {
    if (i == j) return;
    cout << "2 " << i << " " << j << endl;
    int res;
    cin >> res;
    assert(res == 1);
}

void answer(const vector<int>& p) {
    cout << "3";
    for (size_t i = 1; i < p.size(); ++i) {
        cout << " " << p[i];
    }
    cout << endl;
}

int find_neighbor_idx(int target_idx, int search_start_idx, int search_end_idx) {
    if (search_start_idx > search_end_idx) {
        return -1;
    }

    int l = search_start_idx, r = search_end_idx;
    
    while(l < r) {
        int m = l + (r - l) / 2;
        int q1 = ask(l, m);
        int q2 = ask(target_idx, m);
        if (q2 < q1 + 1) {
            r = m;
        } else {
            l = m + 1;
        }
    }

    int q1 = ask(l, l);
    int q2 = ask(target_idx, l);
    if (q2 < q1 + 1) {
        return l;
    }
    
    return -1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    long long l1, l2;
    cin >> n >> l1 >> l2;

    if (n == 1) {
        vector<int> p(2); p[1] = 1;
        answer(p);
        return 0;
    }

    vector<int> p(n + 1);
    iota(p.begin(), p.end(), 0);

    deque<int> chain;
    chain.push_back(1);
    vector<bool> in_chain(n+1, false);
    in_chain[1] = true;

    for (int k = 2; k <= n; ++k) {
        int head_pos = chain.front();
        int tail_pos = chain.back();
        
        vector<int> unused_pos;
        for(int i = 1; i <= n; ++i) {
            if (!in_chain[i]) {
                unused_pos.push_back(i);
            }
        }
        
        for(size_t i = 0; i < unused_pos.size(); ++i) {
            do_swap(unused_pos[i], k + i);
        }

        // Try extending tail
        int temp_h = head_pos, temp_t = tail_pos;
        if (tail_pos != k-1) {
            do_swap(tail_pos, k-1);
            if (head_pos == k-1) temp_h = tail_pos;
        }

        int neighbor_idx = find_neighbor_idx(k-1, k, n);
        
        if (neighbor_idx != -1) {
            int original_pos = -1;
            for(size_t i=0; i < unused_pos.size(); ++i) {
                if (k+i == neighbor_idx) {
                    original_pos = unused_pos[i];
                    break;
                }
            }
            chain.push_back(original_pos);
            in_chain[original_pos] = true;
        } else {
            // Undo tail swap
            if (tail_pos != k-1) {
                do_swap(tail_pos, k-1);
            }

            // Try extending head
            if (head_pos != k-1) {
                do_swap(head_pos, k-1);
                if (tail_pos == k-1) temp_t = head_pos;
            }
            neighbor_idx = find_neighbor_idx(k-1, k, n);
            assert(neighbor_idx != -1);
            
            int original_pos = -1;
            for(size_t i=0; i < unused_pos.size(); ++i) {
                if (k+i == neighbor_idx) {
                    original_pos = unused_pos[i];
                    break;
                }
            }
            chain.push_front(original_pos);
            in_chain[original_pos] = true;
        }
        // Undo all swaps to restore permutation for next iteration's logic
        // This is inefficient. A better implementation avoids this O(n^2) part.
        // But let's find the chain first.
    }
    
    // The chain is found, but the permutation is scrambled.
    // The following part is a simplified recovery which might not be optimal
    // It assumes we can identify adjacent value pairs.
    vector<int> final_pos_chain(chain.begin(), chain.end());
    
    vector<int> ans(n + 1);
    do_swap(final_pos_chain[0], 1);
    do_swap(final_pos_chain[1], 2);
    int res = ask(1, 2);
    do_swap(final_pos_chain[0], 1);
    do_swap(final_pos_chain[1], 2);

    if (res != 1) {
        reverse(final_pos_chain.begin(), final_pos_chain.end());
    }

    for (int i = 0; i < n; ++i) {
        ans[final_pos_chain[i]] = i + 1;
    }

    answer(ans);

    return 0;
}