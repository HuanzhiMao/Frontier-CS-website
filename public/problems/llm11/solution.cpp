#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
#define N 30 + 5

constexpr int Fx[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
constexpr char Char[] = "RDLU";
int n, m, sr, sc, er, ec, one_cnt, vis_cnt;
char s[N][N];
bool Vis[N][N];
string ret;
vector<int> ending_path;

inline bool Check(int x, int y, int dir)
{
	int tx = x + Fx[dir][0], ty = y + Fx[dir][1];
	return 1 <= tx && tx <= n && 1 <= ty && ty <= m && s[tx][ty] == '1';
}

void Go(int x, int y, int dir)
{
	int rx = er, ry = ec, rdir = dir ^ 2;
	int step = 0;
	for (; Check(x, y, rdir) && Check(rx, ry, dir); step ++)
	{
		x += Fx[rdir][0], y += Fx[rdir][1];
		rx += Fx[dir][0], ry += Fx[dir][1];
		ret.push_back(Char[rdir]);
	}
	if (!Check(x, y, rdir) && Check(rx, ry, dir))
		ret.push_back(Char[rdir]);
	for (int i = 1; i <= step + 1; i++)
		ret.push_back(Char[dir]);
}

void dfs(int x, int y)
{
	vis_cnt++;
	Vis[x][y] = true;
	for (int k = 0; k < 4; k ++)
	{
		int tx = x + Fx[k][0], ty = y + Fx[k][1];
		if (Check(x, y, k) && !Vis[tx][ty])
		{
			Go(x, y, k);
			if (!Vis[er][ec])
				ending_path.push_back(k);
			dfs(tx, ty);
			Go(tx, ty, k ^ 2);
			if (!Vis[er][ec])
				ending_path.pop_back();
		}
	}
}

int main()
{
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; i++)
	{
		scanf("%s", s[i] + 1);
		for (int j = 1; j <= m; j++)
			one_cnt += s[i][j] - '0';
	}
	scanf("%d%d%d%d", &sr, &sc, &er, &ec);
	dfs(sr, sc);
	if (vis_cnt != one_cnt)
		puts("-1");
	else
	{
		for (int d : ending_path)
		{
			Go(sr, sc, d);
			sr += Fx[d][0];
			sc += Fx[d][1];
		}
		printf("%s", ret.c_str());
		reverse(ret.begin(), ret.end());
		printf("%s\n", ret.c_str());
	}
	return 0;
}

