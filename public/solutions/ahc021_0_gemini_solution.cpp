#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

const int N = 30;
std::vector<std::vector<int>> b;
std::vector<std::tuple<int, int, int, int>> ops;

void do_swap(int r1, int c1, int r2, int c2) {
    ops.emplace_back(r1, c1, r2, c2);
    std::swap(b[r1][c1], b[r2][c2]);
}

void sift_down(int r, int c) {
    int cur_r = r;
    int cur_c = c;
    
    while (cur_r < N - 1) {
        int child1_r = cur_r + 1;
        int child1_c = cur_c;
        int child2_r = cur_r + 1;
        int child2_c = cur_c + 1;
        
        int min_child_r, min_child_c;
        if (b[child1_r][child1_c] < b[child2_r][child2_c]) {
            min_child_r = child1_r;
            min_child_c = child1_c;
        } else {
            min_child_r = child2_r;
            min_child_c = child2_c;
        }
        
        if (b[cur_r][cur_c] > b[min_child_r][min_child_c]) {
            do_swap(cur_r, cur_c, min_child_r, min_child_c);
            cur_r = min_child_r;
            cur_c = min_child_c;
        } else {
            break;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    b.resize(N);
    for (int i = 0; i < N; ++i) {
        b[i].resize(i + 1);
        for (int j = 0; j <= i; ++j) {
            std::cin >> b[i][j];
        }
    }
    
    for (int r = N - 2; r >= 0; --r) {
        for (int c = 0; c <= r; ++c) {
            sift_down(r, c);
        }
    }
    
    std::cout << ops.size() << "\n";
    for (const auto& op : ops) {
        std::cout << std::get<0>(op) << " " << std::get<1>(op) << " " << std::get<2>(op) << " " << std::get<3>(op) << "\n";
    }
    
    return 0;
}