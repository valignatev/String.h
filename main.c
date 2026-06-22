#include <stdio.h>
#include <stdlib.h>

#define STRING_DEF static
#define STRING_IMPLEMENTATION
#include "String.h"

int main() {
    String string = string_lit("Hello!");
    printf("string: %s, string_count: %ld\n", string.data, string.count);

    String string1 = string_from_cstring("Hellou!");
    printf("string1: %s, string_count: %ld\n", string1.data, string1.count);

    String strings[10];

    printf("\nsplit empty string\n");
    String lit_empty = string_lit("");
    size_t splitted_amount1 = string_split(lit_empty, string_lit(","), &strings[0], 10, NULL);
    printf("splitted_amount1: %ld\n", splitted_amount1);

    printf("\nsplit by 1 char separator\n");
    String lit = string_lit("a, b, c, d, e");
    size_t splitted_amount2 = string_split(lit, string_lit(","), &strings[0], 10, NULL);

    printf("splitted_amount2: %ld\n", splitted_amount2);
    for (int i = 0; i < splitted_amount2; i++) {
        printf("chunk: %.*s\n", string_spread(strings[i]));
    }

    printf("\nsplit by separator with more than 1 character\n");
    String lit_hard = string_lit("one;;two;;three;;four;;пять");
    size_t splitted_amount3 = string_split(lit_hard, string_lit(";;"), &strings[0], 10, NULL);

    printf("splitted_amount3: %ld\n", splitted_amount3);
    for (int i = 0; i < splitted_amount3; i++) {
        printf("chunk: %.*s\n", string_spread(strings[i]));
    }

    printf("\nstarts and ends with separator\n");
    String lit_kek = string_lit("kekwlolkekhahakekkeklmaokek");
    size_t splitted_amount4 = string_split(lit_kek, string_lit("kek"), &strings[0], 10, NULL);

    printf("splitted_amount3: %ld\n", splitted_amount4);
    for (int i = 0; i < splitted_amount4; i++) {
        printf("chunk: %.*s\n", string_spread(strings[i]));
    }

    printf("\nBuffer limit\n");
    String limited = string_lit("a,b,c,d,e");
    String outbuf[2];
    size_t splitted_amount5 = string_split(limited, string_lit(","), outbuf, 2, NULL);

    printf("splitted_amount5: %ld\n", splitted_amount5);
    printf("outbuf[0]: '%.*s' (len=%ld)\n", string_spread(outbuf[0]), outbuf[0].count);
    if (splitted_amount5 > 1) {
        printf("outbuf[1]: '%.*s' (len=%ld)\n", string_spread(outbuf[1]), outbuf[1].count);
    }
    if (splitted_amount5 > 2) {
        printf("WARNING: returned %ld but only provided 2 slots!\n", splitted_amount5);
    }

    printf("\ntrim_left\n");
    String trimmable1 = string_make("\n hi!", 5);
    String trimmed1 = string_trim_left(trimmable1);

    printf("trimmed1: \"%.*s\"\n", string_spread(trimmed1));

    printf("\ntrim_right\n");
    String trimmable2 = string_make("h i!\t\v\f    ", 11);
    String trimmed2 = string_trim_right(trimmable2);

    printf("trimmed2: \"%.*s\"\n", string_spread(trimmed2));

    printf("\ntrim\n");
    String trimmable3 = string_make("\n\t$h i!\t\v\f    ", 14);
    String trimmed3 = string_trim(trimmable3);

    printf("trimmed3: \"%.*s\"\n", string_spread(trimmed3));

    printf("\nto_cstring\n");
    String str = string_make("Hello, world!", 13);
    char *buf = malloc(str.count + 1);
    string_to_cstring(str, buf);

    printf("cstr: \"%s\"\n", buf);
    
    return 0;
}
