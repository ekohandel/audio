/*
 * Copyright (c) 2020 Abe Kohandel <abe@electronshepherds.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <kernel.h>
#include <device.h>
#include <init.h>
#include <drivers/pinmux.h>
#include <sys/sys_io.h>

#include <pinmux/stm32/pinmux_stm32.h>

/* pin assignments for OLIMEX-STM32-P407 board */
static const struct pin_config pinconf[] = {
#ifdef CONFIG_UART_3
	{STM32_PIN_PD8, STM32F4_PINMUX_FUNC_PD8_USART3_TX},
	{STM32_PIN_PD9, STM32F4_PINMUX_FUNC_PD9_USART3_RX},
#endif	/* CONFIG_UART_2 */
#ifdef CONFIG_I2S_3
	{STM32_PIN_PA15, STM32F4_PINMUX_FUNC_PA15_I2S3_WS},
	{STM32_PIN_PB3,  STM32F4_PINMUX_FUNC_PB3_I2S3_CK},
	{STM32_PIN_PB5,  STM32F4_PINMUX_FUNC_PB5_I2S3_SD},
	{STM32_PIN_PC7,  STM32F4_PINMUX_FUNC_PC7_I2S3_MCK},
#endif	/* CONFIG_I2S_3 */
};

static int pinmux_stm32_init(struct device *port)
{
	ARG_UNUSED(port);

	stm32_setup_pins(pinconf, ARRAY_SIZE(pinconf));

	return 0;
}

SYS_INIT(pinmux_stm32_init, PRE_KERNEL_1,
		CONFIG_PINMUX_STM32_DEVICE_INITIALIZATION_PRIORITY);
