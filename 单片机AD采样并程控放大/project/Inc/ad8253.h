#ifndef  __AD8253_H
#define __AD8253_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"
     
#define A0(n)  (n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET))    
#define A1(n)  (n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET))     
     
void AD8253_Gain_1(void); 
void AD8253_Gain_10(void);      
void AD8253_Gain_100(void);      
void AD8253_Gain_1000(void);      
        
#ifdef __cplusplus
}
#endif
#endif





