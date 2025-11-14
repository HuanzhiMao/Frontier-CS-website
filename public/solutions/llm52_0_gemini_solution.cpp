#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

int n, l1, l2;
std::vector<int> p_ans;
std::vector<int> curr_pos_of;
std::vector<int> orig_pos_at;

int ask(int l, int r) {
    std::cout << "1 " << l << " " << r << std::endl;
    int res;
    std::cin >> res;
    return res;
}

void do_swap(int i, int j) {
    if (i == j) return;
    std::cout << "2 " << i << " " << j << std::endl;
    int res;
    std::cin >> res;
    std::swap(curr_pos_of[orig_pos_at[i]], curr_pos_of[orig_pos_at[j]]);
    std::swap(orig_pos_at[i], orig_pos_at[j]);
}

void answer() {
    std::cout << "3";
    for (int i = 1; i <= n; ++i) {
        std::cout << " " << p_ans[i];
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> n >> l1 >> l2;

    p_ans.resize(n + 1, 0);

    if (n == 1) {
        p_ans[1] = 1;
        answer();
        return 0;
    }

    curr_pos_of.resize(n + 1);
    orig_pos_at.resize(n + 1);
    std::iota(curr_pos_of.begin() + 1, curr_pos_of.end(), 1);
    std::iota(orig_pos_at.begin() + 1, orig_pos_at.end(), 1);

    int pos_e1 = -1, pos_e2 = -1;

    for (int i = 1; i <= n; ++i) {
        do_swap(1, i);
        int c1 = ask(1, n);
        int c2 = ask(2, n);
        int num_neighbors = 1 - c1 + c2;
        if (num_neighbors == 1) {
            if (pos_e1 == -1) {
                pos_e1 = i;
            } else {
                pos_e2 = i;
            }
        }
        do_swap(1, i);
    }
    
    p_ans[pos_e1] = 1;
    p_ans[pos_e2] = n;

    int curr_val_orig_pos = pos_e1;
    int prev_val_orig_pos = -1;

    for (int v = 1; v <= n - 2; ++v) {
        int phys_pos_v = curr_pos_of[curr_val_orig_pos];
        do_swap(1, phys_pos_v);

        if (v > 1) {
            int phys_pos_prev = curr_pos_of[prev_val_orig_pos];
            do_swap(2, phys_pos_prev);
        }

        int low = (v == 1) ? 2 : 3;
        int high = n;
        int next_phys_pos = -1;

        int target_neighbors = (v == 1) ? 1 : 2;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            int c1 = ask(1, mid);
            int c2 = ask(2, mid);
            int num_neighbors_in_prefix = 1 - c1 + c2;
            
            if (num_neighbors_in_prefix == target_neighbors) {
                next_phys_pos = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        int next_val_orig_pos = orig_pos_at[next_phys_pos];
        p_ans[next_val_orig_pos] = v + 1;
        
        prev_val_orig_pos = curr_val_orig_pos;
        curr_val_orig_pos = next_val_orig_pos;
    }

    answer();

    return 0;
}