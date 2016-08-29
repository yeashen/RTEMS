/**
 * @file
 * @ingroup arm_hi3518e
 * @brief Global BSP definitions.
 */

/*-------------------------------------------------------------------------+
| bsp.h - ARM BSP
|
| Copyright (c) Ray,Xu  mailto:ray.cn AT gmail dot com
|
|  The license and distribution terms for this file may be
|  found in the file LICENSE in this distribution or at
|  http://www.rtems.org/license/LICENSE.
+--------------------------------------------------------------------------*/


#ifndef LIBBSP_ARM_HI3518E_BSP_H
#define LIBBSP_ARM_HI3518E_BSP_H

#include <bspopts.h>
#include <bsp/default-initial-extension.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup arm_hi3518e hi3518e Support
 * @ingroup bsp_arm
 * @brief hi3518e Support Package
 */

/**
 *  This file will not be pre-installed because the hi3518e BSP uses
 *  the bsp.h in gp32 currently.  This file is a placeholder. If you
 *  need to add something specical for your hi3518e BSP please override
 *  this file with your own and change Makefile.am
 */

#ifdef __cplusplus
}
#endif

#endif /* _BSP_H */
