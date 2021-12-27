/*
 * MLX90621_BG.c
 *
 *  Created on: 9 de fev de 2021
 *      Author: brenog
 */


#include "MLX90621_BG.h"

const uint8_t refreshRate=8;
int16_t k_t1_scale=0, k_t2_scale=0, resolution=0,configuration=0,cpix=0;
uint16_t MLXir = 0x60;
int16_t irData[64];

//Teste

int16_t readConfig(void)
{
	uint8_t configLow = 0, configHigh = 0, buff[2];
	uint32_t comando = 0x02920001;
	while(HAL_I2C_Mem_Read2(&hi2c1, (uint16_t)MLXir<<1, comando, I2C_MEMADD_SIZE_32BIT, buff, 2, 1000) != HAL_OK);
	configLow = buff[0];
	configHigh = buff[1];
	configuration = ((int16_t)(configHigh << 8) | configLow);
	return configuration;
}

void setConfiguration(void)
{
	uint8_t Hz_LSB, buff[5];
	switch (refreshRate){
	case 0:
		Hz_LSB = 0b00111111;
		break;
	case 1:
		Hz_LSB = 0b00111110;
		break;
	case 2:
		Hz_LSB = 0b00111101;
		break;
	case 4:
		Hz_LSB = 0b00111100;
		break;
	case 8:
		Hz_LSB = 0b00111011;
		break;
	case 16:
		Hz_LSB = 0b00111010;
		break;
	case 32:
		Hz_LSB = 0b00111001;
		break;
	default:
		Hz_LSB = 0b00111110;
	}
	uint8_t defaultConfig_H = 0b00000100;
	/*buff[0] = 0x03;
	buff[1] = (Hz_LSB - 0x55);
	buff[2] = Hz_LSB;
	buff[3] = (eepromData[0xF6] - 0x55);
	buff[4] = eepromData[0xF6];*/
	buff[0] = 0x03;
	buff[1] = ((uint8_t)Hz_LSB - 0x55);
	buff[2] = Hz_LSB;
	buff[3] = defaultConfig_H - 0x55;
	buff[4] = defaultConfig_H;
	while(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)MLXir<<1, buff, 5, 100) != HAL_OK);
	resolution = (readConfig() & 0x30) >> 4;
}

/*---------------------------------------------*/

MLX * MLX_Init(I2C_HandleTypeDef * hi2c)
{
	uint8_t pacote[5];
	MLX * mlx = malloc(sizeof(*mlx));
	mlx->hi2c = hi2c;
	if (NULL == mlx) {return NULL;}

	//Read EEPROM
	uint8_t eepromData[256];
	uint8_t cmd = 0x00;
	while(HAL_I2C_Mem_Read(mlx->hi2c,  (uint16_t)0xA0, (uint16_t)0x00, sizeof(cmd), eepromData, 256, 100) != HAL_OK);

	//Salvando as constantes de acordo com a tabela 8
	memcpy(mlx->delA, &eepromData, sizeof(*mlx->delA)*NUM_PIXELS);
	memcpy(mlx->TaDep, &eepromData[0x40], sizeof(*mlx->TaDep)*NUM_PIXELS);
	memcpy(mlx->delAlpha, &eepromData[0x80], sizeof(*mlx->delAlpha)*NUM_PIXELS);
	mlx->Ks_scale = eepromData[0xC0];
	mlx->Ks4_EE = eepromData[0xC4];
	mlx->AcommL = eepromData[0xD0];
	mlx->AcommH = eepromData[0xD1];
	mlx->AcpL = eepromData[0xD3];
	mlx->AcpH = eepromData[0xD4];
	mlx->Bcp = eepromData[0xD5];
	mlx->alphaCPL = eepromData[0xD6];
	mlx->alphaCPH = eepromData[0xD7];
	mlx->tgc = eepromData[0xD8];
	mlx->delAlphaScale = eepromData[0xD9];
	mlx->Bi_scale = eepromData[0xD9];
	memcpy(&(mlx->Vth), &eepromData[0xDA], sizeof(mlx->Vth));
	memcpy(&(mlx->Kt1), &eepromData[0xDC], sizeof(mlx->Kt1));
	memcpy(&(mlx->Kt2), &eepromData[0xDE], sizeof(mlx->Kt2));
	mlx->Kt_scale = eepromData[0xD2];
 	mlx->alpha0L = eepromData[0xE0];
	mlx->alpha0H = eepromData[0xE1];
	mlx->alpha0Scale = eepromData[0xE2];
	mlx->delAlphaScale = eepromData[0xE3];
	mlx->epsilL = eepromData[0xE4];
	mlx->epsilH = eepromData[0xE5];
	mlx->KsTaL = eepromData[0xE6];
	mlx->KsTaH = eepromData[0xE7];
	memcpy(&(mlx->config), &eepromData[0xF5], sizeof(mlx->config));
	mlx->OSCtrim = eepromData[0xF7];

	//Write Oscillator Trim
	//pg. 10 datasheet
	pacote[0] = WRITE_OSC_TRIM;
	pacote[1] = mlx->OSCtrim-0xAA;
	pacote[2] = mlx->OSCtrim;
	pacote[3] = 0x56;
	pacote[4] = 0;
	while(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)MLXir<<1, pacote,  sizeof(*pacote)*5, 100) != HAL_OK);

	setConfiguration();

	UART_print("eeprom\n");
	for(int k = 0; k < 256; k++)
	{
		UART_print("(#%d)-> %d ", k,  eepromData[k]);
		if((k+1)%32 == 0) UART_print("\n");
	}
	UART_print("\n\n\n");

	free(mlx);
	return mlx;
}

