#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int G;
    cin >> G;
    
    if (G == -1) return 0;
    
    for (int hand = 0; hand < G; hand++) {
        string command;
        cin >> command;
        
        if (command == "-1") return 0;
        
        // STATE h r a b P k
        int h, r, a, b, P, k;
        cin >> h >> r >> a >> b >> P >> k;
        
        // ALICE c1 v1 c2 v2
        cin >> command;
        int c1, v1, c2, v2;
        cin >> c1 >> v1 >> c2 >> v2;
        
        // BOARD
        cin >> command;
        for (int i = 0; i < 2 * k; i++) {
            int card_info;
            cin >> card_info;
        }
        
        while (true) {
            // Simple strategy: use RATE to get win probability
            int sample_size = min(1000, 3000000 / G);
            cout << "RATE " << sample_size << endl;
            
            cin >> command;
            if (command == "-1") return 0;
            
            double win_rate, tie_rate;
            cin >> win_rate >> tie_rate;
            
            // Simple decision logic
            double total_good = win_rate + tie_rate * 0.5;
            
            // Aggressive strategy based on win probability
            if (total_good > 0.65) {
                // Strong hand - raise aggressively
                int raise_amount = min(a, max(1, P / 2 + 5));
                cout << "ACTION RAISE " << raise_amount << endl;
            } else if (total_good > 0.5) {
                // Decent hand - moderate raise
                int raise_amount = min(a, max(1, P / 3 + 2));
                cout << "ACTION RAISE " << raise_amount << endl;
            } else if (total_good > 0.35) {
                // Marginal hand - check
                cout << "ACTION CHECK" << endl;
            } else {
                // Weak hand - fold
                cout << "ACTION FOLD" << endl;
            }
            
            cin >> command;
            if (command == "-1") return 0;
            
            if (command == "OPP") {
                string opp_action;
                cin >> opp_action;
                
                if (opp_action == "CHECK") {
                    // Continue to next round or showdown
                } else if (opp_action == "FOLD" || opp_action == "CALL") {
                    // Hand might continue or end
                }
                
                // Check for RESULT
                cin >> command;
                if (command == "-1") return 0;
                
                if (command == "RESULT") {
                    int delta;
                    cin >> delta;
                    break; // End of this hand
                } else if (command == "STATE") {
                    // Continue with next round - put back the STATE
                    cin >> h >> r >> a >> b >> P >> k;
                    
                    cin >> command; // ALICE
                    cin >> c1 >> v1 >> c2 >> v2;
                    
                    cin >> command; // BOARD
                    for (int i = 0; i < 2 * k; i++) {
                        int card_info;
                        cin >> card_info;
                    }
                    continue;
                }
            }
        }
    }
    
    // Read final score
    string command;
    cin >> command;
    if (command == "SCORE") {
        double final_score;
        cin >> final_score;
    }
    
    return 0;
}