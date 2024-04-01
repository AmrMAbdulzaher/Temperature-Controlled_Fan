#ifndef ADC_CFG_H_
#define ADC_CFG_H_

/*Configure the ADC prescaler division factor, options:
 * 															1- DIVISON_BY_2
 * 															2- DIVISON_BY_4
 * 															3- DIVISON_BY_8
 * 															4- DIVISON_BY_16
 * 															5- DIVISON_BY_32
 * 															6- DIVISON_BY_64
 * 															7- DIVISON_BY_128
 * 															*/
#define ADC_u8PRESCALER_DIVISON_FACTOR						DIVISON_BY_128

/*Configure the ADC bit resolution, options:
 * 															1- EIGHT_BITS
 * 															2- TEN_BITS
 */
#define ADC_u8RESOLUTION									TEN_BITS

#define ADC_u32TIMEOUT										50000U

#endif /* ADC_CFG_H_ */
