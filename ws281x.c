#include "ws281x.h"

#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

static uint8_t raw_data[256 * 24]; //!< Raw data buffer

/**
 * Convert an integer to a NeoPixel value
 *
 * \param val
 *   The integer to convert
 * \param ret
 *   The return value
 */
static void int_to_neo_pixel(uint8_t val, int* ret) {
    for (int cnt = 0; cnt < 8; cnt++) ret[cnt] = (val & (1 << (7 - cnt))) ? 0xF8 : 0xC0;
}

/**
 * Convert an RGB color to a bit-banged value
 *
 * \param color
 *   The color to convert
 * \param ret
 *   The return value
*/
static void rgb_to_bit_bang(ws281x_color* color, int* ret) {
    int_to_neo_pixel(color->g, ret);
    int_to_neo_pixel(color->r, ret + 8);
    int_to_neo_pixel(color->b, ret + 16);
}

int ws281x_begin(ws281x_strip* strip) {
    strip->pixels = calloc(strip->pixels_len, sizeof(ws281x_color));

    return strip->spi_fd = open(strip->spi_dev, O_RDWR);
}

int ws281x_show(ws281x_strip* strip) {
    // create raw data
    int col[24];
    for (int cnt = 0; cnt < strip->pixels_len; cnt++) {
        rgb_to_bit_bang(strip->pixels + cnt, col);

        for (int cnt_col = 0; cnt_col < 24; cnt_col++) {
            raw_data[cnt * 24 + cnt_col] = col[cnt_col];
        }
    }

    // write raw data
    struct spi_ioc_transfer spi = {
        .tx_buf = (uint64_t) raw_data,
        .len = strip->pixels_len * 24,
        .delay_usecs = 0,
        .speed_hz = 800 * 1024 * 8,
        .bits_per_word = 8
    };
    int ret = ioctl(strip->spi_fd, SPI_IOC_MESSAGE(1), &spi);
    usleep(100);

    return ret;
}

void ws281x_close(ws281x_strip* strip) {
    close(strip->spi_fd);
    free(strip->pixels);
}
