#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int MAX_ASIX_SIZE = 100;
const int MAX_MAGNITUDE = 1e9;
const int MAX_SPACES = 100;
const int TESTS = 100000;
const int MAX_NEWLINES = 100;

#define PROG_PATH "./task7"
#define TMP_INPUT_PATH "./tmp_input"
#define TMP_OUTPUT_PATH "./tmp_output"
#define CMD PROG_PATH " < " TMP_INPUT_PATH " > " TMP_OUTPUT_PATH

#define IMPL(a, b) (!(a) || (b))

typedef struct {
    int n, m;
} result_t;

void generate_spaces(FILE *input) {
    int spaces = 1 + (rand() % MAX_SPACES);
    while (spaces--) {
        char c = rand() % 3 == 0 ? '\t' : ' ';
        fputc(c, input);
    }
}

void generate_newline(FILE *input) {
    int newlines = 1 + (rand() % MAX_NEWLINES);
    while (newlines--) {
        fputc('\n', input);
        if (rand() % 3 == 0)
            fputc('\r', input);
    }
}

result_t generate_testcase(FILE *input) {
    result_t res = {.n = rand() % MAX_ASIX_SIZE, rand() % MAX_ASIX_SIZE};
    if (res.n * res.m == 0)
        res.m = res.n = 0;

    for (int i = 0; i < res.n; ++i) {
        for (int j = 0; j < res.m; ++j) {
            generate_spaces(input);

            int num = rand() % MAX_MAGNITUDE;
            if (rand() % 3 == 0)
                num *= -1;

            fprintf(input, "%d", num);

            generate_spaces(input);
        }

        if (IMPL(i == res.n - 1, rand() % 2 == 0))
            generate_newline(input);
    }

    if (rand() % 2 == 0)
        generate_spaces(input);

    fflush(input);

    return res;
}

bool test(FILE *output, result_t expected) {
    int n = INT32_MAX, m = INT32_MAX;
    fscanf(output, "%d %d", &n, &m);
    return n == expected.n && m == expected.m;
}

int main() {
    srand(time(NULL));

    FILE *input = fopen(TMP_INPUT_PATH, "w+");
    FILE *output = fopen(TMP_OUTPUT_PATH, "w+");

    for (int i = 0; i < TESTS; ++i) {
        printf("Test %d/%d ", i + 1, TESTS);
        fflush(stdout);

        ftruncate(fileno(input), 0);
        rewind(input);
        ftruncate(fileno(output), 0);
        rewind(output);
        fflush(output);

        result_t exp = generate_testcase(input);
        printf("%d*%d=%d ", exp.n, exp.m, exp.n * exp.m);
        fflush(stdout);

        int exit_code = system(CMD);
        if (exit_code != 0 || !test(output, exp)) {
            puts("failed");
            printf("exit code: %d\n", exit_code);
            puts("input in " TMP_INPUT_PATH);
            puts("output in " TMP_OUTPUT_PATH);
            printf("expected: %d %d\n", exp.n, exp.m);
            break;
        }

        puts("ok");
        fflush(stdout);
    }
}
