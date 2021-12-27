/*
 * MLX90621_BG.h
 *
 *  Created on: 9 de fev de 2021
 *      Author: brenog
 */

#ifndef INC_MLX90621_BG_H_
#define INC_MLX90621_BG_H_

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f0xx_hal.h"
#include "i2c.h"


#define WRITE_OSC_TRIM			0x04
#define WRITE_CONFIG_REG		0x03
#define MLX_READ_RAM_CMND		0x02
#define CONFIG_REG_ADDR			0x92
#define NUM_PIXELS				64
#define POR_FLAG_MASK			0x800
#define CONFIG_MASK				0x03
#define EEPROM_MASK				0x0F
#define ABSOLUTE_TEMP           273.15
#define TWO_BYTE_MAX 		    32768




typedef struct {
	//Olhar tabela 8 no datasheet / Table 8
	uint16_t rawIR[64];
	uint16_t ptat;			//Absolute ambient temperature data of the device itself (package temperature)
	uint8_t delA[64];		//IR pixel individual offset delta coefficient
	int8_t TaDep[64]; 		//64 member array of Individual Ta dependence (slope) of IR pixels offset
	uint8_t delAlpha[64];	//64 member array of Individual sensitivity coefficients
	uint8_t Ks_scale;
	int8_t  Ks4_EE; 		//Sensitivity To dependence (slope), for BAB and BAD Ks4 = 0, Sx = 0
	int8_t   AcommL;		//IR pixel common offset coefficient (low)
	int8_t  AcommH;			//IR pixel common offset coefficient (high)
	int8_t	AcpL;			//Compensation pixel individual offset coefficient (low)
	int8_t   AcpH;			//Compensation pixel individual offset coefficient (high)
	uint8_t   Bcp;			//Individual Ta dependence (slope) of the compensation pixel offset
	uint8_t  alphaCPL;		//Sensitivity coefficient of the compensation pixel (low)
	uint8_t  alphaCPH;		//Sensitivity coefficient of the compensation pixel (high)
	int8_t   tgc;			//Thermal gradient coefficient
	uint8_t  Ai_scale;		//Scaling coeff for the IR pixels offset (delA[])
	uint8_t  Bi_scale;		//Scaling coeff of the IR pixels offset Ta dep. (TaDep[])
	int16_t Vth; 			// Vth0 of absolute temperature sensor
	int16_t Kt1; 			// Kt1 of absolute temperature sensor
	int16_t Kt2; 			// Kt2 of absolute temperature sensor
	int8_t Kt_scale; 		// First nibble is Kt1 scale, second nibble is Kt2 scale
	uint8_t  alpha0L;		//Common sensitivity coefficient of IR pixels (low)
	uint8_t  alpha0H;		//Common sensitivity coefficient of IR pixels (high)
	uint8_t  alpha0Scale; 	//Scaling coefficient for common sensitivity
	uint8_t  delAlphaScale; //Scaling coefficient for individual sensitivity
	uint8_t  epsilL; 		//Emissivity (low)
	uint8_t  epsilH;		//Emissivity (high)
	int8_t   KsTaL;			//KsTa (fixed scale coefficient = 20) (low)
	int8_t   KsTaH;			//KsTa (fixed scale coefficient = 20) (high)
	uint16_t config;		//Configuration register of mlx sensor
	uint8_t	 OSCtrim;		//Oscillator Trim
	uint16_t CPIX;			//VCP - Compensation pixel readings
	I2C_HandleTypeDef * hi2c;
} MLX;


MLX * MLX_Init(I2C_HandleTypeDef * hi2c);
HAL_StatusTypeDef MLX_Read_IT(MLX * mlx);
void MLX_read(MLX * mlx, int16_t * IR);
double Calc_Ta(MLX* mlx);
double Calc_Vir_Compensated(MLX* mlx, int8_t i, int8_t j, double Ta);
double Calc_Alpha_Compensated(MLX* mlx, int8_t i, int8_t j, double Ta);
double Calc_Ks4(MLX* mlx);
double MLX_CalcTemp(MLX* mlx, int8_t i, int8_t j);



//Para teste
void setConfiguration(void);
int16_t readConfig(void);




#endif /* INC_MLX90621_BG_H_ */
