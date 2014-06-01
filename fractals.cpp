/******************************************************************************
 * Fractals (Explore factals for our wedding ring.)
 *
 * TODO: Implement a multi-threaded version.
 * TODO: Look into making a gpu accellerated version.
 * TODO: Implement mandelbot set iteration.
 *
 *****************************************************************************/
#include "CImg.h"
#include <iostream>
#include <sstream>

using namespace cimg_library;

struct color {
    double r,g,b;
    color(double _rgb) {
        r = g = b = _rgb;
    }
    color(double _r, double _g, double _b) :
        r(_r), g(_g), b(_b)
    { }
};

void print_usage(const char *argv[])
{
    std::cout << "Usage: " << argv[0] << " <out_file>" << std::endl;
}

#define IMG_SIZE (2048*8)
#define SCALE 4.0
#define X_OFFSET 0.0
#define Y_OFFSET 0.0

#define ITERATIONS 200
color get_julia_color(double a, double b, double x, double y)
{
    for (int i=0; i<ITERATIONS; i++) {
        if (x*x + y*y > 4.0) {
            double grey = (double)(ITERATIONS - i) / (double)ITERATIONS;
            return color(grey);
        }
        double xx = x*x - y*y + a;
        double yy = 2.0*x*y + b;
        x = xx; y = yy;
    }
    return color(0, 0, 0);
}

int main(int argc, const char *argv[])
{
    double a = 0.0;
    double b = 0.0;
    const char *filename = NULL;

    // Process the commandline.
    switch (argc) {
        case 2: {
            filename = argv[1];
            break;
        }
        case 4: {
            std::istringstream a_ss(argv[1]); a_ss >> a;
            std::istringstream b_ss(argv[2]); b_ss >> b;
            filename = argv[3];
            break;
        }
        default: {
            print_usage(argv);
            return 0;
        }
    }

    CImg<double> img(IMG_SIZE, IMG_SIZE, 1, 3, 0);

    for (int i=0; i<IMG_SIZE; i++) {
        double x0 = ((double)i / (IMG_SIZE-1)) * SCALE - SCALE * 0.5 - X_OFFSET;
        for (int j=0; j<IMG_SIZE; j++) {
            double y0 = ((double)j / (IMG_SIZE-1)) * SCALE - SCALE * 0.5
                    - Y_OFFSET;
            color c = get_julia_color(a, b, -x0, -y0);
            img(i,j,0) = c.r * 2000.0;
            img(i,j,1) = c.g * 2000.0;
            img(i,j,2) = c.b * 2000.0;
        }
        std::cout << "." << std::flush;
    }
    std::cout << std::endl;

    std::cout << "saving: " << filename << std::endl;
    img.save_png(filename, 3 * sizeof(double));

    return 0;
}
