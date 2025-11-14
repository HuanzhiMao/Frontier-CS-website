#include <bits/stdc++.h>

using namespace std;

vector<string> get_words(const string& prefix, int max_rem) {
  vector<string> words;
  int known = 0;
  int power = 1;
  while (true) {
    int request = known + power;
    if (request > max_rem) request = max_rem;
    cout << "query " << prefix << " " << request << endl;
    cout.flush();
    int num;
    cin >> num;
    words.resize(num);
    for (int i = 0; i < num; ++i) {
      cin >> words[i];
    }
    if (num < request || known >= max_rem) {
      return words;
    }
    known = num;
    power *= 2;
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);
  int T;
  cin >> T;
  for (int t = 0; t < T; ++t) {
    int N;
    cin >> N;
    vector<string> allwords;
    int collected = 0;
    for (char ch = 'a'; ch <= 'z'; ++ch) {
      if (collected >= N) break;
      string prefix(1, ch);
      vector<string> sub = get_words(prefix, N - collected);
      for (const auto& w : sub) {
        allwords.push_back(w);
      }
      collected += sub.size();
    }
    cout << "answer";
    for (const auto& w : allwords) {
      cout << " " << w;
    }
    cout << endl;
    cout.flush();
  }
  return 0;
}