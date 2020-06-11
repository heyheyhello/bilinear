#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

// typedef struct {
//   uint32_t* pixels;
//   unsigned int w;
//   unsigned int h;
// } image_t;

// #define getByte(value, n) (value >> (n * 8) & 0xFF)

// uint32_t getpixel(image_t* image, unsigned int x, unsigned int y) {
//   return image->pixels[(y * image->w) + x];
// }

// float lerp(float s, float e, float t) {
//   return s + (e - s) * t;
// }

// float blerp(float c00, float c10, float c01, float c11, float tx, float ty) {
//   return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
// }

// void putpixel(image_t* image, unsigned int x, unsigned int y, uint32_t color) {
//   image->pixels[(y * image->w) + x] = color;
// }

// void scale(image_t* src, image_t* dst, float scalex, float scaley) {
//   int newWidth = (int)src->w * scalex;
//   int newHeight = (int)src->h * scaley;
//   int x, y;
//   for (x = 0, y = 0; y < newHeight; x++) {
//     if (x > newWidth) {
//       x = 0;
//       y++;
//     }
//     float gx = x / (float)(newWidth) * (src->w - 1);
//     float gy = y / (float)(newHeight) * (src->h - 1);
//     int gxi = (int)gx;
//     int gyi = (int)gy;
//     uint32_t result = 0;
//     uint32_t c00 = getpixel(src, gxi, gyi);
//     uint32_t c10 = getpixel(src, gxi + 1, gyi);
//     uint32_t c01 = getpixel(src, gxi, gyi + 1);
//     uint32_t c11 = getpixel(src, gxi + 1, gyi + 1);
//     uint8_t i;
//     for (i = 0; i < 3; i++) {
//       result |=
//           (uint8_t)blerp(getByte(c00, i), getByte(c10, i), getByte(c01, i),
//                          getByte(c11, i), gx - gxi, gy - gyi)
//           << (8 * i);
//     }
//     putpixel(dst, x, y, result);
//   }
// }

// Grayscale conversion (Still BRG code format (not a typo of RGB))

static int RGB2Gray(char red, char green, char blue) {
  // this is a commonly used formula
  double gray = 0.2989 * red + 0.5870 * green + 0.1140 * blue;
  return (int)gray;
}

void BMP_gray(BMP_Image* img) {
  int pxl;
  for (pxl = 0; pxl < (img->data_size); pxl += 3) {
    unsigned char gray = RGB2Gray(img->data[pxl + 2], img->data[pxl + 1], img->data[pxl]);
    img->data[pxl + 2] = gray;
    img->data[pxl + 1] = gray;
    img->data[pxl] = gray;
  }
}

int main(int argc, char** argv) {
  // Check arguments
  if (argc < 3) {
    printf("Usage: bmp.o <input> <output>\n");
    return EXIT_FAILURE;
  }

  // open the input file
  BMP_Image* img = BMP_open(argv[1]);
  if (img == NULL) {
    return EXIT_FAILURE;
  }

  // We'll change this out for our interpolation method later
  BMP_gray(img);

  if (BMP_save(img, argv[2]) == 0) {
    printf("Failed writing\n");
    BMP_destroy(img);
    return EXIT_FAILURE;
  }
  BMP_destroy(img);
  return EXIT_SUCCESS;
}
