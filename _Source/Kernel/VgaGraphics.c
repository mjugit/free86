#include "Include/VgaGraphics.h"


/**
 * @brief The width of a single character in pixels for the monospace font.
 * @def CHAR_WIDTH
 *
 * This constant defines the horizontal size of each character in the
 * 8x8 monospace font used for rendering text in VGA modes.
 * Each character is exactly 8 pixels wide.
 */
#define CHAR_WIDTH 8



/**
 * @brief The number of bytes per row in VGA planar graphics mode.
 *
 * In VGA planar graphics mode, each row of pixels is represented by
 * a number of bytes. Each byte encodes 8 pixels (1 bit per pixel per plane),
 * and the total bytes per row is calculated by dividing the screen width
 * (in pixels) by 8.
 */
static const uint16_t VGA_BYTES_PER_ROW = VGA_SCREEN_X / 8;


/**
 * @brief 8x8 pixel monospace font bitmap for ASCII characters.
 *
 * This array contains the 8x8 pixel bitmaps for the first 128 ASCII characters.
 * Each character is represented by 8 bytes, where each byte corresponds to
 * one row of the character. Each bit in a byte represents a single pixel:
 * - `1` indicates that the pixel is "on".
 * - `0` indicates that the pixel is "off".
 *
 * The character encoding follows the standard ASCII table:
 * - Index `0x00` corresponds to the null character (NUL).
 * - Index `0x20` corresponds to a space (' ').
 * - Index `0x41` corresponds to the letter 'A'.
 *
 * This font is commonly used for text rendering in VGA modes that support
 * character-based or planar graphics rendering.
 */
