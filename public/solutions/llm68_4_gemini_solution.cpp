#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <list>

// Function to perform a query for a given pen index.
// It handles printing the query and reading the judge's response.
int query(int pen_idx) {
    std::cout << "0 " << pen_idx << std::endl;
    int response;
    std::cin >> response;
    return response;
}

// Function to output the final answer.
void answer(int p1, int p2) {
    std::cout << "1 " << p1 << " " << p2 << std::endl;
}

// Main logic to solve a single test case.
void solve() {
    int n;
    std::cin >> n;
    
    // Use a list as a pool of candidates for efficient removal from the front
    // and addition to the back (cycling).
    std::list<int> candidates;
    for(int i = 0; i < n; ++i) {
        candidates.push_back(i);
    }

    // Repeatedly take 3 candidates, find the one with the least ink,
    // discard it, and put the other two back into the pool.
    // This process eliminates one pen per step until only two remain.
    while (candidates.size() > 2) {
        // Take the first three candidates from the list.
        int p1 = candidates.front(); candidates.pop_front();
        int p2 = candidates.front(); candidates.pop_front();
        int p3 = candidates.front(); candidates.pop_front();

        // Find the pen with the minimum ink among p1, p2, p3
        // by querying them in a round-robin fashion until one becomes empty.
        int min_pen = -1;
        while(min_pen == -1) {
            if(query(p1) == 0) {
                min_pen = p1;
                break;
            }
            if(query(p2) == 0) {
                min_pen = p2;
                break;
            }
            if(query(p3) == 0) {
                min_pen = p3;
                break;
            }
        }

        // The two pens that did not run out of ink (the survivors)
        // are added back to the end of the list to be considered in later rounds.
        if (p1 != min_pen) candidates.push_back(p1);
        if (p2 != min_pen) candidates.push_back(p2);
        if (p3 != min_pen) candidates.push_back(p3);
    }

    // The last two candidates in the list are our answer.
    answer(candidates.front(), candidates.back());
}

int main() {
    // Fast I/O is good practice in competitive programming.
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }

    return 0;
}