/* Generated by re2c 1.0.3 on Fri Apr 13 16:21:01 2018 */
#line 1 "strings.re"
#include <stdio.h>
#include <strings.h>
#include <string.h>

// 告诉 re2c 生成 `#define YYMAXFILL n`
#define YYMAXFILL 1


struct input_t {
    size_t len;
    char *str;

    input_t(const char *s) : len(strlen(s)), str(new char[len + YYMAXFILL])
    {
        memcpy(str, s, len);
        memset(str + len, 'a', YYMAXFILL);
    }
    ~input_t()
    {
        delete[]str;
    }
};

static bool lex(const input_t & input)
{
    const char *YYCURSOR = input.str;
    const char *const YYLIMIT = input.str + input.len + YYMAXFILL;
    
#line 32 "strings.cc"
{
	char yych;
	if (YYLIMIT <= YYCURSOR) return false;
	yych = *YYCURSOR;
	switch (yych) {
	case '"':	goto yy4;
	case '\'':	goto yy6;
	default:	goto yy2;
	}
yy2:
	++YYCURSOR;
#line 35 "strings.re"
	{ return false; }
#line 46 "strings.cc"
yy4:
	++YYCURSOR;
	if (YYLIMIT <= YYCURSOR) return false;
	yych = *YYCURSOR;
	switch (yych) {
	case '"':	goto yy8;
	default:	goto yy4;
	}
yy6:
	++YYCURSOR;
	if (YYLIMIT <= YYCURSOR) return false;
	yych = *YYCURSOR;
	switch (yych) {
	case '\'':	goto yy8;
	default:	goto yy6;
	}
yy8:
	++YYCURSOR;
#line 36 "strings.re"
	{ return YYLIMIT - YYCURSOR == YYMAXFILL; }
#line 67 "strings.cc"
}
#line 37 "strings.re"

}

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i) {
        input_t arg(argv[i]);
        printf("%s: %s\n", lex(arg) ? "str" : "err", argv[i]);
    }
    return 0;
}