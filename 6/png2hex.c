#include <assert.h>
#include <ctype.h>
#include <spng.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

const size_t IMG_SIDE_IN_PIXELS = 100;

const char HEADER[] = "424d667500000000000036000000280000006400000064000000010018000000000030750000"
                      "00000000000000000000000000000000";

void png_error(const char *spng_function_name, int ret) {
    fprintf(stderr, "%s() error: %s\n", spng_function_name, spng_strerror(ret));
    exit(1);
}

void png2hex(FILE *png_input, FILE *hex_output) {
    spng_ctx *ctx = spng_ctx_new(0);
    size_t img_max_size = IMG_SIDE_IN_PIXELS * IMG_SIDE_IN_PIXELS * 3;
    spng_set_chunk_limits(ctx, img_max_size, img_max_size);

    spng_set_png_file(ctx, png_input);

    struct spng_ihdr ihdr;
    int ret = spng_get_ihdr(ctx, &ihdr);
    if (ret)
        png_error("spng_get_ihdr", ret);

    size_t img_size;
    ret = spng_decoded_image_size(ctx, SPNG_FMT_RGB8, &img_size);
    if (ret)
        png_error("spng_decoded_image_size", ret);

    if (img_size != IMG_SIDE_IN_PIXELS * IMG_SIDE_IN_PIXELS * 3) {
        fprintf(stderr, "expected %zux%zu image, found %zu bytes\n", IMG_SIDE_IN_PIXELS,
                IMG_SIDE_IN_PIXELS, img_size);
        exit(1);
    }

    uint8_t *img_buf = malloc(img_size);
    ret = spng_decode_image(ctx, img_buf, img_size, SPNG_FMT_RGB8, 0);
    if (ret)
        png_error("spng_decode_image", ret);

    fwrite(HEADER, sizeof HEADER - 1, 1, hex_output);

    for (size_t i = IMG_SIDE_IN_PIXELS - 1; i < IMG_SIDE_IN_PIXELS; --i) {
        for (size_t j = 0; j < 3 * IMG_SIDE_IN_PIXELS; j += 3) {
#define PIXEL(offs) img_buf[i * IMG_SIDE_IN_PIXELS * 3 + j + (offs)]
            /* Order of colors in png:  RGB
             * Order of colors for hex: BGR */
            fprintf(hex_output, "%.2x%.2x%.2x", PIXEL(2), PIXEL(1), PIXEL(0));
#undef PIXEL
        }
    }
}

uint8_t read_nibble(FILE *f) {
    int c = fgetc(f);
    if (c == EOF) {
        fputs("Unexpeceted EOF", stderr);
        exit(1);
    }
    c = tolower(c);
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('a' <= c && c <= 'f')
        return 10 + c - 'a';
    else {
        fprintf(stderr, "Invalid character 0x%x - `%c`\n", c, c);
        exit(1);
    }
}

uint8_t read_byte(FILE *f) {
    return (read_nibble(f) << 4) | read_nibble(f);
}

void hex2png(FILE *hex_input, FILE *png_output) {
    int c;
    while (iscntrl((c = fgetc(hex_input))))
        ;

    size_t h = sizeof HEADER - 2;
    while (h--)
        fgetc(hex_input);

    const size_t IMG_BUF_SIZE = IMG_SIDE_IN_PIXELS * IMG_SIDE_IN_PIXELS * 3;
    uint8_t *img_buf = malloc(IMG_BUF_SIZE);
    for (size_t i = IMG_SIDE_IN_PIXELS - 1; i + 1 != 0; --i) {
        for (size_t j = 0; j < 3 * IMG_SIDE_IN_PIXELS; j += 3) {
            /* Order of colors in hex:  BGR
             * Order of colors for png: RGB */
            img_buf[i * IMG_SIDE_IN_PIXELS * 3 + j + 2] = read_byte(hex_input);
            img_buf[i * IMG_SIDE_IN_PIXELS * 3 + j + 1] = read_byte(hex_input);
            img_buf[i * IMG_SIDE_IN_PIXELS * 3 + j + 0] = read_byte(hex_input);
        }
    }

    spng_ctx *ctx = spng_ctx_new(SPNG_CTX_ENCODER);
    spng_set_png_file(ctx, png_output);
    spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);

    struct spng_ihdr ihdr = {0};
    ihdr.width = ihdr.height = IMG_SIDE_IN_PIXELS;
    ihdr.color_type = SPNG_COLOR_TYPE_TRUECOLOR;
    ihdr.bit_depth = 8;
    spng_set_ihdr(ctx, &ihdr);

    int ret = spng_encode_image(ctx, img_buf, IMG_BUF_SIZE, SPNG_FMT_PNG, SPNG_ENCODE_FINALIZE);
    if (ret)
        png_error("spng_encode_image", ret);

    free(img_buf);
    spng_ctx_free(ctx);
}

int main(int argc, char **argv) {
    bool reverse;
    if (argc == 1)
        reverse = false;
    else if (argc == 2 && strcmp(argv[1], "-r") == 0)
        reverse = true;
    else {
        fprintf(stderr, "Usage %s [-r]\n", argv[0]);
        return 1;
    }

    if (reverse)
        hex2png(stdin, stdout);
    else
        png2hex(stdin, stdout);
}
