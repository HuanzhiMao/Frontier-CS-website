#include <iostream>
#include <vector>
#include <numeric>

// Using std namespace for competitive programming convenience
using namespace std;

// Global variables to store the state of the game
int n, m;
vector<int> pillars[55];
vector<pair<int, int>> moves;

// Function to perform a move and record it
void perform_move(int from, int to) {
    // A move is only valid if 'from' and 'to' are different
    if (from == to) return;

    // Get the top ball from the 'from' pillar
    int ball = pillars[from].back();
    pillars[from].pop_back();

    // Place the ball on top of the 'to' pillar
    pillars[to].push_back(ball);

    // Record the move
    moves.push_back({from, to});
}

// Function to check if a pillar is sorted correctly
// A pillar 'p' is sorted if it contains 'm' balls, all of which are of color 'p'
bool is_pillar_sorted(int p) {
    if (pillars[p].size() != m) {
        return false;
    }
    for (int ball_color : pillars[p]) {
        if (ball_color != p) {
            return false;
        }
    }
    return true;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Read problem size
    cin >> n >> m;

    // Read initial ball configuration
    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j < m; ++j) {
            int color;
            cin >> color;
            pillars[i].push_back(color);
        }
    }

    // A vector to keep track of which pillars are already sorted
    vector<bool> pillar_is_finalized(n + 2, false);

    int finalized_pillars_count = 0;
    while (finalized_pillars_count < n) {
        // Update the count of finalized pillars
        int current_finalized_count = 0;
        for (int i = 1; i <= n; ++i) {
            if (is_pillar_sorted(i)) {
                pillar_is_finalized[i] = true;
            }
            if (pillar_is_finalized[i]) {
                current_finalized_count++;
            }
        }
        finalized_pillars_count = current_finalized_count;
        if (finalized_pillars_count == n) {
            break;
        }

        // The pillar n+1 serves as a temporary buffer.
        // If the buffer is empty, we need to start a new "chain" of moves.
        if (pillars[n + 1].empty()) {
            int start_pillar = -1;
            // Find a pillar that is not yet sorted.
            // Prefer one with a mismatched ball on top to make immediate progress.
            for (int i = 1; i <= n; ++i) {
                if (!pillar_is_finalized[i] && !pillars[i].empty() && pillars[i].back() != i) {
                    start_pillar = i;
                    break;
                }
            }
            // If all non-sorted pillars have correctly colored balls on top,
            // we must move one to uncover the mismatched balls below.
            if (start_pillar == -1) {
                for (int i = 1; i <= n; ++i) {
                    if (!pillar_is_finalized[i] && !pillars[i].empty()) {
                        start_pillar = i;
                        break;
                    }
                }
            }
            // Move the top ball of the chosen pillar to the buffer to start the process.
            if (start_pillar != -1) {
                perform_move(start_pillar, n + 1);
            } else {
                // This case should not be reached if the puzzle is not yet solved.
                break;
            }
        }
        
        // Now the buffer is not empty. Process the ball on top of the buffer.
        int ball_color_in_buffer = pillars[n + 1].back();
        int destination_pillar = ball_color_in_buffer;

        // If the destination pillar is already full, we need to make space.
        // The problem guarantees that a finalized pillar will not need to be disturbed.
        if (pillars[destination_pillar].size() == m) {
            // Move the top ball from the destination to the buffer.
            // This places a new "problem" on top of our buffer stack,
            // which will be resolved in the next iteration.
            perform_move(destination_pillar, n + 1);
        } else {
            // The destination pillar has space, so move the ball from the buffer to it.
            perform_move(n + 1, destination_pillar);
        }
    }

    // Output the total number of moves and the sequence of moves.
    cout << moves.size() << endl;
    for (const auto& move : moves) {
        cout << move.first << " " << move.second << endl;
    }

    return 0;
}