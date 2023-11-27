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

bool color_eq(color_t c1, color_t c2) {
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

#define IMG_SIDE_IN_PIXELS 100

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

const int MAX_RECTS_ATTEMPTS = 3000;
const int MAX_CIRCLES_ATTEMPTS = 3000;
const int MAX_TRIANGLES_ATTEMPTS = 300;
const int MAX_RECT_SIZE = 98;
const int MAX_CIRCLE_SIZE = 98;
const int MAX_TRIANGLE_SIZE = 98;

color_t img_buf[IMG_SIDE_IN_PIXELS][IMG_SIDE_IN_PIXELS];
color_t img_buf_tmp[IMG_SIDE_IN_PIXELS][IMG_SIDE_IN_PIXELS];
color_t background_color;

void copy_img_buf(void *dest, void *src) {
    memcpy(dest, src, sizeof img_buf);
}

void write_png(FILE *f) {
    spng_ctx *ctx = spng_ctx_new(SPNG_CTX_ENCODER);
    spng_set_png_file(ctx, f);
    spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);

    struct spng_ihdr ihdr = {0};
    ihdr.width = ihdr.height = IMG_SIDE_IN_PIXELS;
    ihdr.color_type = SPNG_COLOR_TYPE_TRUECOLOR;
    ihdr.bit_depth = 8;
    spng_set_ihdr(ctx, &ihdr);

    int ret = spng_encode_image(ctx, img_buf, sizeof img_buf, SPNG_FMT_PNG, SPNG_ENCODE_FINALIZE);
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
    img_buf[coord.y][coord.x] = col;
}

