re2c -o integers.cc integers.re
g++ -o integers.out integers.cc
./integers.out 123 0xfF 010 0B101 ?