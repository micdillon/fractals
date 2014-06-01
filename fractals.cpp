/******************************************************************************
 * Fractals (Explore factals for our wedding ring.)
 *
 * TODO: Implement a multi-threaded version.
 * TODO: Look into make a gpu accellerated version.
 *
 *****************************************************************************/
#include "CImg.h"
#include <iostream>

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

#define A -0.6615
#define B 0.3788

#define IMG_SIZE (2048*8)
#define SCALE 4.0
#define X_OFFSET 0.0
#define Y_OFFSET 0.0

#define ITERATIONS 200
color get_color(double x, double y)
{
    for (int i=0; i<ITERATIONS; i++) {
        if (x*x + y*y > 4.0) {
            double grey = (double)(ITERATIONS - i) / (double)ITERATIONS;
            return color(grey);
        }
        double xx = x*x - y*y + A;
        double yy = 2.0*x*y + B;
        x = xx; y = yy;
    }
    return color(0, 0, 0);
}

int main(int argc, const char *argv[])
{
    const char *filename = NULL;

    // Process the commandline.
    switch (argc) {
        case 2:
            filename = argv[1];
            break;
        default:
            print_usage(argv);
            return 0;
    }

    CImg<double> img(IMG_SIZE, IMG_SIZE, 1, 3, 0);

    for (int i=0; i<IMG_SIZE; i++) {
        double x0 = ((double)i / (IMG_SIZE-1)) * SCALE - SCALE * 0.5 - X_OFFSET;
        for (int j=0; j<IMG_SIZE; j++) {
            double y0 = ((double)j / (IMG_SIZE-1)) * SCALE - SCALE * 0.5
                    - Y_OFFSET;
            color c = get_color(-x0, -y0);
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
