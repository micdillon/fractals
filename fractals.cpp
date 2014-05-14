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

void print_usage(const char *argv[])
{
    std::cout << "Usage: " << argv[0] << " <out_file>" << std::endl;
}

#define ITERATIONS 100
bool is_convergent(double x, double y)
{
    double a = -1, b = 0.0;
    for (int i=0; i<ITERATIONS; i++) {
        x = x*x - y*y + a;
        y = 2.0*x*y + b;
        if (x*x + y*y > 25.0) {
            return false;
        }
    }
    return true;
}

#define SUBDIVISIONS 100
double get_convergence_count(double x0, double x1, double y0, double y1)
{
    double value = 1.0;
    for (int i=0; i<SUBDIVISIONS; i++) {
        for (int j=0; j<SUBDIVISIONS; j++) {
            double x = ((double)i / (double)SUBDIVISIONS) * (x1 - x0) + x0;
            double y = ((double)j / (double)SUBDIVISIONS) * (y1 - y0) + y0;
            if (is_convergent(x,y)) {
                value -= (1.0 / (double)(SUBDIVISIONS * SUBDIVISIONS));
            }
        }
    }
    return value;
}

#define IMG_SIZE 1024*20
#define SCALE 0.075
#define X_OFFSET 6.5
#define Y_OFFSET 6.5

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

    CImg<double> img(IMG_SIZE,IMG_SIZE,1,3,0);

    for (int i=0; i<IMG_SIZE-1; i++) {
        for (int j=0; j<IMG_SIZE-1; j++) {
            double x0 = ((double)i / (double)IMG_SIZE) * SCALE - X_OFFSET;
            double x1 = ((double)(i+1) / (double)IMG_SIZE) * SCALE - X_OFFSET;
            double y0 = ((double)j / (double)IMG_SIZE) * SCALE - Y_OFFSET;
            double y1 = ((double)(j+1) / (double)IMG_SIZE) * SCALE - Y_OFFSET;
            double val = get_convergence_count(x0,x1,y0,y1);

            img(i,j,0) = img(i,j,1) = img(i,j,2) = val * 255.0;
        }
        std::cout << "." << std::flush;
    }
    std::cout << std::endl;

    std::cout << "saving: " << filename << std::endl;
    img.save_png(filename, 3 * sizeof(double));

    return 0;
}
