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
        
        // Strategy: predict the majority vote among other participants
        int count_1 = 0;
        for (char c : predictions) {
            if (c == '1') count_1++;
        }
        
        int my_prediction = (count_1 > n / 2) ? 1 : 0;
        cout << my_prediction << endl;
        cout.flush();
        
        int actual;
        cin >> actual;
        
        // Update error counts for other participants
        for (int i = 0; i < n; i++) {
            if ((predictions[i] - '0') != actual) {
                participant_errors[i]++;
            }
        }
    }
    
    return 0;
}