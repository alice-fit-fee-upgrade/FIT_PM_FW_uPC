#include "adt7311_driver.h"

  adt7311_clear_faults();

    R16,0x8
    R17,0x50
  adt7311_send_1_byte();

    R16,0x20
    R17,0x0
    R18,0x23
  adt7311_send_2_bytes();

    R16,0x30
    R17,0x0
    R18,0x1e
  adt7311_send_2_bytes()

/** 
 * @brief Reads the value of a register SPI interface device.
 *
 * @param dev              - The device structure.
 * @param register_address - Address of the register.
 * @param data - Pointer to the register value
 *
 * @return register_value  - Value of the register.
 */
static int adt7311_reg_read(struct adt7420_dev *dev, uint16_t reg_addr, uint16_t *p_data)
{
	uint8_t data_buf[3] = { 0, 0, 0 };
	uint8_t data_len = ((reg_addr & ADT7311_REG_LEN_bm) >> ADT7311_REG_LEN_bp);

	data_buf[0] = (ADT7311_CMD_READ | (reg_addr & ADT7311_ADDR_MSK) << 3);

	if (no_os_spi_write_and_read(dev->spi_desc, data_buf, data_len))
		return -1;

	if (data_len == 2)
    {
		*p_data = data_buf[1];
    }
	else
    {
		*p_data = (data_buf[1] << 8 | data_buf[2]);
    }

	return 0;
}

/**
 * @brief Write a value of a register via SPI
 * @param dev - The device structure.
 * @param register_address - Address of the register.
 * @param data - Pointer to the register value
 * @return 0 in case of success, -1 otherwise.
 */
int adt7311_reg_write(struct adt7420_dev *dev, uint16_t reg_addr, uint32_t data)
{
	uint8_t data_buf[3] = { 0, 0, 0 };
	uint8_t data_len = ((reg_addr & ADT7311_REG_LEN_bm) >> ADT7311_REG_LEN_bp);

    data_buf[0] = (ADT7311_CMD_WRITE_MSK & (reg_addr & ADT7311_ADDR_MSK) << 3);

    if (3 == data_len)
    {
        data_buf[1] = (data >> 8);
        data_buf[2] = (data & 0xFF);
    }
    else
    {
		data_buf[1] = (data & 0xFF);
	}

	return no_os_spi_write_and_read(dev->spi_desc, data_buf, data_len);
}

/**
 * @brief Sets the value of a register SPI/I2C.
 *
 * @param dev              - The device structure.
 * @param register_address - Address of the register.
 * @param mask		   - Bit Mask of the bit to be written
 * @param value		   - Value of the bit
 *
 * @return 0 in case of success, -1 otherwise.
 */

static int adt7311_reg_update(struct adt7420_dev *dev,uint16_t register_address, uint8_t mask, uint8_t value)
{
	uint16_t regval;

	if (adt7420_reg_read(dev, register_address, &regval))
    {
		return -1;
    }

	regval &= ~mask;
	regval |= value;

	return adt7420_reg_write(dev, register_address, regval);
}

/***
 * @brief Reads the temperature data and converts it to Celsius degrees.
 *
 * @param dev - The device structure.
 *
 * @return temperature - Temperature in degrees Celsius.
 */
float adt7420_get_temperature(struct adt7420_dev *dev)
{
	uint16_t temp = 0;
	float temp_c = 0;

	adt7311_reg_read(dev, ADT7311_REG_TEMP, &temp);

	if (dev->resolution_setting) 
    {
		if (temp & ADT7420_16BIT_SIGN)
			/*! Negative temperature */
			temp_c = (float)((int32_t)temp - ADT7420_16BIT_NEG) / ADT7420_16BIT_DIV;
		else
			/*! Positive temperature */
			temp_c = (float)temp / ADT7420_16BIT_DIV;
	} 
    else 
    {
		temp >>= 3;
		if (temp & ADT7420_13BIT_SIGN)
			/*! Negative temperature */
			temp_c = (float)((int32_t)temp - ADT7420_13BIT_NEG) / ADT7420_13BIT_DIV;
		else
			/*! Positive temperature */
			temp_c = (float)temp / ADT7420_13BIT_DIV;
	}

	return temp_c;
}

int32_t adt7311_init(struct adt7420_dev **device, struct adt7420_init_param init_param)
{

}