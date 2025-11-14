#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

// Function to make a query and get the response
pair<int, int> query(const vector<int>& indices) {
    cout << "0 " << indices.size();
    for (int idx : indices) {
        cout << " " << idx;
    }
    cout << endl;
    int m1, m2;
    cin >> m1 >> m2;
    if (m1 > m2) swap(m1, m2);
    return {m1, m2};
}

// Function to find the medians of a 4-element value set
pair<int, int> get_medians(vector<int> values) {
    sort(values.begin(), values.end());
    return {values[1], values[2]};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    vector<int> p(n + 1, 0);

    // Phase 1: Partitioning
    vector<pair<int, int>> medians_cache(n + 1);
    int s2 = -1;

    for (int i = 4; i <= n; ++i) {
        medians_cache[i] = query({1, 2, 3, i});
    }

    map<int,int> s2_counts;
    s2_counts[medians_cache[4].first]++;
    s2_counts[medians_cache[4].second]++;
    s2_counts[medians_cache[5].first]++;
    s2_counts[medians_cache[5].second]++;
    
    if(s2_counts[medians_cache[4].first] > 1) s2 = medians_cache[4].first;
    else s2 = medians_cache[4].second;

    vector<int> other_medians;
    for (int i = 4; i <= n; ++i) {
        int other_m = (medians_cache[i].first == s2) ? medians_cache[i].second : medians_cache[i].first;
        other_medians.push_back(other_m);
    }

    int s1 = other_medians[0], s3 = other_medians[0];
    for (int m : other_medians) {
        s1 = min(s1, m);
        s3 = max(s3, m);
    }
    
    vector<int> I1, I2, I3;
    for (int i = 4; i <= n; ++i) {
        int other_m = (medians_cache[i].first == s2) ? medians_cache[i].second : medians_cache[i].first;
        if (other_m == s1) {
            I1.push_back(i);
        } else if (other_m == s3) {
            I3.push_back(i);
        } else {
            I2.push_back(i);
            p[i] = other_m;
        }
    }

    // Phase 2: Find p[1], p[2], p[3]
    if (!I1.empty() && !I3.empty()) {
        int i1 = I1[0], i3 = I3[0];
        pair<int, int> m12 = query({1, 2, i1, i3});
        pair<int, int> m13 = query({1, 3, i1, i3});
        
        map<int, int> vals;
        vals[m12.first]++; vals[m12.second]++;
        vals[m13.first]++; vals[m13.second]++;
        
        int p1_val = -1, p2_val = -1, p3_val = -1;
        for(auto const& [val, count] : vals){
            if(count == 2) p1_val = val;
        }
        if(m12.first == p1_val) p2_val = m12.second; else p2_val = m12.first;
        if(m13.first == p1_val) p3_val = m13.second; else p3_val = m13.first;
        p[1] = p1_val; p[2] = p2_val; p[3] = p3_val;
    } else {
        int r1, r2;
        vector<int> pool;
        pool.insert(pool.end(), I1.begin(), I1.end());
        pool.insert(pool.end(), I2.begin(), I2.end());
        pool.insert(pool.end(), I3.begin(), I3.end());
        r1 = pool[0]; r2 = pool[1];
        
        int pr1 = p[r1] ? p[r1] : (find(I1.begin(), I1.end(), r1) != I1.end() ? 0 : n+2);
        int pr2 = p[r2] ? p[r2] : (find(I1.begin(), I1.end(), r2) != I1.end() ? 0 : n+2);

        pair<int, int> m12 = query({1, 2, r1, r2});
        pair<int, int> m13 = query({1, 3, r1, r2});
        
        auto check = [&](int v1, int v2) {
            return get_medians({v1, v2, pr1, pr2});
        };
        
        if (m12 == check(s1,s3) || m12 == check(s3,s1)) { p[3] = s2;}
        else if(m13 == check(s1,s3) || m13 == check(s3,s1)) { p[2] = s2;}
        else { p[1] = s2; }
        
        int idx_s2 = -1, idx_A = -1, idx_B = -1;
        for(int i=1; i<=3; ++i) if(p[i] == s2) idx_s2 = i;
        for(int i=1; i<=3; ++i) if(p[i] == 0) { if(idx_A == -1) idx_A = i; else idx_B = i; }
        
        pair<int, int> m_A = query({idx_A, idx_s2, r1, r2});
        if (m_A == check(s1, s2) || m_A == check(s2,s1)) { p[idx_A] = s1; p[idx_B] = s3; }
        else { p[idx_A] = s3; p[idx_B] = s1; }
    }
    
    // Phase 3: Find idx_1 and idx_n
    int idx_s1 = -1, idx_s2 = -1, idx_s3 = -1;
    for(int i=1; i<=3; ++i) {
        if(p[i] == s1) idx_s1 = i;
        if(p[i] == s2) idx_s2 = i;
        if(p[i] == s3) idx_s3 = i;
    }

    int idx_1 = idx_s1;
    for (int i : I1) {
        pair<int, int> m = query({i, idx_1, idx_s2, idx_s3});
        if (m.first == p[idx_1]) {
           idx_1 = i;
        }
    }
    p[idx_1] = 1;

    int idx_n = idx_s3;
    for (int i : I3) {
        pair<int, int> m = query({i, idx_n, idx_s1, idx_s2});
        if (m.second == p[idx_n]) {
            idx_n = i;
        }
    }
    p[idx_n] = n;
    
    // Phase 4: Find all remaining values
    vector<int> unknowns;
    for (int i = 1; i <= n; ++i) {
        if (p[i] == 0) {
            unknowns.push_back(i);
        }
    }

    for (size_t i = 0; i + 1 < unknowns.size(); i += 2) {
        int u1 = unknowns[i];
        int u2 = unknowns[i+1];
        pair<int, int> m = query({u1, u2, idx_1, idx_n});
        p[u1] = m.first;
        p[u2] = m.second;
    }
    if (unknowns.size() % 2 == 1) {
        int u = unknowns.back();
        int other = -1;
        for(int i=1; i<=n; ++i) {
            if (i != u && i != idx_1 && i != idx_n) {
                other = i;
                break;
            }
        }
        pair<int, int> m = query({u, other, idx_1, idx_n});
        if(m.first == p[other]) p[u] = m.second;
        else p[u] = m.first;
    }

    // Final answer
    vector<pair<int, int>> sorted_p;
    for (int i = 1; i <= n; ++i) {
        sorted_p.push_back({p[i], i});
    }
    sort(sorted_p.begin(), sorted_p.end());

    int i1 = sorted_p[n/2 - 1].second;
    int i2 = sorted_p[n/2].second;
    
    cout << "1 " << i1 << " " << i2 << endl;

    return 0;
}