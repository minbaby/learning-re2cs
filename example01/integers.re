#include <stdio.h>
#include "../common/common.h"

enum num_t 
{
    ERR, // 错误
    BIN, // 二进制
    OCT, // 八进制
    DEC, // 十进制
    HEX  // 十六进制
};

static num_t lex(const char *YYCURSOR)
{
    // YYMARKER (line 7) is needed because rules overlap:
    // it backs up the input position of the longest successful match.
    // Imagine we have overlapping rules "a" and "abc" and input string "abd": by the time "a" matches, there’s still a chance to match "abc", but when the lexer sees 'd', it must roll back.
    // (You might wonder why YYMARKER is exposed at all: why not make it a local variable like yych? The reason is, all input pointers must be updated by YYFILL as explained in the Large input example.)
    // 内置的指定名称，不能修改为其他名称。回溯用。
    const char *YYMARKER;

    /*!re2c
        re2c:define:YYCTYPE = char;
        re2c:yyfill:enable = 0;

        end = "\x00";           // 定义一个结束符（NULL）
        bin = '0b' [01]+;       //  以 0b 开头，且只有01， （这个算是一个正则？），中间的空格是被忽略的，可以删掉或者有多个
        oct = "0" [0-7]*;
        dec = [1-9][0-9]*;
        hex = '0x' [0-9a-fA-F]+;

        *       { return ERR; } // 默认规则，返回错误
        bin end { return BIN; }
        oct end { return OCT; }
        dec end { return DEC; }
        hex end { return HEX; }
    */
}

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i) {
        switch (lex(argv[i])) {
            case ERR:
                printf("错误\n"); 
                break;
            case BIN:
                printf("二进制\n");
                break;
            case OCT:
                printf("八进制\n"); 
                break;
            case DEC:
                printf("十进制\n");
                break;
            case HEX:
                printf("十六进制\n");
                break;
        }
    }
    return 0;
}
