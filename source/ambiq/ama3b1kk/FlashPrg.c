/* Flash OS Routines
 * Copyright (c) 2009-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file FlashPrg.c */

#include "FlashOS.h"
#include "FlashPrg.h"

//*****************************************************************************
//
// Define AM_CMSIS_REGS to indicate that CMSIS registers are supported.
//
//*****************************************************************************
#define AM_CMSIS_REGS       1

//*****************************************************************************
//
// C99
//
//*****************************************************************************
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#if AM_CMSIS_REGS
#include "apollo3.h"
#else // AM_CMSIS_REGS
#ifdef __IAR_SYSTEMS_ICC__
#include "intrinsics.h"     // __CLZ() and other intrinsics
#endif // AM_CMSIS_REGS
#endif

#include "regs/am_reg_base_addresses.h"

#include "regs/am_reg_macros.h"

#include "regs/am_reg.h"
#include "regs/am_reg_m4.h"
#include "regs/am_reg_jedec.h"

#include "hal/am_hal_cachectrl.h"
#include "hal/am_hal_clkgen.h"
#include "hal/am_hal_flash.h"

uint32_t Init(uint32_t adr, uint32_t clk, uint32_t fnc)
{
    // Called to configure the SoC. Should enable clocks
    //  watchdogs, peripherals and anything else needed to
    //  access or program memory. Fnc parameter has meaning
    //  but currently isnt used in MSC programming routines
    
    uint32_t stat = AM_HAL_STATUS_SUCCESS;

    // Set the clock frequency.
    stat = am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0);  if(stat != AM_HAL_STATUS_SUCCESS){ return stat; }

    // Set the default cache configuration
    stat = am_hal_cachectrl_config(&am_hal_cachectrl_defaults);         if(stat != AM_HAL_STATUS_SUCCESS){ return stat; }
    stat = am_hal_cachectrl_enable();                                   if(stat != AM_HAL_STATUS_SUCCESS){ return stat; }
    
    return (0);
}

uint32_t UnInit(uint32_t fnc)
{
    // When a session is complete this is called to powerdown
    //  communication channels and clocks that were enabled
    //  Fnc parameter has meaning but isnt used in MSC program
    //  routines
    return (0);
}

uint32_t EraseChip(void)
{
    // Execute a sequence that erases the entire of flash memory region 

    uint32_t stat = AM_HAL_STATUS_SUCCESS;

    stat = am_hal_flash_mass_erase(AM_HAL_FLASH_PROGRAM_KEY, 1);        if(stat != AM_HAL_STATUS_SUCCESS){ return stat; }
    stat = am_hal_flash_mass_erase(AM_HAL_FLASH_PROGRAM_KEY, 0);        if(stat != AM_HAL_STATUS_SUCCESS){ return stat; }

    return (0);
}

uint32_t EraseSector(uint32_t adr)
{
    // Execute a sequence that erases the sector that adr resides in

    uint32_t stat = AM_HAL_STATUS_SUCCESS;

    stat = am_hal_flash_page_erase(AM_HAL_FLASH_PROGRAM_KEY, AM_HAL_FLASH_ADDR2INST( adr ), AM_HAL_FLASH_ADDR2PAGE( adr ));
    if(stat != AM_HAL_STATUS_SUCCESS){ return stat; }

    return (0);
}

uint32_t ProgramPage(uint32_t adr, uint32_t sz, uint32_t *buf)
{
    // Program the contents of buf starting at adr for length of sz

    uint32_t stat = AM_HAL_STATUS_SUCCESS;
    
    stat = am_hal_flash_program_main(AM_HAL_FLASH_PROGRAM_KEY, buf, (uint32_t*)adr, sz);
    if(stat != AM_HAL_STATUS_SUCCESS){ return stat; }

    return (0);
}

/*
uint32_t BlankCheck(uint32_t adr, uint32_t sz, uint8_t pat)
{
    // Check that the memory at address adr for length sz is 
    //  empty or the same as pat
    return 1;
}*/

/*
uint32_t Verify(uint32_t adr, uint32_t sz, uint32_t *buf)
{
    // Given an adr and sz compare this against the content of buf
    return 1;
}*/