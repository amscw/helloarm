/**
  ******************************************************************************
  * @file    system_MDR32F9Qx.c
  * @author  Phyton Application Team
  * @version V1.4.0
  * @date    11/06/2010
  * @brief   CMSIS Cortex-M3 Device Peripheral Access Layer System Source File.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, PHYTON SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 Phyton</center></h2>
  ******************************************************************************
  * FILE system_MDR32F9Qx.c
  */


/** @addtogroup __CMSIS CMSIS
  * @{
  */

/** @defgroup MDR1986VE9x
 *  @{
 */

/** @addtogroup __MDR32F9QX MDR32F9QX System
  * @{
  */

/** @addtogroup System_Private_Includes System Private Includes
  * @{
  */

#include "MDR32Fx.h"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_eeprom.h"
#include "MDR32F9Qx_rst_clk.h"

/** @} */ /* End of group System_Private_Includes */

/** @addtogroup __MDR32F9QX_System_Private_Variables MDR32F9QX System Private Variables
  * @{
  */

/*******************************************************************************
*  Clock Definitions
*******************************************************************************/
  uint32_t SystemCoreClock = (uint32_t)8000000;         /*!< System Clock Frequency (Core Clock)
                                                         *   default value */

/** @} */ /* End of group __MDR32F9QX_System_Private_Variables */

/** @addtogroup __MDR32F9QX_System_Private_Functions MDR32F9QX System Private Functions
  * @{
  */

/**
  * @brief  Update SystemCoreClock according to Clock Register Values
  * @note   None
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate (void)
{
  uint32_t cpu_c1_freq, cpu_c2_freq, cpu_c3_freq;
  uint32_t pll_mul;

  /* Compute CPU_CLK frequency */

  /* Determine CPU_C1 frequency */
  if ((MDR_RST_CLK->CPU_CLOCK & (uint32_t)0x00000002) == (uint32_t)0x00000002)
  {
    cpu_c1_freq = HSE_Value;
  }
  else
  {
    cpu_c1_freq = HSI_Value;
  }

  if ((MDR_RST_CLK->CPU_CLOCK & (uint32_t)0x00000001) == (uint32_t)0x00000001)
  {
    cpu_c1_freq /= 2;
  }

  /* Determine CPU_C2 frequency */
  cpu_c2_freq = cpu_c1_freq;

  if ((MDR_RST_CLK->CPU_CLOCK & (uint32_t)0x00000004) == (uint32_t)0x00000004)
  {
    /* Determine CPU PLL output frequency */
    pll_mul = ((MDR_RST_CLK->PLL_CONTROL >> 8) & (uint32_t)0x0F) + 1;
    cpu_c2_freq *= pll_mul;
  }

  /*Select CPU_CLK from HSI, CPU_C3, LSE, LSI cases */
  switch ((MDR_RST_CLK->CPU_CLOCK >> 8) & (uint32_t)0x03)
  {
    case 0 :
      /* HSI */
      SystemCoreClock = HSI_Value;
      break;
    case 1 :
      /* CPU_C3 */
      /* Determine CPU_C3 frequency */
      cpu_c3_freq = cpu_c2_freq / ((MDR_RST_CLK->CPU_CLOCK >> 4 & (uint32_t)0x0F) + 1);
      SystemCoreClock = cpu_c3_freq;
      break;
    case 2 :
      /* LSE */
      SystemCoreClock = LSE_Value;
      break;
    default : /* case 3 */
      /* LSI */
      SystemCoreClock = LSI_Value;
      break;
  }
}

/**
  * @brief  Setup the microcontroller system
  *         RST clock configuration to the default reset state
  *         Setup SystemCoreClock variable.
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
void SystemInit (void)
{
#ifndef MDR_NO_RST_VTOR
	SCB->VTOR = 0x08000000;
#endif

	// Set EEPROM Latency for MCU Clock up to 100 MHz
	EEPROM_SetLatency(EEPROM_Latency_3);

	// Enable HSE (CPU_C1)
	RST_CLK_HSEconfig(RST_CLK_HSE_ON);
	while (RST_CLK_HSEstatus() != SUCCESS);

	// Enable PLL (CPU_C2)
	RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10);
	RST_CLK_CPU_PLLcmd(ENABLE);
	while( RST_CLK_CPU_PLLstatus() != SUCCESS);
	RST_CLK_CPU_PLLuse(ENABLE);

	// Set clock prescaler (CPU_C3) & source (HCLK/CPU_CLK)
	RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);
	RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);

	// Update the SystemCoreClock
	SystemCoreClockUpdate();

  extern void DWT_Init(void);
  DWT_Init();
}

/** @} */ /* End of group __MDR32F9QX_System_Private_Functions */

/** @} */ /* End of group __MDR32F9QX */

/** @} */ /* End of group MDR1986VE9x */

/** @} */ /* End of group __CMSIS */

/******************* (C) COPYRIGHT 2010 Phyton *********************************
*
* END OF FILE system_MDR32F9Qx.c */
