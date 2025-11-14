#include <bits/stdc++.h>
using namespace std;

int compute_M(const vector<vector<int>>& board, int N) {
  int m = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (board[i][j] == 0) continue;
      if (j + 1 < N && board[i][j + 1] != 0 && (board[i][j] & 4) && (board[i][j + 1] & 1)) m++;
      if (i + 1 < N && board[i + 1][j] != 0 && (board[i][j] & 8) && (board[i + 1][j] & 2)) m++;
    }
  }
  return m;
}

int compute_S(const vector<vector<int>>& board, int N) {
  vector<vector<int>> adj(N * N);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (board[i][j] == 0) continue;
      int u = i * N + j;
      if (j + 1 < N && board[i][j + 1] != 0 && (board[i][j] & 4) && (board[i][j + 1] & 1)) {
        int v = i * N + j + 1;
        adj[u].push_back(v);
        adj[v].push_back(u);
      }
      if (i + 1 < N && board[i + 1][j] != 0 && (board[i][j] & 8) && (board[i + 1][j] & 2)) {
        int v = (i + 1) * N + j;
        adj[u].push_back(v);
        adj[v].push_back(u);
      }
    }
  }
  vector<bool> visited(N * N, false);
  int max_s = 0;
  for (int start = 0; start < N * N; start++) {
    if (visited[start] || board[start / N][start % N] == 0) continue;
    stack<int> st;
    st.push(start);
    visited[start] = true;
    int sum_deg = 0;
    int v = 0;
    while (!st.empty()) {
      int u = st.top();
      st.pop();
      v++;
      for (int vv : adj[u]) {
        sum_deg++;
        if (!visited[vv]) {
          visited[vv] = true;
          st.push(vv);
        }
      }
    }
    int e = sum_deg / 2;
    if (e == v - 1) {
      max_s = max(max_s, v);
    }
  }
  return max_s;
}

struct State {
  vector<vector<int>> board;
  int ex, ey, s, m;
  string path;
};

int main() {
  int N, T;
  cin >> N >> T;
  vector<vector<int>> grid(N, vector<int>(N));
  int ex, ey;
  for (int i = 0; i < N; i++) {
    string s;
    cin >> s;
    for (int j = 0; j < N; j++) {
      char c = s[j];
      int val;
      if (c >= '0' && c <= '9') val = c - '0';
      else val = 10 + c - 'a';
      grid[i][j] = val;
      if (val == 0) {
        ex = i;
        ey = j;
      }
    }
  }
  int full = N * N - 1;
  string best_path = "";
  double best_score = 0.0;
  int init_s = compute_S(grid, N);
  double init_score = (init_s < full) ? round(500000.0 * init_s / full) : round(500000.0 * (2 - 0.0 / T));
  best_score = init_score;
  const int W = 50;
  vector<State> current;
  int init_m = compute_M(grid, N);
  current.push_back({grid, ex, ey, init_s, init_m, ""});
  int dx[4] = {-1, 0, 1, 0};
  int dy[4] = {0, 1, 0, -1};
  string chs = "URDL";
  for (int depth = 0; depth < T; depth++) {
    vector<State> next;
    for (auto &st : current) {
      for (int d = 0; d < 4; d++) {
        int nx = st.ex + dx[d];
        int ny = st.ey + dy[d];
        if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
        vector<vector<int>> nboard = st.board;
        swap(nboard[st.ex][st.ey], nboard[nx][ny]);
        int ns = compute_S(nboard, N);
        int nm = compute_M(nboard, N);
        string npath = st.path + chs[d];
        next.push_back({nboard, nx, ny, ns, nm, npath});
      }
    }
    sort(next.begin(), next.end(), [](const State& a, const State& b) {
      if (a.s != b.s) return a.s > b.s;
      return a.m > b.m;
    });
    if (next.size() > W) next.resize(W);
    current = move(next);
    for (auto &st : current) {
      int kk = st.path.size();
      double sc = (st.s < full) ? round(500000.0 * st.s / full) : round(500000.0 * (2 - (double)kk / T));
      if (sc > best_score) {
        best_score = sc;
        best_path = st.path;
      }
    }
  }
  cout << best_path << endl;
  return 0;
}