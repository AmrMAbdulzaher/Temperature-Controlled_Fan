#include "../../Library/STD_TYPES.h"
#include "../../Library/BIT_MATH.h"
#include "../../Library/ErrType.h"

#include "../../MCAL/DIO/DIO_interface.h"
#include "../../MCAL/Timer/Timer_interface.h"

#include "DCMOTOR_cfg.h"
#include "DCMOTOR_interface.h"
#include "DCMOTOR_prv.h"


uint8 DCMOTOR_u8RotateCW(const DCMOTOR_Config_t* Copy_pstConfig)
{
	uint8 Local_ErrorState = OK;

	if(Copy_pstConfig != NULL)
	{
		DIO_u8SetPinValue(Copy_pstConfig->u8Port,Copy_pstConfig->u8Pin2,DIO_u8PIN_LOW);
		DIO_u8SetPinValue(Copy_pstConfig->u8Port,Copy_pstConfig->u8Pin1,DIO_u8PIN_HIGH);
	}
	else
	{
		Local_ErrorState = NULL_PTR_ERR;
	}

	return Local_ErrorState;
}
uint8 DCMOTOR_u8RotateCCW(const DCMOTOR_Config_t* Copy_pstConfig)
{
	uint8 Local_ErrorState = OK;

	if(Copy_pstConfig != NULL)
	{
		DIO_u8SetPinValue(Copy_pstConfig->u8Port,Copy_pstConfig->u8Pin1,DIO_u8PIN_LOW);
		DIO_u8SetPinValue(Copy_pstConfig->u8Port,Copy_pstConfig->u8Pin2,DIO_u8PIN_HIGH);
	}
	else
	{
		Local_ErrorState = NULL_PTR_ERR;
	}

	return Local_ErrorState;
}

uint8 DCMOTOR_u8VarSpeed(const DCMOTOR_Config_t* Copy_pstConfig, uint8 Copy_u8Speed)
{
	uint8 Local_ErrorState = OK;

	if(Copy_pstConfig != NULL)
	{

		if(Copy_pstConfig -> VarPin == PB3)
		{
			Timer0_voidSetCompareMatchValue(Copy_u8Speed);

			Timer0_Config_t Local_stPWMTimer0 =
			{
					.WGM = WGM_FAST_PWM,
					.OC_Mode = OC_MODE_CLR,
					.CLK = PRESCALER_8
			};

			Timer0_u8Init(&Local_stPWMTimer0);
		}
		else if(Copy_pstConfig -> VarPin == PD7)
		{
			Timer2_voidSetCompareMatchValue(Copy_u8Speed);

			Timer2_Config_t Local_stPWMTimer2 =
			{
					.WGM = WGM_FAST_PWM,
					.OC_Mode = OC_MODE_CLR,
					.CLK = T2_PRESCALER_8
			};

			Timer2_u8Init(&Local_stPWMTimer2);
		}
		else
		{
			Local_ErrorState = NOK;
		}

	}
	else
	{
		Local_ErrorState = NULL_PTR_ERR;
	}

	return Local_ErrorState;
}

uint8 DCMOTOR_u8Stop(const DCMOTOR_Config_t* Copy_pstConfig)
{
	uint8 Local_ErrorState = OK;
	if(Copy_pstConfig != NULL)
	{
		DIO_u8SetPinValue(Copy_pstConfig->u8Port,Copy_pstConfig->u8Pin1,DIO_u8PIN_LOW);
		DIO_u8SetPinValue(Copy_pstConfig->u8Port,Copy_pstConfig->u8Pin2,DIO_u8PIN_LOW);
	}
	else
	{
		Local_ErrorState = NULL_PTR_ERR;
	}
	return Local_ErrorState;
}
