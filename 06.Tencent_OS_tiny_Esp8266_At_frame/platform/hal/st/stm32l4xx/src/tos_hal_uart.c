#include "tos_k.h"
#include "tos_hal.h"
#include "stm32l4xx_hal.h"
#include "usart.h"

extern uint8_t esp_8266_data;
__API__ int tos_hal_uart_recv_init(hal_uart_port_t port)
{
	switch((int)port)
	{
		case HAL_UART_PORT_0:
			HAL_UART_Receive_IT(&hlpuart1, &esp_8266_data, 1);
			break ;
		case HAL_UART_PORT_1:
			break ;
		case HAL_UART_PORT_2:
			break ;
		case HAL_UART_PORT_3:
			break ;
	}
	return 0 ;
}

__API__ int tos_hal_uart_init(hal_uart_t *uart, hal_uart_port_t port)
{
    if (!uart) {
        return -1;
    }
    if (port == HAL_UART_PORT_0) {
        uart->private_uart = &hlpuart1;
        MX_LPUART1_UART_Init();
    } else if (port == HAL_UART_PORT_1) {
        uart->private_uart = &huart1;
        MX_USART1_UART_Init();
    } else if (port == HAL_UART_PORT_2) {
        uart->private_uart = &huart2;
        MX_USART2_UART_Init();
    } else if (port == HAL_UART_PORT_3) {
        uart->private_uart = &huart3;
        MX_USART3_UART_Init();
    }
		/*开启串口接收中断*/
		tos_hal_uart_recv_init(port);
    return 0;
}

__API__ int tos_hal_uart_write(hal_uart_t *uart, const uint8_t *buf, size_t size, uint32_t timeout)
{
    HAL_StatusTypeDef hal_status;
    UART_HandleTypeDef *uart_handle;

    if (!uart || !buf) {
        return -1;
    }

    if (!uart->private_uart) {
        return -1;
    }

    uart_handle = (UART_HandleTypeDef *)uart->private_uart;

    hal_status = HAL_UART_Transmit(uart_handle, (uint8_t *)buf, size, timeout);
    if (hal_status != HAL_OK) {
        return -1;
    }
    return 0;
}

__API__ int tos_hal_uart_read(hal_uart_t *uart, const uint8_t *buf, size_t size, uint32_t timeout)
{
    HAL_StatusTypeDef hal_status;
    UART_HandleTypeDef *uart_handle;

    if (!uart || !buf) {
        return -1;
    }

    if (!uart->private_uart) {
        return -1;
    }

    uart_handle = (UART_HandleTypeDef *)uart->private_uart;

    hal_status = HAL_UART_Receive(uart_handle, (uint8_t *)buf, size, timeout);
    if (hal_status != HAL_OK) {
        return -1;
    }
    return 0;
}

__API__ int tos_hal_uart_deinit(hal_uart_t *uart)
{
    HAL_StatusTypeDef hal_status;
    UART_HandleTypeDef *uart_handle;

    if (!uart) {
        return -1;
    }

    if (!uart->private_uart) {
        return -1;
    }

    uart_handle = (UART_HandleTypeDef *)uart->private_uart;

    hal_status = HAL_UART_DeInit(uart_handle);
    HAL_UART_MspDeInit(uart_handle);

    if (hal_status != HAL_OK) {
        return -1;
    }

    return 0;
}

