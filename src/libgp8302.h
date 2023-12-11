#ifndef LIBGP8302_H
#define LIBGP8302_H

#include <stdint.h>
#include <libi2c-user/i2c-user.h>

typedef struct gp8302_struct
{
    struct I2cDevice i2c_dev;
    uint16_t digital;

    int calibration;
    uint16_t calib_dac_4;
    uint16_t calib_dac_20;

} gp8302_td;

int gp8302_init(gp8302_td *gp8302, const char *i2c_bus, int i2c_dev_addr);
void gp8302_deinit(gp8302_td *gp8302);
void gp8302_calibration4_20mA(gp8302_td *gp8302, uint16_t dac_4, uint16_t dac_20);
float gp8202_output_mA(gp8302_td *gp8302, uint16_t dac);
uint16_t gp8302_output(gp8302_td *gp8302, float current_mA);

#endif /* LIBGP8302_H */
