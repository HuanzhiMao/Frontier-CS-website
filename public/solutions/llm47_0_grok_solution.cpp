#include <bits/stdc++.h>
using namespace std;

struct Placement {
    string type;
    int x, y, rot;
};

struct Item {
    string type;
    int w, h, v, limit;
};

int main() {
    string input;
    string ln;
    while (getline(cin, ln)) {
        input += ln + "\n";
    }

    smatch m;
    int W, H;
    bool allow_rotate;
    if (regex_search(input, m, regex(R"("W":\s*(\d+))"))) W = stoi(m[1]);
    if (regex_search(input, m, regex(R"("H":\s*(\d+))"))) H = stoi(m[1]);
    if (regex_search(input, m, regex(R"("allow_rotate":\s*(true|false))"))) allow_rotate = (m[1] == "true");

    size_t start = input.find("\"items\":[");
    if (start == string::npos) return 1;
    start += 9;
    size_t end = input.rfind("]");
    if (end == string::npos) return 1;
    string items_str = input.substr(start, end - start);

    vector<Item> items;
    size_t pos = 0;
    while (true) {
        size_t start_obj = items_str.find('{', pos);
        if (start_obj == string::npos) break;
        size_t end_obj = items_str.find('}', start_obj);
        if (end_obj == string::npos) break;
        string obj = items_str.substr(start_obj, end_obj - start_obj + 1);
        Item it;
        if (regex_search(obj, m, regex(R"("type":\s*"([^"]*)")"))) it.type = m[1];
        if (regex_search(obj, m, regex(R"("w":\s*(\d+))"))) it.w = stoi(m[1]);
        if (regex_search(obj, m, regex(R"("h":\s*(\d+))"))) it.h = stoi(m[1]);
        if (regex_search(obj, m, regex(R"("v":\s*(\d+))"))) it.v = stoi(m[1]);
        if (regex_search(obj, m, regex(R"("limit":\s*(\d+))"))) it.limit = stoi(m[1]);
        items.push_back(it);
        pos = end_obj + 1;
    }

    int M = items.size();
    vector< function<bool(int, int)> > comparators;
    comparators.push_back([&](int a, int b) {
        double da = (double)items[a].v / (items[a].w * items[a].h);
        double db = (double)items[b].v / (items[b].w * items[b].h);
        return da > db;
    });
    comparators.push_back([&](int a, int b) {
        int aa = items[a].w * items[a].h;
        int ab = items[b].w * items[b].h;
        return aa > ab;
    });
    comparators.push_back([&](int a, int b) {
        return items[a].h > items[b].h;
    });
    comparators.push_back([&](int a, int b) {
        return items[a].w > items[b].w;
    });
    comparators.push_back([&](int a, int b) {
        return items[a].v > items[b].v;
    });

    long long best_profit = 0;
    vector<Placement> best_placements;
    vector<int> height(W, 0);
    for (auto& cmp : comparators) {
        vector<int> ord(M);
        iota(ord.begin(), ord.end(), 0);
        sort(ord.begin(), ord.end(), cmp);
        vector<int> rem(M);
        for (int i = 0; i < M; i++) rem[i] = items[i].limit;
        vector<Placement> curr_place;
        long long curr_profit = 0;
        fill(height.begin(), height.end(), 0);
        for (int idx : ord) {
            while (rem[idx] > 0) {
                int best_py = INT_MAX / 2;
                int best_px = INT_MAX / 2;
                int best_rot = -1;
                for (int rot = 0; rot < (allow_rotate ? 2 : 1); rot++) {
                    int ww = rot ? items[idx].h : items[idx].w;
                    int hh = rot ? items[idx].w : items[idx].h;
                    if (ww > W) continue;
                    int num_windows = W - ww + 1;
                    if (num_windows <= 0) continue;
                    vector<int> window_max(num_windows);
                    deque<int> dq;
                    for (int i = 0; i < W; i++) {
                        while (!dq.empty() && height[dq.back()] <= height[i]) dq.pop_back();
                        dq.push_back(i);
                        if (!dq.empty() && dq.front() == i - ww) dq.pop_front();
                        if (i >= ww - 1) {
                            int pxx = i - (ww - 1);
                            window_max[pxx] = height[dq.front()];
                        }
                    }
                    int min_m = INT_MAX / 2;
                    for (int mm : window_max) min_m = min(min_m, mm);
                    if (min_m + hh > H) continue;
                    int this_py = min_m;
                    int this_px = INT_MAX / 2;
                    for (int pxx = 0; pxx < num_windows; pxx++) {
                        if (window_max[pxx] == min_m) {
                            this_px = pxx;
                            break;
                        }
                    }
                    bool better = (this_py < best_py) || (this_py == best_py && this_px < best_px);
                    if (better) {
                        best_py = this_py;
                        best_px = this_px;
                        best_rot = rot;
                    }
                }
                if (best_rot == -1) break;
                int ww = (best_rot ? items[idx].h : items[idx].w);
                int hh = (best_rot ? items[idx].w : items[idx].h);
                for (int i = best_px; i < best_px + ww; i++) {
                    height[i] = best_py + hh;
                }
                curr_place.push_back({items[idx].type, best_px, best_py, best_rot});
                curr_profit += items[idx].v;
                rem[idx]--;
            }
        }
        if (curr_profit > best_profit) {
            best_profit = curr_profit;
            best_placements = move(curr_place);
        }
    }

    cout << "{\"placements\":[";
    bool first = true;
    for (auto& p : best_placements) {
        if (!first) cout << ",";
        cout << "{\"type\":\"" << p.type << "\",\"x\":" << p.x << ",\"y\":" << p.y << ",\"rot\":" << p.rot << "}";
        first = false;
    }
    cout << "]}";
    return 0;
}