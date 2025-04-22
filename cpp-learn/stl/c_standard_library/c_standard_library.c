/*
 The C++ Programming Language, 4th Edition
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static inline void fclosep(FILE **fp)
{
    if (*fp) fclose(*fp);
}

#define _cleanup_fclose_ __attribute__((cleanup(fclosep)))

int main()
{
    /**
     * Files
     */

    char filename[20] = "test.c";

    FILE *file = fopen(filename, "w+");
    if (file == NULL) {
        fprintf(stderr, "cannot open file: %s", filename);
        exit(1);
    }

    const char *content = "int main() {\n}\n";

    int res = fprintf(file, "%s", content);
    if (res < 0) {
        fprintf(stderr, "error writing to file: %s", filename);
    }

    fclose(file);

    // CGG/Clang RAII: https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization
    _cleanup_fclose_ FILE *f = fopen(filename, "w+");
    if (f == NULL) {
        fprintf(stderr, "cannot open file: %s", filename);
        exit(1);
    }

    /**
     * The printf() Family
     */

    int x = 5;
    const char *s = "Pedersen";
    res = printf("the value of x is '%d' and the value of s is '%s'\n", x, s);
    assert(res == 55);

    char *line_format = "#line %d \"%s\"\n";
    int line = 13;
    char *file_name = "C++/main.c";

    printf("int a;\n");
    printf(line_format, line, filename);

    // char c = 'q';
    // printf("bad input char: %s", c); // %s should have been %c

    const int buf_size = 32;
    int y;
    char b[buf_size];
    //    int i = scanf("the value of x is '%d' and the value of s is '%s'\n", &y, b);

    // Stdio Character Functions
    //
    // x=getc(st)
    // x=putc(c,st)
    // x=getchar()
    // x=putchar(c)
    // x=ungetc(c,st)

    /**
     * C-Style Strings
     */

    // x=strlen(s)
    // p=strcpy(s,s2)
    // p=strcat(s,s2)
    // x=strcmp(s, s2)
    // p=strncpy(s,s2,n)
    // p=strncat(s,s2,n)
    // x=strncmp(s,s2,n)
    // p=strchr(s,c)
    // p=strrchr(s,c)
    // p=strstr(s,s2)
    // p=strpbrk(s,s2)
    // x=atof(s)
    // x=atoi(s)
    // x=atol(s)
    // x=atoll(s)
    // x=strtod(s,p)
    // x=strtof(s,p)
    // x=strtold(s,p)
    // x=strtol(s,p,b)
    // x=strtoll(s,p,b)
    // x=strtoul(s,p,b)
    // x=strtoull(s,p,b)

    /**
     * Memory
     *
     * The memory manipulation functions operate on "raw memory" (no type known) through
     * void* pointers ( const void* pointers for memory that is only read)
     */

    // C-Style Memory Operations
    //
    // q=memcpy(p,p2,n)
    // q=memmove(p,p2,n)
    // x=memcmp(p,p2,n)
    // q=memchr(p,c,n)
    // q=memset(p,c,n)
    // p=calloc(n,s)
    // p=malloc(n)
    // q=realloc(p,n)
    // free(p)

    int *pint;

    pint = malloc(sizeof(int));
    if (pint == NULL) {
        fprintf(stderr, "cannot allocate");
        exit(1);
    }

    *pint = 22;
    (*pint)++;
    assert(*pint == 23);

    free(pint);

    /**
     * Date and Time
     */

    // Date and Time Types
    //
    // clock_t
    // time_t
    // tm

    /*
        struct tm {
            int tm_sec;     // second of minute [0:61]; 60 and 61 represent leap seconds
            int tm_min;     // minute of hour [0:59]
            int tm_hour;    // hour of day [0:23]
            int tm_mday;    // day of month [1:31]
            int tm_mon;     // month of year [0:11]; 0 means January (note: not [1:12])
            int tm_year;    // year since 1900; 0 means year 1900, and 115 means 2015
            int tm_wday;    // days since Sunday [0:6]; 0 means Sunday
            int tm_yday;    // days since January 1 [0:365]; 0 means January 1
            int tm_isdst;   // hours of daylight savings time
        };
     */

    // Date and Time Functions
    //
    // t=clock()
    // t=time(pt)
    // d=difftime(t2,t1)
    // ptm=localtime(pt)
    // ptm=gmtime(pt)
    // t=mktime(ptm)
    // p=asctime(ptm)
    // p=ctime(t)
    // n=strftime(p,max,fmt,ptm)

    /**
     * Etc.
     */

    // abort()
    // exit(n)
    // system(s)
    // qsort(b,n,s,cmp)
    // bsearch(k,b,n,s,cmp)
    // d=rand()
    // srand(d)

    // Integer Type Aliases (N can be 8, 16, 32, or 64)
    //
    // int_N_t
    // uint_N_t
    // int_leastN_t
    // uint_leastN_t
    // int_fastN_t
    // uint_leastN_t
}
