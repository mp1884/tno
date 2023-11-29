#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x, y;
} vec2_t;

typedef struct {
    uint8_t r, g, b;
} pixel_t;

static inline bool pixel_eq(pixel_t p1, pixel_t p2) {
    return p1.r == p2.r && p1.g == p2.g && p1.b == p2.b;
}

typedef struct {
    pixel_t color;
    vec2_t lt, size;
} rect_t;

typedef struct listnode_t {
    rect_t rect;
    struct listnode_t *next;
} listnode_t;

#define IMG_SIDE 100
#define HEADER_SIZE 54

listnode_t *rects[IMG_SIDE] = {NULL};
pixel_t img[IMG_SIDE][IMG_SIDE];
pixel_t background;

uint8_t parse_nibble() {
    int c = getchar();
    assert(c != EOF);
    c = tolower(c);
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    else
        assert(0);
}

uint8_t parse_byte() {
    return (parse_nibble() << 4) | parse_nibble();
}

void parse_img() {
    int c;
    while (1) {
        c=getchar();
        if (c == ' ' || c == '\r' || c == '\n' || c == '\t')
            continue;
        break;
    }

    fseek(stdin, HEADER_SIZE * 2 - 1, SEEK_CUR);

    /* int cnt = 0; */
    /* for (int i = 0; i < IMG_SIDE; ++i) { */
    for (int i = IMG_SIDE - 1; i >= 0; --i) {
        for (int j = 0; j < IMG_SIDE; ++j) {
            img[i][j].b = parse_byte();
            img[i][j].g = parse_byte();
            img[i][j].r = parse_byte();
            /* ++cnt; */
        }
    }

    /* return; */
}

void merge_or_add_rect(rect_t r) {
    listnode_t **n = &rects[r.lt.x];
    while (*n != NULL) {
        rect_t *nr = &(*n)->rect;
        if (nr->size.x == r.size.x && nr->lt.y + nr->size.y == r.lt.y
            && pixel_eq(nr->color, r.color)) {
            ++nr->size.y;
            return;
        }

        n = &(*n)->next;
    }

    *n = malloc(sizeof **n);
    (*n)->next = NULL;
    (*n)->rect = r;
}

void update_rects(int y) {
    pixel_t last_pixel = img[y][0];
    rect_t last_rect = {.color = last_pixel, .lt = {0, y}, .size = {0, 1}};
    for (int x = 0; x < IMG_SIDE; ++x) {
        if (pixel_eq(img[y][x], last_pixel)) {
            ++last_rect.size.x;
        } else {
            merge_or_add_rect(last_rect);
            last_pixel = img[y][x];
            last_rect = (rect_t){.color = last_pixel, .lt = {x, y}, .size = {1, 1}};
        }
    }
    merge_or_add_rect(last_rect);
}

bool pixel_not_eq(pixel_t p1, vec2_t p2_coord) {
    if (0 <= p2_coord.x && p2_coord.x < IMG_SIDE && 0 <= p2_coord.y && p2_coord.y < IMG_SIDE)
        return !pixel_eq(p1, img[p2_coord.y][p2_coord.x]);
    else
        return true;
}

bool rect_color_not_eq(rect_t r, pixel_t color) {
    for (int y = 0; y < r.size.y; ++y)
        for (int x = 0; x < r.size.x; ++x)
            if (!pixel_not_eq(color, (vec2_t){r.lt.x + x, r.lt.y + y}))
                return false;
    return true;
}

bool is_real_rect(const rect_t *r) {
    int x = r->lt.x, y = r->lt.y, sx = r->size.x, sy = r->size.y;

    if (pixel_eq(r->color, background))
        return false;

#define CHECK_RECT(_x, _y, _sx, _sy)                                                               \
    rect_color_not_eq((rect_t){.lt = {(_x), (_y)}, .size = {(_sx), (_sy)}}, r->color)

    return CHECK_RECT(x, y - 1, sx, 1) && CHECK_RECT(x - 1, y, 1, sy)
           && CHECK_RECT(x + sx, y, 1, sy) && CHECK_RECT(x, y + sy, sx, 1);
#undef CHECK_RECT
}

int main() {
    freopen(NULL, "rb", stdin);
    parse_img();
    background = img[0][0];
    for (int y = 0; y < IMG_SIDE; ++y)
        update_rects(y);

    int ans = 0;
    for (int i = 0; i < IMG_SIDE; ++i) {
        int rects_in_col = 0;
        listnode_t *cur = rects[i];
        while (cur != NULL) {
            bool ret = is_real_rect(&cur->rect);
            if (ret)
                ++rects_in_col;
            cur = cur->next;
        }
        ans += rects_in_col;
    }

    printf("%d\n", ans);
}
