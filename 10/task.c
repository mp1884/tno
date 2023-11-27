#include <assert.h>
#include <stdbool.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

bool is_pending_hum50 = false;
int hum, t = 0;

int get_pow_quarters() {
    if (hum <= 39)
        return 0;
    if (hum <= 59)
        return 1;
    if (hum <= 69)
        return 2;
    if (hum <= 79)
        return 3;
    if (hum <= 100)
        return 4;

    assert(0);
}

int main() {
    hum = 90;
    do {
        if (is_pending_hum50 && hum < 50)
            continue;

        if (hum <= 39) {
            is_pending_hum50 = true;
            continue;
        }

        int q = get_pow_quarters();

        if (t >= 22 || t <= 9)
            q = MAX(q, 2);


    } while (t != 24);
}
