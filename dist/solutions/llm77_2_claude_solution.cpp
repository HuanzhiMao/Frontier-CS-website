#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<int> mistakes(n, 0);
    
    for (int round = 0; round < m; round++) {
        string predictions;
        cin >> predictions;
        
        // Find participant with minimum mistakes so far
        int min_mistakes = mistakes[0];
        int best_participant = 0;
        for (int i = 1; i < n; i++) {
            if (mistakes[i] < min_mistakes) {
                min_mistakes = mistakes[i];
                best_participant = i;
            }
        }
        
        // Follow the best participant's prediction
        int izzy_guess = predictions[best_participant] - '0';
        cout << izzy_guess << endl;
        cout.flush();
        
        int actual;
        cin >> actual;
        
        // Update mistake counts for all participants
        for (int i = 0; i < n; i++) {
            if ((predictions[i] - '0') != actual) {
                mistakes[i]++;
            }
        }
    }
    
    return 0;
}