#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

using namespace std;

int N, R;
vector<pair<int, int>> connections;
vector<int> parent;

// Given a switch state string, query the judge
int ask(const string& s) {
    cout << "? " << s << endl;
    int result;
    cin >> result;
    return result;
}

// Build parent pointers for the tree structure
void build_tree() {
    parent.assign(2 * N + 1, -1);
    for (int i = 0; i < N; ++i) {
        parent[connections[i].first] = i;
        parent[connections[i].second] = i;
    }
}

// force0 procedure to set output of a switch to a desired value.
// It works by setting inputs to its gate (if any) to (0,0), resulting in slot_out=0.
// Then sets the switch itself to val^slot_out.
void force0(int k, int val, string& s) {
    if (s[k] != '?') { // Already set by a previous force call in this query construction
        return;
    }
    if (k >= N) {
        s[k] = val + '0';
    } else {
        force0(connections[k].first, 0, s);
        force0(connections[k].second, 0, s);
        s[k] = val + '0'; // slot_out is 0, so out = 0^S_k. We want out=val, so S_k=val.
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> R;
    connections.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> connections[i].first >> connections[i].second;
    }
    build_tree();

    string ans(N, '?');
    vector<bool> known(N, false);
    int known_count = 0;

    vector<int> probes;
    for (int i = 0; i < R && N + i <= 2 * N; ++i) {
        probes.push_back(N + i);
    }

    // Phase 1: Probe paths from R switches to identify all-OR and all-AND paths.
    // This reveals the types of gates on these homogeneous paths.
    string s0(2 * N + 1, '0');
    // The output for all-zeroes is always 0, no need to query.
    for (int p : probes) {
        s0[p] = '1';
        if (ask(s0) == 1) { // Path from p to 0 is all ORs
            int curr = p;
            while (curr != -1 && parent[curr] != -1) {
                int p_gate = parent[curr];
                if (!known[p_gate]) {
                    ans[p_gate] = '|';
                    known[p_gate] = true;
                    known_count++;
                }
                curr = p_gate;
            }
        }
        s0[p] = '0';
    }
    
    string s1(2 * N + 1, '1');
    int base_s1_out = ask(s1);

    for (int p : probes) {
        s1[p] = '0';
        if (ask(s1) != base_s1_out) { // Path from p to 0 is all ANDs
            int curr = p;
            while (curr != -1 && parent[curr] != -1) {
                int p_gate = parent[curr];
                if (!known[p_gate]) {
                    ans[p_gate] = '&';
                    known[p_gate] = true;
                    known_count++;
                }
                curr = p_gate;
            }
        }
        s1[p] = '1';
    }

    // Phase 2: Iteratively determine remaining gates.
    // A gate can be tested if all gates on the path from its parent to the root are known.
    while (known_count < N) {
        bool changed_in_iteration = false;
        for (int i = 0; i < N; ++i) {
            if (known[i]) continue;

            bool path_known = true;
            int curr = i;
            while (parent[curr] != -1) {
                curr = parent[curr];
                if (!known[curr]) {
                    path_known = false;
                    break;
                }
            }
            if (!path_known) continue;
            
            // This gate `i` is testable.
            string s_base(2 * N + 1, '?');

            // Set up a "transparent" path from gate i to root 0.
            // A change in slot_out(i) should propagate to out(0).
            curr = i;
            while (parent[curr] != -1) {
                int p_gate = parent[curr];
                int sibling = (connections[p_gate].first == curr) ? connections[p_gate].second : connections[p_gate].first;
                int enable_val = (ans[p_gate] == '&') ? 1 : 0;
                force0(sibling, enable_val, s_base);
                curr = p_gate;
            }
            
            // Two queries to distinguish AND and OR for gate i
            // A: set inputs to (0,1). slot_out is 0 for AND, 1 for OR.
            string s_a = s_base;
            force0(connections[i].first, 0, s_a);
            force0(connections[i].second, 1, s_a);
            
            // B: set inputs to (0,0). slot_out is 0 for both.
            string s_b = s_base;
            force0(connections[i].first, 0, s_b);
            force0(connections[i].second, 0, s_b);

            // Fill in unspecified switches with '0'
            for(int j = 0; j < 2 * N + 1; ++j) {
                if (s_a[j] == '?') s_a[j] = '0';
                if (s_b[j] == '?') s_b[j] = '0';
            }

            int res_a = ask(s_a);
            int res_b = ask(s_b);

            if (res_a != res_b) {
                ans[i] = '|';
            } else {
                ans[i] = '&';
            }
            known[i] = true;
            known_count++;
            changed_in_iteration = true;
        }
        if(!changed_in_iteration && known_count < N){
            // This case should ideally not be reached if the graph is a single tree rooted at 0.
            // It could happen if there are disconnected components or logic error.
            // As a fallback, break to prevent infinite loops on tricky cases.
            break;
        }
    }

    cout << "! " << ans << endl;

    return 0;
}