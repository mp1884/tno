#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

int c;
bool eof = false;

#define MAX(a, b) ((a) < (b) ? (b) : (a))

void next() {
    c = getchar();
    if (c == EOF)
        eof = true;
}

void skipws() {
    while (c == ' ' || c == '\t' || c == '\r')
        next();
}

bool newline() {
    if (c != '\n')
        return false;

    next();
    return true;
}

bool number() {
    if (c != '-' && !isdigit(c))
        return false;

    while (c == '-' || isdigit(c))
        next();

    return true;
}

int main() {
    int linecnt = 0, numcnt1 = 0, numcnt2 = 0;
    next();
    while (1) {
        skipws();

        if (eof)
            break;

        if (newline()) {
            if (numcnt2 != 0) {
                ++linecnt;
                numcnt1 = MAX(numcnt1, numcnt2);
                numcnt2 = 0;
            }
        } else if (number()) {
            ++numcnt2;
        } else {
            // TLE.
            while (1) {
            }
        }
    }

    if (numcnt2 != 0) {
        ++linecnt;
        numcnt1 = MAX(numcnt1, numcnt2);
    }

    printf("%d %d\n", linecnt, numcnt1);
}
