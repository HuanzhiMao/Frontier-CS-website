#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <queue>
#include <algorithm>
#include <map>

using namespace std;

const int N = 20;
const int M = 40;

struct Point {
    int r, c;
};

bool operator==(const Point& a, const Point& b) {
    return a.r == b.r && a.c == b.c;
}
bool operator!=(const Point& a, const Point& b) {
    return !(a == b);
}
bool operator<(const Point& a, const Point& b) {
    if (a.r != b.r) return a.r < b.r;
    return a.c < b.c;
}

struct State {
    Point pos;
    vector<string> grid;
    vector<pair<char, char>> actions;
    vector<Point> targets;
    int current_target_idx;
    map<Point, int> target_map;

    State(Point start_pos, const vector<Point>& initial_targets) {
        pos = start_pos;
        grid.assign(N, string(N, '.'));
        targets = initial_targets;
        current_target_idx = 0;
        for(size_t i=0; i<initial_targets.size(); ++i) {
            target_map[initial_targets[i]] = i;
        }
    }

    bool is_valid(int r, int c) {
        return r >= 0 && r < N && c >= 0 && c < N;
    }

    bool is_block(int r, int c) {
        if (!is_valid(r, c)) return true;
        return grid[r][c] == '#';
    }

    Point get_neighbor(Point p, char dir) {
        if (dir == 'U') return {p.r - 1, p.c};
        if (dir == 'D') return {p.r + 1, p.c};
        if (dir == 'L') return {p.r, p.c - 1};
        if (dir == 'R') return {p.r, p.c + 1};
        return p;
    }

    void apply_action(char action, char dir) {
        actions.push_back({action, dir});
        Point next_pos = get_neighbor(pos, dir);

        if (action == 'A') {
            if (is_valid(next_pos.r, next_pos.c)) {
                if (grid[next_pos.r][next_pos.c] == '#') {
                    grid[next_pos.r][next_pos.c] = '.';
                } else {
                    grid[next_pos.r][next_pos.c] = '#';
                }
            }
        } else if (action == 'M') {
            pos = next_pos;
        } else if (action == 'S') {
            Point current = pos;
            while (true) {
                Point p = get_neighbor(current, dir);
                if (is_block(p.r, p.c)) {
                    pos = current;
                    break;
                }
                current = p;
            }
        }
    }
    
