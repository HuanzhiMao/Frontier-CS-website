#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// Function to ask a query and parse the response
// Returns a vector of pairs, where each pair represents a closest pair of doors.
// The pairs are sorted (u < v).
std::vector<std::pair<int, int>> ask(int x, int y, int z) {
    std::cout << "? " << x << " " << y << " " << z << std::endl;
    int r;
    std::cin >> r;
    std::vector<std::pair<int, int>> result;
    for (int i = 0; i < r; ++i) {
        int u, v;
        std::cin >> u >> v;
        if (u > v) std::swap(u, v);
        result.push_back({u, v});
    }
    return result;
}

// Helper to check if a pair {u, v} exists in the response vector.
bool contains_pair(const std::vector<std::pair<int, int>>& vec, int u, int v) {
    if (u > v) std::swap(u, v);
    for (const auto& p : vec) {
        if (p.first == u && p.second == v) {
            return true;
        }
    }
    return false;
}

// Recursive function to sort points on an arc defined by p_start and p_end.
// Uses a quicksort-like partitioning strategy based on "betweenness" queries.
std::vector<int> sort_arc(const std::vector<int>& points, int p_start, int p_end) {
    if (points.size() <= 1) {
        return points;
    }

    // Using a random pivot to avoid worst-case quicksort behavior.
    int pivot_idx = rand() % points.size();
    int pivot = points[pivot_idx];
    
    std::vector<int> left, right;
    for (int i = 0; i < points.size(); ++i) {
        if (i == pivot_idx) continue;
        int s = points[i];
        
        auto res = ask(p_start, pivot, s);
        
        // s is "between" p_start and pivot if it lies on the minor arc defined by them.
        // This is true if {p_start, pivot} is NOT among the closest pairs in the query ? p_start pivot s.
        // If s is between p_start and pivot, it comes before pivot in the sorted arc from p_start.
        if (!contains_pair(res, p_start, pivot)) {
            left.push_back(s);
        } else {
            right.push_back(s);
        }
    }

    std::vector<int> sorted_left = sort_arc(left, p_start, pivot);
    std::vector<int> sorted_right = sort_arc(right, pivot, p_end);
    
    std::vector<int> result = sorted_left;
    result.push_back(pivot);
    result.insert(result.end(), sorted_right.begin(), sorted_right.end());
    
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    srand(time(0));

    int k, n;
    std::cin >> k >> n;

    if (n <= 2) {
        std::cout << "!";
        for (int i = 0; i < n; ++i) {
            std::cout << " " << i;
        }
        std::cout << std::endl;
        return 0;
    }

    // Step 1: Pick two arbitrary doors as a baseline.
    int p1 = 0, p2 = 1;

    // Step 2: Partition the remaining doors into two sets:
    // s_minor: doors on the minor arc between p1 and p2.
    // s_major: doors on the major arc between p1 and p2.
    std::vector<int> s_minor, s_major;
    for (int i = 2; i < n; ++i) {
        auto res = ask(p1, p2, i);
        // A point i is on the major arc of (p1, p2) if the side p1-p2 is the
        // shortest side of the triangle p1-p2-i.
        if (contains_pair(res, p1, p2)) {
            s_major.push_back(i);
        } else {
            s_minor.push_back(i);
        }
    }

    // Step 3: Sort the points on each arc.
    std::vector<int> sorted_minor = sort_arc(s_minor, p1, p2);
    std::vector<int> sorted_major = sort_arc(s_major, p1, p2);

    // Step 4: Combine the sorted lists to form the full circular order.
    // The order is p1 -> sorted_minor -> p2 -> sorted_major_reversed -> p1
    std::vector<int> final_order;
    final_order.push_back(p1);
    final_order.insert(final_order.end(), sorted_minor.begin(), sorted_minor.end());
    final_order.push_back(p2);
    std::reverse(sorted_major.begin(), sorted_major.end());
    final_order.insert(final_order.end(), sorted_major.begin(), sorted_major.end());

    // Output the answer. Any valid starting point and direction is accepted.
    std::cout << "!";
    for (int i = 0; i < n; ++i) {
        std::cout << " " << final_order[i];
    }
    std::cout << std::endl;

    return 0;
}