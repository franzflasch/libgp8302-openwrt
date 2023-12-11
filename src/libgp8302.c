#include <libgp8302.h>
#include <string.h>
#include <stdio.h>

#define GP8302_DEF_I2C_ADDR                 0x58  ///< The default I2C address of the I2C current DAC module
#define GP8302_CONFIG_CURRENT_REG           0x02  ///< Configure current sensor register
#define GP8302_CURRENT_RESOLUTION           0x0FFF///< Current resolution: 12 bits, 0x0FFF
#define GP8302_MAX_CURRENT                  25    ///< Maximum switching current: 25mA
#define GP8302_STORE_TIMING_HEAD            0x02  ///< Store function timing start head
#define GP8302_STORE_TIMING_ADDR            0x10  ///< The first address for entering store timing
#define GP8302_STORE_TIMING_CMD1            0x03  ///< The command 1 to enter store timing
#define GP8302_STORE_TIMING_CMD2            0x00  ///< The command 2 to enter store timing
#define GP8302_STORE_TIMING_DELAY           10    ///< Store procedure interval delay time: 10ms, more than 7ms

int gp8302_init(gp8302_td *gp8302, const char *i2c_bus, int i2c_dev_addr)
{
	int rc = 0;

    memset(gp8302, 0, sizeof(gp8302_td));

	/*
	 * Set the I2C bus filename and slave address,
	 */
	gp8302->i2c_dev.filename = i2c_bus;
	gp8302->i2c_dev.addr = i2c_dev_addr;

	/*
	 * Start the I2C device.
	 */
	rc = i2c_start(&gp8302->i2c_dev);
	if (rc) {
		printf("failed to start i2c device\r\n");
		return rc;
	}

    return rc;
}

void gp8302_deinit(gp8302_td *gp8302)
{
    i2c_stop(&gp8302->i2c_dev);
}

void gp8302_calibration4_20mA(gp8302_td *gp8302, uint16_t dac_4, uint16_t dac_20)
{
    if((dac_4 >= dac_20) || (dac_20 > GP8302_CURRENT_RESOLUTION))
        return;

    gp8302->calib_dac_4 = dac_4;
    gp8302->calib_dac_20 = dac_20;
    gp8302->calibration = 1;
}

float gp8202_output_mA(gp8302_td *gp8302, uint16_t dac)
{
    uint8_t i2c_buf[2] = { 0 };

    if(dac > GP8302_CURRENT_RESOLUTION)
        return gp8302->digital;

    gp8302->digital = dac & GP8302_CURRENT_RESOLUTION;

    i2c_buf[0] = ((gp8302->digital << 4) & 0xF0);
    i2c_buf[1] = ((gp8302->digital >> 4) & 0xFF);

    i2c_writen_reg(&gp8302->i2c_dev, GP8302_CONFIG_CURRENT_REG, i2c_buf, sizeof(i2c_buf));

    float tmp = ((float)gp8302->digital/((float)GP8302_CURRENT_RESOLUTION * 1.0))*(float)GP8302_MAX_CURRENT;

    return tmp;
}

uint16_t gp8302_output(gp8302_td *gp8302, float current_mA)
{
    if(current_mA < 0)
        current_mA = 0;

    if(current_mA > GP8302_MAX_CURRENT)
        current_mA = GP8302_MAX_CURRENT;

    if(gp8302->calibration && (current_mA >= 4) && (current_mA <= 20))
    {
        gp8302->digital = gp8302->calib_dac_4 + ((current_mA - 4)*(gp8302->calib_dac_20 - gp8302->calib_dac_4)/(20-4));
        if(((gp8302->calib_dac_4 + ((current_mA - 4)*(gp8302->calib_dac_20 - gp8302->calib_dac_4)/((20-4)*1.0))) - gp8302->digital)*10 >= 5)
            gp8302->digital += 1;
    }
    else
    {
        gp8302->digital = (current_mA * GP8302_CURRENT_RESOLUTION)/GP8302_MAX_CURRENT;

        if((((current_mA * GP8302_CURRENT_RESOLUTION)/(GP8302_MAX_CURRENT*1.0)) - gp8302->digital)*10 >= 5)
            gp8302->digital += 1;
    }

    gp8202_output_mA(gp8302, gp8302->digital);

    return gp8302->digital;
}

void gp8302_store(gp8302_td *gp8302)
{
    (void) gp8302;
    return;
}
