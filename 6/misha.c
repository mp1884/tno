#include <math.h>
#include <spng.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    int x, y;
} vec2_t;

typedef struct {
    uint8_t r, g, b;
} color_t;

typedef enum { OK, CAN_REROLL, OVERLAP } status_t;

bool color_eq(color_t c1, color_t c2) {
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

#define IMG_SIDE 100

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

const int MAX_RECTS_ATTEMPTS = 300;
const int MAX_CIRCLES_ATTEMPTS = 30;
/* const int MAX_RECTS_ATTEMPTS = 0; */
/* const int MAX_CIRCLES_ATTEMPTS = 0; */
const int MAX_TRIANGLES_ATTEMPTS = 3000;
const int MAX_RECT_SIZE = 98;
const int MAX_CIRCLE_SIZE = 98;
const int MAX_TRIANGLE_SIZE = 98;

color_t img[IMG_SIDE][IMG_SIDE], img_tmp[IMG_SIDE][IMG_SIDE];
color_t background_color;

void copy_img_buf(void *dest, void *src) {
    memcpy(dest, src, sizeof img);
}

void write_png(FILE *f) {
    spng_ctx *ctx = spng_ctx_new(SPNG_CTX_ENCODER);
    spng_set_png_file(ctx, f);
    spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);

    struct spng_ihdr ihdr = {0};
    ihdr.width = ihdr.height = IMG_SIDE;
    ihdr.color_type = SPNG_COLOR_TYPE_TRUECOLOR;
    ihdr.bit_depth = 8;
    spng_set_ihdr(ctx, &ihdr);

    int ret = spng_encode_image(ctx, img, sizeof img, SPNG_FMT_PNG, SPNG_ENCODE_FINALIZE);
    if (ret) {
        fprintf(stderr, "spng_encode_image() error: %s\n", spng_strerror(ret));
        exit(1);
    }

    spng_ctx_free(ctx);
}

int rand_range(int a, int b) {
    return a + (rand() % (b - a + 1));
}

color_t rand_color() {
    return (color_t){rand_range(0, 255), rand_range(0, 255), rand_range(0, 255)};
}

color_t rand_fig_color() {
    color_t c;
    while (color_eq(c = rand_color(), background_color))
        ;
    return c;
}

void draw_pixel(vec2_t coord, color_t col) {
    img[coord.y][coord.x] = col;
}

bool test_pixel_is_free(vec2_t coord) {
    return color_eq(img[coord.y][coord.x], background_color);
}

bool try_draw_pixel(vec2_t coord, color_t col) {
    if (!test_pixel_is_free(coord))
        return false;

    draw_pixel(coord, col);
    return true;
}

bool test_rect_is_free(vec2_t lt, vec2_t size) {
    for (int x = 0; x < size.x; ++x)
        for (int y = 0; y < size.y; ++y)
            if (!test_pixel_is_free((vec2_t){lt.x + x, lt.y + y}))
                return false;
    return true;
}

bool rand_rect(vec2_t *lt, vec2_t *size) {
    *lt = (vec2_t){rand_range(1, IMG_SIDE - 2), rand_range(1, IMG_SIDE - 2)};
    *size = (vec2_t){rand_range(1, MIN(MAX_RECT_SIZE, IMG_SIDE - lt->x - 1)),
                     rand_range(1, MIN(MAX_RECT_SIZE, IMG_SIDE - lt->x - 1))};
    if (size->x <= 0 || size->y <= 0)
        return false;

    return true;
}

status_t draw_rand_rect() {
    color_t c = rand_fig_color();
    vec2_t lt, size;

    if (!rand_rect(&lt, &size))
        return CAN_REROLL;

    if (!(test_rect_is_free((vec2_t){lt.x, lt.y - 1}, (vec2_t){size.x, 1})
          && test_rect_is_free((vec2_t){lt.x - 1, lt.y}, (vec2_t){1, size.y})
          && test_rect_is_free((vec2_t){lt.x + size.x, lt.y}, (vec2_t){1, size.y})
          && test_rect_is_free((vec2_t){lt.x, lt.y + size.y}, (vec2_t){size.x, 1})))
        return OVERLAP;

    for (int x = 0; x < size.x; ++x)
        for (int y = 0; y < size.y; ++y)
            if (!try_draw_pixel((vec2_t){lt.x + x, lt.y + y}, c))
                return OVERLAP;
    return OK;
}

