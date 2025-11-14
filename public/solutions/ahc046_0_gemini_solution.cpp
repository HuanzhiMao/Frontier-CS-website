#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

const int N = 20;
const int M = 40;

struct Point {
    int r, c;
};

Point current_pos;

// Outputs a single move command and updates the current position.
void move_one(int dr, int dc) {
    if (dr == -1) cout << "M U" << endl;
    else if (dr == 1) cout << "M D" << endl;
    else if (dc == -1) cout << "M L" << endl;
    else if (dc == 1) cout << "M R" << endl;
    current_pos.r += dr;
    current_pos.c += dc;
}

// Outputs a single slide command and updates the current position.
void slide_one(int dr, int dc) {
    if (dr == -1) {
        cout << "S U" << endl;
        current_pos.r = 0;
    } else if (dr == 1) {
        cout << "S D" << endl;
        current_pos.r = N - 1;
    } else if (dc == -1) {
        cout << "S L" << endl;
        current_pos.c = 0;
    } else if (dc == 1) {
        cout << "S R" << endl;
        current_pos.c = N - 1;
    }
}

// Moves from a start point to an end point using only 'Move' actions,
// one axis at a time.
void move_to(int end_r, int end_c) {
    int start_r = current_pos.r;
    int start_c = current_pos.c;
    
    int dr = (end_r > start_r) ? 1 : -1;
    int dc = (end_c > start_c) ? 1 : -1;

    for (int i = 0; i < abs(end_r - start_r); ++i) {
        move_one(dr, 0);
    }
    for (int i = 0; i < abs(end_c - start_c); ++i) {
        move_one(0, dc);
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n_in, m_in;
    cin >> n_in >> m_in;
    
    Point initial_pos;
    cin >> initial_pos.r >> initial_pos.c;

    vector<Point> targets(M - 1);
    for (int i = 0; i < M - 1; ++i) {
        cin >> targets[i].r >> targets[i].c;
    }
    
    current_pos = initial_pos;

    for (const auto& target : targets) {
        // This strategy moves between targets by first aligning the column, then the row.
        // For each alignment (a 1D move), it chooses the cheapest option among:
        // 1. A direct sequence of 'Move' actions.
        // 2. A 'Slide' to one edge of the board, followed by 'Move' actions.
        // This is a greedy approach that locally optimizes the path for each target.
        // Since the board starts empty and this strategy doesn't use 'Alter',
        // the pathfinding is simplified to Manhattan distance calculations.

        // Plan horizontal move from current_pos.c to target.c
        int h_cost_direct = abs(current_pos.c - target.c);
        int h_cost_bounce_l = 1 + target.c;
        int h_cost_bounce_r = 1 + (N - 1 - target.c);

        int min_h_cost = h_cost_direct;
        int h_plan = 0; // 0: direct, 1: bounce left, 2: bounce right

        if (h_cost_bounce_l < min_h_cost) {
            min_h_cost = h_cost_bounce_l;
            h_plan = 1;
        }
        if (h_cost_bounce_r < min_h_cost) {
            min_h_cost = h_cost_bounce_r;
            h_plan = 2;
        }

        // Plan vertical move from current_pos.r to target.r
        int v_cost_direct = abs(current_pos.r - target.r);
        int v_cost_bounce_u = 1 + target.r;
        int v_cost_bounce_d = 1 + (N - 1 - target.r);

        int min_v_cost = v_cost_direct;
        int v_plan = 0; // 0: direct, 1: bounce up, 2: bounce down

        if (v_cost_bounce_u < min_v_cost) {
            min_v_cost = v_cost_bounce_u;
            v_plan = 1;
        }
        if (v_cost_bounce_d < min_v_cost) {
            min_v_cost = v_cost_bounce_d;
            v_plan = 2;
        }

        // Execute horizontal move to align column
        if (h_plan == 0) {
            move_to(current_pos.r, target.c);
        } else if (h_plan == 1) {
            slide_one(0, -1);
            move_to(current_pos.r, target.c);
        } else { // h_plan == 2
            slide_one(0, 1);
            move_to(current_pos.r, target.c);
        }

        // Execute vertical move to align row
        if (v_plan == 0) {
            move_to(target.r, current_pos.c);
        } else if (v_plan == 1) {
            slide_one(-1, 0);
            move_to(target.r, current_pos.c);
        } else { // v_plan == 2
            slide_one(1, 0);
            move_to(target.r, current_pos.c);
        }
    }

    return 0;
}