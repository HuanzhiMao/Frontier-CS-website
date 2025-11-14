#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <map>
#include <chrono>
#include <random>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

const long long R = 10000;

struct Point {
    long long x, y;
};

struct Strawberry {
    int id;
    int x, y;
};

struct Line {
    double theta, rho;
    long long A, B, C;

    void update_abc() {
        const double scale = 1e6;
        A = round(scale * cos(theta));
        B = round(scale * sin(theta));
        C = round(-scale * rho);
    }
    
    void canonicalize() {
        theta = fmod(theta, 2 * M_PI);
        if (theta < 0) theta += 2 * M_PI;
        if (theta >= M_PI) {
            theta -= M_PI;
            rho = -rho;
        }
    }

    __int128 side(const Strawberry& s) const {
        return (__int128)A * s.x + (__int128)B * s.y + C;
    }
};

int N, K;
vector<int> a;
vector<Strawberry> strawberries;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct TrieNode {
    map<int, TrieNode*> children;
    int count;
    TrieNode() : count(0) {}
    ~TrieNode() {
        for (auto& p : children) {
            delete p.second;
        }
    }
};

void insert_trie(TrieNode* root, const vector<int>& key) {
    TrieNode* curr = root;
    for (int k : key) {
        if (curr->children.find(k) == curr->children.end()) {
            curr->children[k] = new TrieNode();
        }
        curr = curr->children[k];
    }
    curr->count++;
}

void get_counts(TrieNode* node, vector<int>& counts) {
    if (node->count > 0) {
        counts.push_back(node->count);
    }
    for (auto& p : node->children) {
        get_counts(p.second, counts);
    }
}

long long calculate_score(vector<Line>& lines) {
    for (auto& line : lines) {
        line.update_abc();
    }
    
    if (lines.empty()) {
        vector<int> b(11, 0);
        if (N > 0 && N <= 10) {
            b[N]++;
        }
        long long score = 0;
        for (int d = 1; d <= 10; ++d) {
            score += min(a[d], b[d]);
        }
        return score;
    }

    TrieNode* root = new TrieNode();
    for (const auto& s : strawberries) {
        vector<int> key;
        key.reserve(lines.size());
        for (const auto& line : lines) {
            key.push_back(line.side(s) > 0);
        }
        insert_trie(root, key);
    }

    vector<int> piece_sizes;
    get_counts(root, piece_sizes);
    delete root;

    vector<int> b(11, 0);
    for (int size : piece_sizes) {
        if (size > 0 && size <= 10) {
            b[size]++;
        }
    }

    long long score = 0;
    for (int d = 1; d <= 10; ++d) {
        score += min(a[d], b[d]);
    }
    return score;
}

