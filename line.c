#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <stddef.h>
#include "siml.h"

#define ABS(x)  (x < 0 ? -x : x)
#define NEG(x)  (x < 0 ? 1 : 0)

void
swap(void *a, void *b, size_t len) {
    unsigned char *pa = a;
    unsigned char *pb = b;
    unsigned char tmp;

    for (size_t i = 0; i != len; ++i) {
        tmp = pa[i];
        pa[i] = pb[i];
        pb[i] = tmp;
    }
}

void
draw_line(Image *image, Color color, 
          int x0, int y0, int x1, int y1)
{
    // Account for vector with negative direction
    if (x1 < x0) {
        swap(&x0, &x1, sizeof(int));
        swap(&y0, &y1, sizeof(int));
    } 

    int dx = x1 - x0;
    int dy = y1 - y0;
    int err = 0;
    int y = y0;
    int x = x0;
    int stepx, stepy;
    stepx = stepy = 1;

    if (dx < 0) { dx = -dx; stepx = -1; }
    if (dy < 0) { dy = -dy; stepy = -1; }

    // if the slope is greater than 1:
    if (dy > dx) {
        for (y = y0; y <= y1; y+=stepy) {
            plot(x, y, image, color);
            err += dx;
            if ((err << 1) >= dy) {
                x += stepx;
                err -= dy;
            }
        }
    }
    // if the slope is less than or equal to on, handles positive 
    // and negative slope thanks to the swap at the start
    else {
        for (x = x0; x <= x1; x+=stepx) {
            plot(x, y, image, color);
            err += dy;
            if ((err << 1) >= dx) {
                y += stepy;
                err -= dx;
            }
        }
    }
}

void orient_graphically(Image *image) {
    /* This function takes an image and re-orients it, 
     * so that increasing y moves upward, rather than the
     * default within siml (and most image libraries) where y 
     * increases downward, used to make reasoning about the octents
     * a little simpler.
     */

    Image copy = {
        .width  = image->width,
        .height = image->height,
        .bpp    = image->bpp,
        .data   = malloc(image->width * image->height * image->bpp),
        .size   = image->width * image->height * image->bpp
    };

    memcpy(copy.data, image->data, image->size);

    for (int row = 0, pixel = 0; pixel + row < image->size; ++pixel) {
        image->data[pixel + row + 1] = 
            copy.data[(row + (image->width * image->bpp)) - pixel + 1];

        if (pixel == image->width * image->bpp) {
            pixel = 0;
            row += image->width * image->bpp;
        }
    }

    memcpy(copy.data, image->data, image->size);

    for (int i = 0; i < image->size; ++i) {
        image->data[i] = copy.data[image->size - 1 - i];
    }

    free(copy.data);
}

void 
naive_line(Image *image, Color color, int x0, int y0, int x1, int y1) 
{
    int w = x1 - x0;
    int h = y1 - y0;
    double m = h/(double)w;
    double y = y0;
    for (int x = x0; x <= x1; ++x) {
        plot(x, y, image, color);
        y += m; 
    }
}

int
main()
{
    int width = 800;
    int height = 800;
    int bpp = 3;
    Color color = { 0, 0, 0 };

    Image image  = {
        .width  = width,
        .height = height,
        .bpp    = bpp,
        .data   = malloc(width * height * bpp),
        .size   = width * height * bpp
    };

    memset(image.data, 0xff, image.size);

    // Horizontal and vertical lines
    draw_line(&image, color, 400, 400, 400, 800); // Vertical line from   (400, 400) towards larger y       -- WORKING
    draw_line(&image, color, 400, 400, 0, 400);   // Horizontal line from (400, 400) towards smaller x      -- WORKING
    draw_line(&image, color, 400, 400, 800, 400); // Horizontal line from (400, 400) towards larger x       -- WORKING
    draw_line(&image, color, 400, 400, 400, 0);   // Vertical line from   (400, 400) towards smaller y      -- WORKING

    // Positive slope, p1 to the right of and above p0
    draw_line(&image, color, 400, 400, 600, 800); // positive large slope with [1 < m < infinity]           -- WORKING
    draw_line(&image, color, 400, 400, 800, 800); // positive slope with [m == 1]                           -- WORKING
    draw_line(&image, color, 400, 400, 800, 600); // positive small slope with [0 < m < 1]                  -- WORKING

    // Positive slope, p1 to the left of and below of p0
    draw_line(&image, color, 400, 400, 200, 0);   // positive large slope with [1 < m < infinity], x1 < x0  -- WORKING
    draw_line(&image, color, 400, 400, 0, 0);     // positive slope with [m == 1], x1 < x0                  -- WORKING
    draw_line(&image, color, 400, 400, 0, 200);   // positive small slope with [0 < m < 1], x1 < x0         -- WORKING

    // Negative slope, p1 to the right of and below p0
    draw_line(&image, color, 400, 400, 800, 0);   // negative slope with [m == 1]                           -- WORKING
    draw_line(&image, color, 400, 400, 800, 200); // negative small slope with [ -1 < m < 0]                -- WORKING
    draw_line(&image, color, 400, 400, 600, 0);   // negative large slope with [ -infinity < m < -1]        -- WORKING

    // Negative slope, p1 to the left of and above p0
    draw_line(&image, color, 400, 400, 0, 800);   // negative slope with [m == 1]                           -- WORKING
    draw_line(&image, color, 400, 400, 0, 600);   // negative small slope with [ -1 < m < 0]                -- WORKING
    draw_line(&image, color, 200, 800, 400, 400); // negative small slope with [ -1 < m < 0]                -- WORKING

    orient_graphically(&image);

    stbi_write_png("output.png", 
                    image.width, 
                    image.height, 
                    image.bpp, 
                    image.data, 
                    image.width * image.bpp);

    free(image.data);
}
