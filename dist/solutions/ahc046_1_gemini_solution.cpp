#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <cmath>
#include <map>
#include <algorithm>

using namespace std;

const int N = 20;
const int M = 40;

struct Pos {
    int r, c;
    bool operator<(const Pos& other) const {
        if (r != other.r) return r < other.r;
        return c < other.c;
    }
    bool operator==(const Pos& other) const {
        return r == other.r && c == other.c;
    }
};

int manhattan(Pos p1, Pos p2) {
    return abs(p1.r - p2.r) + abs(p1.c - p2.c);
}

vector<Pos> targets;
bool has_block[N][N];
Pos current_pos;
vector<pair<char, char>> actions;

bool is_valid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

void do_action(char action_char, char dir_char) {
    actions.push_back({action_char, dir_char});
    int dr = 0, dc = 0;
    if (dir_char == 'U') dr = -1;
    else if (dir_char == 'D') dr = 1;
    else if (dir_char == 'L') dc = -1;
    else if (dir_char == 'R') dc = 1;

    if (action_char == 'M') {
        current_pos.r += dr;
        current_pos.c += dc;
    } else if (action_char == 'S') {
        while (true) {
            int nr = current_pos.r + dr;
            int nc = current_pos.c + dc;
            if (!is_valid(nr, nc) || has_block[nr][nc]) {
                break;
            }
            current_pos.r = nr;
            current_pos.c = nc;
        }
    } else if (action_char == 'A') {
        int ar = current_pos.r + dr;
        int ac = current_pos.c + dc;
        if (is_valid(ar, ac)) {
            has_block[ar][ac] = !has_block[ar][ac];
        }
    }
}


