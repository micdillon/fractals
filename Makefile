all: fractals

fractals: fractals.cpp
	clang++ -std=c++11 -Wall -W -pedantic -Dcimg_display=0 -O3 -lm fractals.cpp -o fractals

clean:
	rm -rf fractals *.o *.png
