#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<int> participant_mistakes(n, 0);
    
    for (int wager = 0; wager < m; wager++) {
        string predictions;
        cin >> predictions;
        
        // Strategy: follow the majority vote, or if tied, predict 1
        int count_ones = 0;
        for (char c : predictions) {
            if (c == '1') count_ones++;
        }
        
        int izzy_guess;
        if (count_ones > n - count_ones) {
            izzy_guess = 1;
        } else if (count_ones < n - count_ones) {
            izzy_guess = 0;
        } else {
            izzy_guess = 1; // tie-breaker
        }
        
        cout << izzy_guess << endl;
        cout.flush();
        
        int actual;
        cin >> actual;
        
        // Update mistakes for other participants
        for (int i = 0; i < n; i++) {
            int pred = predictions[i] - '0';
            if (pred != actual) {
                participant_mistakes[i]++;
            }
        }
    }
    
    return 0;
}