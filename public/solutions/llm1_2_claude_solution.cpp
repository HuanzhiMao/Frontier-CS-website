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
        size_t start = pos + 1;
        size_t end = input.find("\"", start);
        if (end == string::npos) break;
        
        string name = input.substr(start, end - start);
        
        // Find the array
        size_t bracket_start = input.find("[", end);
        if (bracket_start == string::npos) break;
        
        size_t bracket_end = input.find("]", bracket_start);
        if (bracket_end == string::npos) break;
        
        string array_content = input.substr(bracket_start + 1, bracket_end - bracket_start - 1);
        
        // Parse numbers
        vector<long long> numbers;
        stringstream ss(array_content);
        string token;
        while (getline(ss, token, ',')) {
            // Remove whitespace
            token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
            if (!token.empty()) {
                numbers.push_back(stoll(token));
            }
        }
        
        if (numbers.size() == 4) {
            Item item;
            item.name = name;
            item.quantity = numbers[0];
            item.value = numbers[1];
            item.mass = numbers[2];
            item.volume = numbers[3];
            item.ratio = (double)item.value / (double)(item.mass + item.volume);
            items.push_back(item);
        }
        
        pos = bracket_end;
    }
    
    // Sort by value density (value per unit of constraint)
    sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return a.ratio > b.ratio;
    });
    
    long long max_mass = 20000000;  // 20 kg in mg
    long long max_volume = 25000000; // 25 liters in µliters
    
    map<string, int> result;
    
    // Initialize all to 0
    for (const auto& item : items) {
        result[item.name] = 0;
    }
    
    // Greedy approach with some optimization
    long long current_mass = 0;
    long long current_volume = 0;
    
    // Try multiple strategies and pick the best
    long long best_value = 0;
    map<string, int> best_result = result;
    
    // Strategy 1: Pure greedy by ratio
    for (const auto& item : items) {
        int can_take = item.quantity;
        can_take = min(can_take, (int)((max_mass - current_mass) / item.mass));
        can_take = min(can_take, (int)((max_volume - current_volume) / item.volume));
        
        if (can_take > 0) {
            result[item.name] = can_take;
            current_mass += can_take * item.mass;
            current_volume += can_take * item.volume;
        }
    }
    
    // Calculate value
    long long total_value = 0;
    for (const auto& item : items) {
        total_value += result[item.name] * item.value;
    }
    
    if (total_value > best_value) {
        best_value = total_value;
        best_result = result;
    }
    
    // Strategy 2: Try different orderings
    vector<Item> items_by_value = items;
    sort(items_by_value.begin(), items_by_value.end(), [](const Item& a, const Item& b) {
        return a.value > b.value;
    });
    
    result.clear();
    for (const auto& item : items) {
        result[item.name] = 0;
    }
    current_mass = 0;
    current_volume = 0;
    
    for (const auto& item : items_by_value) {
        int can_take = item.quantity;
        can_take = min(can_take, (int)((max_mass - current_mass) / item.mass));
        can_take = min(can_take, (int)((max_volume - current_volume) / item.volume));
        
        if (can_take > 0) {
            result[item.name] = can_take;
            current_mass += can_take * item.mass;
            current_volume += can_take * item.volume;
        }
    }
    
    total_value = 0;
    for (const auto& item : items) {
        total_value += result[item.name] * item.value;
    }
    
    if (total_value > best_value) {
        best_value = total_value;
        best_result = result;
    }
    
    // Output
    cout << "{\n";
    bool first = true;
    for (const auto& item : items) {
        if (!first) cout << ",\n";
        cout << " \"" << item.name << "\": " << best_result[item.name];
        first = false;
    }
    cout << "\n}\n";
    
    return 0;
}