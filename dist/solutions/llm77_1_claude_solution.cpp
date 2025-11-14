#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<int> participant_errors(n, 0);
    
    for (int wager = 0; wager < m; wager++) {
        string predictions;
        cin >> predictions;
        
        // Find majority vote among other participants
        int ones = 0, zeros = 0;
        for (char c : predictions) {
            if (c == '1') ones++;
            else zeros++;
        }
        
        // Make prediction based on majority
        int my_prediction = (ones > zeros) ? 1 : 0;
        cout << my_prediction << endl;
        cout.flush();
        
        // Read actual outcome
        int actual;
        cin >> actual;
        
        // Update error counts for other participants
        for (int i = 0; i < n; i++) {
            int pred = predictions[i] - '0';
            if (pred != actual) {
                participant_errors[i]++;
            }
        }
    }
    
    return 0;
}