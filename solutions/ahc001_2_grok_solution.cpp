#include <bits/stdc++.h>
using namespace std;

int main() {
  int n;
  cin >> n;
  vector<int> X(n), Y(n);
  vector<long long> R(n);
  for (int i = 0; i < n; i++) {
    cin >> X[i] >> Y[i] >> R[i];
  }
  vector<int> A(n), B(n), C(n), D(n);
  auto recurse = [&](auto&& self, int xl, int xr, int yl, int yr, vector<int> comp) -> void {
    int m = comp.size();
    if (m == 0) return;
    if (m == 1) {
      int i = comp[0];
      A[i] = xl;
      B[i] = yl;
      C[i] = xr;
      D[i] = yr;
      return;
    }
    vector<int> sorted_comp = comp;
    sort(sorted_comp.begin(), sorted_comp.end(), [&](int a, int b) {
      return X[a] < X[b];
    });
    vector<long long> prefix(m + 1, 0);
    for (int i = 1; i <= m; i++) {
      prefix[i] = prefix[i - 1] + R[sorted_comp[i - 1]];
    }
    long long total_r = prefix[m];
    double current_area = (double)(xr - xl) * (yr - yl);
    double best_diff = 1e100;
    int best_type = -1; // 0 vertical, 1 horizontal
    int best_j = -1;
    int best_split_pos = -1;
    // vertical
    for (int j = 1; j < m; j++) {
      int x_prev = X[sorted_comp[j - 1]];
      int x_next = X[sorted_comp[j]];
      if (x_next <= x_prev) continue;
      int w_min_ = x_prev + 1 - xl;
      int w_max_ = x_next - xl;
      int w_upper = min(w_max_, (xr - xl) - 1);
      if (w_min_ > w_upper) continue;
      double sum_left_r = prefix[j];
      double target = sum_left_r / total_r * current_area;
      double h_ = yr - yl;
      double ideal_w = target / h_;
      int ww = max(w_min_, min(w_upper, (int)round(ideal_w)));
      double this_diff = fabs((double)ww * h_ - target);
      if (this_diff < best_diff) {
        best_diff = this_diff;
        best_type = 0;
        best_j = j;
        best_split_pos = xl + ww;
      }
    }
    // horizontal
    sort(sorted_comp.begin(), sorted_comp.end(), [&](int a, int b) {
      return Y[a] < Y[b];
    });
    prefix[0] = 0;
    for (int i = 1; i <= m; i++) {
      prefix[i] = prefix[i - 1] + R[sorted_comp[i - 1]];
    }
    for (int j = 1; j < m; j++) {
      int y_prev = Y[sorted_comp[j - 1]];
      int y_next = Y[sorted_comp[j]];
      if (y_next <= y_prev) continue;
      int h_min_ = y_prev + 1 - yl;
      int h_max_ = y_next - yl;
      int h_upper = min(h_max_, (yr - yl) - 1);
      if (h_min_ > h_upper) continue;
      double sum_left_r = prefix[j];
      double target = sum_left_r / total_r * current_area;
      double w_ = xr - xl;
      double ideal_h = target / w_;
      int hh = max(h_min_, min(h_upper, (int)round(ideal_h)));
      double this_diff = fabs((double)hh * w_ - target);
      if (this_diff < best_diff) {
        best_diff = this_diff;
        best_type = 1;
        best_j = j;
        best_split_pos = yl + hh;
      }
    }
    vector<int> left_comp, right_comp;
    if (best_type == 0) {
      int s = best_split_pos;
      for (auto id : comp) {
        if (X[id] <= s - 1) {
          left_comp.push_back(id);
        } else if (X[id] >= s) {
          right_comp.push_back(id);
        }
      }
      self(self, xl, s, yl, yr, left_comp);
      self(self, s, xr, yl, yr, right_comp);
    } else if (best_type == 1) {
      int s = best_split_pos;
      for (auto id : comp) {
        if (Y[id] <= s - 1) {
          left_comp.push_back(id);
        } else if (Y[id] >= s) {
          right_comp.push_back(id);
        }
      }
      self(self, xl, xr, yl, s, left_comp);
      self(self, xl, xr, s, yr, right_comp);
    }
  };
  vector<int> all_ids(n);
  iota(all_ids.begin(), all_ids.end(), 0);
  recurse(recurse, 0, 10000, 0, 10000, all_ids);
  for (int i = 0; i < n; i++) {
    cout << A[i] << " " << B[i] << " " << C[i] << " " << D[i] << "\n";
  }
}