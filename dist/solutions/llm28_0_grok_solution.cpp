#include <bits/stdc++.h>
using namespace std;

pair<int, vector<string>> do_query(const string& s, int k) {
  cout << "query " << s << " " << k << endl;
  cout.flush();
  int num;
  cin >> num;
  vector<string> words(num);
  for (int i = 0; i < num; i++) {
    cin >> words[i];
  }
  return {num, words};
}

int main() {
  int T;
  cin >> T;
  for (int t = 0; t < T; t++) {
    int N;
    cin >> N;
    set<string> all_words;
    for (char ch = 'a'; ch <= 'z'; ch++) {
      string S(1, ch);
      vector<string> group;
      int attempt = 1;
      while (true) {
        int this_k = min(attempt, N);
        auto [num, words] = do_query(S, this_k);
        if (num < this_k) {
          group = words;
          break;
        }
        if (this_k == N) {
          group = words;
          break;
        }
        attempt *= 2;
      }
      for (auto& w : group) {
        all_words.insert(w);
      }
    }
    cout << "answer";
    for (auto& w : all_words) {
      cout << " " << w;
    }
    cout << endl;
    cout.flush();
  }
  return 0;
}