HAL_StatusTypeDef MLX_Read_IT(MLX * mlx)
{
	// CHECK THAT POR FLAG IS NOT CLEARED
	/*if(((mlx->config) & (0x0400)) == 0) {
		MLX_Init(hi2c);
		return HAL_BUSY;

		colocar readConfig()
	}*/


	// Sensor Data (Temperature / PTAT)
	uint8_t ptatLow=0,ptatHigh=0;
	uint32_t comando = 0x02400001;
	uint8_t buff[2] = {0};
	while(HAL_I2C_Mem_Read2(mlx->hi2c, (uint16_t)MLXir<<1, comando, I2C_MEMADD_SIZE_32BIT, buff, 2, 1000) != HAL_OK);
	ptatLow = buff[0];
	ptatHigh = buff[1];
	mlx->ptat = ((uint16_t) (ptatHigh << 8) | ptatLow);


	//CPIX
	comando = 0x02410001;
	uint8_t cpixLow=0,cpixHigh=0;
	while(HAL_I2C_Mem_Read2(mlx->hi2c, (uint16_t)MLXir<<1, comando, I2C_MEMADD_SIZE_32BIT, buff, 2, 1000) != HAL_OK);
	cpixLow = buff[0];
	cpixHigh = buff[1];
	mlx->CPIX = ((uint16_t) (cpixHigh << 8) | cpixLow);


	// Tire Data (Temperature)
	comando = 0x02000140;
	while(HAL_I2C_Mem_Read2(mlx->hi2c, (uint16_t)MLXir<<1, comando, I2C_MEMADD_SIZE_32BIT, mlx->rawIR, 128, 1000) != HAL_OK);


	return HAL_OK;
}

void MLX_read(MLX * mlx, int16_t * IR)
{
	  uint8_t k = 0, ind = 0;


	  if(MLX_Read_IT(mlx) == HAL_OK) {
		  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

		  for(uint8_t i = 0; i < 4; i++) {
			  for(uint8_t j = 0; j < 16; j++) {
				  double t0 = MLX_CalcTemp(mlx, i, j);
				  IR[k] = t0;
				 // UART_print("%d ", IR[k]);

				 // if((k+1)%16 == 0) UART_print("\n");
				  k++;
			  }
		  }
	  }
	  UART_print("\n");
}


double Calc_Ta(MLX* mlx) 	// Temperatura absoluta
{
	// Ta = (-Kt1 + sqrt(power(Kt1, 2)-4*Kt2*(Vth-PTAT_DATA)))/(2*Kt2) + 25

	// Calculating Vth
	uint8_t config_reg = (uint8_t) ((mlx->config) >> 4) & CONFIG_MASK;
	double v = (mlx->Vth) / pow(2, (3-config_reg));
	uint8_t Kt_scale_shift1 = (uint8_t) ((mlx->Kt_scale) >> 4) & EEPROM_MASK;
	double kt1 = (mlx->Kt1) / (pow(2, Kt_scale_shift1) * pow(2, (3-config_reg)));
	uint8_t Kt_scale_shift2 = (uint8_t) (mlx->Kt_scale) & EEPROM_MASK;
	double kt2 = ((mlx->Kt2) / (pow(2, Kt_scale_shift2+10) * pow(2, (3-config_reg)))); //não consigo casa decimal?
	uint16_t PTAT_data = mlx->ptat;

	//Ta
	double numer =  -kt1 + sqrt(pow(kt1, 2) - (4 * kt2 * (v - PTAT_data)));
	double Ta = (numer / (2*(kt2))) + 25;
	return Ta;
}



