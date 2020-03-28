g++ -c main.cpp rect/rect.cpp -std=c++14
windres --input snapback.rc --output snapback.res --output-format=coff
g++ main.o rect.o snapback.res -o snapback.exe
