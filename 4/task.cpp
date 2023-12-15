#include <bits/stdc++.h>
using namespace std;

#define MAXN 1000

int n;
int xs[MAXN], hs[MAXN], lfall[MAXN], rfall[MAXN], dpm[MAXN];
vector<int> dp[MAXN];

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", xs + i);
    }
    for (int i = 0; i < n; ++i) {
        scanf("%d", hs + i);
    }

    fill(lfall, lfall + n, -1);
    fill(rfall, rfall + n, -1);
    for (int i = 0; i < n; ++i) {
        auto lit = lower_bound(xs, xs + n, xs[i] - hs[i]);
        if (lit == xs + n)
            continue;
        int l = distance(xs, lit);
        if (l == i)
            continue;

        int minl = l;
        for (int j = i - 1; j >= l; --j)
            if (lfall[j] != -1)
                minl = min(minl, lfall[j]);

        lfall[i] = minl;
    }
    for (int i = n - 1; i >= 0; --i) {
        auto rit = upper_bound(xs, xs + n, xs[i] + hs[i]) - 1;
        int r = distance(xs, rit);
        if (r == i)
            continue;

        int maxr = r;
        for (int j = i + 1; j <= r; ++j)
            if (rfall[j] != -1)
                maxr = max(maxr, rfall[j]);

        rfall[i] = maxr;
    }

    for (int i = 0; i < n; ++i) {
        int prev = (i == 0) ? 0 : dpm[i - 1];
        dpm[i] = prev + 1;
        int l = lfall[i], r = rfall[i];
        if (l != -1) {
            if (l == 0)
                dpm[i] = 1;
            else
                dpm[i] = min(dpm[i], dpm[l - 1] + 1);
        }

        for (const auto v : dp[i])
            dpm[i] = min(dpm[i], v);

        if (r != -1) {
            dp[r].push_back(prev + 1);
        }
    }

    printf("%d\n", dpm[n - 1]);
}
