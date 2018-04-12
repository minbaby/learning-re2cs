NAME="strings"

rm ./$NAME.out
re2c -o $NAME.cc $NAME.re
g++ -o $NAME.out $NAME.cc

./$NAME.out '"a momentary"' '""' '"lap"se"' '"of' '"' '"rea""son"' ''
