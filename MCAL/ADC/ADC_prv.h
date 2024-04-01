#ifndef ADC_PRV_H_
#define ADC_PRV_H_

#define DIVISON_BY_2			1u 			/*0b001*/
#define DIVISON_BY_4			2u 			/*0b010*/
#define DIVISON_BY_8			3u 			/*0b011*/
#define DIVISON_BY_16			4u 			/*0b100*/
#define DIVISON_BY_32			5u 			/*0b101*/
#define DIVISON_BY_64			6u 			/*0b110*/
#define DIVISON_BY_128			7u 			/*0b111*/

#define PRESCALER_BIT_MASK		0b11111000
#define PRESCALER_BIT_POS		0u

#define CHANNEL_BIT_MASK		0b11100000
#define CHANNEL_BIT_POS			0u

#define EIGHT_BITS				1u
#define TEN_BITS				2u

#define IDLE					0u
#define BUSY					1u

#define SINGLE_CONV				0u
#define CHAIN_CONV				1u

static void voidHandleSingleConversionAsynch(void);
static void voidHandleChainConversionAsynch(void);

#endif /* ADC_PRV_H_ */


