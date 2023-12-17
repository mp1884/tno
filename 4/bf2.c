#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#define MAXN 1000

int house_x[MAXN];
int house_h[MAXN];

int hss_n;

int destroy(int h_i, bool to_right, bool * is_dstr) {
    int x = house_x[h_i];
    int h = house_h[h_i];
    is_dstr[h_i] = 1;

    if(to_right){
        for (int i=h_i+1; i<hss_n && house_x[i] <= x+h; i++) {
            if (is_dstr[i]) continue;
            destroy(i, to_right, is_dstr);
        }
    } else {
        for (int i=h_i-1; i>=0 && house_x[i] >= x-h; i--) {
            if (is_dstr[i]) continue;
            destroy(i, to_right, is_dstr);
        }
    }
}

int min_destroys = INT_MAX;

void tree(const bool * is_dstr, int step) {
    bool * is_dstr_buf = malloc(sizeof(bool)*hss_n); 

    

    bool is_empty = 1;
    for(int i=0;i<hss_n;i++) {
        if(is_dstr[i]) continue;
        is_empty = 0;

        memcpy(is_dstr_buf, is_dstr, sizeof(bool)*hss_n);
        destroy(i, 1, is_dstr_buf);
        tree(is_dstr_buf, step+1);

        memcpy(is_dstr_buf, is_dstr, sizeof(bool)*hss_n);
        destroy(i, 0, is_dstr_buf);
        tree(is_dstr_buf, step+1);

    }
    if (is_empty && step < min_destroys) min_destroys = step;

    free(is_dstr_buf);
}

int main() {
    scanf("%d", &hss_n);
    

    for(int i=0;i<hss_n;i++) {
        scanf("%d", house_x+i);
    }
    for(int i=0;i<hss_n;i++) {
        scanf("%d", house_h+i);
    }

    bool * is_dstr_buf = calloc(hss_n, sizeof(bool));

    tree(is_dstr_buf, 0);

    printf("%d\n", min_destroys);

    free(is_dstr_buf);
}
