#pragma once

#include <stdint.h>

typedef struct {
    uint8_t r, g, b; //!< RGB values
} ws281x_color; //!< Color struct

typedef struct {
    ws281x_color* pixels; //!< [in,out] Pixels
    int pixels_len; //!< [in] Number of pixels

    char* spi_dev; //!< [in] SPI device
    int spi_fd; //!< [out] SPI file descriptor
} ws281x_strip; //!< Strip struct

/**
 * Initialize the ws281x strip
 *
 * \param strip
 *   The strip to initialize
 *
 * \return
 *   open return value
 */
int ws281x_begin(ws281x_strip* strip);

/**
 * Show the pixels
 *
 * \param strip
 *   The strip to show
 *
 * \return
 *   ioctl return value
*/
int ws281x_show(ws281x_strip* strip);

/**
 * Close the ws281x strip
 *
 * \param strip
 *   The strip to close
 */
void ws281x_close(ws281x_strip* strip);
