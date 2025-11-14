#include <bits/stdc++.h>
using namespace std;

struct Item {
    string name;
    int quantity;
    long long value;
    long long mass;
    long long volume;
    double ratio;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    string line;
    string input = "";
    while (getline(cin, line)) {
        input += line;
    }
    
    vector<Item> items;
    
    // Parse JSON manually
    size_t pos = 0;
    while ((pos = input.find("\"", pos)) != string::npos) {
        pos++;
        size_t end = input.find("\"", pos);
        if (end == string::npos) break;
        
        string name = input.substr(pos, end - pos);
        pos = end + 1;
        
        // Find the array
        pos = input.find("[", pos);
        if (pos == string::npos) break;
        pos++;
        
        vector<long long> values;
        string numStr = "";
        bool inNumber = false;
        
        while (pos < input.length() && input[pos] != ']') {
            if (isdigit(input[pos])) {
                numStr += input[pos];
                inNumber = true;
            } else if (inNumber) {
                values.push_back(stoll(numStr));
                numStr = "";
                inNumber = false;
            }
            pos++;
        }
        if (!numStr.empty()) {
            values.push_back(stoll(numStr));
        }
        
        if (values.size() == 4) {
            Item item;
            item.name = name;
            item.quantity = values[0];
            item.value = values[1];
            item.mass = values[2];
            item.volume = values[3];
            item.ratio = (double)item.value / max(item.mass / 1000000.0 + item.volume / 1000000.0, 1.0);
            items.push_back(item);
        }
        pos++;
    }
    
    // Greedy approach with multiple strategies
    map<string, int> bestSolution;
    long long bestValue = 0;
    
    // Strategy 1: Value per mass ratio
    vector<Item> sorted1 = items;
    sort(sorted1.begin(), sorted1.end(), [](const Item& a, const Item& b) {
        return (double)a.value / a.mass > (double)b.value / b.mass;
    });
    
    map<string, int> solution1;
    long long totalMass = 0, totalVolume = 0, totalValue = 0;
    
    for (const auto& item : sorted1) {
        solution1[item.name] = 0;
        for (int i = 0; i < item.quantity; i++) {
            if (totalMass + item.mass <= 20000000 && totalVolume + item.volume <= 25000000) {
                totalMass += item.mass;
                totalVolume += item.volume;
                totalValue += item.value;
                solution1[item.name]++;
            } else break;
        }
    }
    
    if (totalValue > bestValue) {
        bestValue = totalValue;
        bestSolution = solution1;
    }
    
    // Strategy 2: Value per volume ratio
    vector<Item> sorted2 = items;
    sort(sorted2.begin(), sorted2.end(), [](const Item& a, const Item& b) {
        return (double)a.value / a.volume > (double)b.value / b.volume;
    });
    
    map<string, int> solution2;
    totalMass = totalVolume = totalValue = 0;
    
    for (const auto& item : sorted2) {
        solution2[item.name] = 0;
        for (int i = 0; i < item.quantity; i++) {
            if (totalMass + item.mass <= 20000000 && totalVolume + item.volume <= 25000000) {
                totalMass += item.mass;
                totalVolume += item.volume;
                totalValue += item.value;
                solution2[item.name]++;
            } else break;
        }
    }
    
    if (totalValue > bestValue) {
        bestValue = totalValue;
        bestSolution = solution2;
    }
    
    // Strategy 3: Value per combined constraint
    vector<Item> sorted3 = items;
    sort(sorted3.begin(), sorted3.end(), [](const Item& a, const Item& b) {
        double ratioA = (double)a.value / (a.mass / 20000000.0 + a.volume / 25000000.0);
        double ratioB = (double)b.value / (b.mass / 20000000.0 + b.volume / 25000000.0);
        return ratioA > ratioB;
    });
    
    map<string, int> solution3;
    totalMass = totalVolume = totalValue = 0;
    
    for (const auto& item : sorted3) {
        solution3[item.name] = 0;
        for (int i = 0; i < item.quantity; i++) {
            if (totalMass + item.mass <= 20000000 && totalVolume + item.volume <= 25000000) {
                totalMass += item.mass;
                totalVolume += item.volume;
                totalValue += item.value;
                solution3[item.name]++;
            } else break;
        }
    }
    
    if (totalValue > bestValue) {
        bestValue = totalValue;
        bestSolution = solution3;
    }
    
    cout << "{" << endl;
    bool first = true;
    for (const auto& item : items) {
        if (!first) cout << "," << endl;
        cout << " \"" << item.name << "\": " << bestSolution[item.name];
        first = false;
    }
    cout << endl << "}" << endl;
    
    return 0;
}