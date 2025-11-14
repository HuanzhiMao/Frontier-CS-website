#include <bits/stdc++.h>

using namespace std;

int main() {
  int N, M;
  cin >> N >> M;
  vector<pair<int,int>> pos(M);
  for(int k=0; k<M; k++) {
    cin >> pos[k].first >> pos[k].second;
  }
  vector<pair<char,char>> actions;
  int cr = pos[0].first, cc = pos[0].second;
  for(int k=1; k<M; k++) {
    int tr = pos[k].first, tc = pos[k].second;
    const int INF = 1e9;
    vector<vector<int>> dist(N, vector<int>(N, INF));
    vector<vector<pair<int,int>>> prev_pos(N, vector<pair<int,int>>(N, {-1,-1}));
    vector<vector<pair<char,char>>> prev_act(N, vector<pair<char,char>>(N, {' ',' '}));
    queue<pair<int,int>> q;
    dist[cr][cc] = 0;
    q.push({cr,cc});
    while(!q.empty()) {
      auto [r,c] = q.front(); q.pop();
      if(r > 0) {
        int nr = r-1, nc = c;
        if(dist[nr][nc] > dist[r][c] + 1) {
          dist[nr][nc] = dist[r][c] + 1;
          prev_pos[nr][nc] = {r,c};
          prev_act[nr][nc] = {'M','U'};
          q.push({nr,nc});
        }
      }
      if(r < N-1) {
        int nr = r+1, nc = c;
        if(dist[nr][nc] > dist[r][c] + 1) {
          dist[nr][nc] = dist[r][c] + 1;
          prev_pos[nr][nc] = {r,c};
          prev_act[nr][nc] = {'M','D'};
          q.push({nr,nc});
        }
      }
      if(c > 0) {
        int nr = r, nc = c-1;
        if(dist[nr][nc] > dist[r][c] + 1) {
          dist[nr][nc] = dist[r][c] + 1;
          prev_pos[nr][nc] = {r,c};
          prev_act[nr][nc] = {'M','L'};
          q.push({nr,nc});
        }
      }
      if(c < N-1) {
        int nr = r, nc = c+1;
        if(dist[nr][nc] > dist[r][c] + 1) {
          dist[nr][nc] = dist[r][c] + 1;
          prev_pos[nr][nc] = {r,c};
          prev_act[nr][nc] = {'M','R'};
          q.push({nr,nc});
        }
      }
      if(r > 0) {
        int nr = 0, nc = c;
        if(dist[nr][nc] > dist[r][c] + 1) {
          dist[nr][nc] = dist[r][c] + 1;
          prev_pos[nr][nc] = {r,c};
          prev_act[nr][nc] = {'S','U'};
          q.push({nr,nc});
        }
      }
      if(r < N-1) {
        int nr = N-1, nc = c;
        if(dist[nr][nc] > dist[r][c] + 1) {
          dist[nr][nc] = dist[r][c] + 1;
          prev_pos[nr][nc] = {r,c};
          prev_act[nr][nc] = {'S','D'};
          q.push({nr,nc});
        }
      }
      if(c > 0) {
        int nr = r, nc = 0;
        if(dist[nr][nc] > dist[r][c] + 1) {
          dist[nr][nc] = dist[r][c] + 1;
          prev_pos[nr][nc] = {r,c};
          prev_act[nr][nc] = {'S','L'};
          q.push({nr,nc});
        }
      }
      if(c < N-1) {
        int nr = r, nc = N-1;
        if(dist[nr][nc] > dist[r][c] + 1) {
          dist[nr][nc] = dist[r][c] + 1;
          prev_pos[nr][nc] = {r,c};
          prev_act[nr][nc] = {'S','R'};
          q.push({nr,nc});
        }
      }
    }
    vector<pair<char,char>> seg;
    int cur_r = tr, cur_c = tc;
    while(cur_r != cr || cur_c != cc) {
      auto act = prev_act[cur_r][cur_c];
      seg.push_back(act);
      auto p = prev_pos[cur_r][cur_c];
      cur_r = p.first;
      cur_c = p.second;
    }
    reverse(seg.begin(), seg.end());
    for(auto a: seg) actions.push_back(a);
    cr = tr;
    cc = tc;
  }
  for(auto [a,d]: actions) {
    cout << a << " " << d << endl;
  }
  return 0;
}