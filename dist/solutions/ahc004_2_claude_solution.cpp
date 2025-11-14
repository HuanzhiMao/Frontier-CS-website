#include <bits/stdc++.h>
using namespace std;

int N, M;
vector<string> strings;
vector<vector<char>> grid;

bool canPlace(const string& s, int row, int col, int dir) {
    for (int i = 0; i < s.length(); i++) {
        int r, c;
        if (dir == 0) { // horizontal
            r = row;
            c = (col + i) % N;
        } else { // vertical
            r = (row + i) % N;
            c = col;
        }
        if (grid[r][c] != '.' && grid[r][c] != s[i]) {
            return false;
        }
    }
    return true;
}

void placeString(const string& s, int row, int col, int dir) {
    for (int i = 0; i < s.length(); i++) {
        int r, c;
        if (dir == 0) { // horizontal
            r = row;
            c = (col + i) % N;
        } else { // vertical
            r = (row + i) % N;
            c = col;
        }
        grid[r][c] = s[i];
    }
}

bool isSubsequence(const string& s) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // Check horizontal
            bool match = true;
            for (int k = 0; k < s.length(); k++) {
                if (grid[i][(j + k) % N] != s[k]) {
                    match = false;
                    break;
                }
            }
            if (match) return true;
            
            // Check vertical
            match = true;
            for (int k = 0; k < s.length(); k++) {
                if (grid[(i + k) % N][j] != s[k]) {
                    match = false;
                    break;
                }
            }
            if (match) return true;
        }
    }
    return false;
}

int main() {
    cin >> N >> M;
    strings.resize(M);
    grid.assign(N, vector<char>(N, '.'));
    
    for (int i = 0; i < M; i++) {
        cin >> strings[i];
    }
    
    // Sort strings by length (longer first)
    sort(strings.begin(), strings.end(), [](const string& a, const string& b) {
        return a.length() > b.length();
    });
    
    // Try to place each string
    for (const string& s : strings) {
        bool placed = false;
        
        // Try all positions and directions
        for (int dir = 0; dir < 2 && !placed; dir++) {
            for (int i = 0; i < N && !placed; i++) {
                for (int j = 0; j < N && !placed; j++) {
                    if (canPlace(s, i, j, dir)) {
                        placeString(s, i, j, dir);
                        placed = true;
                    }
                }
            }
        }
    }
    
    // Fill remaining empty cells with random characters
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 7);
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == '.') {
                grid[i][j] = 'A' + dis(gen);
            }
        }
    }
    
    // Output the grid
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << grid[i][j];
        }
        cout << '\n';
    }
    
    return 0;
}