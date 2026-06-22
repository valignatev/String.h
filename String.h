#ifndef STRING_HEADER
#define STRING_HEADER

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef STRING_DEF
#define STRING_DEF
#endif


typedef struct {
    int64_t count;
    const char* data;
} String;


static String string_whitespaces = {6, "\t\n\v\f\r "};


// printf helper
#define string_spread(string) (int)string.count, string.data


STRING_DEF String string_make(const char *data, int64_t count);
STRING_DEF String string_from_cstring(const char *data);
STRING_DEF bool string_eq(String a, String b);
STRING_DEF bool string_starts_with(String source, String prefix);
STRING_DEF String string_trim(String string);
STRING_DEF String string_trim_left(String string);
STRING_DEF String string_trim_right(String string);

/*
Returns how many "chunks" were written to "out". If it equals to "out_count", you might
want to check how many bytes from input string were consumed (stored in *consumed), and maybe call this again
with the new input string (count -= consumed, data += consumed).
You can pass NULL to "consumed", and it'll be ignored.
*/
STRING_DEF size_t string_split(String string, String separator, String *out, size_t out_count, size_t *consumed);
// You need to guarantee that out has string.count + 1 space (for null terminator).
STRING_DEF void string_to_cstring(String string, char *out);
STRING_DEF size_t string_cstrlen(const char *data);


// You need -1 at the end, because sizeof on char* includes null terminator.
#define string_lit(char_star) string_make(char_star, sizeof(char_star) - 1)


#endif // STRING_HEADER


#ifdef STRING_IMPLEMENTATION

STRING_DEF String string_make(const char *data, int64_t count) {
    String result;
    result.count = count;
    result.data = data;

    return result;
}


STRING_DEF String string_from_cstring(const char *data) {
    String result;
    result.count = string_cstrlen(data);
    result.data = data;

    return result;
}


STRING_DEF bool string_eq(String a, String b) {
    if (a.count != b.count) {
        return false;
    }

    if (a.data == b.data) {
        return true;
    }

    const char *left = a.data;
    const char *right = b.data;

    for (int64_t i = 0; i < a.count; i++, left++, right++) {
        if (*left != *right) {
            return false;
        }
    }

    return true;
}


STRING_DEF bool string_starts_with(String source, String prefix) {
    if (source.count == 0 || prefix.count == 0) {
        return false;
    }

    if (source.count < prefix.count) {
        return false;
    }

    source.count = prefix.count;
    // This can also be solved through string_index_of, which I don't have yet!
    return string_eq(source, prefix);
}


STRING_DEF size_t string_split(String string, String separator, String *out, size_t out_count, size_t *consumed) {
    if (string.count == 0 || out_count == 0 || separator.count == 0) {
        if (out) {
            out->count = string.count;
            out->data = string.data;
        }

        return 0;
    }

    size_t result = 1;

    out->data = string.data;
    out->count = 0;

    while (string.count > 0 && out_count > 0) {
        if (string_starts_with(string, separator)) {

            out_count -= 1;
            if (out_count > 0) {
                string.data += separator.count;
                string.count -= separator.count;

                out += 1;
                out->data = string.data;
                out->count = 0;

                result += 1;

            }

            if (consumed) {
                *consumed += separator.count;
            }

        } else {
            string.data += 1;
            string.count -= 1;
            out->count += 1;

            if (consumed) {
                *consumed += 1;
            }
        }
    }

    return result;
}


STRING_DEF String string_trim_left(String string) {
    int64_t stop_at = 0;
    for (int i = 0; i < string.count; i++) {
        bool found = false;
        
        for (int j = 0; j < string_whitespaces.count; j++) {
            if (string_whitespaces.data[j] == string.data[i]) {
                stop_at++;
                found = true;
                break;
            }
        }

        if (!found) {
            break;
        }
    }

    string.count -= stop_at;
    string.data += stop_at;
    return string;
}


STRING_DEF String string_trim_right(String string) {
    int64_t stop_at = 0;
    for (int i = string.count - 1; i >= 0; i--) {
        bool found = false;
        
        for (int j = 0; j < string_whitespaces.count; j++) {
            if (string_whitespaces.data[j] == string.data[i]) {
                stop_at++;
                found = true;
                break;
            }
        }

        if (!found) {
            break;
        }
    }

    string.count -= stop_at;
    return string;
}


STRING_DEF String string_trim(String string) {
    return string_trim_right(string_trim_left(string));
}


STRING_DEF size_t string_cstrlen(const char *data) {
    size_t result = 0;
    while (data[result] != 0) {
        result++;
    }

    return result;
}


STRING_DEF void string_to_cstring(String string, char *out) {
    for (int i = 0; i < string.count; i++) {
        *out = string.data[i];
        out++;
    }
    *out = 0;
}


#endif // STRING_IMPLEMENTATION
