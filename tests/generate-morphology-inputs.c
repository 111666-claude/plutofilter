/*
 * Generates the synthetic morphology reference fixtures under examples/ (alongside the other
 * filter inputs):
 *
 *   morphology-bright-spot.png  a single bright pixel on a dark background
 *   morphology-line-art.png     opaque lines and a soft-alpha shape on transparency
 *   morphology-edge-map.png     a one-pixel rectangular frame on a black background
 *
 * The pixels are written as plain straight-alpha RGBA; example__load_input() performs the
 * sRGB-to-linear and premultiply steps when a filter consumes them, matching every other
 * fixture in this repository.
 */

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../examples/stb_image_write.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define FIXTURE_SIZE 64

static void set_pixel(uint8_t* image, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    uint8_t* pixel = image + (y * FIXTURE_SIZE + x) * 4;
    pixel[0] = r;
    pixel[1] = g;
    pixel[2] = b;
    pixel[3] = a;
}

static void fill_rect(uint8_t* image, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    for(int y = y0; y < y1; y++)
        for(int x = x0; x < x1; x++)
            set_pixel(image, x, y, r, g, b, a);
}

int main(int argc, char* argv[])
{
    if(argc != 2)
        return 1;

    const char* output_dir = argv[1];
    char path[512];

    /* Bright spot: one white pixel at the center of a uniform dark gray field. */
    uint8_t* spot = calloc(FIXTURE_SIZE * FIXTURE_SIZE, 4);
    fill_rect(spot, 0, 0, FIXTURE_SIZE, FIXTURE_SIZE, 32, 32, 32, 255);
    set_pixel(spot, FIXTURE_SIZE / 2, FIXTURE_SIZE / 2, 255, 255, 255, 255);
    snprintf(path, sizeof(path), "%s/morphology-bright-spot.png", output_dir);
    stbi_write_png(path, FIXTURE_SIZE, FIXTURE_SIZE, 4, spot, FIXTURE_SIZE * 4);
    free(spot);

    /* Transparent line art: fully opaque cross strokes plus a soft-alpha square. */
    uint8_t* lines = calloc(FIXTURE_SIZE * FIXTURE_SIZE, 4);
    fill_rect(lines, 30, 8, 34, 56, 200, 40, 40, 255);   /* vertical stroke */
    fill_rect(lines, 8, 30, 56, 34, 40, 80, 220, 255);   /* horizontal stroke */
    fill_rect(lines, 44, 44, 58, 58, 40, 200, 90, 128);  /* translucent block */
    snprintf(path, sizeof(path), "%s/morphology-line-art.png", output_dir);
    stbi_write_png(path, FIXTURE_SIZE, FIXTURE_SIZE, 4, lines, FIXTURE_SIZE * 4);
    free(lines);

    /* Edge map: a one-pixel white rectangular frame, black and fully opaque elsewhere. */
    uint8_t* edges = calloc(FIXTURE_SIZE * FIXTURE_SIZE, 4);
    fill_rect(edges, 0, 0, FIXTURE_SIZE, FIXTURE_SIZE, 0, 0, 0, 255);
    fill_rect(edges, 8, 8, 56, 9, 255, 255, 255, 255);    /* top edge */
    fill_rect(edges, 8, 55, 56, 56, 255, 255, 255, 255);  /* bottom edge */
    fill_rect(edges, 8, 8, 9, 56, 255, 255, 255, 255);    /* left edge */
    fill_rect(edges, 55, 8, 56, 56, 255, 255, 255, 255);  /* right edge */
    snprintf(path, sizeof(path), "%s/morphology-edge-map.png", output_dir);
    stbi_write_png(path, FIXTURE_SIZE, FIXTURE_SIZE, 4, edges, FIXTURE_SIZE * 4);
    free(edges);

    return 0;
}
