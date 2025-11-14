#include <bits/stdc++.h>
using namespace std;

struct Company {
  int x, y, r, a, b, c, d;
};

void do_assignment(vector<Company>& comp) {
  int n = comp.size();
  vector<int> order(n);
  iota(order.begin(), order.end(), 0);
  sort(order.begin(), order.end(), [&](int i, int j) {
    if (comp[i].x != comp[j].x) return comp[i].x < comp[j].x;
    return comp[i].y < comp[j].y;
  });
  int current = 0;
  int jj = 0;
  while (jj < n) {
    int curr_x = comp[order[jj]].x;
    vector<int> group;
    while (jj < n && comp[order[jj]].x == curr_x) {
      group.push_back(order[jj]);
      jj++;
    }
    int k = group.size();
    long long sum_r = 0;
    for (int ii : group) sum_r += comp[ii].r;
    long long ideal_group_w = (sum_r + 5000LL) / 10000;
    int ideal_group_r = current + ideal_group_w;
    int min_group_r = curr_x + 1;
    int max_group_r = (jj == n ? 10000 : comp[order[jj]].x);
    int chosen_group_r = ideal_group_r;
    chosen_group_r = max(chosen_group_r, min_group_r);
    chosen_group_r = min(chosen_group_r, max_group_r);
    int group_w = chosen_group_r - current;
    int current_y = 0;
    for (int m = 0; m < k; m++) {
      int ii = group[m];
      int yi = comp[ii].y;
      int min_d = yi + 1;
      double ideal_h_float = (double)comp[ii].r / group_w;
      int ideal_h = round(ideal_h_float);
      int ideal_d = current_y + ideal_h;
      int max_d = (m == k - 1 ? 10000 : comp[group[m + 1]].y);
      int chosen_d = ideal_d;
      chosen_d = max(chosen_d, min_d);
      chosen_d = min(chosen_d, max_d);
      comp[ii].a = current;
      comp[ii].b = current_y;
      comp[ii].c = chosen_group_r;
      comp[ii].d = chosen_d;
      current_y = chosen_d;
    }
    if (current_y < 10000) {
      int last_ii = group.back();
      long long s = (long long)(comp[last_ii].c - comp[last_ii].a) * (comp[last_ii].d - comp[last_ii].b);
      long long rr = comp[last_ii].r;
      double ratio = (double)min(s, rr) / max(s, rr);
      double p = 1 - (1 - ratio) * (1 - ratio);
      long long s2 = (long long)(comp[last_ii].c - comp[last_ii].a) * (10000 - comp[last_ii].b);
      double ratio2 = (double)min(s2, rr) / max(s2, rr);
      double p2 = 1 - (1 - ratio2) * (1 - ratio2);
      if (p2 > p + 1e-9) {
        comp[last_ii].d = 10000;
      }
    }
    current = chosen_group_r;
  }
  if (current < 10000) {
    int last_ii = order.back();
    long long s = (long long)(comp[last_ii].c - comp[last_ii].a) * (comp[last_ii].d - comp[last_ii].b);
    long long rr = comp[last_ii].r;
    double ratio = (double)min(s, rr) / max(s, rr);
    double p = 1 - (1 - ratio) * (1 - ratio);
    long long s2 = (long long)(10000 - comp[last_ii].a) * (comp[last_ii].d - comp[last_ii].b);
    double ratio2 = (double)min(s2, rr) / max(s2, rr);
    double p2 = 1 - (1 - ratio2) * (1 - ratio2);
    if (p2 > p + 1e-9) {
      comp[last_ii].c = 10000;
    }
  }
}

int main() {
  int n;
  cin >> n;
  vector<Company> comp(n);
  for (int i = 0; i < n; i++) {
    cin >> comp[i].x >> comp[i].y >> comp[i].r;
  }
  vector<Company> comp_x = comp;
  do_assignment(comp_x);
  double sum_p_x = 0;
  vector<array<int, 4>> res_x(n);
  for (int i = 0; i < n; i++) {
    res_x[i] = {comp_x[i].a, comp_x[i].b, comp_x[i].c, comp_x[i].d};
    long long s = (long long)(comp_x[i].c - comp_x[i].a) * (comp_x[i].d - comp_x[i].b);
    long long rr = comp_x[i].r;
    double alpha = (s == 0 ? 0.0 : (double)min(s, rr) / max(s, rr));
    sum_p_x += 1 - (1 - alpha) * (1 - alpha);
  }
  vector<Company> comp_y = comp;
  for (auto& c : comp_y) {
    swap(c.x, c.y);
  }
  do_assignment(comp_y);
  double sum_p_y = 0;
  vector<array<int, 4>> res_y(n);
  for (int i = 0; i < n; i++) {
    int a = comp_y[i].b;
    int b = comp_y[i].a;
    int c = comp_y[i].d;
    int d = comp_y[i].c;
    res_y[i] = {a, b, c, d};
    long long s = (long long)(c - a) * (d - b);
    long long rr = comp_y[i].r;
    double alpha = (s == 0 ? 0.0 : (double)min(s, rr) / max(s, rr));
    sum_p_y += 1 - (1 - alpha) * (1 - alpha);
  }
  vector<array<int, 4>>* chosen_res = (sum_p_x >= sum_p_y ? &res_x : &res_y);
  for (auto& arr : *chosen_res) {
    cout << arr[0] << " " << arr[1] << " " << arr[2] << " " << arr[3] << endl;
  }
  return 0;
}