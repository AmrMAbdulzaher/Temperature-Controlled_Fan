#ifndef DCMOTOR_INTERFACE_H
#define DCMOTOR_INTERFACE_H

typedef enum
{
	PB3,
	PD7
}DCMOTOR_VAR_PIN;

typedef struct
{
	uint8 u8Port;
	uint8 u8Pin1;
	uint8 u8Pin2;
	DCMOTOR_VAR_PIN VarPin;
}DCMOTOR_Config_t;



uint8 DCMOTOR_u8RotateCW(const DCMOTOR_Config_t* Copy_pstConfig);
uint8 DCMOTOR_u8RotateCCW(const DCMOTOR_Config_t* Copy_pstConfig);
uint8 DCMOTOR_u8VarSpeed(const DCMOTOR_Config_t* Copy_pstConfig, uint8 Copy_u8Speed);
uint8 DCMOTOR_u8Stop(const DCMOTOR_Config_t* Copy_pstConfig);

#endif
