# 语法

[TOC]

> source: http://re2c.org/manual/syntax/syntax.html
> 使用 google 自动翻译的，如果存在错误请提交 pr 修正。

一个程序可以包含任意个 re2c 代码块。每一个代码块可以包含 `定义`， `配置`, 和 包含 `正则` 的 `规则`。生成的词法分析器使用接口代码与外部世界进行通信。

## 规则
----

规则由一个正则表达式和一个用户定义的操作组成：一个 C/C++ 代码块，在匹配成功时执行。 Action 可以是用大括号 `{` 和 `}` 括起来的任意代码块，也可以是不带大括号的代码块，前面加上 `:=`，最后是一个没有空格的换行符。

如果多个规则匹配，则 re2c 更喜欢最长匹配。 如果规则匹配相同的字符串，则较早的规则具有优先权。

有一种特殊的规则：使用 `*` 而不是正则表达式的默认规则。 它始终具有最低的优先级，匹配任何代码单元（无论是有效的还是无效的）并且仅消耗一个代码单元。 请注意，默认规则不同于 `[^]` ，它匹配任何有效的代码点并且可以消耗多个代码单元。 在可变长度编码的情况下*是匹配无效输入字符的唯一可能方式。

如果使用 `-c` `--conditions` 选项，则规则具有更复杂的形式，在有关条件的章节中进行描述。

## 正则表达式
----

对于正则表达式，re2c使用以下语法：