void pure_move(Pos dest) {
    int dr_dir = (dest.r > current_pos.r) ? 1 : -1;
    while (current_pos.r != dest.r) {
        do_action('M', (dr_dir == 1 ? 'D' : 'U'));
    }
    int dc_dir = (dest.c > current_pos.c) ? 1 : -1;
    while (current_pos.c != dest.c) {
        do_action('M', (dc_dir == 1 ? 'R' : 'L'));
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n_dummy, m_dummy;
    cin >> n_dummy >> m_dummy;

    targets.resize(M);
    for (int i = 0; i < M; ++i) {
        cin >> targets[i].r >> targets[i].c;
    }

    current_pos = targets[0];
    
    for (int k = 0; k < M - 1; ++k) {
        Pos A = current_pos;
        Pos B = targets[k + 1];

        if (has_block[B.r][B.c]) {
            Pos best_adj = {-1, -1};
            int min_dist = 1e9;
            
            int dr[] = {-1, 1, 0, 0};
            int dc[] = {0, 0, -1, 1};

            for (int i = 0; i < 4; ++i) {
                Pos adj = {B.r - dr[i], B.c - dc[i]};
                if (is_valid(adj.r, adj.c) && !has_block[adj.r][adj.c]) {
                    int d = manhattan(current_pos, adj);
                    if (d < min_dist) {
                        min_dist = d;
                        best_adj = adj;
                    }
                }
            }
            pure_move(best_adj);

            char remove_dir = ' ';
            if(current_pos.r < B.r) remove_dir = 'D';
            else if(current_pos.r > B.r) remove_dir = 'U';
            else if(current_pos.c < B.c) remove_dir = 'R';
            else remove_dir = 'L';
            do_action('A', remove_dir);
            A = current_pos;
        }

        double max_net_gain = 0.0;
        Pos best_alter_from_pos = {-1, -1};
        Pos best_block_pos = {-1, -1};

        if (k + 1 < M - 1) {
            Pos C = targets[k + 2];
            
            int dr_bc = abs(B.r - C.r);
            if (dr_bc > 2) {
                int benefit = dr_bc - 1;
                Pos block_pos = {C.r > B.r ? C.r + 1 : C.r - 1, C.c};
                if(is_valid(block_pos.r, block_pos.c) && !has_block[block_pos.r][block_pos.c]){
                    int dr_adj[] = {-1, 1, 0, 0}; int dc_adj[] = {0, 0, -1, 1};
                    for(int i = 0; i < 4; ++i) {
                        Pos alter_from = {block_pos.r + dr_adj[i], block_pos.c + dc_adj[i]};
                        if(!is_valid(alter_from.r, alter_from.c)) continue;
                        
                        int extra_cost = (manhattan(A, alter_from) + 1 + manhattan(alter_from, B)) - manhattan(A, B);
                        double net_gain = (double)benefit - extra_cost;
                        if(net_gain > max_net_gain){
                            max_net_gain = net_gain;
                            best_alter_from_pos = alter_from;
                            best_block_pos = block_pos;
                        }
                    }
                }
            }

            int dc_bc = abs(B.c - C.c);
            if (dc_bc > 2) {
                int benefit = dc_bc - 1;
                Pos block_pos = {C.r, C.c > B.c ? C.c + 1 : C.c - 1};
                if(is_valid(block_pos.r, block_pos.c) && !has_block[block_pos.r][block_pos.c]){
                    int dr_adj[] = {-1, 1, 0, 0}; int dc_adj[] = {0, 0, -1, 1};
                    for(int i=0; i<4; ++i){
                        Pos alter_from = {block_pos.r + dr_adj[i], block_pos.c + dc_adj[i]};
                        if(!is_valid(alter_from.r, alter_from.c)) continue;

                        int extra_cost = (manhattan(A, alter_from) + 1 + manhattan(alter_from, B)) - manhattan(A, B);
                        double net_gain = (double)benefit - extra_cost;
                        if(net_gain > max_net_gain){
                            max_net_gain = net_gain;
                            best_alter_from_pos = alter_from;
                            best_block_pos = block_pos;
                        }
                    }
                }
            }
        }
        
        if (max_net_gain > 0) {
            pure_move(best_alter_from_pos);
            char alter_dir = ' ';
            if(best_alter_from_pos.r < best_block_pos.r) alter_dir = 'D';
            else if(best_alter_from_pos.r > best_block_pos.r) alter_dir = 'U';
            else if(best_alter_from_pos.c < best_block_pos.c) alter_dir = 'R';
            else alter_dir = 'L';
            do_action('A', alter_dir);
            pure_move(B);
        } else {
            Pos stop_v = {-1, -1};
            if (A.r != B.r) {
                int r = A.r;
                int dr = (B.r > A.r ? 1 : -1);
                while(true) {
                    int nr = r + dr;
                    if(!is_valid(nr, A.c) || has_block[nr][A.c]) { stop_v = {r, A.c}; break; }
                    r = nr;
                }
            }
            Pos stop_h = {-1, -1};
            if (A.c != B.c) {
                int c = A.c;
                int dc = (B.c > A.c ? 1 : -1);
                while(true) {
                    int nc = c + dc;
                    if(!is_valid(A.r, nc) || has_block[A.r][nc]) { stop_h = {A.r, c}; break; }
                    c = nc;
                }
            }

            int cost_move = manhattan(A, B);
            int cost_slide_vh = 1e9, cost_slide_hv = 1e9;
            
            if (A.r != B.r && stop_v.r == B.r && stop_v.c == A.c) {
                 cost_slide_hv = 1 + manhattan({B.r, A.c}, B);
            }
            if (A.c != B.c && stop_h.c == B.c && stop_h.r == A.r) {
                 cost_slide_vh = 1 + manhattan({A.r, B.c}, B);
            }

            if (cost_move <= cost_slide_vh && cost_move <= cost_slide_hv) {
                pure_move(B);
            } else if (cost_slide_hv <= cost_slide_vh) {
                do_action('S', B.r > A.r ? 'D' : 'U');
                pure_move(B);
            } else {
                do_action('S', B.c > A.c ? 'R' : 'L');
                pure_move(B);
            }
        }
    }

    for (const auto& p : actions) {
        cout << p.first << " " << p.second << "\n";
    }

    return 0;
}