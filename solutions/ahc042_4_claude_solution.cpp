#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    int N;
    cin >> N;
    vector<string> board(N);
    for (int i = 0; i < N; i++) {
        cin >> board[i];
    }
    
    vector<pair<char, int>> moves;
    
    // Find all oni positions and remove them
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 'x') {
                // Check upward direction
                bool canUp = true;
                for (int k = 0; k < i; k++) {
                    if (board[k][j] == 'o') {
                        canUp = false;
                        break;
                    }
                }
                
                if (canUp) {
                    // Remove oni by shifting up then down
                    for (int k = 0; k <= i; k++) {
                        moves.push_back({'U', j});
                    }
                    for (int k = 0; k <= i; k++) {
                        moves.push_back({'D', j});
                    }
                    continue;
                }
                
                // Check downward direction
                bool canDown = true;
                for (int k = i + 1; k < N; k++) {
                    if (board[k][j] == 'o') {
                        canDown = false;
                        break;
                    }
                }
                
                if (canDown) {
                    // Remove oni by shifting down then up
                    for (int k = i; k < N; k++) {
                        moves.push_back({'D', j});
                    }
                    for (int k = i; k < N; k++) {
                        moves.push_back({'U', j});
                    }
                    continue;
                }
                
                // Check leftward direction
                bool canLeft = true;
                for (int k = 0; k < j; k++) {
                    if (board[i][k] == 'o') {
                        canLeft = false;
                        break;
                    }
                }
                
                if (canLeft) {
                    // Remove oni by shifting left then right
                    for (int k = 0; k <= j; k++) {
                        moves.push_back({'L', i});
                    }
                    for (int k = 0; k <= j; k++) {
                        moves.push_back({'R', i});
                    }
                    continue;
                }
                
                // Check rightward direction
                bool canRight = true;
                for (int k = j + 1; k < N; k++) {
                    if (board[i][k] == 'o') {
                        canRight = false;
                        break;
                    }
                }
                
                if (canRight) {
                    // Remove oni by shifting right then left
                    for (int k = j; k < N; k++) {
                        moves.push_back({'R', i});
                    }
                    for (int k = j; k < N; k++) {
                        moves.push_back({'L', i});
                    }
                    continue;
                }
            }
        }
    }
    
    for (auto move : moves) {
        cout << move.first << " " << move.second << "\n";
    }
    
    return 0;
}