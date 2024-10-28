#ifndef SUPPORT_TESTING_DEFINES_H_
#define SUPPORT_TESTING_DEFINES_H_

//#define ASSERT(expr) (void)0U
#include <stdint.h>


//GPIO
#define GPIOA			1	//(GPIO_TypeDef *)
#define GPIOB			2
#define GPIOC			3
#define GPIOD			4
#define GPIOE			5

#define GPIO_PIN_12		((uint16_t)12)
#define GPIO_PIN_13		((uint16_t)13)
#define GPIO_PIN_14		((uint16_t)14)
#define GPIO_PIN_15		((uint16_t)15)


typedef enum
{
  SUCCESS = 0,
  ERROR = !SUCCESS
} ErrorStatus;


/**
  * @brief  HAL Status structures definition
  */
typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

typedef struct
{
  uint8_t *Instance;
} I2C_HandleTypeDef;




#endif //SUPPORT_TESTING_DEFINES
