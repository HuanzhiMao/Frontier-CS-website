#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

// DSU structure to manage equivalence classes of bakeries
struct DSU {
    std::vector<int> parent;
    DSU(int n) {
        parent.resize(n);
        std::iota(parent.begin(), parent.end(), 0);
    }
    int find(int i) {
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
        }
    }
};

// Interaction helper functions
char query(int c) {
    std::cout << "? " << c << std::endl;
    char response;
    std::cin >> response;
    return response;
}

void reset() {
    std::cout << "R" << std::endl;
}

void answer(int d) {
    std::cout << "! " << d << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, k;
    std::cin >> n >> k;

    // The strategy is to identify bakeries with the same cake type by partitioning
    // them into blocks and comparing bakeries within and across blocks. A DSU
    // data structure tracks which bakeries are known to have the same cake type.

    // All bakeries {1, ..., n} are partitioned into blocks of size k/2.
    // This size is chosen because we can load two such blocks into memory (size k)
    // and observe interactions.
    int block_size = std::max(1, k / 2);
    int num_blocks = (n + block_size - 1) / block_size;

    std::vector<std::vector<int>> blocks(num_blocks);
    for (int i = 0; i < n; ++i) {
        blocks[i / block_size].push_back(i + 1);
    }

    DSU dsu(n + 1);
    std::vector<bool> identified(n + 1, false);
    int distinct_count = 0;
    
    // Iterate through all bakeries. If a bakery's type hasn't been identified,
    // it represents a new distinct type. We then find all other bakeries with this
    // same cake type.
    for (int i = 1; i <= n; ++i) {
        if (dsu.find(i) == i) { // Process each representative of a component
            if (!identified[i]) {
                distinct_count++;
                identified[i] = true;

                // To find all bakeries with the same type as i, we fill the memory
                // with cake type a_i. This is done by querying bakery i k times.
                reset();
                for (int rep = 0; rep < k; ++rep) {
                    query(i);
                }

                // Now, any query for a bakery j that returns 'Y' must mean a_j == a_i,
                // because the memory is saturated with a_i.
                // We check all other bakeries that are not yet fully identified.
                for (int j = 1; j <= n; ++j) {
                    if (dsu.find(j) == j && !identified[j]) {
                        if (query(j) == 'Y') {
                            dsu.unite(i, j);
                            identified[j] = true;
                        }
                    }
                }
            }
        }
    }

    answer(distinct_count);

    return 0;
}