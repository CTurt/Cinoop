#pragma once

#define RGB(r, g, b) (((b) >> 3) + (((g) & 0x1C) << 3) + (((((g) & 0xE0) >> 5) + ((r) & 0xF8)) << 8))

inline void clearScreen(void);

#define drawPixelRGB(x, y, r, g, b) drawPixelRGBFramebuffer(0, x, y, r, g, b)
void drawPixelRGBFramebuffer(u8 *fb, int x, int y, u8 r, u8 g, u8 b);

#define drawBox(x, y, width, height, r, g, b) drawBoxFramebuffer(0, x, y, width, height, r, g, b)
inline void drawBoxFramebuffer(u8 *fb, int x, int y, int width, int height, u8 r, u8 g, u8 b);

#define drawString(sx, sy, text, ...) drawStringFramebuffer(0, sx, sy, text, ##__VA_ARGS__)
void drawStringFramebuffer(u8 *fb, int sx, int sy, char *text, ...);