bool test_pixel_is_free(vec2_t coord) {
    return color_eq(img_buf[coord.y][coord.x], background_color);
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

bool draw_rand_rect() {
    vec2_t lt = {rand_range(1, IMG_SIDE_IN_PIXELS - 2), rand_range(1, IMG_SIDE_IN_PIXELS - 2)};
    vec2_t size = {rand_range(1, MIN(MAX_RECT_SIZE, IMG_SIDE_IN_PIXELS - lt.x - 1)),
                   rand_range(1, MIN(MAX_RECT_SIZE, IMG_SIDE_IN_PIXELS - lt.x - 1))};
    color_t c = rand_fig_color();

    if (size.x <= 0 || size.y <= 0)
        return false;

    if (!(test_rect_is_free((vec2_t){lt.x, lt.y - 1}, (vec2_t){size.x, 1})
          && test_rect_is_free((vec2_t){lt.x - 1, lt.y}, (vec2_t){1, size.y})
          && test_rect_is_free((vec2_t){lt.x + size.x, lt.y}, (vec2_t){1, size.y})
          && test_rect_is_free((vec2_t){lt.x, lt.y + size.y}, (vec2_t){size.x, 1})))
        return false;

    for (int x = 0; x < size.x; ++x)
        for (int y = 0; y < size.y; ++y)
            if (!try_draw_pixel((vec2_t){lt.x + x, lt.y + y}, c))
                return false;
    return true;
}

void draw_line(vec2_t p1, vec2_t p2, color_t c) {
    float x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y;
    float k = (y2 - y1) / (x2 - x1);
    int x, stopx, y, stopy;
    float k_acc = 0;
    int kd;
    if (fabs(k) > 1.0) {
        if (p1.x < p2.x) {
            x = p1.x, stopx = p2.x;
            y = p1.y, stopy = p2.y;
        } else {
            x = p2.x, stopx = p1.x;
            y = p2.y, stopy = p1.y;
        }
        while (x <= stopx) {
            k_acc += k;
            kd = k_acc;
            k_acc -= kd;
            for (int i = 0; i < MIN(abs(kd), abs(stopy - y) + 1); ++i) {
                int yd = kd >= 0 ? i : -i;
                try_draw_pixel((vec2_t){x, y + yd}, c);
            }
            y += kd;
            ++x;
        }
    } else {
        if (p1.y < p2.y) {
            x = p1.x, stopx = p2.x;
            y = p1.y, stopy = p2.y;
        } else {
            x = p2.x, stopx = p1.x;
            y = p2.y, stopy = p1.y;
        }
        k = 1 / k;
        while (y <= stopy) {
            k_acc += k;
            kd = k_acc;
            k_acc -= kd;
            for (int i = 0; i < MIN(abs(kd), abs(stopx - x) + 1); ++i) {
                int xd = kd >= 0 ? i : -i;
                try_draw_pixel((vec2_t){x + xd, y}, c);
            }
            x += kd;
            ++y;
        }
    }
}

bool try_draw_circ_line(int x, int y1, int y2, int prev_y1, int prev_y2, color_t col) {
    vec2_t lt1, s1, lt2, s2;
    s1 = (vec2_t){1, abs(y1 - prev_y1)}, s2 = (vec2_t){1, abs(y2 - prev_y2)};
    lt1.y = y1, lt2.y = y2;

    if (prev_y1 >= y1) {
        lt1.x = lt2.x = x - 1;
    } else {
        lt1.x = lt2.x = x;
    }

    if (!(test_rect_is_free(lt1, s1) && test_rect_is_free(lt2, s2)))
        return false;

    if (!(test_pixel_is_free((vec2_t){x, y1 - 1}) && test_pixel_is_free((vec2_t){x, y2 + 1})))
        return false;

    for (int y = y1; y <= y2; ++y)
        if (!try_draw_pixel((vec2_t){x, y}, col))
            return false;

    return true;
}

bool draw_rand_circ() {
    vec2_t c = {rand_range(2, IMG_SIDE_IN_PIXELS - 3), rand_range(2, IMG_SIDE_IN_PIXELS - 3)};
    int radius;
    radius = rand_range(1, MIN(MIN(c.x - 1, IMG_SIDE_IN_PIXELS - c.x - 1), MAX_CIRCLE_SIZE / 2));
    radius =
        MIN(radius,
            rand_range(1, MIN(MIN(c.y - 1, IMG_SIDE_IN_PIXELS - c.y - 1), MAX_CIRCLE_SIZE / 2)));
    color_t col = rand_fig_color();

    int prev_y1, prev_y2;
    for (int x = -radius; x <= radius; ++x) {
        int y2 = (sqrt(radius * radius - x * x));
        int y1 = -y2;

        if (x == -radius) {
            prev_y1 = y1, prev_y2 = y2;
            if (!test_rect_is_free((vec2_t){c.x + x - 1, c.y + y1},
                                   (vec2_t){1, prev_y2 - prev_y1 + 1}))
                return false;
        }

        if (!try_draw_circ_line(c.x + x, c.y + y1, c.y + y2, c.y + prev_y1, c.y + prev_y2, col))
            return false;

        prev_y1 = y1, prev_y2 = y2;
    }

    if (!test_rect_is_free((vec2_t){c.x + radius + 1, c.y + prev_y1},
                           (vec2_t){1, prev_y2 - prev_y1 + 1}))
        return false;

    return true;
}

void fill_background() {
    for (int x = 0; x < IMG_SIDE_IN_PIXELS; ++x)
        for (int y = 0; y < IMG_SIDE_IN_PIXELS; ++y)
            img_buf[y][x] = background_color;
}

void test_lines() {
    /* vec2_t p1 = {96, 77}, p2 = {0, 98}; */
    vec2_t p1 = {1, 1}, p2 = {20, 36};
    color_t line_col = rand_fig_color(), dot_col = rand_fig_color();
    draw_line(p1, p2, line_col);
    /* draw_pixel(p1, dot_col); */
    /* draw_pixel(p2, dot_col); */
}

bool draw_figure(bool (*callback)(), int *attempts) {
    if (*attempts <= 0)
        return false;

    copy_img_buf(img_buf_tmp, img_buf);
    --*attempts;
    bool ret = callback();
    if (!ret)
        copy_img_buf(img_buf, img_buf_tmp);
    return ret;
}

void draw_figures(void (*callback)(), int attempts_limit) {
    while (attempts_limit--)
        callback();
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

    int ra = rand_range(0, MAX_RECTS_ATTEMPTS), ca = rand_range(0, MAX_CIRCLES_ATTEMPTS);
    int rects = 0;
    /* circles = 1; */
    /* rects = 0; */
    while (1) {
        if (draw_figure(draw_rand_rect, &ra))
            ++rects;
        draw_figure(draw_rand_circ, &ca);

        if (ra + ca <= 0)
            break;
    }

    char filename[1000];
    sprintf(filename, "tests/b%ld_%d.png", seed, rects);

    FILE *f = fopen(filename, "wb+");
    if (f == NULL) {
        fprintf(stderr, "cannot open file %s\n", filename);
        return 1;
    }
    write_png(f);
    fprintf(stderr, "seed: %ld\n", seed);
}
