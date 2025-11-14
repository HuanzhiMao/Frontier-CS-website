#include <bits/stdc++.h>
using namespace std;

struct Bin {
  int W, H;
  bool allow_rotate;
};

struct ItemType {
  string type;
  int w, h, v, limit;
  int remaining;
};

struct FreeRect {
  int x, y, w, h;
};

struct Placement {
  string type;
  int x, y, rot;
};

pair<long long, vector<Placement>> pack(const vector<int>& type_order, const Bin& bin, vector<ItemType> types) {
  for (auto& t : types) t.remaining = t.limit;
  vector<FreeRect> free_rects = {{0, 0, bin.W, bin.H}};
  vector<Placement> placements;
  long long total_profit = 0;
  for (int idx : type_order) {
    ItemType& t = types[idx];
    while (t.remaining > 0) {
      int best_i = -1;
      int best_rot = 0;
      int best_waste = INT_MAX;
      int best_y = INT_MAX;
      int best_x = INT_MAX;
      for (int i = 0; i < free_rects.size(); i++) {
        auto& fr = free_rects[i];
        // rot 0
        {
          int ww = t.w, hh = t.h;
          if (fr.w >= ww && fr.h >= hh) {
            int waste = (fr.w - ww) + (fr.h - hh);
            int cy = fr.y;
            int cx = fr.x;
            bool better = false;
            if (waste < best_waste) better = true;
            else if (waste == best_waste && cy < best_y) better = true;
            else if (waste == best_waste && cy == best_y && cx < best_x) better = true;
            if (better) {
              best_waste = waste;
              best_y = cy;
              best_x = cx;
              best_i = i;
              best_rot = 0;
            }
          }
        }
        // rot 1
        if (bin.allow_rotate) {
          int ww = t.h, hh = t.w;
          if (fr.w >= ww && fr.h >= hh) {
            int waste = (fr.w - ww) + (fr.h - hh);
            int cy = fr.y;
            int cx = fr.x;
            bool better = false;
            if (waste < best_waste) better = true;
            else if (waste == best_waste && cy < best_y) better = true;
            else if (waste == best_waste && cy == best_y && cx < best_x) better = true;
            if (better) {
              best_waste = waste;
              best_y = cy;
              best_x = cx;
              best_i = i;
              best_rot = 1;
            }
          }
        }
      }
      if (best_i == -1) break;
      // place
      auto& fr = free_rects[best_i];
      int ww = (best_rot ? t.h : t.w);
      int hh = (best_rot ? t.w : t.h);
      int px = fr.x;
      int py = fr.y;
      placements.push_back({t.type, px, py, best_rot});
      total_profit += t.v;
      // split
      vector<FreeRect> new_free;
      if (fr.w > ww && hh > 0) {
        new_free.push_back({fr.x + ww, fr.y, fr.w - ww, hh});
      }
      if (fr.h > hh && fr.w > 0) {
        new_free.push_back({fr.x, fr.y + hh, fr.w, fr.h - hh});
      }
      // erase
      free_rects[best_i] = free_rects.back();
      free_rects.pop_back();
      // add new
      free_rects.insert(free_rects.end(), new_free.begin(), new_free.end());
      t.remaining--;
    }
  }
  return {total_profit, placements};
}