bool try_draw_circ_line(int x, int y1, int y2, int prev_y1, int prev_y2, color_t col) {
    vec2_t lt1, s1, lt2, s2;
    s1 = (vec2_t){1, abs(y1 - prev_y1)}, s2 = (vec2_t){1, abs(y2 - prev_y2)};
    lt1.y = y1, lt2.y = y2;

    if (prev_y1 >= y1)
        lt1.x = x - 1;
    else
        lt1.x = x;

    if (prev_y2 <= y2)
        lt2.x = x - 1;
    else
        lt2.x = x;

    if (!(test_rect_is_free(lt1, s1) && test_rect_is_free(lt2, s2)))
        return false;

    if (!(test_pixel_is_free((vec2_t){x, y1 - 1}) && test_pixel_is_free((vec2_t){x, y2 + 1})))
        return false;

    for (int y = y1; y <= y2; ++y)
        if (!try_draw_pixel((vec2_t){x, y}, col))
            return false;

    return true;
}

#define POW2(x) ((x) * (x))

status_t draw_rand_circ() {
    vec2_t c = {rand_range(2, IMG_SIDE - 3), rand_range(2, IMG_SIDE - 3)};

    int radius;
    radius = rand_range(1, MIN(MIN(c.x - 1, IMG_SIDE - c.x - 1), MAX_CIRCLE_SIZE / 2));
    radius = MIN(radius, rand_range(1, MIN(MIN(c.y - 1, IMG_SIDE - c.y - 1), MAX_CIRCLE_SIZE / 2)));
    color_t col = rand_fig_color();

    if (!(1 <= c.x - radius && c.x + radius <= IMG_SIDE - 1 && 1 <= c.y - radius
          && c.y + radius <= IMG_SIDE - 1))
        return false;

    int prev_y1, prev_y2;
    for (int x = -radius; x <= radius; ++x) {
        int y2 = (sqrt(POW2(radius) - POW2(x)));
        int y1 = -y2;

        if (x == -radius) {
            prev_y1 = y1, prev_y2 = y2;
            if (!test_rect_is_free((vec2_t){c.x + x - 1, c.y + y1},
                                   (vec2_t){1, prev_y2 - prev_y1 + 1}))
                return OVERLAP;
        }

        if (!try_draw_circ_line(c.x + x, c.y + y1, c.y + y2, c.y + prev_y1, c.y + prev_y2, col))
            return OVERLAP;

        prev_y1 = y1, prev_y2 = y2;
    }

    if (!test_rect_is_free((vec2_t){c.x + radius + 1, c.y + prev_y1},
                           (vec2_t){1, prev_y2 - prev_y1 + 1}))
        return OVERLAP;

    return OK;
}

int y_intersect_with_x(vec2_t a, vec2_t b, int x) {
    if (a.x == b.x)
        return INT32_MAX;

    float x1 = a.x, x2 = b.x, y1 = a.y, y2 = b.y;
    return (y2 - y1) / (x2 - x1) * (x - x1) + y1;
}

int triangle_vert_sort_cmp(const void *v1, const void *v2) {
    int x1 = ((vec2_t *)v1)->x, x2 = ((vec2_t *)v2)->x;
    if (x1 > x2)
        return 1;
    if (x1 < x2)
        return -1;
    return 0;
}

int interval_intersection(int y1, int y2, int py1, int py2) {
    int a = MAX(y1, py1);
    int b = MIN(y2, py2);
    return b - a;
}

