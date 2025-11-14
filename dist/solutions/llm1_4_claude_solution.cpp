#include <bits/stdc++.h>
using namespace std;

struct Item {
    string name;
    int quantity;
    long long value;
    long long mass;
    long long volume;
    double efficiency;
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
        size_t nameStart = pos + 1;
        size_t nameEnd = input.find("\"", nameStart);
        if (nameEnd == string::npos) break;
        
        string name = input.substr(nameStart, nameEnd - nameStart);
        
        size_t arrayStart = input.find("[", nameEnd);
        if (arrayStart == string::npos) break;
        
        size_t arrayEnd = input.find("]", arrayStart);
        if (arrayEnd == string::npos) break;
        
        string arrayContent = input.substr(arrayStart + 1, arrayEnd - arrayStart - 1);
        
        vector<long long> values;
        stringstream ss(arrayContent);
        string token;
        while (getline(ss, token, ',')) {
            token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
            if (!token.empty()) {
                values.push_back(stoll(token));
            }
        }
        
        if (values.size() == 4) {
            Item item;
            item.name = name;
            item.quantity = values[0];
            item.value = values[1];
            item.mass = values[2];
            item.volume = values[3];
            item.efficiency = (double)item.value / max(item.mass / 20000.0, item.volume / 25000.0);
            items.push_back(item);
        }
        
        pos = arrayEnd;
    }
    
    // Sort by efficiency
    sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return a.efficiency > b.efficiency;
    });
    
    map<string, int> result;
    for (const auto& item : items) {
        result[item.name] = 0;
    }
    
    long long totalMass = 0;
    long long totalVolume = 0;
    const long long MAX_MASS = 20000000; // 20 kg in mg
    const long long MAX_VOLUME = 25000000; // 25 liters in µliters
    
    // Greedy approach with some optimization
    for (int iter = 0; iter < 3; iter++) {
        for (auto& item : items) {
            int currentTaken = result[item.name];
            if (currentTaken >= item.quantity) continue;
            
            // Try to add more items of this type
            for (int add = 1; add <= item.quantity - currentTaken; add++) {
                long long newMass = totalMass + add * item.mass;
                long long newVolume = totalVolume + add * item.volume;
                
                if (newMass <= MAX_MASS && newVolume <= MAX_VOLUME) {
                    result[item.name] = currentTaken + add;
                    totalMass = newMass;
                    totalVolume = newVolume;
                } else {
                    break;
                }
            }
        }
    }
    
    // Try local optimization
    for (int opt = 0; opt < 100; opt++) {
        bool improved = false;
        for (int i = 0; i < items.size() && !improved; i++) {
            for (int j = 0; j < items.size() && !improved; j++) {
                if (i == j) continue;
                
                if (result[items[i].name] > 0 && result[items[j].name] < items[j].quantity) {
                    // Try swapping one item from i to j
                    long long massChange = -items[i].mass + items[j].mass;
                    long long volumeChange = -items[i].volume + items[j].volume;
                    long long valueChange = -items[i].value + items[j].value;
                    
                    if (totalMass + massChange <= MAX_MASS && 
                        totalVolume + volumeChange <= MAX_VOLUME &&
                        valueChange > 0) {
                        result[items[i].name]--;
                        result[items[j].name]++;
                        totalMass += massChange;
                        totalVolume += volumeChange;
                        improved = true;
                    }
                }
            }
        }
        if (!improved) break;
    }
    
    cout << "{\n";
    bool first = true;
    for (const auto& item : items) {
        if (!first) cout << ",\n";
        cout << " \"" << item.name << "\": " << result[item.name];
        first = false;
    }
    cout << "\n}\n";
    
    return 0;
}