- `'foo'` 区分大小写的字符串文字
- `'foo'` 不区分大小写的字符串
- `[a-xyz]`，`[^ a-xyz]` 字符类（可能否定）
- `.` 除了换行符之外的任何字符
- 字符类别R和S的 `R \ S` 差异
- R *零或多个R的出现
- R +出现一次或多次R
- `R？`可选的R
- 重复R = n次n次
- 重复R至少n次
- 从n次到m次的R {n，m}重复
- `(R)` 只是 `R`;括号用于覆盖优先级或POSIX风格的子匹配
- R S连接：R后跟S
- `R | S`选择：R或S
- `R / S` loohakead：R后跟S，但S不被消耗
- 名称定义为名称的正则表达式（或Flex兼容模式中的文字字符串“name”）
- '{name}`在Flex兼容模式下定义为名称的正则表达式
- `@ stag's-tag：将@stag匹配的最后一个输入位置保存在名为stag的变量中
- `＃mtag` m-tag：将#mtag匹配的所有输入位置保存在名为mtag的变量中

字符类和字符串文字可以包含以下转义序列：`\a`，`\b`，`\f`，`\n`，`\r`，`\t`，`\v`，`\\`，八进制转义`\ooo`和十六进制转义`\xhh`，`\uhhhh`和`\Uhhhhhhhh`。

## 定义
----

命名定义的格式为`name = regexp`; 其中`name`是由字母，数字和下划线组成的标识符，`regexp`是一个正则表达式。 用`-F``--flex-syntax`选项命名的定义也是`name regexp`形式。 每个名称在使用前都应该定义。

## 接口代码
----

以下是词法分析器为了与外部世界进行交互而可能使用的所有符号列表。 这些符号应该由用户定义，无论是以就地配置的形式，还是C / C ++变量，函数，宏和其他语言结构。 哪些原语是必要的取决于具体的用例。

### yyaccept
无符号整数类型的L值，用于保存上次匹配规则的编号。只有使用-f --stores-state选项，用户的显式定义才是必需的。
### YYBACKUP（）
备份当前输入位置（仅与--input自定义选项一起使用）。
YYBACKUPCTX（）
为尾随上下文备份当前输入位置（仅与--input自定义选项一起使用）。
### yych
用于保存当前输入字符的YYCTYPE类型的L值。只有使用-f --stores-state选项，用户的显式定义才是必需的。
### YYCONDTYPE
条件标识符的类型（仅与-c --conditions选项一起使用）。应该使用/ *！类型：re2c * /指令或使用-t --type-header选项生成。
### YYCTXMARKER
类型YYCTYPE *的L值，用于备份尾部上下文的输入位置。只有在正则表达式使用lookahead操作符/时才需要。
### YYCTYPE
输入字符的类型（代码单位）。通常，对于ASCII，EBCDIC和UTF-8编码，它应该是无符号字符，对于UTF-16或UCS-2编码，无符号短符号，对于UTF-32编码，无符号整数。
### YYCURSOR
用作指向当前输入符号的指针的YYCTYPE *类型的L值。最初YYCURSOR指向第一个字符，并在匹配过程中由词法分析器进行提前。当规则匹配时，YYCURSOR指向匹配字符串的最后一个字符。
### YYDEBUG（状态，符号）
类似于函数的基元，用于转储调试信息（仅用于-d --debug-output选项）。 YYDEBUG应该不返回任何值并接受两个参数：state（或者词法分析状态或-1）和符号（当前输入符号）。
### YYFILL（n）
当没有足够的输入时，由词法分析器调用的类似函数的基元。 YYFILL应该不返回任何值并提供至少n个附加字符。 n的最大值等于YYMAXFILL，它可以用/ *！max：re2c * /指令获得。
### YYGETCONDITION（）
表示当前条件标识符的YYCONDTYPE类型的R值（仅与-c --conditions选项一起使用）。
### YYGETSTATE（）
表示当前词法分析器状态的带符号整数类型的R值（仅与-f --storable-state选项一起使用）。词法分析器状态的初始值应为-1。
### YYLESSTHAN（n）
当且仅当少于n个输入字符（仅与--input自定义选项一起使用）时，布尔类型的R值为true。
### YYLIMIT
标记输入结束的YYCTYPE *类型的R值（YYLIMIT [-1]应该是最后一个输入字符）。 Lexer比较YYCURSOR和YYLIMIT以确定是否有足够的输入字符。
### YYMARKER
用于备份成功匹配输入位置的YYCTYPE *类型的L值。如果存在可能也匹配的重叠较长规则，则这可能是必需的。
### YYMTAGP（t）
将当前输入位置追加到m-tag t的历史记录（仅与-T - 标签选项一起使用）。
### YYMTAGN（t）
将默认值附加到m-tag t的历史记录（仅与-T --tags选项一起使用）。
### YYMAXFILL
表示YYFILL参数最大值的整数常量，由/ *！max：re2c * /指令自动生成。
### YYMAXNMATCH
积分常量，表示规则中捕获组的最大数量，由/ *！maxnmatch：re2c * /指令自动生成（仅与--posix-capture选项一起使用）。
### yynmatch
无符号整数类型的L值，用于保存匹配规则中捕获组的数量。仅与-P --posix-captureures选项一起使用。
### YYPEEK（）
表示当前输入字符的YYCTYPE类型的R值（仅与--input自定义选项一起使用）。
### yypmatch
用于保存与匹配规则中的捕获括号相对应的s标签的值的l值的数组。数组长度必须至少为yynmatch * 2（理想情况下为YYMAXNMATCH * 2）。仅与-P --posix-captureures选项一起使用。
### YYRESTORE（）
恢复输入位置（仅与--input自定义选项一起使用）。
### YYRESTORECTX（）
从尾部上下文的值中恢复输入位置（仅与--input自定义选项一起使用）。
### YYRESTORETAG（t）
从s-tag t的值恢复输入位置（仅与--input自定义选项一起使用）。
### YYSETCONDITION（条件）
将当前条件标识符设置为条件（仅与-c --conditions选项一起使用）。
### YYSETSTATE（状态）
将当前词法分析状态设置为状态（仅与-f --storable-state选项一起使用）。参数状态是有符号整型。
### YYSKIP（）
将输入位置提前到下一个字符（仅用于通用API）。
### YYSTAGP（t）
将当前输入位置保存为s-tag t（仅与-T - 标签和 - 输入自定义选项一起使用）。
### YYSTAGN（t）
将缺省值保存为s-tag t（仅与-T - 标签和 - 输入自定义选项一起使用）。

##配置
----

### re2c：cgoto：threshold = 9;
使用-g --computed-gotos选项时，此值指定触发生成跳转表的复杂性阈值，而不是嵌套的if语句和位掩码。
### re2c：cond：divider ='/ * *********************************** * /' ;
允许自定义条件块的分频器。可以使用@@来插入条件名称。
### re2c：cond：divider @ cond = @@;
指定在re2c：cond：divider中将替换为条件名称的占位符。
### re2c：condenumprefix = yyc;
指定用于条件标识符的前缀。
### re2c：cond：goto @ cond = @@;
指定将在re2c：cond：goto中用条件标签替换的占位符。
### re2c：cond：goto ='goto @@;';
允许自定义用于：=>样式规则的goto语句。可以使用@@来插入条件名称。
### re2c：condprefix = yyc;
指定用于条件标签的前缀。
### re2c：define：YYBACKUPCTX ='YYBACKUPCTX';
用指定的字符串替换YYBACKUPCTX标识符。
### re2c：define：YYBACKUP ='YYBACKUP';
用指定的字符串替换YYBACKUP标识符。
### re2c：define：YYCONDTYPE ='YYCONDTYPE';
用于条件标识符的枚举类型。
### re2c：define：YYCTXMARKER ='YYCTXMARKER';
用指定的标识符替换YYCTXMARKER占位符。
### re2c：define：YYCTYPE ='YYCTYPE';
用指定的类型替换YYCTYPE占位符。
### re2c：define：YYCURSOR ='YYCURSOR';
用指定的标识符替换YYCURSOR占位符。
### re2c：define：YYDEBUG ='YYDEBUG';
用指定的标识符替换YYDEBUG占位符。
### re2c：define：YYFILL @ len ='@@';
在YYFILL中出现的任何此文本将被替换为实际参数。
### re2c：define：YYFILL：naked = 0;
在YYFILL和下面的分号之后控制括号中的参数。如果为零，则省略参数和分号。如果非零，则除非re2c：yyfill：参数设置为零;否则将生成参数。分号无条件生成。
### re2c：define：YYFILL ='YYFILL';
定义YYFILL的替代。默认情况下，re2c在YYFILL后面用括号和分号生成一个参数。如果您需要使YYFILL成为任意语句而不是调用，请将re2c：define：YYFILL：naked设置为非零值。
### re2c：define：YYGETCONDITION：naked = 0;
在YYGETCONDITION后控制括号。如果为零，则省略括号。如果非零，则会生成括号。
### re2c：define：YYGETCONDITION ='YYGETCONDITION';
替换YYGETCONDITION。默认情况下，re2c在YYGETCONDITION后生成括号。将re2c：define：YYGETCONDITION：naked设置为非零以省略括号。
### re2c：define：YYGETSTATE：naked = 0;
控制YYGETSTATE后面的括号。如果为零，则省略括号。如果非零，则生成它们。
### re2c：define：YYGETSTATE ='YYGETSTATE';
替换YYGETSTATE。默认情况下，re2c在YYGETSTATE之后生成括号。设置re2c：define：YYGETSTATE：裸到非零省略括号。
### re2c：define：YYLESSTHAN ='YYLESSTHAN';
用指定的字符串替换YYLESSTHAN标识符。
### re2c：define：YYLIMIT ='YYLIMIT';
用指定的标识符替换YYLIMIT占位符。
### re2c：define：YYMARKER ='YYMARKER';
用指定的标识符替换YYMARKER占位符。
### re2c：define：YYMTAGN ='YYMTAGN';
用指定的字符串替换YYMTAGN标识符。
### re2c：define：YYMTAGP ='YYMTAGP';
用指定的字符串替换YYMTAGP标识符。
### re2c：define：YYPEEK ='YYPEEK';
用指定的字符串替换YYPEEK标识符。
### re2c：define：YYRESTORECTX ='YYRESTORECTX';
用指定的字符串替换YYRESTORECTX标识符。
### re2c：define：YYRESTORE ='YYRESTORE';
用指定的字符串替换YYRESTORE标识符。
### re2c：define：YYRESTORETAG ='YYRESTORETAG';
用指定的字符串替换YYRESTORETAG标识符。
### re2c：define：YYSETCONDITION @ cond ='@@';
在YYSETCONDITION中出现的任何此文本将被替换为实际参数。
### re2c：define：YYSETCONDITION：naked = 0;
控制括号中的参数和YYSETCONDITION后的分号。如果为零，则省略参数和分号。如果非零，则会生成参数和分号。
re2c：define：YYSETCONDITION ='YYSETCONDITION';
替代YYSETCONDITION。默认情况下，re2c在YYSETCONDITION后面用括号分号生成一个参数。如果您需要使YYSETCONDITION成为任意语句而不是调用，请将re2c：define：YYSETCONDITION：naked设置为非零。
### re2c：define：YYSETSTATE：naked = 0;
控制括号中的参数和YYSETSTATE后的分号。如果为零，则省略参数和分号。如果非零，则会生成参数和分号。
### re2c：define：YYSETSTATE @ state ='@@';
在YYSETSTATE中出现的任何此文本将被替换为实际参数。
### re2c：define：YYSETSTATE ='YYSETSTATE';
替补YYSETSTATE。默认情况下，re2c在YYSETSTATE之后用括号和分号生成一个参数。如果您需要使YYSETSTATE成为任意语句而不是调用，请将re2c：define：YYSETSTATE：naked设置为非零。
### re2c：define：YYSKIP ='YYSKIP';
用指定的字符串替换YYSKIP标识符。
### re2c：define：YYSTAGN ='YYSTAGN';
用指定的字符串替换YYSTAGN标识符。
### re2c：define：YYSTAGP ='YYSTAGP';
用指定的字符串替换YYSTAGP标识符。
### re2c：flags：8或re2c：flags：utf-8
与-8 --utf-8命令行选项相同。
### re2c：flags：b或re2c：flags：位向量
与-b -bit-vectors命令行选项相同。
### re2c：flags：不区分大小写= 0;
与不区分大小写的命令行选项相同。
### re2c：flags：case-inverted = 0;
与--case-inverted命令行选项相同。
### re2c：flags：d或re2c：flags：调试输出
与-d --debug-output命令行选项相同。
### re2c：flags：dfa-minimization ='moore';
与--dfa-minimization命令行选项相同。
### re2c：flags：eager-skip = 0;
与--eager-skip命令行选项相同。
### re2c：flags：e或re2c：flags：ecb
与-e --ecb命令行选项相同。
### re2c：flags：empty-class ='match-empty';
与--empty-class命令行选项相同。
### re2c：flags：encoding-policy ='ignore';
与--encoding-policy命令行选项相同。
### re2c：flags：g或re2c：flags：computed-gotos
与-g --computed-gotos命令行选项相同。
### re2c：flags：我或者re2c：flags：no-debug-info
与-i --no-debug-info命令行选项相同。
### re2c：flags：input ='default';
与--input命令行选项相同。
### re2c：flags：lookahead = 1;
与倒转的--no-lookahead命令行选项相同。
### re2c：flags：optimize-tags = 1;
与倒排--no-optimize-tags命令行选项相同。
### re2c：flags：P或re2c：flags：posix-captures
与-P --posix-captures命令行选项相同。
### re2c：flags：s或re2c：flags：嵌套ifs
与-s --nested-ifs命令行选项相同。
### re2c：flags：T或者re2c：flags：标签
与-T --tags命令行选项相同。
### re2c：flags：u或re2c：flags：unicode
与-u --unicode命令行选项相同。
### re2c：flags：w或re2c：flags：宽字符
与-w --wide-chars命令行选项相同。
### re2c：flags：x或re2c：flags：utf-16
与-x --utf-16命令行选项相同。
### re2c：indent：string ='\ t';
指定用于缩进的字符串。需要一个只包含空格的字符串（除非需要其他外部工具）。指定空格的最简单方法是将它们用单引号或双引号括起来。如果你不想要任何缩进，可以将其设置为“'。
re2c：indent：top = 0;
指定要使用的最小缩进量。要求数值大于或等于零。
### re2c：labelprefix ='yy';
允许更改编号标签的前缀。默认是yy。可以设置任何在标签名称中有效的字符串。
### re2c：label：yyFillLabel ='yyFillLabel';
覆盖yyFillLabel标签的名称。
### re2c：label：yyNext ='yyNext';
覆盖yyNext标签的名称。
### re2c：startlabel = 0;
如果设置为非零整数，则即使扫描仪本身未使用，也会生成下一个扫描仪块的开始标签。否则，只有在需要时才会生成正常的类似yy0的开始标签。如果设置为文本值，则无论是否使用正常起始标签，都会生成包含该文本的标签。在生成开始标签后，该设置重置为0。
### re2c：状态：abort = 0;
如果不为零，且-f --storable-state开关处于活动状态，则YYGETSTATE块将包含中止的缺省情况，并将使用-1情况进行初始化。
### re2c：state：nextlabel = 0;
当-f --storable-state处于活动状态时用于控制YYGETSTATE块是否后接yyNext：标签线。除了使用yyNext之外，通常还可以使用配置startlabel来强制特定的开始标签或默认为yy0作为开始标签。通过放置/ *！getstate：re2c * /注释来代替使用专用标签，通常最好将YYGETSTATE代码与实际的扫描程序代码分开。
### re2c：tags：expression ='@@';
允许定制re2c地址标记变量的方式：默认情况下，它会发出形式为yyt <N>的表达式，但如果标记变量被定义为结构体中的字段，或者由于任何其他原因需要特殊访问器，这可能会很不方便。例如，设置re2c：tags：expression = p - > @@将导致p-> yyt <N>。
### re2c:tags:prefix = 'yyt';
Allows to override prefix of tag variables.
### re2c:variable:yyaccept = yyaccept;
Overrides the name of the yyaccept variable.
### re2c:variable:yybm = 'yybm';
Overrides the name of the yybm variable.
### re2c:variable:yych = 'yych';
Overrides the name of the yych variable.
### re2c:variable:yyctable = 'yyctable';
When both -c --conditions and -g --computed-gotos are active, re2c will use this variable to generate a static jump table for YYGETCONDITION.
### re2c：variable：yystable ='yystable';
已过时。
re2c：variable：yytarget ='yytarget';
覆盖yytarget变量的名称。
### re2c：yybm：hex = 0;
如果设置为零，将使用小数表。否则，将生成一个十六进制表。
### re2c：yych：conversion = 0;
当此设置不为零时，只要yych被读取，re2c就会自动生成转换代码。在这种情况下，类型必须使用`re2c：define：YYCTYPE`定义。
### re2c：yych：emit = 1;
将其设置为零以抑制yych的生成。
### re2c：yyfill：check = 1;
这可以设置为0来抑制基于YYCURSOR和YYLIMIT的前提条件检查的生成。当YYLIMIT + YYMAXFILL始终可用时，此选项非常有用。
### re2c：yyfill：enable = 1;
将其设置为零以抑制YYFILL（n）的生成。在使用此功能时，请务必确认生成的扫描程序不会超出可用输入的范围，因为允许此类行为可能会给您的程序带来严重的安全问题。
### re2c：yyfill：parameter = 1;
控制YYFILL后面括号中的参数。如果为零，则省略参数。如果非零，则会生成参数，除非re2c：define：YYFILL：naked被设置为非零。

