// interactor.cpp
// Build: g++ -std=gnu++17 -O2 -pipe -static -s interactor.cpp -o interactor
// Run  : interactor <input> <output> <answer> [--seed=...]
// input: 一行“n s”
// 协议：
//   - 选手输出：walk x (0 <= x <= 1e9) —— 交互器回：label[pos]
//   - 选手输出：guess g —— 结束；若 g==n 则按 q 计分，否则得 0
//
// 规则（按你的要求）：
//   - 初始 label[s]=s，已用数集合 used={s}
//   - 当 walk 到某点 pos 且未分配过值：重复随机 r∈[1..n]，直到 r∉used，然后 label[pos]=r, used 加入 r
//   - walk 次数上限 MAX_Q=200000
//
// 评分：f(q) 为 log10 空间分段线性插值：
//   (1,100), (10000,95), (20000,60), (50000,30)，并在 200000 处外推到 0
//   正确时 quitp(f/100)，错误时 quitp(0)

#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

static const long long MAX_Q = 200000LL;

// 连续、单调递减、log10 分段线性插值
static double score_from_queries(long long q) {
    if (q <= 1) return 100.0;

    const double t  = log10((double)q);
    const double t1 = 0.0;                   // log10(1)
    const double t2 = 4.0;                   // log10(10000)
    const double t3 = log10(20000.0);        // ~4.30103
    const double t4 = log10(50000.0);        // ~4.69897
    const double t5 = log10(200000.0);       // ~5.30103

    auto lerp = [](double a, double b, double x, double xa, double xb) {
        if (xa == xb) return a;
        double w = (x - xa) / (xb - xa);
        return a + (b - a) * w;
    };

    if (t <= t2)          return lerp(100.0, 95.0, t, t1, t2);
    else if (t <= t3)     return lerp(95.0,  60.0, t, t2, t3);
    else if (t <= t4)     return lerp(60.0,  30.0, t, t3, t4);
    else if (t <= t5)     return max(0.0, lerp(30.0,   0.0, t, t4, t5));
    else                  return 0.0;
}

int main(int argc, char* argv[]) {
    registerInteraction(argc, argv);

    // 读入 n, s
    long long n = inf.readLong(1LL, 1000000000LL, "n");
    long long s = inf.readLong(1LL, n,            "s");

    long long pos  = s;
    long long qcnt = 0;

    // 节点 -> 可见数值；只在第一次到达节点时分配
    unordered_map<long long, long long> label;
    label.reserve(1 << 16);
    label.max_load_factor(0.7f);

    // 已用数集合，避免重复分配
    unordered_set<long long> used;
    used.reserve(1 << 16);
    used.max_load_factor(0.7f);

    // 初始化
    label[s] = s;
    used.insert(s);

    // 抽取一个未使用的随机数（你保证 n 很大，抽中未用数期望次数很低）
    auto take_random_unused_number = [&]() -> long long {
        while (true) {
            long long r = rnd.next(1LL, n);
            if (!used.count(r)) {
                used.insert(r);
                return r;
            }
        }
    };

    auto tell = [&](long long v) {
        cout << v << '\n' << flush;
    };

    while (true) {
        string cmd = ouf.readWord();  // 读一个由非空白字符组成的词


        if (cmd == "walk") {
            long long x = ouf.readLong(0LL, 1000000000LL, "x");
            // cerr<<x<<"\n";
            long long step = (n == 1 ? 0 : (x % n));
            pos = ((pos - 1 + step) % n) + 1;

            if (!label.count(pos)) {
                long long val = take_random_unused_number();
                label[pos] = val;
            }

            ++qcnt;
            if (qcnt > MAX_Q) {
                quitf(_wa, "Too many queries: %lld (max %lld)", qcnt, MAX_Q);
            }

            tell(label[pos]);

        } else if (cmd == "guess") {
            long long g = ouf.readLong(1LL, 1000000000LL, "n_guess");
            bool ok = (g == n);

            if (!ok) {
                quitp(0.0, "Wrong answer. hidden n=%lld, your guess=%lld, queries=%lld", n, g, qcnt);
            } else {
                double score = score_from_queries(qcnt);
                double part  = max(0.0, min(100.0, score)) / 100.0;
                quitp(part, "OK. n=%lld, queries=%lld, Ratio: %.4f", n, qcnt, part);
            }

        } else {
            quitf(_wa, "Unknown command: '%s' (expected 'walk' or 'guess')", cmd.c_str());
        }
    }
}
