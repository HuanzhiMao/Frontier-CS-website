#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int G;
    cin >> G;
    
    int total_rate_budget = 3000000;
    int used_rate_budget = 0;
    
    for (int hand = 0; hand < G; hand++) {
        string cmd;
        while (cin >> cmd) {
            if (cmd == "-1") {
                return 0;
            }
            
            if (cmd == "STATE") {
                int h, r, a, b, P, k;
                cin >> h >> r >> a >> b >> P >> k;
                
                // Read hole cards
                cin >> cmd; // "ALICE"
                int c1, v1, c2, v2;
                cin >> c1 >> v1 >> c2 >> v2;
                
                // Read board
                cin >> cmd; // "BOARD"
                vector<pair<int,int>> board;
                for (int i = 0; i < k; i++) {
                    int suit, val;
                    cin >> suit >> val;
                    board.push_back({suit, val});
                }
                
                // Simple strategy based on hand strength and position
                bool should_rate = (used_rate_budget < total_rate_budget - 50000);
                double win_rate = 0.5, tie_rate = 0.05;
                
                if (should_rate) {
                    int rate_samples = min(1000, (total_rate_budget - used_rate_budget) / (G - hand + 1));
                    if (rate_samples >= 100) {
                        cout << "RATE " << rate_samples << endl;
                        string rates_cmd;
                        cin >> rates_cmd;
                        if (rates_cmd == "-1") return 0;
                        cin >> win_rate >> tie_rate;
                        used_rate_budget += rate_samples;
                    }
                }
                
                // Calculate expected value and decide action
                double hand_strength = win_rate + 0.5 * tie_rate;
                
                // Preflop: be more aggressive with premium hands
                if (r == 1) {
                    bool premium = false;
                    // Check for pairs, high cards
                    if (v1 == v2 && v1 >= 10) premium = true; // High pairs
                    if ((v1 >= 12 && v2 >= 10) || (v1 >= 10 && v2 >= 12)) premium = true; // High cards
                    if (v1 == 14 || v2 == 14) premium = true; // Aces
                    
                    if (premium || hand_strength > 0.65) {
                        int raise_size = min(a, max(1, P / 3));
                        cout << "ACTION RAISE " << raise_size << endl;
                    } else if (hand_strength > 0.45) {
                        cout << "ACTION CHECK" << endl;
                    } else {
                        cout << "ACTION FOLD" << endl;
                    }
                }
                // Post-flop: more based on actual hand strength
                else {
                    if (hand_strength > 0.7) {
                        int raise_size = min(a, max(1, P / 2));
                        cout << "ACTION RAISE " << raise_size << endl;
                    } else if (hand_strength > 0.55) {
                        int raise_size = min(a, max(1, P / 4));
                        cout << "ACTION RAISE " << raise_size << endl;
                    } else if (hand_strength > 0.4) {
                        cout << "ACTION CHECK" << endl;
                    } else {
                        cout << "ACTION FOLD" << endl;
                    }
                }
                
                // Read opponent response
                string opp_cmd, opp_action;
                cin >> opp_cmd >> opp_action;
                if (opp_cmd == "-1") return 0;
                
                if (opp_action == "FOLD" || opp_action == "CHECK") {
                    if (opp_action == "FOLD") {
                        cin >> cmd; // "RESULT"
                        int delta;
                        cin >> delta;
                    }
                    // Continue to next round or showdown
                } else if (opp_action == "CALL") {
                    int call_amount;
                    cin >> call_amount;
                    // Continue to next round or showdown
                }
                
                break; // Move to next state or end of hand
            }
            else if (cmd == "RESULT") {
                int delta;
                cin >> delta;
                break; // End of hand
            }
            else if (cmd == "SCORE") {
                double score;
                cin >> score;
                return 0; // End of match
            }
        }
    }
    
    return 0;
}