double Calc_Vir_Compensated(MLX* mlx, int8_t i, int8_t j, double Ta) {

	// Vir Offset Compensation
	double Ai, Bi;
	uint8_t address = (i + 4*j);
	int8_t Ta0 = 25;
	uint8_t config_reg = (uint8_t) ((mlx->config)>>4) & CONFIG_MASK;

	int16_t Vir = (mlx->rawIR)[address];


	int16_t Acomm = 256*(uint8_t)(mlx->AcommH) + (uint8_t)(mlx->AcommL) - 65536;

	uint8_t delAij = (mlx->delA)[address];

	uint8_t delA_scale = (uint8_t) ((mlx->delAlphaScale)>>6) & EEPROM_MASK; //mudei, antes era >>4

	Ai = (Acomm + delAij*pow(2, delA_scale))/pow(2, 3-config_reg);

	int8_t Bij = (mlx->TaDep)[address];
	uint8_t Bi_scale = (uint8_t) (mlx->Bi_scale) & EEPROM_MASK;

	Bi = Bij/(pow(2, Bi_scale)*pow(2, 3-config_reg));

	double Vir_Offcompensated = Vir - (Ai + Bi*(Ta-Ta0));

	// Vir Thermal Gradient Compensation
	int16_t Acp = 256*(mlx->AcpH) + (mlx->AcpL);
	if(Acp > 32768) Acp = Acp - 65536;

	double Bcp = mlx->Bcp;
	if(Bcp > 127) Bcp = Bcp - 256;
	Bcp = (Bcp) / (pow(2, Bi_scale));

	//double VirCP_Offcompensated
	double VirCP_Offcompensated = (int16_t)mlx->CPIX - (Acp + Bcp*(Ta-25));

	double tgc = (mlx->tgc)/32; 	//tgc = 0 na eeprom? Deu certo

	double VirTGC_Compensated = Vir_Offcompensated - tgc*VirCP_Offcompensated;
	//double VirTGC_Compensated = Vir_Offcompensated - 0.75*VirCP_Offcompensated;

	// Emissivity compensation
	double epsil = (256*(mlx->epsilH)+(mlx->epsilL))/TWO_BYTE_MAX;

	double Vir_Compensated = VirTGC_Compensated/epsil;

	return Vir_Compensated;

}

double Calc_Alpha_Compensated(MLX* mlx, int8_t i, int8_t j, double Ta)
{

	// alpha_comp = (1 + KsTa*(Ta-Ta0))*(alpha_ij-tgc*alpha_cp);

	uint8_t TA0 = 25;
	double KsTa = (256*(mlx->KsTaH)+(mlx->KsTaL))/pow(2,20);

//	/KsTa = 524/pow(2,20);

	// Calculating alpha_ij
	uint8_t config_reg = (uint8_t) ((mlx -> config) >> 4) & CONFIG_MASK;
	double alpha0 = (256*(mlx->alpha0H)+(mlx->alpha0L))/pow(2, (mlx->alpha0Scale));
	uint8_t address = i + 4*j;
	double delAlpha = ((mlx->delA)[address])/pow(2,(mlx->delAlphaScale));
	double alpha_ij = (alpha0 + delAlpha)/pow(2, 3-config_reg);

	// Calculating alpha_cp
	double alpha_cp = (256*(mlx->alphaCPH)+(mlx->alphaCPL))/(pow(2,(mlx->alpha0Scale))*
																pow(2, 3-config_reg));

	double alpha_comp = (1 + KsTa*(Ta-TA0))*(alpha_ij-(mlx->tgc)*alpha_cp);
	//double alpha_comp = (1 + KsTa*(Ta-TA0))*(alpha_ij-(0.75)*alpha_cp);

	return alpha_comp;
}

double Calc_Ks4(MLX* mlx)
{

	double Ks4 = (mlx->Ks4_EE)/pow(2, (mlx->Ks_scale)+8);
	return Ks4; //Ks4 = 0 no BAB e BAD
}

double MLX_CalcTemp(MLX* mlx, int8_t i, int8_t j)
{

	// verifica se está no range
	//if(!(i<4 && j<16)) { exit(EXIT_FAILURE); }

	// Calculando parâmetros para usar no cálculo de To
	double Ta = Calc_Ta(mlx);
	double Vir_Compensated = Calc_Vir_Compensated(mlx, i, j, Ta);
	double Alpha_Compensated = Calc_Alpha_Compensated(mlx, i, j, Ta);
	double Ks4 = Calc_Ks4(mlx);
	double TaK4 = pow(Ta+ABSOLUTE_TEMP, 4);

	// Cálculo de Sx, Sx = 0 em BAB e BAD
	double tmp1 = pow(Alpha_Compensated, 3)*Vir_Compensated;
	double tmp2 = pow(Alpha_Compensated, 4)*TaK4;
	double Sx = Ks4*pow(tmp1+tmp2, 1.0/4.0);

	// Cálculo To
	double tmp3 = Alpha_Compensated*(1-Ks4*ABSOLUTE_TEMP) + Sx;
	double t0 = pow((Vir_Compensated/tmp3)+TaK4, 0.25) - ABSOLUTE_TEMP;

	return t0;
}
