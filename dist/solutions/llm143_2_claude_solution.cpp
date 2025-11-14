#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

int main() {
    int G;
    cin >> G;
    
    int total_budget = 3000000;
    int used_budget = 0;
    
    for (int g = 0; g < G; g++) {
        string line;
        getline(cin, line); // consume newline
        
        while (true) {
            getline(cin, line);
            if (line == "-1") return 0;
            
            stringstream ss(line);
            string cmd;
            ss >> cmd;
            
            if (cmd == "STATE") {
                int h, r, a, b, P, k;
                ss >> h >> r >> a >> b >> P >> k;
                
                // Read Alice's cards
                getline(cin, line);
                stringstream alice_ss(line);
                string alice_str;
                alice_ss >> alice_str; // "ALICE"
                int c1, v1, c2, v2;
                alice_ss >> c1 >> v1 >> c2 >> v2;
                
                // Read board
                getline(cin, line);
                
                // Simple strategy based on round and hand strength estimation
                int remaining_budget = total_budget - used_budget;
                
                // Use RATE query if we have budget
                double win_rate = 0.5;
                if (remaining_budget > 1000) {
                    int query_size = min(1000, remaining_budget);
                    cout << "RATE " << query_size << endl;
                    used_budget += query_size;
                    
                    getline(cin, line);
                    if (line == "-1") return 0;
                    
                    stringstream rate_ss(line);
                    string rates_str;
                    double tie_rate;
                    rate_ss >> rates_str >> win_rate >> tie_rate;
                }
                
                // Simple strategy
                bool should_raise = false;
                int raise_amount = 5;
                
                // Preflop strategy - raise with high cards
                if (r == 1) {
                    int max_val = max(v1, v2);
                    bool is_pair = (v1 == v2);
                    bool high_cards = (max_val >= 10) || is_pair;
                    
                    if (high_cards || win_rate > 0.6) {
                        should_raise = true;
                        raise_amount = min(a, P/2 + 5);
                    }
                }
                // Post-flop strategy
                else {
                    if (win_rate > 0.65) {
                        should_raise = true;
                        raise_amount = min(a, P/2 + 10);
                    } else if (win_rate > 0.55 && r <= 2) {
                        should_raise = true;
                        raise_amount = min(a, 10);
                    }
                }
                
                // Don't raise with very weak hands
                if (win_rate < 0.3) {
                    should_raise = false;
                }
                
                // Output action
                if (should_raise && raise_amount > 0) {
                    cout << "ACTION RAISE " << raise_amount << endl;
                } else if (win_rate < 0.25) {
                    cout << "ACTION FOLD" << endl;
                } else {
                    cout << "ACTION CHECK" << endl;
                }
                
            } else if (cmd == "OPP") {
                // Just read opponent's action, continue
                continue;
            } else if (cmd == "RESULT") {
                // Hand finished
                break;
            } else if (cmd == "SCORE") {
                // Match finished
                return 0;
            }
        }
    }
    
    return 0;
}