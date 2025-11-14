#include <bits/stdc++.h>
using namespace std;

int n;
int query_count = 0;
int guess_count = 0;

bool ask(const vector<int>& S) {
    int k = (int)S.size();
    if (k <= 0) {
        // Should not happen as per our logic
        k = 1;
    }
    cout << "? " << k;
    for (int x : S) cout << " " << x;
    cout << endl;
    cout.flush();

    string ans;
    if (!(cin >> ans)) exit(0);
    query_count++;
    return ans[0] == 'Y' || ans[0] == 'y';
}

bool make_guess(int g) {
    cout << "! " << g << endl;
    cout.flush();
    string ans;
    if (!(cin >> ans)) exit(0);
    guess_count++;
    return ans.size() >= 2 && ans[0] == ':' && ans[1] == ')';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n)) return 0;

    vector<int> cand;
    cand.reserve(n);
    for (int i = 1; i <= n; ++i) cand.push_back(i);

    vector<char> markPrev(n + 1, 0);
    vector<int> prevSet;
    bool havePrev = false;
    bool prevAns = false;

    auto rebuild_cand = [&](const vector<int>& query, bool ans, const vector<char>& markPrevLocal, bool prevAnsLocal, vector<int>& candLocal) {
        vector<char> markCur(n + 1, 0);
        for (int x : query) markCur[x] = 1;
        vector<int> newCand;
        newCand.reserve(candLocal.size());
        for (int x : candLocal) {
            bool inA = prevAnsLocal ? (markPrevLocal[x] != 0) : (markPrevLocal[x] == 0);
            bool inB = ans ? (markCur[x] != 0) : (markCur[x] == 0);
            if (inA || inB) newCand.push_back(x);
        }
        candLocal.swap(newCand);
    };

    auto remove_from_cand = [&](int x) {
        vector<int> newCand;
        newCand.reserve(cand.size());
        for (int y : cand) if (y != x) newCand.push_back(y);
        cand.swap(newCand);
    };

    while (true) {
        if ((int)cand.size() <= 2) {
            for (int x : cand) {
                if (make_guess(x)) return 0;
                else remove_from_cand(x);
            }
            return 0;
        }

        if (!havePrev) {
            int k = (int)cand.size() / 2;
            if (k <= 0) k = 1;
            prevSet.clear();
            prevSet.reserve(k);
            for (int i = 0; i < k; ++i) prevSet.push_back(cand[i]);

            // update markPrev
            fill(markPrev.begin(), markPrev.end(), 0);
            for (int x : prevSet) markPrev[x] = 1;

            bool ans = ask(prevSet);
            prevAns = ans;
            havePrev = true;
            continue;
        }

        // Build A and outside A
        vector<int> outsideA;
        outsideA.reserve(cand.size());
        int a_sz = 0;
        for (int x : cand) {
            bool inA = prevAns ? (markPrev[x] != 0) : (markPrev[x] == 0);
            if (inA) a_sz++;
            else outsideA.push_back(x);
        }
        vector<int> query;
        if ((int)outsideA.size() == 0) {
            // Cannot reduce this step; ask any non-empty subset
            query.clear();
            query.push_back(cand[0]);
        } else {
            int s_out = ((int)outsideA.size() + 1) / 2; // ceil
            query.clear();
            query.reserve(s_out);
            for (int i = 0; i < s_out; ++i) query.push_back(outsideA[i]);
        }

        bool ans2 = ask(query);

        // Update candidates using union of previous and current
        rebuild_cand(query, ans2, markPrev, prevAns, cand);

        // Prepare for next iteration: current becomes previous
        fill(markPrev.begin(), markPrev.end(), 0);
        for (int x : query) markPrev[x] = 1;
        prevSet = query;
        prevAns = ans2;

        // Safety: if we approach query limit too closely, finish with guesses
        if (query_count >= 52) {
            // Try to finish with guesses
            while ((int)cand.size() > 2 && query_count < 53) {
                // One more question to attempt reduction
                // Use same strategy with current prev
                vector<int> outsideA2;
                outsideA2.reserve(cand.size());
                int a_sz2 = 0;
                for (int x : cand) {
                    bool inA = prevAns ? (markPrev[x] != 0) : (markPrev[x] == 0);
                    if (inA) a_sz2++;
                    else outsideA2.push_back(x);
                }
                vector<int> q2;
                if ((int)outsideA2.size() == 0) {
                    q2.push_back(cand[0]);
                } else {
                    int s_out2 = ((int)outsideA2.size() + 1) / 2;
                    q2.reserve(s_out2);
                    for (int i = 0; i < s_out2; ++i) q2.push_back(outsideA2[i]);
                }
                bool ans3 = ask(q2);
                rebuild_cand(q2, ans3, markPrev, prevAns, cand);
                fill(markPrev.begin(), markPrev.end(), 0);
                for (int x : q2) markPrev[x] = 1;
                prevSet = q2;
                prevAns = ans3;
            }
            if ((int)cand.size() <= 2) {
                for (int x : cand) {
                    if (make_guess(x)) return 0;
                    else remove_from_cand(x);
                }
                return 0;
            } else {
                // Fall back: try two guesses arbitrarily (may fail if more than 2 remain, but should not happen)
                if (!cand.empty()) {
                    if (make_guess(cand[0])) return 0;
                    else remove_from_cand(cand[0]);
                }
                if (!cand.empty()) {
                    if (make_guess(cand[0])) return 0;
                    else remove_from_cand(cand[0]);
                }
                return 0;
            }
        }
    }

    return 0;
}