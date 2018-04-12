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

typedef enum num_t num_t_t;

// 这里原来返回是 `static num_t lex`，但是使用 vscode 的时候会提示有问题
// 经过查资料，https://stackoverflow.com/questions/1102542/how-to-define-an-enumerated-type-enum-in-c
// 默认使用是需要使用 enum 作为前缀的，如果不想使用的话，可以使用 typeof
static enum num_t lex(const char *YYCURSOR)
{
    const char *YYMARKER;
    /*!re2c
        re2c:define:YYCTYPE = char;
        re2c:yyfill:enable = 0;

        end = "\x00";
        bin = '0b' [01]+;
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

static num_t_t lexNew(const char *YYCURSOR)
{
    return lex(YYCURSOR);
}

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i) {
        switch (lexNew(argv[i])) {
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
