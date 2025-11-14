#include <bits/stdc++.h>
using namespace std;

struct Operation {
    string op;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    cin >> N;
    
    vector<vector<int>> h(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> h[i][j];
        }
    }
    
    vector<Operation> operations;
    int currentLoad = 0;
    int currentRow = 0, currentCol = 0;
    
    // Simple greedy approach: visit each cell in row-major order
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // Move to target cell (i, j)
            while (currentRow != i || currentCol != j) {
                if (currentRow < i) {
                    operations.push_back({"D"});
                    currentRow++;
                } else if (currentRow > i) {
                    operations.push_back({"U"});
                    currentRow--;
                } else if (currentCol < j) {
                    operations.push_back({"R"});
                    currentCol++;
                } else if (currentCol > j) {
                    operations.push_back({"L"});
                    currentCol--;
                }
            }
            
            // Handle the current cell
            if (h[i][j] > 0) {
                // Need to remove soil (load onto truck)
                int toLoad = h[i][j];
                operations.push_back({"+" + to_string(toLoad)});
                currentLoad += toLoad;
                h[i][j] = 0;
            } else if (h[i][j] < 0) {
                // Need to add soil (unload from truck)
                int toUnload = -h[i][j];
                if (currentLoad >= toUnload) {
                    operations.push_back({"-" + to_string(toUnload)});
                    currentLoad -= toUnload;
                    h[i][j] = 0;
                }
            }
        }
    }
    
    // If we still have remaining load or unfixed cells, make another pass
    if (currentLoad > 0) {
        for (int i = 0; i < N && currentLoad > 0; i++) {
            for (int j = 0; j < N && currentLoad > 0; j++) {
                if (h[i][j] < 0) {
                    // Move to this cell
                    while (currentRow != i || currentCol != j) {
                        if (currentRow < i) {
                            operations.push_back({"D"});
                            currentRow++;
                        } else if (currentRow > i) {
                            operations.push_back({"U"});
                            currentRow--;
                        } else if (currentCol < j) {
                            operations.push_back({"R"});
                            currentCol++;
                        } else if (currentCol > j) {
                            operations.push_back({"L"});
                            currentCol--;
                        }
                    }
                    
                    int toUnload = min(currentLoad, -h[i][j]);
                    operations.push_back({"-" + to_string(toUnload)});
                    currentLoad -= toUnload;
                    h[i][j] += toUnload;
                }
            }
        }
    }
    
    for (const auto& op : operations) {
        cout << op.op << "\n";
    }
    
    return 0;
}