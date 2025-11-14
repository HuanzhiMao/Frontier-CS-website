#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <set>
#include <queue>
#include <map>

// Function to ask a query
int query(int u, int v, int w) {
    std::cout << "0 " << u << " " << v << " " << w << std::endl;
    int result;
    std::cin >> result;
    if (result == -1) exit(0);
    return result;
}

// Function to remove an edge from adjacency list
void remove_edge(std::vector<std::vector<int>>& adj, int u, int v) {
    adj[u].erase(std::remove(adj[u].begin(), adj[u].end(), v), adj[u].end());
    adj[v].erase(std::remove(adj[v].begin(), adj[v].end(), u), adj[v].end());
}

// Function to add an edge to adjacency list
void add_edge(std::vector<std::vector<int>>& adj, int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}

// Function to find path between two nodes using BFS
std::vector<int> get_path(int start, int end, int n, const std::vector<std::vector<int>>& adj) {
    std::queue<int> q;
    q.push(start);
    std::map<int, int> parent;
    std::vector<bool> visited(n + 1, false);
    visited[start] = true;
    
    int current_node;
    bool found = false;
    while (!q.empty()) {
        current_node = q.front();
        q.pop();

        if (current_node == end) {
            found = true;
            break;
        }

        for (int neighbor : adj[current_node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current_node;
                q.push(neighbor);
            }
        }
    }
    
    if (!found) return {};

    std::vector<int> path;
    int at = end;
    while (true) {
        path.push_back(at);
        if (at == start) break;
        at = parent[at];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    if (n == 3) {
        int center = query(1, 2, 3);
        std::cout << "1 ";
        for (int i = 1; i <= 3; ++i) {
            if (i != center) {
                std::cout << center << " " << i << " ";
            }
        }
        std::cout << std::endl;
        return 0;
    }

    std::vector<std::vector<int>> adj(n + 1);
    add_edge(adj, 1, 2);

    std::set<int> leaves;
    leaves.insert(1);
    leaves.insert(2);

    for (int i = 3; i <= n; ++i) {
        int u = *leaves.begin();
        int v = *std::next(leaves.begin());
        
        int center = query(u, v, i);

        if (center == i) { // i is on the path between u and v
            std::vector<int> path = get_path(u, v, n, adj);
            
            int low = 0, high = path.size() - 1, ans_idx = 0;

            while(low <= high){
                int mid_idx = low + (high - low) / 2;
                if (mid_idx == 0) {
                    low = mid_idx + 1;
                    continue;
                }
                if (query(u, path[mid_idx], i) == i){
                    ans_idx = mid_idx;
                    low = mid_idx + 1;
                } else {
                    high = mid_idx - 1;
                }
            }
            int p1 = path[ans_idx];
            int p2 = path[ans_idx + 1];

            remove_edge(adj, p1, p2);
            add_edge(adj, p1, i);
            add_edge(adj, i, p2);
        } else { // i attaches to center
            add_edge(adj, center, i);
            leaves.insert(i);
            if (adj[center].size() > 1 && leaves.count(center)) {
                leaves.erase(center);
            }
        }
    }

    std::cout << "1 ";
    for (int i = 1; i <= n; ++i) {
        for (int neighbor : adj[i]) {
            if (i < neighbor) {
                std::cout << i << " " << neighbor << " ";
            }
        }
    }
    std::cout << std::endl;

    return 0;
}