static const uint8_t _MonospaceFont8x8[128][8] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0000 (nul)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0001
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0002
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0003
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0004
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0005
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0006
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0007
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0008
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0009
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0010
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0011
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0012
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0013
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0014
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0015
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0016
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0017
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0018
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0019
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0020 (space)
    { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},   // U+0021 (!)
    { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0022 (")
    { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},   // U+0023 (#)
    { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00},   // U+0024 ($)
    { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00},   // U+0025 (%)
    { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00},   // U+0026 (&)
    { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0027 (')
    { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},   // U+0028 (()
    { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},   // U+0029 ())
    { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},   // U+002A (*)
    { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00},   // U+002B (+)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+002C (,)
    { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00},   // U+002D (-)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+002E (.)
    { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},   // U+002F (/)
    { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},   // U+0030 (0)
    { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},   // U+0031 (1)
    { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},   // U+0032 (2)
    { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},   // U+0033 (3)
    { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},   // U+0034 (4)
    { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},   // U+0035 (5)
    { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},   // U+0036 (6)
    { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},   // U+0037 (7)
    { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+0038 (8)
    { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},   // U+0039 (9)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+003A (:)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+003B (;)
    { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00},   // U+003C (<)
    { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00},   // U+003D (=)
    { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},   // U+003E (>)
    { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00},   // U+003F (?)
    { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00},   // U+0040 (@)
    { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},   // U+0041 (A)
    { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},   // U+0042 (B)
    { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},   // U+0043 (C)
    { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},   // U+0044 (D)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},   // U+0045 (E)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00},   // U+0046 (F)
    { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00},   // U+0047 (G)
    { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},   // U+0048 (H)
    { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0049 (I)
    { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},   // U+004A (J)
    { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},   // U+004B (K)
    { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00},   // U+004C (L)
    { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},   // U+004D (M)
    { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},   // U+004E (N)
    { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},   // U+004F (O)
    { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},   // U+0050 (P)
    { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00},   // U+0051 (Q)
    { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00},   // U+0052 (R)
    { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00},   // U+0053 (S)
    { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0054 (T)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00},   // U+0055 (U)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0056 (V)
    { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},   // U+0057 (W)
    { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},   // U+0058 (X)
    { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},   // U+0059 (Y)
    { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},   // U+005A (Z)
    { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00},   // U+005B ([)
    { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},   // U+005C (\)
    { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00},   // U+005D (])
    { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},   // U+005E (^)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},   // U+005F (_)
    { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0060 (`)
    { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00},   // U+0061 (a)
    { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00},   // U+0062 (b)
    { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00},   // U+0063 (c)
    { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00},   // U+0064 (d)
    { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00},   // U+0065 (e)
    { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00},   // U+0066 (f)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0067 (g)
    { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00},   // U+0068 (h)
    { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0069 (i)
    { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E},   // U+006A (j)
    { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},   // U+006B (k)
    { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+006C (l)
    { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00},   // U+006D (m)
    { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00},   // U+006E (n)
    { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+006F (o)
    { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},   // U+0070 (p)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},   // U+0071 (q)
    { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00},   // U+0072 (r)
    { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00},   // U+0073 (s)
    { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   // U+0074 (t)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},   // U+0075 (u)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0076 (v)
    { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00},   // U+0077 (w)
    { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},   // U+0078 (x)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0079 (y)
    { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},   // U+007A (z)
    { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00},   // U+007B ({)
    { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},   // U+007C (|)
    { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00},   // U+007D (})
    { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+007E (~)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    // U+007F
};



/**
 * @brief Pointer to the starting address of the VGA video memory buffer.
 *
 * This pointer references the beginning of the VGA framebuffer memory,
 * allowing direct pixel manipulation. Each bit corresponds to a specific
 * pixel, since we use planar graphics mode with 4 bit color depth.
 */
static volatile uint8_t *_VideoMemory = (uint8_t*) VIDEO_MEMORY_ADDRESS;




/**
 * @brief Writes a single byte value to the specified VGA port.
 *
 * This function outputs an 8-bit value to a given VGA I/O port.
 * It uses inline assembly with the `outb` instruction to send
 * the specified value to the given port.
 *
 * @param port The VGA port to which the byte will be written.
 * @param value The 8-bit value to be sent.
 */
inline void _VGA_WriteByte(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}


/**
 * @brief Reads a single byte from the specified VGA port.
 *
 * This function retrieves an 8-bit value from a given VGA I/O port.
 * It uses inline assembly with the `inb` instruction to read the
 * byte from the provided port.
 *
 * @param port The VGA port from which the byte will be read.
 * @return The 8-bit value read from the specified port.
 */
static inline unsigned char _VGA_ReadByte(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}




/**
 * @brief Sets the VGA plane mask register.
 *
 * The plane mask determines which VGA memory planes are writable.
 *
 * @param planeMask A bitmask where each bit enables or disables
 *                  writing to a specific memory plane (0-3).
 */
void VGA_SetPlaneMask(uint8_t planeMask) {
  // Sequencer index for the plane mask register.
  const uint8_t PLANE_MASK_INDEX = 0x02;
  
  _VGA_WriteByte(VGA_SEQUENCER_INDEX, PLANE_MASK_INDEX);
  _VGA_WriteByte(VGA_SEQUENCER_DATA, planeMask);
}


/**
 * @brief Sets the VGA bit mask register.
 *
 * The bit mask determines which bits within a memory plane are affected
 * during write operations. It allows selective manipulation of pixel data
 * at a finer granularity than the plane mask.
 *
 * @param bitMask A bitmask where each bit enables or disables writing
 *                to a specific bit position (0-7) within the targeted plane.
 */
void VGA_SetBitMask(uint8_t bitMask) {
  // Graphics controller index for the bit mask register.
  const uint8_t BITMASK_INDEX = 0x08;

  _VGA_WriteByte(VGA_GRAPHICS_INDEX, BITMASK_INDEX);
  _VGA_WriteByte(VGA_GRAPHICS_DATA, bitMask);
}




/**
 * @brief Sets a color in the VGA palette.
 *
 * This function updates a specific color in the VGA Digital-to-Analog Converter (DAC) palette.
 * VGA palette colors are defined using 6-bit values for each of the red, green, and blue channels.
 *
 * @param index The palette index (0-255) to be updated.
 * @param red64 The intensity of the red channel (0-63).
 * @param green64 The intensity of the green channel (0-63).
 * @param blue64 The intensity of the blue channel (0-63).
 */
void VGA_SetPaletteColor(uint8_t index, uint8_t red64, uint8_t green64, uint8_t blue64) {
  _VGA_WriteByte(VGA_DAC_INDEX, index);
  _VGA_WriteByte(VGA_DAC_DATA, red64);
  _VGA_WriteByte(VGA_DAC_DATA, green64);
  _VGA_WriteByte(VGA_DAC_DATA, blue64);
}


/**
 * @brief Updates the color mapping between the 16-color palette and the 256 colors available in VGA.
 *
 * This function assigns a specific color from the 256-color VGA palette to one of the 16 hardware
 * palette entries used for indexed color modes. It enables and configures the VGA attribute controller
 * for this purpose.
 *
 * @param color The 5-bit index (0-31) of the hardware palette entry to redefine.
 * @param paletteIndex The 8-bit index (0-255) of the color in the VGA palette to assign.
 *
 * @remark The calls to `_VGA_ReadByte()` are necessary to synchronize the VGA controller
 *         and ensure the attribute controller is ready for configuration.
 */
void VGA_PickColor(color_t color, uint8_t paletteIndex) {
    // Synchronize with the VGA controller by reading the status port
    _VGA_ReadByte(VGA_STATUS);

    // Redefine the color mapping
    _VGA_WriteByte(VGA_ATTRIBUTE_INDEX, (color & 0x1f));
    _VGA_WriteByte(VGA_ATTRIBUTE_INDEX, (paletteIndex & 0xff));

    // Enable video output by setting the video enable bit
    _VGA_ReadByte(VGA_STATUS);
    _VGA_WriteByte(VGA_ATTRIBUTE_INDEX, 0x20);
}




/**
 * @brief Sets a block of pixels in VGA planar graphics mode.
 *
 * This function modifies a block of pixels at a specified memory offset.
 * It uses a bitmask to determine which bits (pixels) within a byte should
 * be affected and assigns them a specified color. The function handles
 * both clearing and setting the pixels in a memory-efficient manner.
 *
 * @param offset The memory offset in the VGA framebuffer where the block resides.
 * @param bitMask A bitmask indicating which pixels (bits) in the byte should be modified.
 *                Each bit corresponds to a pixel in planar graphics mode.
 * @param color The VGA plane mask (0-15) that determines which color planes are affected.
 *              This effectively sets the pixel color.
 */
void VGA_SetBlock(uint16_t offset, uint8_t bitMask, color_t color) {
  VGA_SetBitMask(bitMask);
  VGA_SetPlaneMask(0xf);
  _VideoMemory[offset] &= ~bitMask;
  VGA_SetPlaneMask(color);
  _VideoMemory[offset] |= bitMask;
}


/**
 * @brief Sets a single pixel to a specified color in VGA planar graphics mode.
 *
 * This function calculates the memory location and bit position of a pixel
 * based on its (x, y) coordinates and sets it to the specified color. It uses
 * `VGA_SetBlock` to apply the appropriate bitmask and color.
 *
 * @param x The horizontal coordinate of the pixel (0 to VGA_SCREEN_X - 1).
 * @param y The vertical coordinate of the pixel (0 to VGA_SCREEN_Y - 1).
 * @param color The VGA plane mask (0-15) that determines the color of the pixel.
 */
void VGA_SetPixel(uint16_t x, uint16_t y, color_t color) {
  uint16_t byte_offset = (y * VGA_BYTES_PER_ROW) + (x / 8);
  uint8_t bit_position = x % 8;
  uint8_t bit_mask = 1 << (7 - bit_position);

  VGA_SetBlock(byte_offset, bit_mask, color);
}


/**
 * @brief Draws a filled rectangle in VGA planar graphics mode.
 *
 * This function renders a filled rectangle of a given size and color,
 * starting at a specific (x, y) position. It calculates the necessary
 * bitmasks for bytes at the rectangle's edges and ensures proper alignment
 * for non-byte-aligned coordinates.
 *
 * @param startx The horizontal coordinate of the rectangle's top-left corner.
 * @param starty The vertical coordinate of the rectangle's top-left corner.
 * @param sizex The width of the rectangle in pixels.
 * @param sizey The height of the rectangle in pixels.
 * @param color The VGA plane mask (0-15) that determines the color of the rectangle.
 */
void VGA_SetRect(uint16_t startx, uint16_t starty, uint16_t sizex, uint16_t sizey, color_t color) {
  uint16_t bytesPerRectRow = (startx + sizex + 7) / 8 - startx / 8;
  uint16_t startOffsetInBytes = starty * VGA_BYTES_PER_ROW + (startx / 8);

  for (uint16_t yIndex = 0; yIndex < sizey; yIndex++) {
    for (uint16_t xIndex = 0; xIndex < bytesPerRectRow; xIndex++) {
      uint8_t bitmask = 0xFF;
      if (xIndex == 0 && (startx % 8 != 0))
        bitmask = 0xFF >> (startx % 8);
      
      if (xIndex == bytesPerRectRow - 1 && ((startx + sizex) % 8 != 0))
        bitmask &= 0xFF << (8 - ((startx + sizex) % 8));
      
      VGA_SetBlock(startOffsetInBytes + xIndex, bitmask, color);
    }

    startOffsetInBytes += VGA_BYTES_PER_ROW;
  }
}




/**
 * @brief Draws a single character on the screen in a specific color.
 *
 * This function renders a character from the monospace font at a specified
 * (x, y) position on the screen. It retrieves the 8x8 bitmap for the character
 * and uses the `VGA_DrawPixel` function to draw each pixel in the specified color.
 *
 * @param startx The horizontal coordinate of the top-left corner of the character.
 * @param starty The vertical coordinate of the top-left corner of the character.
 * @param character The ASCII character to be drawn.
 * @param color The VGA plane mask (0-15) that determines the color of the character.
 *
 * @note Ensure that `startx` and `starty` are within the screen bounds to avoid
 *       out-of-bounds memory access.
 */
void VGA_PrintChar(uint16_t startx, uint16_t starty, const char character, color_t color) {
  const uint8_t *bitmap = _MonospaceFont8x8[(uint8_t)character];
  
  for (uint16_t row = 0; row < 8; row++) {
    uint16_t current_row = starty + row + 1;
    uint8_t bitmap_row = bitmap[row];

    for (uint16_t col = 0; col < 7; col++) {
      uint16_t current_col = startx + col + 1;
      uint8_t bitmask = 1 << col;
      
      if (bitmap_row & bitmask)
	VGA_SetPixel(current_col, current_row, color);
    }
  }
}


/**
 * @brief Draws a string on the screen in a specific color.
 *
 * This function renders a null-terminated string starting at the specified
 * (x, y) position. Each character is drawn using the `VGA_DrawChar` function,
 * and characters are spaced according to the monospace font's width.
 *
 * @param startx The horizontal coordinate of the top-left corner of the string.
 * @param starty The vertical coordinate of the top-left corner of the string.
 * @param text A pointer to the null-terminated string to be drawn.
 * @param color The VGA plane mask (0-15) that determines the color of the string.
 *
 * @note Ensure that the string fits within the screen bounds. Line wrapping is
 *       not handled by this function.
 */
void VGA_PrintString(uint16_t startx, uint16_t starty, const char *text, color_t color) {
    for (short index = 0; text[index]; index++) {
        VGA_PrintChar(startx + (index * CHAR_WIDTH), starty, text[index], color);
    }
}


/**
 * @brief Draws a horizontal line on the screen.
 *
 * This function draws a straight horizontal line starting from the specified
 * (x, y) position, extending for the given width in pixels, and using the specified color.
 *
 * @param startx The horizontal starting position of the line.
 * @param starty The vertical position of the line.
 * @param sizex The width of the line in pixels.
 * @param color The VGA plane mask (0-15) that determines the color of the line.
 */
void VGA_DrawHorizontalLine(uint16_t startx, uint16_t starty, uint16_t sizex, color_t color) {
    for (uint16_t posx = startx; posx < startx + sizex; posx++) {
        VGA_SetPixel(posx, starty, color);
    }
}


/**
 * @brief Draws a vertical line on the screen.
 *
 * This function draws a straight vertical line starting from the specified
 * (x, y) position, extending for the given height in pixels, and using the specified color.
 *
 * @param startx The horizontal position of the line.
 * @param starty The vertical starting position of the line.
 * @param sizey The height of the line in pixels.
 * @param color The VGA plane mask (0-15) that determines the color of the line.
 */
void VGA_DrawVerticalLine(uint16_t startx, uint16_t starty, uint16_t sizey, color_t color) {
    for (uint16_t posy = starty; posy < starty + sizey; posy++) {
        VGA_SetPixel(startx, posy, color);
    }
}




/**
 * @brief Initializes the default system color palette.
 *
 * This function configures the VGA palette with a predefined set of 16 colors.
 * It assigns RGB values to the 256-color VGA palette and maps the first 16
 * palette entries to standard color names (e.g., Black, Red, Green).
 * These colors are commonly used in text mode and graphical interfaces.
 *
 * The colors are initialized using `VGA_SetPaletteColor` to define the RGB values
 * and `VGA_PickColor` to map them to the 16-color hardware palette.
 */
void VGA_InitializeColorPalette(void) {
  VGA_SetPaletteColor(0, 5, 5, 5);
  VGA_PickColor(Black, 0);
  
  VGA_SetPaletteColor(1, 40, 16, 16);
  VGA_PickColor(Red, 1);
  
  VGA_SetPaletteColor(2, 35, 37, 15);
  VGA_PickColor(Green, 2);
  
  VGA_SetPaletteColor(3, 55, 36, 23);
  VGA_PickColor(Yellow, 3);
  
  VGA_SetPaletteColor(4, 23, 32, 39);
  VGA_PickColor(Blue, 4);
  
  VGA_SetPaletteColor(5, 33, 25, 35);
  VGA_PickColor(Purple, 5);
  
  VGA_SetPaletteColor(6, 23, 35, 33);
  VGA_PickColor(Cyan, 6);
  
  VGA_SetPaletteColor(7, 49, 50, 49);
  VGA_PickColor(LightGray, 7);
  
  VGA_SetPaletteColor(8, 13, 14, 16);
  VGA_PickColor(DarkGray, 8);
  
  VGA_SetPaletteColor(9, 50, 25, 25);
  VGA_PickColor(LightRed, 9);
  
  VGA_SetPaletteColor(10, 45, 47, 25);
  VGA_PickColor(LightGreen, 10);
  
  VGA_SetPaletteColor(11, 60, 49, 29);
  VGA_PickColor(LightYellow, 11);
  
  VGA_SetPaletteColor(12, 32, 40, 47);
  VGA_PickColor(LightBlue, 12);
  
  VGA_SetPaletteColor(13, 44, 37, 46);
  VGA_PickColor(LightPurple, 13);
  
  VGA_SetPaletteColor(14, 34, 47, 45);
  VGA_PickColor(LightCyan, 14);
  
  VGA_SetPaletteColor(15, 63, 63, 63);
  VGA_PickColor(White, 15);
}


/**
 * @brief Draws a test image to validate VGA drawing functionality.
 *
 * This function creates a test pattern consisting of two rows of colored rectangles
 * and displays text in all 16 colors of the VGA palette. It is intended for verifying
 * that colors, shapes, and text rendering work as expected in VGA mode.
 *
 * The test image includes:
 * - 16 rectangles (8 per row), each 45x45 pixels, drawn in sequential VGA colors.
 * - Two lines of text in each of the 16 colors, showcasing uppercase and lowercase letters.
 */
void VGA_DrawTestImage(void) {
  color_t color = 0;
  int posx = 100;
  int posy = 50;
  
  for (int row = 0; row < 2; row++) {
    for (int box = 0; box < 8; box++) {

      VGA_SetRect(posx, posy, 45, 45, color);
      posx += 50;
      color++;
    }
    posx = 100;
    posy += 50;
  }

  for (color_t textcolor = 0; textcolor < 16; textcolor++) {
    VGA_PrintString(50, 200 + (textcolor * 14), "ABCDEFGHIJKLMNOPQRSTUVWXYZ\0", textcolor);
    VGA_PrintString(300, 200 + (textcolor * 14), "abcdefghijklmnopqrstuvwxyz\0", textcolor);
  }
} 
