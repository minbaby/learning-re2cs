re2c -o integers.cc integers.re
g++ -o bin-run integers.cc
./bin-run 123 0xfF 010 0B101 ?