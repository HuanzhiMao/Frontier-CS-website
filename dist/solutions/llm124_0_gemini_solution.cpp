#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>

using namespace std;

int query(const vector<int>& indices) {
    cout << "? " << indices.size();
    for (int i : indices) {
        cout << " " << i;
    }
    cout << endl;
    int result;
    cin >> result;
    return result;
}

void answer(const vector<int>& p) {
    cout << "!";
    for (size_t i = 1; i < p.size(); ++i) {
        cout << " " << p[i];
    }
    cout << endl;
}

long long extended_gcd(long long a, long long b, long long &x, long long &y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    long long x1, y1;
    long long d = extended_gcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

long long mod_inverse(long long a, long long m) {
    long long x, y;
    long long g = extended_gcd(a, m, x, y);
    if (g != 1) return -1;
    return (x % m + m) % m;
}

long long crt(long long a1, long long m1, long long a2, long long m2) {
    if (m1 == -1) return a2;
    if (m2 == -1) return a1;
    if (a1 == -1 || a2 == -1) return -1;
    
    long long g = std::gcd(m1, m2);
    if (a1 % g != a2 % g) return -1;

    long long m1_g = m1 / g;
    long long inv = mod_inverse(m1_g, m2 / g);
    long long k = ( (a2 - a1) / g % (m2/g) + (m2/g) ) % (m2/g) * inv % (m2/g);
    
    return a1 + m1 * k;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    vector<int> s1, s2;
    s1.push_back(1);

    for (int i = 2; i <= n; ++i) {
        if (query({1, i})) {
            s1.push_back(i);
        } else {
            s2.push_back(i);
        }
    }

    int m = n / 2;
    vector<long long> p_mod_m_plus_1(n + 1, -1);
    
    for (int i = 1; i <= n; ++i) {
        vector<int> q_indices;
        q_indices.push_back(i);
        bool i_in_s1 = false;
        for(int x : s1) if(x == i) i_in_s1 = true;

        if (i_in_s1) {
            for (int j : s2) q_indices.push_back(j);
        } else {
            for (int j : s1) q_indices.push_back(j);
        }

        if (q_indices.size() > 1 && query(q_indices)) {
             long long sum_other_group;
            if (i_in_s1) { // Other group is S2
                if ((m % 2 != 0 && s1[0] == 1) || (m % 2 == 0 && s1[0] != 1)) { // S2 values are odd
                    sum_other_group = (long long)m * m;
                } else { // S2 values are even
                    sum_other_group = (long long)m * (m + 1);
                }
            } else { // Other group is S1
                if ((m % 2 != 0 && s1[0] == 1) || (m % 2 == 0 && s1[0] != 1)) { // S1 values are odd
                    sum_other_group = (long long)m * m;
                } else { // S1 values are even
                    sum_other_group = (long long)m * (m + 1);
                }
            }
             p_mod_m_plus_1[i] = (-(sum_other_group % (m + 1)) + (m + 1)) % (m + 1);
        }
    }
    
    bool s1_is_odd;
    int i0 = -1;
    int p_i0 = -1;

    for (int i=1; i<=n; ++i) {
        if (p_mod_m_plus_1[i] != -1) {
            bool i_in_s1 = find(s1.begin(), s1.end(), i) != s1.end();
            long long rem = p_mod_m_plus_1[i];
            
            // Try assuming s1 is odd
            bool is_odd = i_in_s1;
            if (rem % 2 == is_odd) {
                 if (i_in_s1 && i==1 && rem > m) {}
                 else {
                    s1_is_odd = true; i0 = i; p_i0 = rem; break;
                 }
            }
            if (m % 2 == 0 && rem == 0) {
                 long long cand_val = m+1;
                 if (cand_val % 2 == is_odd) {
                    if (i_in_s1 && i==1 && cand_val > m) {}
                    else {
                        s1_is_odd = true; i0 = i; p_i0 = cand_val; break;
                    }
                 }
            }
            
            // Try assuming s1 is even
            is_odd = !i_in_s1;
            long long rem_p_plus_1 = (rem - 1 + m + 1) % (m + 1);
            if (m % 2 != 0) { // p_val is m
                if (m % 2 == !is_odd) {
                    if(i_in_s1 && i==1 && m > n/2){}
                    else {
                         s1_is_odd = false; i0 = i; p_i0 = m; break;
                    }
                }
            } else { // p_val is k*(m+1)-1
                if(rem_p_plus_1==0) {
                    long long cand_val = m;
                    if (cand_val % 2 == !is_odd) {
                        if(i_in_s1 && i==1 && cand_val > n/2){}
                        else{
                            s1_is_odd = false; i0 = i; p_i0 = cand_val; break;
                        }
                    }
                }
            }

        }
    }
    if (i0 == -1) { s1_is_odd = true; i0 = 1; p_i0 = 1; }

    if (!s1_is_odd) swap(s1, s2);
    
    vector<long long> p_mod_m_plus_2(n + 1, -1);
    
    int k0 = -1;
    for(int x : s1) if (x != i0) { k0 = x; break; }

    if (k0 != -1) {
        for(int i : s1) {
            if(i == k0) continue;
            vector<int> q_indices = {i, k0};
            q_indices.insert(q_indices.end(), s2.begin(), s2.end());
            if(query(q_indices)) {
                long long sum_s2 = (long long)m * (m + 1);
                p_mod_m_plus_2[i] = (-(sum_s2 % (m+2)) + (m+2)) % (m+2);
            }
        }
    }

    vector<int> p(n + 1, 0);
    p[i0] = p_i0;
    
    for (int i : s1) {
        if (p[i] != 0) continue;
        long long rem1 = -1, rem2 = -1;
        long long sum_s2_rem1 = ((long long)m*(m+1)) % (m+1);
        if (p_mod_m_plus_1[i] != -1) rem1 = (p_mod_m_plus_1[i] - sum_s2_rem1 + m+1) % (m+1);
        
        if (k0 != -1) {
            long long p_i_plus_pk0_rem2 = p_mod_m_plus_2[i];
            if(i == i0) p_i_plus_pk0_rem2 = p_mod_m_plus_2[k0];
            if (p_i_plus_pk0_rem2 == -1) p_i_plus_pk0_rem2 = 0;

            long long p_i0_rem2 = p[i0] % (m+2);
            long long p_k0_plus_pi0_rem2 = p_mod_m_plus_2[k0 == i ? k0 : i0];
            if (p_k0_plus_pi0_rem2 == -1) p_k0_plus_pi0_rem2 = 0;
            
            long long p_k0_rem2 = (p_k0_plus_pi0_rem2 - p_i0_rem2 + m+2) % (m+2);
            rem2 = (p_i_plus_pk0_rem2 - p_k0_rem2 + m+2) % (m+2);
        }
        
        long long val_i = crt(rem1, m+1, rem2, m+2);
        if (val_i == -1) continue;
        for (int k_val = 1; k_val <= m; ++k_val) {
             int odd_val = 2*k_val-1;
             if (odd_val % ((long long)(m+1)*(m+2)/gcd(m+1,m+2)) == val_i) {
                 p[i] = odd_val;
                 break;
             }
        }
    }
    
    vector<bool> used_s1_vals(m+1, false);
    for(int i : s1) if(p[i] != 0) used_s1_vals[(p[i]+1)/2] = true;
    vector<int> unused_s1_vals;
    for(int i=1; i<=m; ++i) if(!used_s1_vals[i]) unused_s1_vals.push_back(2*i-1);
    
    int current_unused = 0;
    for(int i : s1) if(p[i] == 0) p[i] = unused_s1_vals[current_unused++];
    
    vector<pair<int, int>> s2_info;
    for(int j : s2) {
        long long sum_s1_rem1 = ((long long)m*m) % (m+1);
        long long rem1 = -1;
        if(p_mod_m_plus_1[j] != -1) rem1 = (p_mod_m_plus_1[j] - sum_s1_rem1 + m+1) % (m+1);
        s2_info.push_back({j, (int)rem1});
    }

    vector<bool> s1_partnered(n+1, false);
    for (int j_idx = 0; j_idx < s2_info.size(); ++j_idx) {
        int j = s2_info[j_idx].first;
        int rem1 = s2_info[j_idx].second;
        if (rem1 == -1) continue;

        for (int i : s1) {
            if (s1_partnered[i]) continue;
            int partner_val = n + 1 - p[i];
            if (partner_val % (m+1) == rem1) {
                p[j] = partner_val;
                s1_partnered[i] = true;
                break;
            }
        }
    }
    
    vector<int> s2_unassigned;
    for(int j : s2) if(p[j]==0) s2_unassigned.push_back(j);
    int s2_unassigned_ptr = 0;
    for (int i : s1) {
        if (!s1_partnered[i]) {
            p[s2_unassigned[s2_unassigned_ptr++]] = n + 1 - p[i];
        }
    }
    
    if (p[1] > n / 2) {
        for (int i = 1; i <= n; ++i) p[i] = n + 1 - p[i];
    }

    answer(p);

    return 0;
}