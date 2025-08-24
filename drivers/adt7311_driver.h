#pragma once

#include <stdint.h>

#define ADT7311_LSB_MASK                0x00FF
#define ADT7311_MSB_MASK                0xFF00
#define ADT7311_LSB_OFFSET              0
#define ADT7311_MSB_OFFSET              8
#define ADT7311_CONFIG_FAULT_QUEUE(x)	((x) & 0x03)
#define ADT7311_CONFIG_CT_POL		    ()
#define ADT7311_CONFIG_INT_POL		    ()
#define ADT7311_CONFIG_INT_CT_MODE	    ()
#define ADT7311_MASK_SET_PT_REGISTER	    0x00

/* ADT7311_CONFIG_FAULT_QUEUE(x) options */
#define ADT7311_FAULT_QUEUE_1_FAULT	    0
#define ADT7311_FAULT_QUEUE_2_FAULTS	1
#define ADT7311_FAULT_QUEUE_3_FAULTS	2
#define ADT7311_FAULT_QUEUE_4_FAULTS	3

/* ADT7xxx default ID */
#define ADT7320_DEFAULT_ID		    0xC3
#define ADT7311_DEFAULT_ID		    0xCB

/* ADT7420 address */
#define ADT7420_A0_PIN(x)		(((x) & 0x1) << 0) // I2C Serial Bus Address Selection Pin
#define ADT7420_A1_PIN(x)		(((x) & 0x1) << 1) // I2C Serial Bus Address Selection Pin
#define ADT7420_ADDRESS(x,y)		(0x48 + ADT7420_A1_PIN(x) + ADT7420_A0_PIN(y))




/* ADT7311 command byte */
#define ADT7311_CMD_WRITE_MSK	(0b00111000)            // SPI write command mask
#define ADT7311_CMD_READ		(0b01000000)            // SPI read command
#define ADT7311_ADDR_MSK		(0x07)

/* ADT7311 registers */
#define ADT7311_REG_LEN_8		(0x80)      // 8-bit register indicator
#define ADT7311_REG_LEN_16		(0xC0)      // 16-bit register indicator
#define ADT7311_REG_LEN_bm	    (0xC0)      // register indicator bitmask
#define ADT7311_REG_LEN_bp      (6)         // register indicator bits position

#define ADT7311_REG_STATUS		(ADT7311_REG_LEN_8  | 0x00) // Status info
#define ADT7311_REG_CONFIG		(ADT7311_REG_LEN_8  | 0x01) // Configuration
#define ADT7311_REG_TEMP    	(ADT7311_REG_LEN_16 | 0x02) // Temperature value
#define ADT7311_REG_ID			(ADT7311_REG_LEN_8  | 0x03) // ID value
#define ADT7311_REG_T_CRIT		(ADT7311_REG_LEN_16 | 0x04) // Temperature CRIT setpoint
#define ADT7311_REG_HIST		(ADT7311_REG_LEN_8  | 0x05) // Temperature HYST setpoint
#define ADT7311_REG_T_HIGH      (ADT7311_REG_LEN_16 | 0x06) // Temperature HIGH setpoint
#define ADT7311_REG_T_LOW    	(ADT7311_REG_LEN_16 | 0x07) // Temperature LOW setpoint


/*! Sets the Fault Queue option.*/
int32_t adt7311_set_fault_queue(struct adt7311_dev *dev, uint8_t mode);

/*! Sets comparator/interrupt (CT/INT) mode.*/
int32_t adt7311_set_ct_int_mode(struct adt7311_dev *dev, uint8_t setting);

/*! Sets output polarity for the pins CT/INT (Critical Temp - Over/Under Temp).*/
int32_t adt7311_set_ct_int_polarity(struct adt7311_dev *dev, uint8_t polarity);

/*! Writes data to temperature registers*/
int32_t adt7311_wr_setpoint_reg(struct adt7311_dev *device, uint16_t register_value, uint16_t data);

/*! Get the register address of write type registers */
uint16_t configure_write_type_registers(struct adt7311_dev *dev, uint16_t register_address);

/*! Get the register address based on the register type enum- registers_e */
int32_t adt7311_get_register_address_and_value(struct adt7311_dev *dev, uint16_t register_address, uint16_t *reg_val);