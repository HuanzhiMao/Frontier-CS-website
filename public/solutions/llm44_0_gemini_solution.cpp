#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <limits>

const int MAXN = 200005;

struct Point {
    long long x, y;
    int id;
};

Point points[MAXN];
bool is_prime[MAXN];
bool visited[MAXN];

long double dist_sq(int p1_id, int p2_id) {
    long double dx = points[p1_id].x - points[p2_id].x;
    long double dy = points[p1_id].y - points[p2_id].y;
    return dx * dx + dy * dy;
}

struct Node {
    int point_idx;
    Node *left = nullptr, *right = nullptr;
    long long x1, y1, x2, y2;
};

Node node_pool[MAXN];
int node_pool_ptr = 0;

Node* build(std::vector<int>& p_indices, int l, int r, bool div_x) {
    if (l > r) {
        return nullptr;
    }
    int mid = l + (r - l) / 2;
    std::nth_element(p_indices.begin() + l, p_indices.begin() + mid, p_indices.begin() + r + 1, [&](int a, int b) {
        if (div_x) return points[a].x < points[b].x;
        return points[a].y < points[b].y;
    });

    Node* node = &node_pool[node_pool_ptr++];
    node->point_idx = p_indices[mid];
    
    node->left = build(p_indices, l, mid - 1, !div_x);
    node->right = build(p_indices, mid + 1, r, !div_x);

    node->x1 = node->x2 = points[node->point_idx].x;
    node->y1 = node->y2 = points[node->point_idx].y;

    if (node->left) {
        node->x1 = std::min(node->x1, node->left->x1);
        node->y1 = std::min(node->y1, node->left->y1);
        node->x2 = std::max(node->x2, node->left->x2);
        node->y2 = std::max(node->y2, node->left->y2);
    }
    if (node->right) {
        node->x1 = std::min(node->x1, node->right->x1);
        node->y1 = std::min(node->y1, node->right->y1);
        node->x2 = std::max(node->x2, node->right->x2);
        node->y2 = std::max(node->y2, node->right->y2);
    }
    return node;
}

void find_nearest(Node* curr, int q_id, std::pair<long double, int>& best) {
    if (curr == nullptr) {
        return;
    }

    if (!visited[curr->point_idx]) {
        long double d = dist_sq(q_id, curr->point_idx);
        if (d < best.first) {
            best.first = d;
            best.second = curr->point_idx;
        }
    }
    
    long double dist_sq_bb_left = std::numeric_limits<long double>::max();
    long double dist_sq_bb_right = std::numeric_limits<long double>::max();
    
    if (curr->left) {
        long double dx = 0;
        if (points[q_id].x < curr->left->x1) dx = curr->left->x1 - points[q_id].x;
        if (points[q_id].x > curr->left->x2) dx = points[q_id].x - curr->left->x2;
        long double dy = 0;
        if (points[q_id].y < curr->left->y1) dy = curr->left->y1 - points[q_id].y;
        if (points[q_id].y > curr->left->y2) dy = points[q_id].y - curr->left->y2;
        dist_sq_bb_left = dx * dx + dy * dy;
    }
    if (curr->right) {
        long double dx = 0;
        if (points[q_id].x < curr->right->x1) dx = curr->right->x1 - points[q_id].x;
        if (points[q_id].x > curr->right->x2) dx = points[q_id].x - curr->right->x2;
        long double dy = 0;
        if (points[q_id].y < curr->right->y1) dy = curr->right->y1 - points[q_id].y;
        if (points[q_id].y > curr->right->y2) dy = points[q_id].y - curr->right->y2;
        dist_sq_bb_right = dx * dx + dy * dy;
    }

    if (dist_sq_bb_left < dist_sq_bb_right) {
        if (dist_sq_bb_left < best.first) {
            find_nearest(curr->left, q_id, best);
        }
        if (dist_sq_bb_right < best.first) {
            find_nearest(curr->right, q_id, best);
        }
    } else {
        if (dist_sq_bb_right < best.first) {
            find_nearest(curr->right, q_id, best);
        }
        if (dist_sq_bb_left < best.first) {
            find_nearest(curr->left, q_id, best);
        }
    }
}


void sieve(int n) {
    if (n <= 2) return;
    std::fill(is_prime, is_prime + n, true);
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p * p < n; ++p) {
        if (is_prime[p]) {
            for (int i = p * p; i < n; i += p)
                is_prime[i] = false;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N;
    std::cin >> N;
    for (int i = 0; i < N; ++i) {
        std::cin >> points[i].x >> points[i].y;
        points[i].id = i;
    }

    sieve(N);

    std::vector<int> prime_indices, non_prime_indices;
    for (int i = 1; i < N; ++i) {
        if (is_prime[i]) {
            prime_indices.push_back(i);
        } else {
            non_prime_indices.push_back(i);
        }
    }
    
    Node* prime_root = build(prime_indices, 0, prime_indices.size() - 1, true);
    Node* non_prime_root = build(non_prime_indices, 0, non_prime_indices.size() - 1, true);

    std::vector<int> path;
    path.reserve(N + 1);
    path.push_back(0);
    visited[0] = true;
    int curr_city = 0;
    
    for (int i = 1; i < N; ++i) {
        std::pair<long double, int> best_p = {std::numeric_limits<long double>::max(), -1};
        std::pair<long double, int> best_np = {std::numeric_limits<long double>::max(), -1};

        if (!prime_indices.empty()) {
            find_nearest(prime_root, curr_city, best_p);
        }
        if (!non_prime_indices.empty()) {
            find_nearest(non_prime_root, curr_city, best_np);
        }
        
        int next_city = -1;
        
        if (path.size() % 10 == 9) {
            if (best_p.second != -1 && best_np.second != -1) {
                if (best_p.first < 1.21L * best_np.first) {
                    next_city = best_p.second;
                } else {
                    next_city = best_np.second;
                }
            } else if (best_p.second != -1) {
                next_city = best_p.second;
            } else {
                next_city = best_np.second;
            }
        } else {
            if (best_p.second != -1 && best_np.second != -1) {
                if (best_p.first < best_np.first) {
                    next_city = best_p.second;
                } else {
                    next_city = best_np.second;
                }
            } else if (best_p.second != -1) {
                next_city = best_p.second;
            } else {
                next_city = best_np.second;
            }
        }
        
        path.push_back(next_city);
        visited[next_city] = true;
        curr_city = next_city;
    }

    path.push_back(0);

    std::cout << N + 1 << "\n";
    for (int city_id : path) {
        std::cout << city_id << "\n";
    }

    return 0;
}