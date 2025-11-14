#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
#include <set>

int n;
std::map<std::vector<int>, int> memo;

int ask_query(std::vector<int> s) {
    if (s.empty()) {
        return 0;
    }
    std::sort(s.begin(), s.end());
    s.erase(std::unique(s.begin(), s.end()), s.end());
    if (s.empty()) return 0;

    if (memo.count(s)) {
        return memo[s];
    }

    std::cout << "? " << s.size() << std::endl;
    for (size_t i = 0; i < s.size(); ++i) {
        std::cout << s[i] << (i == s.size() - 1 ? "" : " ");
    }
    std::cout << std::endl;
    int m;
    std::cin >> m;
    if (m == -1) exit(0);
    return memo[s] = m;
}

int edges_between(const std::vector<int>& s1, const std::vector<int>& s2) {
    if (s1.empty() || s2.empty()) {
        return 0;
    }
    int m1 = ask_query(s1);
    int m2 = ask_query(s2);
    std::vector<int> s_union = s1;
    s_union.insert(s_union.end(), s2.begin(), s2.end());
    int m_union = ask_query(s_union);
    return m_union - m1 - m2;
}

std::vector<int> adj[601];
int color[601];

void bfs_color(int start_node, int start_color) {
    std::vector<int> q;
    q.push_back(start_node);
    color[start_node] = start_color;
    int head = 0;
    while(head < q.size()){
        int u = q[head++];
        for (int v : adj[u]) {
            if (color[v] == 0) {
                color[v] = 3 - color[u];
                q.push_back(v);
            }
        }
    }
}

std::pair<int, int> find_edge_in(std::vector<int> s);
int find_endpoint(const std::vector<int>& s1, const std::vector<int>& s2);
int find_neighbor(int u, const std::vector<int>& s2);

std::pair<int, int> find_edge_in(std::vector<int> s) {
    if (s.size() < 2) return {-1, -1};
    if (ask_query(s) == 0) return {-1, -1};

    if (s.size() == 2) return {s[0], s[1]};
    
    int mid = s.size() / 2;
    std::vector<int> s1(s.begin(), s.begin() + mid);
    std::vector<int> s2(s.begin() + mid, s.end());

    if (ask_query(s1) > 0) return find_edge_in(s1);
    if (ask_query(s2) > 0) return find_edge_in(s2);
    
    int u = find_endpoint(s1, s2);
    int v = find_neighbor(u, s2);
    return {u, v};
}

int find_endpoint(const std::vector<int>& s1, const std::vector<int>& s2) {
    int l = 0, r = s1.size() - 1;
    while (l < r) {
        int m = l + (r - l) / 2;
        std::vector<int> sub_s1(s1.begin() + l, s1.begin() + m + 1);
        if (edges_between(sub_s1, s2) > 0) {
            r = m;
        } else {
            l = m + 1;
        }
    }
    return s1[l];
}

int find_neighbor(int u, const std::vector<int>& s2) {
    int l = 0, r = s2.size() - 1;
    while (l < r) {
        int m = l + (r - l) / 2;
        std::vector<int> sub_s2(s2.begin() + l, s2.begin() + m + 1);
        if (edges_between({u}, sub_s2) > 0) {
            r = m;
        } else {
            l = m + 1;
        }
    }
    return s2[l];
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> n;

    if (n == 1) {
        std::cout << "Y 1\n1" << std::endl;
        return 0;
    }

    std::set<int> unvisited_set;
    for(int i = 2; i <= n; ++i) unvisited_set.insert(i);
    std::vector<int> visited_nodes;
    visited_nodes.push_back(1);

    for (int i = 0; i < n - 1; ++i) {
        std::vector<int> unvisited_vec(unvisited_set.begin(), unvisited_set.end());
        if (edges_between(visited_nodes, unvisited_vec) == 0) {
            // This case is for disconnected graphs, problem says graph is connected.
            // But as a robust measure, start a new component.
            int next_node = *unvisited_set.begin();
            visited_nodes.push_back(next_node);
            unvisited_set.erase(unvisited_set.begin());
            continue;
        }
        int v = find_endpoint(unvisited_vec, visited_nodes);
        int u = find_neighbor(v, visited_nodes);
        adj[u].push_back(v);
        adj[v].push_back(u);
        visited_nodes.push_back(v);
        unvisited_set.erase(v);
    }

    for(int i=1; i<=n; ++i){
        if(color[i] == 0){
            bfs_color(i, 1);
        }
    }

    std::vector<int> c1, c2;
    for (int i = 1; i <= n; ++i) {
        if (color[i] == 1) c1.push_back(i);
        else c2.push_back(i);
    }

    if (ask_query(c1) == 0 && ask_query(c2) == 0) {
        std::cout << "Y " << c1.size() << std::endl;
        for (size_t i = 0; i < c1.size(); ++i) {
            std::cout << c1[i] << (i == c1.size() - 1 ? "" : " ");
        }
        std::cout << std::endl;
    } else {
        std::vector<int> bad_set = ask_query(c1) > 0 ? c1 : c2;
        std::pair<int, int> edge = find_edge_in(bad_set);
        int u = edge.first;
        int v = edge.second;

        std::vector<int> q;
        q.push_back(u);
        std::vector<int> parent(n + 1, 0);
        std::vector<bool> visited(n + 1, false);
        visited[u] = true;
        int head = 0;
        while(head < q.size()) {
            int curr = q[head++];
            if (curr == v) break;
            for (int neighbor : adj[curr]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    parent[neighbor] = curr;
                    q.push_back(neighbor);
                }
            }
        }
        
        std::vector<int> cycle;
        int curr = v;
        while (curr != 0) {
            cycle.push_back(curr);
            if (curr == u) break;
            curr = parent[curr];
        }
        std::reverse(cycle.begin(), cycle.end());

        std::cout << "N " << cycle.size() << std::endl;
        for (size_t i = 0; i < cycle.size(); ++i) {
            std::cout << cycle[i] << (i == cycle.size() - 1 ? "" : " ");
        }
        std::cout << std::endl;
    }

    return 0;
}