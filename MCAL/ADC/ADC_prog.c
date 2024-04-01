
#include "../../Library/STD_TYPES.h"
#include "../../Library/BIT_MATH.h"
#include "../../Library/ErrType.h"

#include "ADC_cfg.h"
#include "ADC_interface.h"
#include "ADC_prv.h"
#include "ADC_reg.h"

static uint16* ADC_pu16Result = NULL;
static void(*ADC_pvCallBackNotificationFunc)(void) = NULL;
static uint8 ADC_u8BusyFlag = IDLE;
static uint8 ADC_u8ConversionMode = SINGLE_CONV;

static ADC_Chain_t* ADC_pstChainData = NULL;

void ADC_voidInit(void)
{
	/*Set AVCC as reference voltage*/
	CLR_BIT(ADMUX,ADMUX_REFS1);
	SET_BIT(ADMUX,ADMUX_REFS0);

	/*Select left or right adjust according to resolution configuration*/
#if ADC_u8RESOLUTION == EIGHT_BITS
	SET_BIT(ADMUX,ADMUX_ADLAR);
#elif ADC_u8RESOLUTION == TEN_BITS
	CLR_BIT(ADMUX,ADMUX_ADLAR);
#else
error Wrong ADC_u8RESOLUTION configuration option
#endif

	/*Disable Auto trigger mode*/
	CLR_BIT(ADCSRA,ADCSRA_ADATE);

	/*Disable interrupt*/
	CLR_BIT(ADCSRA,ADCSRA_ADIE);

	/*Enable ADC*/
	SET_BIT(ADCSRA,ADCSRA_ADEN);

	/*Clear the prescaler bits*/
	ADCSRA &= PRESCALER_BIT_MASK;
	/*Configure the prescaler division factor*/
	ADCSRA |= ADC_u8PRESCALER_DIVISON_FACTOR << PRESCALER_BIT_POS;
}

uint8 ADC_u8StartSingleConversionSynch(uint8 Copy_u8Channel, uint16* Copy_pu16Result)
{
	uint8 Local_u8ErrorState= OK;

	if(Copy_pu16Result != NULL)
	{
		/*Check if ADC is IDLE*/
		if(ADC_u8BusyFlag == IDLE)
		{
			uint32 Local_u32Counter = 0u;

			/*ADC is now busy*/
			ADC_u8BusyFlag = BUSY;

			/*clear input channel bits*/
			ADMUX &= CHANNEL_BIT_MASK;
			/*Set input channel*/
			ADMUX |= Copy_u8Channel << CHANNEL_BIT_POS;

			/*Start conversion*/
			SET_BIT(ADCSRA,ADCSRA_ADSC);

			/*Polling (with blocking) on the conversion complete on condition that timeout is not passed yet*/
			while((GET_BIT(ADCSRA,ADCSRA_ADIF)==0u) && (Local_u32Counter<ADC_u32TIMEOUT))
			{
				Local_u32Counter++;
			}
			if(Local_u32Counter == ADC_u32TIMEOUT)
			{
				/*Timeout has passed without the flag being raised*/
				Local_u8ErrorState = TIMEOUT_ERR;
			}
			else
			{
				/*Flag is raised before Timeout*/
				/*clear ADC interrupt flag*/
				SET_BIT(ADCSRA,ADCSRA_ADIF);

				/*get the conversion result*/
#if ADC_u8RESOLUTION == EIGHT_BITS
				*Copy_pu16Result= ADCH;
#elif ADC_u8RESOLUTION == TEN_BITS
				*Copy_pu16Result= ADC;
#endif
			}
			/*ADC is now idle*/
			ADC_u8BusyFlag = IDLE;
		}
		else
		{
			/*ADC is busy, update error state*/
			Local_u8ErrorState = BUSY_ERR;
		}

	}
	else
	{
		Local_u8ErrorState = NULL_PTR_ERR;
	}


	return Local_u8ErrorState;
}


uint8 ADC_u8StartSingleConversionAsynch(uint8 Copy_u8Channel, uint16* Copy_pu16Result, void(*Copy_pvNotificationFunc)(void))
{
	uint8 Local_u8ErrorState= OK;

	if((Copy_pu16Result != NULL) || (Copy_pvNotificationFunc != NULL))
	{
		/*Check if ADC is IDLE*/
		if(ADC_u8BusyFlag == IDLE)
		{
			/*ADC is now busy*/
			ADC_u8BusyFlag = BUSY;

			/*ADC is now in single conversion mode*/
			ADC_u8ConversionMode = SINGLE_CONV;

			/*Convert the local ADC result into global*/
			ADC_pu16Result = Copy_pu16Result;
			/*Convert the local ADC call back into global*/
			ADC_pvCallBackNotificationFunc = Copy_pvNotificationFunc;

			/*clear input channel bits*/
			ADMUX &= CHANNEL_BIT_MASK;
			/*Set input channel*/
			ADMUX |= Copy_u8Channel << CHANNEL_BIT_POS;

			/*Start conversion*/
			SET_BIT(ADCSRA,ADCSRA_ADSC);

			/*Enable ADC conversion complete interrupt*/
			SET_BIT(ADCSRA,ADCSRA_ADIE);
		}
		else
		{
			/*ADC is busy, update error state*/
			Local_u8ErrorState = BUSY_ERR;
		}
	}
	else
	{
		Local_u8ErrorState = NULL_PTR_ERR;
	}


	return Local_u8ErrorState;
}

