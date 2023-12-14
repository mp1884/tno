#include <bits/stdc++.h>
using namespace std;

typedef array<int, 6> decomp;

vector<decomp> ds;

void m_decomps(int m) {
    for (int i1 = 0; i1 <= 100 / 1; ++i1) {
        if (i1 * 1 > m)
            break;
        for (int i2 = 0; i2 <= 100 / 2; ++i2) {
            if (i2 * 2 > m)
                break;
            for (int i5 = 0; i5 <= 100 / 5; ++i5) {
                if (i5 * 5 > m)
                    break;
                for (int i10 = 0; i10 <= 100 / 10; ++i10) {
                    if (i10 * 10 > m)
                        break;
                    for (int i20 = 0; i20 <= 100 / 20; ++i20) {
                        if (i20 * 20 > m)
                            break;
                        for (int i50 = 0; i50 <= 100 / 50; ++i50) {
                            if (i50 * 50 > m)
                                break;
                            int s1 = i1 * 1 + i2 * 2 + i5 * 5 + i10 * 10 + i20 * 20 + i50 * 50;
                            if (s1 != m)
                                continue;
                            ds.push_back({i1, i2, i5, i10, i20, i50});
                        }
                    }
                }
            }
        }
    }
}

bool check(decomp nd) {
    for (const auto md : ds) {
        bool ok = true;
        for (int i = 0; i < 6; ++i) {
            if (md[i] > nd[i]) {
                ok = false;
                break;
            }
        }
        if (ok) {
            return true;
        }
    }
    return false;
}

/* void debug() { */
/*     for (auto d : ds) { */
/*         int i1 = d[0], i2 = d[1], i5 = d[2], i10 = d[3], i20 = d[4], i50 = d[5]; */
/*         printf("debug: 1*%d + 2*%d + 5*%d + 10*%d + 20*%d + 50*%d\n", i1, i2, i5, i10, i20, i50); */
/*     } */
/*     fflush(stdout); */
/* } */

bool solve(int n, int m) {
    ds.clear();
    m_decomps(m);

    for (int i1 = 0; i1 <= 100 / 1; ++i1) {
        if (i1 * 1 > n)
            break;
        for (int i2 = 0; i2 <= 100 / 2; ++i2) {
            if (i2 * 2 > n)
                break;
            for (int i5 = 0; i5 <= 100 / 5; ++i5) {
                if (i5 * 5 > n)
                    break;
                for (int i10 = 0; i10 <= 100 / 10; ++i10) {
                    if (i10 * 10 > n)
                        break;
                    for (int i20 = 0; i20 <= 100 / 20; ++i20) {
                        if (i20 * 20 > n)
                            break;
                        for (int i50 = 0; i50 <= 100 / 50; ++i50) {
                            if (i50 * 50 > n)
                                break;
                            int s1 = i1 * 1 + i2 * 2 + i5 * 5 + i10 * 10 + i20 * 20 + i50 * 50;
                            if (s1 != n)
                                continue;

                            decomp nd = {i1, i2, i5, i10, i20, i50};
                            if (!check(nd))
                                return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        int n, m;
        scanf("%d%d", &n, &m);
        puts(solve(n, m) ? "YES" : "NO");
    }
}
