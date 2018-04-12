NAME="integers"

rm ./$NAME.out
re2c -o $NAME.cc $NAME.re
g++ -o $NAME.out $NAME.cc


DATA="123 0x121 0b1212 010 0B101"

for X in $DATA;
do
    echo "\nTest $X"
    ./$NAME.out $X
done