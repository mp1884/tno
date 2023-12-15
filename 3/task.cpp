#include <bits/stdc++.h>
using namespace std;

typedef array<int, 6> decomp;

vector<decomp> ds;

#define FOR(nom, num) for (int i##nom = 0; i##nom <= (num) / nom; ++i##nom) {
#define ENDFOR                                                                                     \
    }                                                                                              \
    }                                                                                              \
    }                                                                                              \
    }                                                                                              \
    }                                                                                              \
    }

void m_decomps(int m) {
    FOR(1, m)
    FOR(2, m)
    FOR(5, m)
    FOR(10, m)
    FOR(20, m)
    FOR(50, m)
    int s1 = i1 * 1 + i2 * 2 + i5 * 5 + i10 * 10 + i20 * 20 + i50 * 50;
    if (s1 != m)
        continue;
    ds.push_back({i1, i2, i5, i10, i20, i50});
    ENDFOR
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

bool solve(int n, int m) {
    ds.clear();
    m_decomps(m);

    FOR(1, n)
    FOR(2, n)
    FOR(5, n)
    FOR(10, n)
    FOR(20, n)
    FOR(50, n)
    int s1 = i1 * 1 + i2 * 2 + i5 * 5 + i10 * 10 + i20 * 20 + i50 * 50;
    if (s1 != n)
        continue;

    decomp nd = {i1, i2, i5, i10, i20, i50};
    if (!check(nd))
        return false;
    ENDFOR

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
