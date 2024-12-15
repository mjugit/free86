#include "../Include/Shell.h"

/**
 * @brief Draws the border and header for a specified window.
 *
 * This function renders the header and border of a window based on its
 * position and dimensions. The header is filled with a predefined color,
 * and the window's title is displayed in white text. The border surrounds
 * the content area with a consistent thickness.
 *
 * @param window A pointer to the `Window` structure representing the window to be drawn.
 */
void Window_DrawBorder(Window *window) {
    // Draw the header
    uint16_t headerStartX = window->LocationX;
    uint16_t headerSizeX = window->SizeX;
    uint16_t headerStartY = window->LocationY;
    
    VGA_SetRect(headerStartX, headerStartY, headerSizeX, WIN_HEADER_HEIGHT, Blue); // Header background
    VGA_PrintString(headerStartX + 5, headerStartY + 2, window->Title, White);    // Header title text

    // Draw the left border
    uint16_t borderLeftTopX = window->LocationX;
    uint16_t borderLeftTopY = window->LocationY + WIN_HEADER_HEIGHT;
    uint16_t borderLeftSizeY = window->SizeY - WIN_HEADER_HEIGHT;
    
    VGA_SetRect(borderLeftTopX, borderLeftTopY, WIN_BORDER_WIDTH, borderLeftSizeY, Blue);

    // Draw the right border
    uint16_t borderRightTopX = window->LocationX + window->SizeX - WIN_BORDER_WIDTH;
    uint16_t borderRightTopY = window->LocationY + WIN_HEADER_HEIGHT;
    uint16_t borderRightSizeY = window->SizeY - WIN_HEADER_HEIGHT;
    
    VGA_SetRect(borderRightTopX, borderRightTopY, WIN_BORDER_WIDTH, borderRightSizeY, Blue);

    // Draw the bottom border
    uint16_t borderBottomTopX = window->LocationX + WIN_BORDER_WIDTH;
    uint16_t borderBottomTopY = window->SizeY + window->LocationY - WIN_BORDER_WIDTH;
    uint16_t borderBottomSizeX = window->SizeX - (WIN_BORDER_WIDTH * 2);

    VGA_SetRect(borderBottomTopX, borderBottomTopY, borderBottomSizeX, WIN_BORDER_WIDTH, Blue);
}

