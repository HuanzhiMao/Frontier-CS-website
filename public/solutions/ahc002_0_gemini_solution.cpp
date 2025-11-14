struct TileInfo {
      int id;
      long long score;
      vector<pair<int, int>> squares;
      vector<int> adj_tiles;
  };
  vector<TileInfo> tiles_info;