    void move_to(Point dest) {
        if (pos == dest) return;

        queue<Point> q;
        q.push(pos);
        map<Point, Point> parent;
        parent[pos] = {-1, -1};

        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        char dirs[] = {'U', 'D', 'L', 'R'};

        Point found_dest = {-1,-1};

        queue<Point> bfs_q;
        bfs_q.push(pos);
        map<Point, Point> path_parent;
        path_parent[pos] = {-1,-1};

        while(!bfs_q.empty()){
            Point curr = bfs_q.front();
            bfs_q.pop();

            if(curr == dest){
                found_dest = curr;
                break;
            }
            
            for(int i=0; i<4; ++i){
                Point next = {curr.r + dr[i], curr.c + dc[i]};
                if(is_valid(next.r, next.c) && !is_block(next.r, next.c) && path_parent.find(next) == path_parent.end()){
                    path_parent[next] = curr;
                    bfs_q.push(next);
                }
            }
        }
        
        vector<char> path_dirs;
        Point p = found_dest;
        while(p.r != -1 && path_parent.count(p) && path_parent[p].r != -1) {
            Point prev = path_parent[p];
            if (p.r == prev.r + 1) path_dirs.push_back('D');
            else if (p.r == prev.r - 1) path_dirs.push_back('U');
            else if (p.c == prev.c + 1) path_dirs.push_back('R');
            else path_dirs.push_back('L');
            p = prev;
        }
        reverse(path_dirs.begin(), path_dirs.end());

        for (char dir : path_dirs) {
            apply_action('M', dir);
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n_dummy, m_dummy;
    cin >> n_dummy >> m_dummy;

    Point start_pos;
    cin >> start_pos.r >> start_pos.c;

    vector<Point> targets(M - 1);
    vector<Point> all_points;
    all_points.push_back(start_pos);
    map<Point, int> all_points_map;
    all_points_map[start_pos] = 0;

    for (int i = 0; i < M - 1; ++i) {
        cin >> targets[i].r >> targets[i].c;
        all_points.push_back(targets[i]);
        all_points_map[targets[i]] = i+1;
    }

    State state(start_pos, targets);
    
    vector<int> rs, cs;
    for(const auto& p : all_points) {
        rs.push_back(p.r);
        cs.push_back(p.c);
    }
    sort(rs.begin(), rs.end());
    sort(cs.begin(), cs.end());
    int hr = rs[rs.size()/2];
    int hc = cs[cs.size()/2];

    auto is_any_point = [&](int r, int c) {
        return all_points_map.count({r, c});
    };
    
    Point pre_setup_pos = state.pos;
    state.move_to({hr, 0});
    for (int j = 0; j < N; ++j) {
        state.move_to({hr, j});
        if (state.is_valid(hr - 1, j) && !is_any_point(hr-1, j)) state.apply_action('A', 'U');
        if (state.is_valid(hr + 1, j) && !is_any_point(hr+1, j)) state.apply_action('A', 'D');
    }

    state.move_to({0, hc});
    for (int i = 0; i < N; ++i) {
        state.move_to({i, hc});
        if (state.is_valid(i, hc - 1) && !is_any_point(i, hc-1)) state.apply_action('A', 'L');
        if (state.is_valid(i, hc + 1) && !is_any_point(i, hc+1)) state.apply_action('A', 'R');
    }
    
    for (int i = 0; i < M - 1; ++i) {
        Point source = state.pos;
        Point dest = targets[i];

        map<Point, int> dist_s;
        queue<Point> q_s;
        q_s.push(source);
        dist_s[source] = 0;
        while(!q_s.empty()){
            Point curr = q_s.front(); q_s.pop();
            int dr[]={-1,1,0,0}; int dc[]={0,0,-1,1};
            for(int k=0; k<4; ++k){
                Point next = {curr.r + dr[k], curr.c + dc[k]};
                if(state.is_valid(next.r, next.c) && !state.is_block(next.r, next.c) && dist_s.find(next) == dist_s.end()){
                    dist_s[next] = dist_s[curr] + 1;
                    q_s.push(next);
                }
            }
        }
        
        int cost_move = dist_s.count(dest) ? dist_s[dest] : 1e9;

        Point h_entry = {hr, source.c}, h_exit = {hr, dest.c};
        int cost_h = 1e9;
        if(dist_s.count(h_entry)) {
            cost_h = dist_s[h_entry] + 1 + abs(h_exit.r - dest.r) + abs(h_exit.c - dest.c);
        }

        Point v_entry = {source.r, hc}, v_exit = {dest.r, hc};
        int cost_v = 1e9;
        if(dist_s.count(v_entry)) {
            cost_v = dist_s[v_entry] + 1 + abs(v_exit.r - dest.r) + abs(v_exit.c - dest.c);
        }

        if (cost_move <= cost_h && cost_move <= cost_v) {
            state.move_to(dest);
        } else if (cost_h <= cost_v) {
            state.move_to(h_entry);
            if(h_entry.c < h_exit.c) state.apply_action('S', 'R');
            else if(h_entry.c > h_exit.c) state.apply_action('S', 'L');
            state.move_to(dest);
        } else {
            state.move_to(v_entry);
            if(v_entry.r < v_exit.r) state.apply_action('S', 'D');
            else if(v_entry.r > v_exit.r) state.apply_action('S', 'U');
            state.move_to(dest);
        }
    }
    
    for (const auto& action : state.actions) {
        cout << action.first << " " << action.second << "\n";
    }

    return 0;
}