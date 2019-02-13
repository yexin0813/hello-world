/** @addtogroup IO_CTRL
 *  @ingroup PERIPHERAL
 *  @{
 */
#ifndef IO_CTRL_H_
#define IO_CTRL_H_
#include <stdint.h>
#include <stdbool.h>

/**@brief IO pull type */
typedef enum 
{
    IO_PULL_DISABLE,
    IO_PULL_UP,
    IO_PULL_DOWN,    
}io_pull_type_t;

/**@brief external interrupt trigger mode */
typedef enum 
{
    EXT_INT_TRIGGER_HIGH_LEVEL,
    EXT_INT_TRIGGER_LOW_LEVEL,
    EXT_INT_TRIGGER_POS_EDGE,
    EXT_INT_TRIGGER_NEG_EDGE,    
}ext_int_mode_t;

/**
 * @brief Initialize GPIO
 */
void io_init(void);

/**
 * @brief Set All GPIOs' output value
 * @param[in] val 
 */
void io_write_all(uint32_t val);

/**
 * @brief Read All GPIOs' input value
 * @return 
 */
uint32_t io_read_all(void);

/**
 * @brief Read All GPIOs' output value
 * @return 
 */
uint32_t io_out_read_all(void);

/**
 * @brief Read All GPIOs' direction configuration( 1:Output, 0:Input)
 * @return 
 */
uint32_t io_dir_read_all(void);

void io_dir_write_all_noie(uint32_t dir);

/**
 * @brief Config All GPIOs' direction ( 1:Output, 0:Input)
 * @param[in] dir 
 */
void io_dir_write_all(uint32_t dir);

/**
 * @brief Config selected GPIOs' direction as Output
 * @param[in] output_mask 
 */
void io_dir_output_set(uint32_t output_mask);

/**
 * @brief Config selected GPIOs' direction as Input
 * @param[in] input_mask 
 */
void io_dir_input_set(uint32_t input_mask);

/**
 * @brief Config a GPIO's direction as Output
 * @param[in] pin_num 
 */
void io_cfg_output(uint8_t pin_num);

/**
 * @brief Config a GPIO's direction as Input
 * @param[in] pin_num 
 */
void io_cfg_input(uint8_t pin_num);

/**
 * @brief Set a GPIO's direction
 * @param[in] pin_num 
 * @param[in] dir 1:Output, 0:Input
 */
void io_pin_dir_set(uint8_t pin_num,uint8_t dir);

/**
 * @brief Read a GPIO's input level
 * @return 
 */
uint8_t io_pin_read(uint8_t pin_num);

/**
 * @brief Read a GPIO's output level
 * @return 
 */
uint8_t io_pin_out_read(uint8_t pin_num);

/**
 * @brief Set a GPIO's output level
 * @param[in] pin_num 
 * @param[in] val 
 */
void io_pin_write(uint8_t pin_num,uint8_t val);

/**
 * @brief Set a GPIO's output level to high
 * @param[in] pin_num 
 */
void io_pin_set(uint8_t pin_num);

/**
 * @brief Set a GPIO's output level to low
 * @param[in] pin_num 
 */
void io_pin_clear(uint8_t pin_num);

/**
 * @brief Toggle a GPIO's output level
 * @param[in] pin_num 
 */
void io_pin_toggle(uint8_t pin_num);

/**
 * @brief Set 'Pull' Configuration of a GPIO
 * @param[in] pin_num 
 * @param[in] pull
 */
void io_pin_pull_write(uint8_t pin_num,io_pull_type_t pull);

/**
 * @brief Get 'Pull' Configuration of a GPIO
 * @param[in] pin_num 
 * @return
 */
io_pull_type_t io_pin_pull_read(uint8_t pin_num);

void ext_set_touch_cb(void (*cb)(void));

/**
 * @brief Config the External Interrupt for an IO
 * @param[in] pin_num 
 * @param[in] mode
 * @param[in] callback
 */
void io_ext_int_cfg(uint8_t pin_num,ext_int_mode_t mode,void (*callback)());

/**
 * @brief Enable or Disable the External Interrupt for an IO ( the same effect as io_ext_int_pin_en + io_ext_int_mask)
 * @param[in] pin_num 
 * @param[in] enable
 */
void io_ext_int_en(uint8_t pin_num,bool enable);

/**
 * @brief Enable or Disable the pin for External Interrupt
 * @param[in] pin_num 
 * @param[in] enable
 */
void io_ext_int_pin_en(uint8_t pin_num,bool enable);

/**
 * @brief Mask or Unmask the External Interrupt for the pin
 * @param[in] pin_num 
 * @param[in] unmask
 */
void io_ext_int_mask(uint8_t pin_num,bool unmask);



#endif
/** @}*/