__int128 gcd_ex(__int128 a, __int128 b, __int128 &x, __int128 &y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    __int128 x1, y1;
    __int128 d = gcd_ex(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

pair<Point, Point> get_points_on_line(const Line& line) {
    __int128 A = line.A, B = line.B, C_neg = -line.C;
    
    if (A == 0 && B == 0) return {{0,0}, {1,1}};

    __int128 x0, y0;
    __int128 g = gcd_ex(abs(A), abs(B), x0, y0);
    
    if (C_neg % g != 0) return {{0,1}, {1,0}};
    
    __int128 C_g = C_neg / g;
    x0 *= C_g;
    y0 *= C_g;

    if (A < 0) x0 = -x0;
    if (B < 0) y0 = -y0;

    __int128 B_g = B / g;
    __int128 A_g = A / g;
    
    __int128 k = 0;
    if (B_g != 0) {
        k = -x0 / B_g;
    } else { 
        if (A_g != 0) k = y0 / A_g;
    }
    
    x0 += k * B_g;
    y0 -= k * A_g;

    Point p1 = {(long long)x0, (long long)y0};
    Point p2 = {(long long)(x0 + B_g), (long long)(y0 - A_g)};
    
    return {p1, p2};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start_time = chrono::high_resolution_clock::now();

    cin >> N >> K;
    a.resize(11);
    for (int i = 1; i <= 10; ++i) {
        cin >> a[i];
    }
    strawberries.resize(N);
    for (int i = 0; i < N; ++i) {
        strawberries[i].id = i;
        cin >> strawberries[i].x >> strawberries[i].y;
    }

    uniform_real_distribution<> theta_dist(0, M_PI);
    uniform_real_distribution<> rho_dist(-R, R);
    uniform_int_distribution<> strawberry_idx_dist(0, N - 1);

    vector<Line> current_lines;
    long long current_score = calculate_score(current_lines);

    vector<Line> best_lines = current_lines;
    long long best_score = current_score;

    double start_temp = 20;
    double end_temp = 0.1;
    
    const double TIME_LIMIT = 4.8;
    
    while (true) {
        auto now = chrono::high_resolution_clock::now();
        double elapsed = chrono::duration_cast<chrono::duration<double>>(now - start_time).count();
        if (elapsed > TIME_LIMIT) break;

        double temp = start_temp * pow(end_temp / start_temp, elapsed / TIME_LIMIT);
        vector<Line> new_lines = current_lines;
        int move_type = uniform_int_distribution<>(0, 99)(rng);

        if (new_lines.size() < K && (new_lines.empty() || move_type < 20)) { // ADD
            new_lines.push_back({theta_dist(rng), rho_dist(rng)});
        } else if (!new_lines.empty() && move_type < 40) { // REMOVE
            int idx = uniform_int_distribution<>(0, new_lines.size() - 1)(rng);
            new_lines.erase(new_lines.begin() + idx);
        } else if (!new_lines.empty()) { // MODIFY
            int idx = uniform_int_distribution<>(0, new_lines.size() - 1)(rng);
            int modify_type = uniform_int_distribution<>(0,1)(rng);

            if (modify_type == 0 || N < 2) { // Perturb
                normal_distribution<> angle_norm(0, 0.1 * (temp/start_temp) + 1e-4);
                normal_distribution<> rho_norm(0, 500 * (temp/start_temp) + 1e-2);
                new_lines[idx].theta += angle_norm(rng);
                new_lines[idx].rho += rho_norm(rng);
                new_lines[idx].canonicalize();
                new_lines[idx].rho = max(-(double)R, min((double)R, new_lines[idx].rho));
            } else { // Bisector
                int s1_idx = strawberry_idx_dist(rng);
                int s2_idx = strawberry_idx_dist(rng);
                while(s1_idx == s2_idx) s2_idx = strawberry_idx_dist(rng);

                Strawberry s1 = strawberries[s1_idx];
                Strawberry s2 = strawberries[s2_idx];

                double mid_x = (s1.x + s2.x) / 2.0;
                double mid_y = (s1.y + s2.y) / 2.0;
                double dx = s2.x - s1.x;
                double dy = s2.y - s1.y;

                new_lines[idx].theta = atan2(dx, -dy);
                new_lines[idx].rho = mid_x * cos(new_lines[idx].theta) + mid_y * sin(new_lines[idx].theta);
                new_lines[idx].canonicalize();
            }
        }
        
        long long new_score = calculate_score(new_lines);
        double prob = exp((double)(new_score - current_score) / temp);

        if (new_score > current_score || uniform_real_distribution<>(0.0, 1.0)(rng) < prob) {
            current_lines = new_lines;
            current_score = new_score;
        }

        if (current_score > best_score) {
            best_score = current_score;
            best_lines = current_lines;
        }
    }
    
    bool on_line;
    do {
        on_line = false;
        for (auto& line : best_lines) {
            line.update_abc();
            for (const auto& s : strawberries) {
                if (line.side(s) == 0) {
                    on_line = true;
                    line.rho += 1e-9;
                }
            }
        }
    } while(on_line);


    cout << best_lines.size() << endl;
    for (const auto& line : best_lines) {
        pair<Point, Point> pts = get_points_on_line(line);
        cout << pts.first.x << " " << pts.first.y << " " << pts.second.x << " " << pts.second.y << endl;
    }

    return 0;
}