status_t draw_rand_triangle() {
    vec2_t lt, size;
    if (!rand_rect(&lt, &size))
        return CAN_REROLL;

    vec2_t a, b, c;
    a = (vec2_t){rand_range(lt.x, lt.x + size.x), rand_range(lt.y, lt.y + size.y)};
    b = (vec2_t){rand_range(lt.x, lt.x + size.x), rand_range(lt.y, lt.y + size.y)};
    c = (vec2_t){rand_range(lt.x, lt.x + size.x), rand_range(lt.y, lt.y + size.y)};

    float ab = sqrt(POW2(a.x - b.x) + POW2(a.y - b.y));
    float ac = sqrt(POW2(a.x - c.x) + POW2(a.y - c.y));
    float cb = sqrt(POW2(c.x - b.x) + POW2(c.y - b.y));

    if (ab > ac + cb || ac > ab + cb || cb > ab + ac)
        return CAN_REROLL;

    float p = (ab + ac + cb) / 2.0;
    if (sqrt(p * (p - ab) * (p - ac) * (p - cb)) < 10)
        return CAN_REROLL;

    color_t col = rand_fig_color();

    vec2_t verts[3] = {a, b, c};
    qsort(verts, 3, sizeof *verts, triangle_vert_sort_cmp);
    a = verts[0], b = verts[1], c = verts[2];

    if (a.x == c.x)
        return CAN_REROLL;

    int prev_y1, prev_y2, y1, y2;
    bool rect = true;
    for (int x = a.x; x <= c.x; ++x) {
        int yi1 = y_intersect_with_x(a, c, x);
        if (yi1 == INT32_MAX)
            yi1 = MIN(a.y, c.y);

        int yi2;
        if (x <= b.x) {
            yi2 = y_intersect_with_x(a, b, x);
            if (yi2 == INT32_MAX)
                yi2 = MIN(a.y, b.y);
        } else {
            yi2 = y_intersect_with_x(b, c, x);
            if (yi2 == INT32_MAX)
                yi2 = MIN(b.y, c.y);
        }

        y1 = MIN(yi1, yi2), y2 = MAX(yi1, yi2);

        if (x == a.x) {
            prev_y1 = y1, prev_y2 = y2;

            if (!test_rect_is_free((vec2_t){x - 1, y1}, (vec2_t){1, y2 - y1 + 1}))
                return OVERLAP;
        }

        if (interval_intersection(y1, y2, prev_y1, prev_y2) < 0)
            return OVERLAP;

        if (y2 != y1)
            rect = false;

        if (!try_draw_circ_line(x, y1, y2, prev_y1, prev_y2, col))
            return OVERLAP;

        prev_y1 = y1, prev_y2 = y2;
    }

    if (rect)
        return OVERLAP;

    if (!test_rect_is_free((vec2_t){c.x + 1, prev_y1}, (vec2_t){1, prev_y2 - prev_y1 + 1}))
        return OVERLAP;

    return OK;
}

void fill_background() {
    for (int x = 0; x < IMG_SIDE; ++x)
        for (int y = 0; y < IMG_SIDE; ++y)
            img[y][x] = background_color;
}

bool draw_figure(status_t (*callback)(), int *attempts) {
    if (*attempts <= 0)
        return false;

    copy_img_buf(img_tmp, img);
    status_t status = callback();
    if (status != CAN_REROLL) {
        --*attempts;
        if (status == OVERLAP) {
            copy_img_buf(img, img_tmp);
            return false;
        } else
            return true;
    } else
        return false;
}

int main(int argc, char **argv) {
    /* fputs("Тот Миша, который сегодня, не работает.\n" */
    /*       "Поэтому, я сделал своего Мишу. " */
    /*       "Он даже лучше, чем настоящий.\n", */
    /* stderr); */
    long seed;
    if (argc == 1) {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        seed = ts.tv_nsec;
    } else if (argc == 2) {
        sscanf(argv[1], "%ld", &seed);
    } else {
        fprintf(stderr, "Usage: %s [seed]\n", argv[0]);
        return 1;
    }

    srand(seed);

    background_color = rand_color();
    fill_background();

    int ra = rand_range(0, MAX_RECTS_ATTEMPTS), ca = rand_range(0, MAX_CIRCLES_ATTEMPTS),
        ta = rand_range(0, MAX_TRIANGLES_ATTEMPTS);
    int rects = 0;
    while (1) {
        int fig = rand() % 3;
        if (fig == 0) {
            if (draw_figure(draw_rand_rect, &ra))
                ++rects;
        } else if (fig == 1) {
            draw_figure(draw_rand_triangle, &ta);
        } else if (fig == 2) {
            draw_figure(draw_rand_circ, &ca);
        }

        if (ra + ca + ta <= 0)
            break;
    }

    char filename[1000];
    sprintf(filename, "tests/b%ld_%d.png", seed, rects);

    /* write_png(stdout); */
    FILE *f = fopen(filename, "wb+");
    if (f == NULL) {
        fprintf(stderr, "cannot open file %s\n", filename);
        return 1;
    }
    write_png(f);
    /* fprintf(stderr, "seed: %ld\n", seed); */
}