uint8 ADC_u8StartChainConversionAsynch(ADC_Chain_t* Copy_pstObject)
{
	uint8 Local_u8ErrorState = NOK;

	if((Copy_pstObject != NULL) && (Copy_pstObject->ChannelArr != NULL) && (Copy_pstObject->ResultArr != NULL) && (Copy_pstObject->NotificationFunc != NULL) )
	{
		/*Check if ADC is IDLE*/
		if(ADC_u8BusyFlag == IDLE)
		{
			/*ADC is now busy*/
			ADC_u8BusyFlag = BUSY;

			/*ADC is now in single conversion mode*/
			ADC_u8ConversionMode = CHAIN_CONV;

			ADC_pstChainData = Copy_pstObject;

			/*clear input channel bits*/
			ADMUX &= CHANNEL_BIT_MASK;
			/*Set input channel*/
			ADMUX |= ADC_pstChainData->ChannelArr[0] << CHANNEL_BIT_POS;

			/*Start conversion*/
			SET_BIT(ADCSRA,ADCSRA_ADSC);

			/*Enable ADC conversion complete interrupt*/
			SET_BIT(ADCSRA,ADCSRA_ADIE);
		}
		else
		{
			/*ADC is busy, update error state*/
			Local_u8ErrorState = BUSY_ERR;
		}
	}
	else
	{
		Local_u8ErrorState = NULL_PTR_ERR;
	}

	return Local_u8ErrorState;
}

static void voidHandleSingleConversionAsynch(void)
{
	/*ISR comes to complete the work of Start Single Conversion Asynch function*/
	/*Read the result*/
#if ADC_u8RESOLUTION == EIGHT_BITS
	*ADC_pu16Result= ADCH;
#elif ADC_u8RESOLUTION == TEN_BITS
	*ADC_pu16Result= ADC;
#endif

	/*Disable conversion complete interrupt*/
	CLR_BIT(ADCSRA,ADCSRA_ADIE);

	/*ADC is now idle*/
	ADC_u8BusyFlag = IDLE;

	/*Invoke the call back notification function*/
	if(ADC_pvCallBackNotificationFunc != NULL)
	{
		ADC_pvCallBackNotificationFunc();
	}
	else
	{
		/*Do nothing*/
	}
}

static void voidHandleChainConversionAsynch(void)
{
	/*ISR comes to complete the work of Start Chain Conversion Asynch function*/
	/*volatile to cancel optimizion of the variable, static to keep its value*/
	volatile static uint8 Local_u8ChannelIndex = 0;

	/*read the result*/
#if ADC_u8RESOLUTION == EIGHT_BITS
	ADC_pstChainData->ResultArr[Local_u8ChannelIndex]= ADCH;
#elif ADC_u8RESOLUTION == TEN_BITS
	ADC_pstChainData->ResultArr[Local_u8ChannelIndex]= ADC;
#endif
	/*Increment the channel index counter*/
	Local_u8ChannelIndex++;

	if(Local_u8ChannelIndex == ADC_pstChainData->ChainSize)
	{
		/*Chain is finished*/

		/*Rest Channel index counter*/
		Local_u8ChannelIndex = 0;

		/*Disable conversion complete interrupt*/
		CLR_BIT(ADCSRA,ADCSRA_ADIE);

		/*ADC is now idle*/
		ADC_u8BusyFlag = IDLE;

		/*Invoke the call back notification function*/
		if(ADC_pstChainData->NotificationFunc != NULL)
		{
			ADC_pstChainData->NotificationFunc();
		}
		else
		{
			//Do nothing
		}
	}
	else
	{
		/*Chain is not finished yet, start the next conversion*/

		/*clear input channel bits*/
		ADMUX &= CHANNEL_BIT_MASK;
		/*Set input channel*/
		ADMUX |= ADC_pstChainData->ChannelArr[Local_u8ChannelIndex] << CHANNEL_BIT_POS;

		/*Start conversion*/
		SET_BIT(ADCSRA,ADCSRA_ADSC);

	}
}

/*ADC conversion complete ISR*/
__attribute__((signal)) void __vector_16(void);
void __vector_16(void)
{
	/*ISR Guidelines: ISR lines shouldn't be more than 50 lines including comments
	 *So it's better to define static functions and call them in the ISR
	 */
	if(ADC_u8ConversionMode == SINGLE_CONV)
	{
		voidHandleSingleConversionAsynch();
	}
	else if(ADC_u8ConversionMode == CHAIN_CONV)
	{
		voidHandleChainConversionAsynch();
	}
}
