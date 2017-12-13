#ifndef SIML_H
#define SIML_H
#define MAX_COLOR_DEPTH_BYTES 4


typedef struct {
    int height;
    int thickness;
    int x, y;
} Rect;

typedef struct {
    int width;
    int height;
    int bpp;
    int size;
    unsigned char* data;
} Image;

typedef unsigned char Color[MAX_COLOR_DEPTH_BYTES];

int   broken_line_diff      (int x0, int y0, int radius, Image *source, Image * copy, int mode);
float calc                  (Image *source, Image *copy, int x, int y);
int   circ_diff             (int x0, int y0, int radius, Image *source, Image * copy, int mode);
void  draw_circle           (int h, int k, int r, int filled, Color c, Image *image);
void  draw_cross            (int h, int k, int r,  Color c, Image *image);
void  draw_elipse           (int h, int k, int r, int filled, Color c, Image *image);
void  draw_rect             (int x, int y, int w, int h, Color c, Image *image);
int   filter                (int x, int y, Image *image, Color color, int cv);
void  filter_circle         (int h, int k, int r, int filled, int cv, Color c, Image *image);
unsigned char** full_scan   (Image *image);
void  grid_scan             (Image *image, unsigned char colors[][3], int numc);
void  midpoint_circle       (int ux, int uy, int radius, int filled, Color c, Image *image);
void  midpoint_diff         (int x0, int y0, int radius, Image *source, Image *copy);
int   plot                  (int x, int y, Image *image, Color color);
void  sample_point          (Image *image, unsigned char color[], int x, int y);
void  siml_free             (unsigned char **colors, unsigned long nmemb);
void  sloppy_scan           (Image *image, unsigned char colors[][3], int numc);
#endif