int main() {
  string full_input;
  string line;
  while (getline(cin, line)) {
    full_input += line;
  }
  Bin bin;
  vector<ItemType> types;
  size_t p = 0;
  p = full_input.find("\"bin\":{");
  if (p != string::npos) {
    p += 7;
    size_t q = full_input.find("\"W\":", p);
    if (q != string::npos) {
      q += 4;
      while (isspace(full_input[q])) q++;
      size_t start = q;
      while (isdigit(full_input[q])) q++;
      bin.W = stoi(full_input.substr(start, q - start));
    }
    q = full_input.find("\"H\":", p);
    if (q != string::npos) {
      q += 4;
      while (isspace(full_input[q])) q++;
      size_t start = q;
      while (isdigit(full_input[q])) q++;
      bin.H = stoi(full_input.substr(start, q - start));
    }
    q = full_input.find("\"allow_rotate\":", p);
    if (q != string::npos) {
      q += 15;
      while (isspace(full_input[q])) q++;
      string ar_str;
      while (full_input[q] != ',' && full_input[q] != '}' && !isspace(full_input[q])) {
        ar_str += full_input[q++];
      }
      bin.allow_rotate = (ar_str == "true");
    }
  }
  p = full_input.find("\"items\":[");
  if (p != string::npos) {
    p += 9;
    while (true) {
      p = full_input.find("{", p);
      if (p == string::npos) break;
      ItemType it;
      size_t q = full_input.find("\"type\":", p);
      if (q != string::npos) {
        q += 7;
        while (isspace(full_input[q])) q++;
        if (full_input[q] == '"') q++;
        size_t start = q;
        while (full_input[q] != '"') q++;
        it.type = full_input.substr(start, q - start);
        q++;
      }
      q = full_input.find("\"w\":", p);
      if (q != string::npos) {
        q += 4;
        while (isspace(full_input[q])) q++;
        size_t start = q;
        while (isdigit(full_input[q])) q++;
        it.w = stoi(full_input.substr(start, q - start));
      }
      q = full_input.find("\"h\":", p);
      if (q != string::npos) {
        q += 4;
        while (isspace(full_input[q])) q++;
        size_t start = q;
        while (isdigit(full_input[q])) q++;
        it.h = stoi(full_input.substr(start, q - start));
      }
      q = full_input.find("\"v\":", p);
      if (q != string::npos) {
        q += 4;
        while (isspace(full_input[q])) q++;
        size_t start = q;
        while (isdigit(full_input[q])) q++;
        it.v = stoi(full_input.substr(start, q - start));
      }
      q = full_input.find("\"limit\":", p);
      if (q != string::npos) {
        q += 8;
        while (isspace(full_input[q])) q++;
        size_t start = q;
        while (isdigit(full_input[q])) q++;
        it.limit = stoi(full_input.substr(start, q - start));
      }
      types.push_back(it);
      p = full_input.find("}", p);
      if (p != string::npos) p++;
      if (full_input[p] == ']') break;
    }
  }
  vector<vector<int>> orders;
  // density
  {
    vector<pair<long double, int>> sorter;
    for (int i = 0; i < types.size(); i++) {
      long double dens = types[i].v / (long double)(types[i].w * types[i].h);
      sorter.emplace_back(-dens, i);
    }
    sort(sorter.begin(), sorter.end());
    vector<int> ord;
    for (auto& pr : sorter) ord.push_back(pr.second);
    orders.push_back(ord);
  }
  // height
  {
    vector<pair<int, int>> sorter;
    for (int i = 0; i < types.size(); i++) {
      sorter.emplace_back(-types[i].h, i);
    }
    sort(sorter.begin(), sorter.end());
    vector<int> ord;
    for (auto& pr : sorter) ord.push_back(pr.second);
    orders.push_back(ord);
  }
  // area
  {
    vector<pair<long long, int>> sorter;
    for (int i = 0; i < types.size(); i++) {
      long long area = (long long)types[i].w * types[i].h;
      sorter.emplace_back(-area, i);
    }
    sort(sorter.begin(), sorter.end());
    vector<int> ord;
    for (auto& pr : sorter) ord.push_back(pr.second);
    orders.push_back(ord);
  }
  // value
  {
    vector<pair<int, int>> sorter;
    for (int i = 0; i < types.size(); i++) {
      sorter.emplace_back(-types[i].v, i);
    }
    sort(sorter.begin(), sorter.end());
    vector<int> ord;
    for (auto& pr : sorter) ord.push_back(pr.second);
    orders.push_back(ord);
  }
  long long max_profit = -1;
  vector<Placement> best_plac;
  for (auto& ord : orders) {
    auto [prof, plac] = pack(ord, bin, types);
    if (prof > max_profit) {
      max_profit = prof;
      best_plac = plac;
    }
  }
  cout << "{" << endl;
  cout << "  \"placements\": [" << endl;
  for (size_t i = 0; i < best_plac.size(); i++) {
    auto& pl = best_plac[i];
    cout << "    {\"type\":\"" << pl.type << "\",\"x\":" << pl.x << ",\"y\":" << pl.y << ",\"rot\":" << pl.rot << "}";
    if (i < best_plac.size() - 1) cout << ",";
    cout << endl;
  }
  cout << "  ]" << endl;
  cout << "}" << endl;
  return 0;
}