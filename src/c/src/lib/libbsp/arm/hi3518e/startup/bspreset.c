/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : bspreset.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2016/03/16
  Last Modified : 
  Description   : 
  History       : 
  1.Date        : 2016/03/16
   Author       : Juven
   Modification : Created file

 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 
******************************************************************************/

#include <bsp.h>
#include <bsp/bootcard.h>

void bsp_reset(void)
{
	rtems_interrupt_level level;

	rtems_interrupt_disable(level);
		(void)level;

	while(1)
		;
}
