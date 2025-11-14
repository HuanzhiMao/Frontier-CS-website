#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

const int N = 30;

struct Pos {
    int x, y;
};

struct SwapOp {
    Pos p1, p2;
};

std::vector<std::vector<int>> b;
std::vector<SwapOp> operations;

void perform_swap(Pos p1, Pos p2) {
    std::swap(b[p1.x][p1.y], b[p2.x][p2.y]);
    operations.push_back({p1, p2});
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

    for (int x = N - 2; x >= 0; --x) {
        for (int y = 0; y <= x; ++y) {
            int cur_x = x;
            int cur_y = y;
            
            while (cur_x < N - 1) {
                int child1_x = cur_x + 1, child1_y = cur_y;
                int child2_x = cur_x + 1, child2_y = cur_y + 1;
                
                Pos parent_pos = {cur_x, cur_y};
                Pos child1_pos = {child1_x, child1_y};
                Pos child2_pos = {child2_x, child2_y};

                Pos smaller_child_pos;
                if (b[child1_pos.x][child1_pos.y] < b[child2_pos.x][child2_pos.y]) {
                    smaller_child_pos = child1_pos;
                } else {
                    smaller_child_pos = child2_pos;
                }

                if (b[parent_pos.x][parent_pos.y] > b[smaller_child_pos.x][smaller_child_pos.y]) {
                    perform_swap(parent_pos, smaller_child_pos);
                    cur_x = smaller_child_pos.x;
                    cur_y = smaller_child_pos.y;
                } else {
                    break;
                }
            }
        }
    }

    std::cout << operations.size() << std::endl;
    for (const auto& op : operations) {
        std::cout << op.p1.x << " " << op.p1.y << " " << op.p2.x << " " << op.p2.y << std::endl;
    }

    return 0;
}