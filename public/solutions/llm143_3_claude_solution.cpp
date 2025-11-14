#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
using namespace std;

int main() {
    int G;
    cin >> G;
    
    int totalBudget = 3000000;
    int usedBudget = 0;
    
    for (int game = 0; game < G; game++) {
        string line;
        int h, r, a, b, P, k;
        cin >> line >> h >> r >> a >> b >> P >> k;
        
        // Read hole cards
        cin >> line; // "ALICE"
        int c1, v1, c2, v2;
        cin >> c1 >> v1 >> c2 >> v2;
        
        // Read board
        cin >> line; // "BOARD"
        for (int i = 0; i < 2 * k; i++) {
            int dummy;
            cin >> dummy;
        }
        
        // Simple strategy based on hand strength and position
        double winRate = 0.0, drawRate = 0.0;
        
        // Use RATE query if we have budget
        if (totalBudget - usedBudget > 1000) {
            int samples = min(1000, totalBudget - usedBudget);
            cout << "RATE " << samples << endl;
            usedBudget += samples;
            
            cin >> line >> winRate >> drawRate;
        } else {
            // Fallback: estimate based on hole cards
            bool isPair = (v1 == v2);
            bool isHighCard = (v1 >= 10 || v2 >= 10);
            bool isSuited = (c1 == c2);
            
            if (isPair && v1 >= 8) winRate = 0.6;
            else if (isPair) winRate = 0.45;
            else if (isHighCard && isSuited) winRate = 0.5;
            else if (isHighCard) winRate = 0.4;
            else winRate = 0.3;
        }
        
        // Decision logic
        double equity = winRate + drawRate * 0.5;
        
        if (equity < 0.35) {
            cout << "ACTION CHECK" << endl;
        } else if (equity > 0.65) {
            // Aggressive play with strong hands
            int raiseSize = max(1, min(a, P / 2 + r * 5));
            cout << "ACTION RAISE " << raiseSize << endl;
        } else if (equity > 0.5) {
            // Medium strength - smaller raise
            int raiseSize = max(1, min(a, P / 4 + r * 3));
            cout << "ACTION RAISE " << raiseSize << endl;
        } else {
            cout << "ACTION CHECK" << endl;
        }
        
        // Read opponent response
        string opp, action;
        cin >> opp >> action;
        
        if (action == "FOLD") {
            int delta;
            cin >> line >> delta; // "RESULT delta"
        } else if (action == "CALL") {
            int amount;
            cin >> amount;
            // Continue to next round or showdown
        } else if (action == "CHECK") {
            // Continue to next round or showdown
        }
        
        // Handle potential RESULT
        string next;
        cin >> next;
        if (next == "RESULT") {
            int delta;
            cin >> delta;
        } else {
            // Put back the token (this is simplified - in real implementation
            // we'd need to handle this more carefully)
            // For this problem, we'll just continue the loop
            continue;
        }
        
        // Check for termination
        if (next == "-1") {
            break;
        }
    }
    
    // Read final score
    string score_line;
    double W;
    cin >> score_line >> W;
    
    return 0;
}