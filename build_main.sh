python3 build_library.py
g++ main.cpp -Lraytrace -lraytrace -fopenmp -o out -std=c++17 -Ofast
echo output of ./out:
./out