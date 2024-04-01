#include "../../Library/STD_TYPES.h"
#include "../../Library/BIT_MATH.h"

#include "../../MCAL/ADC/ADC_interface.h"

#include "LM35_prv.h"
#include "LM35_cfg.h"
#include "LM35_interface.h"


uint8 LM35_u8GetTempReading(uint8 Copy_u8Channel)
{
	uint16 Local_u16DigitalVal;
	uint8 Local_u8TempVal;

	ADC_u8StartSingleConversionSynch(Copy_u8Channel,&Local_u16DigitalVal);

	Local_u8TempVal= (uint8)((Local_u16DigitalVal * 500u)/1024u);

	return Local_u8TempVal;
}
