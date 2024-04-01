#include "../Library/STD_TYPES.h"
#include "util/delay.h"

#include "../MCAL/DIO/DIO_interface.h"
#include "../MCAL/ADC/ADC_interface.h"
#include "../MCAL/PORT/PORT_interface.h"
#include "../MCAL/GIE/GIE_interface.h"

#include "../HAL/LED/LED_interface.h"
#include "../HAL/BUZZ/BUZZ_interface.h"
#include "../HAL/DCMOTOR/DCMOTOR_interface.h"
#include "../HAL/LM35/LM35_interface.h"


void main(void)
{
	PORT_voidInit();
	GIE_voidEnableGlobal();
	ADC_voidInit();

	DCMOTOR_Config_t Local_stFan =
	{
			.VarPin = PB3
	};

	LED_Config_t Local_stLevelOneLED=
	{
			.u8ACT_type = LED_ACT_HIGH,
			.u8Port = DIO_u8PORTA,
			.u8Pin = DIO_u8PIN2
	};

	LED_Config_t Local_stLevelTwoLED=
	{
			.u8ACT_type = LED_ACT_HIGH,
			.u8Port = DIO_u8PORTA,
			.u8Pin = DIO_u8PIN3
	};

	LED_Config_t Local_stLevelThreeLED=
	{
			.u8ACT_type = LED_ACT_HIGH,
			.u8Port = DIO_u8PORTA,
			.u8Pin = DIO_u8PIN4
	};

	LED_Config_t Local_stOverHeatLED=
	{
			.u8ACT_type = LED_ACT_HIGH,
			.u8Port = DIO_u8PORTA,
			.u8Pin = DIO_u8PIN5
	};

	BUZZ_Config_t Local_stBuzzer =
	{
			.u8ACT_type = LED_ACT_HIGH,
			.u8Port = DIO_u8PORTA,
			.u8Pin = DIO_u8PIN6
	};

	uint8 Local_u8Temperature = 0u;

	while(1)
	{
		Local_u8Temperature = LM35_u8GetTempReading(A0);


		if((Local_u8Temperature >= 25) && (Local_u8Temperature <30))
		{
			/*Set Fan Speed to 30% and Level 1 LED On*/
			DCMOTOR_u8VarSpeed(&Local_stFan, 86);
			LED_u8SetOn(&Local_stLevelOneLED);

			/*Set rest of LEDs and Buzzer Off*/
			BUZZ_u8SetOff(&Local_stBuzzer);
			LED_u8SetOff(&Local_stLevelTwoLED);
			LED_u8SetOff(&Local_stLevelThreeLED);
			LED_u8SetOff(&Local_stOverHeatLED);
		}
		else if((Local_u8Temperature >= 30) && (Local_u8Temperature < 35))
		{
			/*Set Fan Speed to 60% and Level 1 LED On*/
			DCMOTOR_u8VarSpeed(&Local_stFan, 172);
			LED_u8SetOn(&Local_stLevelTwoLED);

			/*Set rest of LEDs and Buzzer Off*/
			BUZZ_u8SetOff(&Local_stBuzzer);
			LED_u8SetOff(&Local_stLevelOneLED);
			LED_u8SetOff(&Local_stLevelThreeLED);
			LED_u8SetOff(&Local_stOverHeatLED);
		}
		else if((Local_u8Temperature >= 35) && (Local_u8Temperature <= 45))
		{
			/*Set Fan Speed to 100% and Level 1 LED On*/
			DCMOTOR_u8VarSpeed(&Local_stFan, 255);
			LED_u8SetOn(&Local_stLevelThreeLED);

			/*Set rest of LEDs and Buzzer Off*/
			BUZZ_u8SetOff(&Local_stBuzzer);
			LED_u8SetOff(&Local_stLevelOneLED);
			LED_u8SetOff(&Local_stLevelTwoLED);
			LED_u8SetOff(&Local_stOverHeatLED);
		}
		else if (Local_u8Temperature > 45)
		{
			Local_u8Temperature = LM35_u8GetTempReading(A0);
			_delay_ms(1000);
			if(Local_u8Temperature > 45)
			{

				/*Set Fan, rest of LEDs off*/
				DCMOTOR_u8Stop(&Local_stFan);
				LED_u8SetOff(&Local_stLevelOneLED);
				LED_u8SetOff(&Local_stLevelTwoLED);
				LED_u8SetOff(&Local_stLevelThreeLED);

				/*Toggle Buzzer and Overheat LED On for 300ms */
				BUZZ_u8Toggle(&Local_stBuzzer);
				LED_u8Toggle(&Local_stOverHeatLED);
				_delay_ms(300);
			}
			else
			{
				//Do nothing
			}
		}
		else
		{
			DCMOTOR_u8Stop(&Local_stFan);
			BUZZ_u8SetOff(&Local_stBuzzer);
			LED_u8SetOff(&Local_stLevelOneLED);
			LED_u8SetOff(&Local_stLevelTwoLED);
			LED_u8SetOff(&Local_stLevelThreeLED);
			LED_u8SetOff(&Local_stOverHeatLED);
		}

	}
}
