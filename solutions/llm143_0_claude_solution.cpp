#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    int G;
    cin >> G;
    
    int total_budget = 3000000;
    int used_budget = 0;
    
    for (int game = 0; game < G; game++) {
        string line;
        cin >> line;
        
        if (line == "-1") {
            return 0;
        }
        
        // Read STATE
        int h, r, a, b, P, k;
        cin >> h >> r >> a >> b >> P >> k;
        
        // Read ALICE cards
        cin >> line; // "ALICE"
        int c1, v1, c2, v2;
        cin >> c1 >> v1 >> c2 >> v2;
        
        // Read BOARD
        cin >> line; // "BOARD"
        for (int i = 0; i < 2 * k; i++) {
            int dummy;
            cin >> dummy;
        }
        
        while (true) {
            // Simple strategy based on hand strength and position
            bool should_raise = false;
            int raise_amount = 0;
            
            // Use RATE query if we have budget
            double win_rate = 0.5;
            double tie_rate = 0.0;
            
            if (used_budget < total_budget - 1000) {
                int samples = min(1000, total_budget - used_budget);
                cout << "RATE " << samples << endl;
                used_budget += samples;
                
                string rates_line;
                cin >> rates_line;
                if (rates_line == "-1") return 0;
                cin >> win_rate >> tie_rate;
            }
            
            // Simple strategy: raise if win rate is good enough
            double equity = win_rate + 0.5 * tie_rate;
            
            if (r == 1) { // Preflop
                // Raise with strong hands or bluff occasionally
                if (equity > 0.6 || (v1 >= 11 && v2 >= 11)) {
                    should_raise = true;
                    raise_amount = min(a, P / 2 + 5);
                }
            } else { // Postflop
                if (equity > 0.65) {
                    should_raise = true;
                    raise_amount = min(a, P / 2 + 10);
                } else if (equity > 0.55 && r >= 3) {
                    should_raise = true;
                    raise_amount = min(a, P / 3 + 5);
                }
            }
            
            // Make sure raise amount is valid
            if (should_raise && raise_amount >= 1 && raise_amount <= a) {
                cout << "ACTION RAISE " << raise_amount << endl;
            } else if (equity < 0.3) {
                cout << "ACTION FOLD" << endl;
            } else {
                cout << "ACTION CHECK" << endl;
            }
            
            // Read opponent response
            string opp_action;
            cin >> opp_action;
            
            if (opp_action == "-1") return 0;
            
            if (opp_action == "OPP") {
                string action_type;
                cin >> action_type;
                
                if (action_type == "FOLD" || action_type == "CHECK") {
                    // Continue to next round or end hand
                } else if (action_type == "CALL") {
                    int call_amount;
                    cin >> call_amount;
                }
            }
            
            // Check if hand is over
            string next_line;
            cin >> next_line;
            
            if (next_line == "-1") return 0;
            
            if (next_line == "RESULT") {
                int delta;
                cin >> delta;
                break; // Hand is over
            } else if (next_line == "STATE") {
                // Continue to next round
                cin >> r >> a >> b >> P >> k;
                
                // Read ALICE cards again
                cin >> line; // "ALICE"
                cin >> c1 >> v1 >> c2 >> v2;
                
                // Read BOARD
                cin >> line; // "BOARD"
                for (int i = 0; i < 2 * k; i++) {
                    int dummy;
                    cin >> dummy;
                }
            }
        }
    }
    
    // Read final score
    string score_line;
    double final_score;
    cin >> score_line >> final_score;
    
    return 0;
}