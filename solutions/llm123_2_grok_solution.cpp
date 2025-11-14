#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);
  cout.tie(NULL);
  int n;
  cin >> n;
  vector<int> cand(n);
  for(int i = 0; i < n; i++) {
    cand[i] = i + 1;
  }
  bitset<100001> qs[54];
  vector<int> answers(54, -1);
  int query_count = 0;
  set<int> wrongs;
  int total_guesses = 0;
  while(true) {
    if(cand.size() > 0 && cand.size() <= 2 && total_guesses < 2) {
      vector<int> remaining = cand;
      while(remaining.size() > 0 && total_guesses < 2) {
        int g = remaining[0];
        cout << "! " << g << endl;
        cout.flush();
        string rep;
        cin >> rep;
        if(rep == ":)") {
          return 0;
        }
        total_guesses++;
        wrongs.insert(g);
        vector<int> tmp;
        for(int y : remaining) {
          if(y != g) tmp.push_back(y);
        }
        remaining = tmp;
      }
      cand = remaining;
    }
    if(cand.size() <= 1) {
      break;
    }
    query_count++;
    if(query_count > 53) {
      assert(false);
    }
    qs[query_count].reset();
    vector<int> s;
    if(query_count == 1) {
      int half = cand.size() / 2;
      for(size_t i = 0; i < half; i++) {
        qs[query_count][cand[i]] = 1;
      }
    } else {
      vector<int> q_list, not_q_list;
      bool target_match = answers[query_count - 1];
      for(int x : cand) {
        if(qs[query_count - 1][x] == target_match) {
          q_list.push_back(x);
        } else {
          not_q_list.push_back(x);
        }
      }
      int qq = q_list.size();
      int ccc = cand.size();
      int mmm = ccc - qq;
      int b1 = mmm / 2;
      int max1 = max(qq + b1, ccc - b1);
      int b2 = b1 + 1;
      int max2 = (b2 <= mmm) ? max(qq + b2, ccc - b2) : INT_MAX;
      int best_bb = (max1 < max2 || (max1 == max2 && b1 <= b2)) ? b1 : b2;
      for(int i = 0; i < best_bb && i < not_q_list.size(); i++) {
        qs[query_count][not_q_list[i]] = 1;
      }
    }
    for(int x : cand) {
      if(qs[query_count][x]) {
        s.push_back(x);
      }
    }
    cout << "? " << s.size();
    for(int v : s) {
      cout << " " << v;
    }
    cout << endl;
    cout.flush();
    string reply;
    cin >> reply;
    answers[query_count] = (reply == "YES" ? 1 : 0);
    vector<int> new_cand;
    for(int x : cand) {
      bool keep = (wrongs.count(x) == 0);
      if(keep) {
        if(query_count == 1) {
          new_cand.push_back(x);
        } else {
          bool match_prev = (qs[query_count - 1][x] == answers[query_count - 1]);
          bool match_curr = (qs[query_count][x] == answers[query_count]);
          if(match_prev || match_curr) {
            new_cand.push_back(x);
          }
        }
      }
    }
    cand = new_cand;
  }
  return 0;
}