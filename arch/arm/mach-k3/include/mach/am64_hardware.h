/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * K3: AM64 SoC definitions, structures etc.
 *
 * (C) Copyright (C) 2020-2021 Texas Instruments Incorporated - https://www.ti.com/
 */
#ifndef __ASM_ARCH_AM64_HARDWARE_H
#define __ASM_ARCH_AM64_HARDWARE_H

#include <config.h>
#ifndef __ASSEMBLY__
#include <linux/bitops.h>
#endif

#define PADCFG_MMR1_BASE				0x000f0000
#define MCU_PADCFG_MMR1_BASE				0x04080000
#define WKUP_CTRL_MMR0_BASE				0x43000000
#define MCU_CTRL_MMR0_BASE				0x04500000
#define CTRL_MMR0_BASE					0x43000000

#define CTRLMMR_WKUP_JTAG_DEVICE_ID		(WKUP_CTRL_MMR0_BASE + 0x18)
#define JTAG_DEV_CORE_NR_MASK			GENMASK(23, 22)
#define JTAG_DEV_CORE_NR_SHIFT			22
#define JTAG_DEV_FEATURES_MASK          GENMASK(17, 13)
#define JTAG_DEV_FEATURES_SHIFT         13
#define JTAG_DEV_ICSS_MASK              BIT(15)
#define JTAG_DEV_ICSS_SHIFT             15

#define JTAG_DEV_FEATURE_HAS_CAN_VAL1			0x5
#define JTAG_DEV_FEATURE_HAS_CAN_VAL2			0x6

#define CTRLMMR_MAIN_DEVSTAT				(CTRL_MMR0_BASE + 0x30)

#define MAIN_DEVSTAT_PRIMARY_BOOTMODE_MASK		0x00000078
#define MAIN_DEVSTAT_PRIMARY_BOOTMODE_SHIFT		3

#define MAIN_DEVSTAT_PRIMARY_BOOTMODE_CFG_MASK		0x00000380
#define MAIN_DEVSTAT_PRIMARY_BOOTMODE_CFG_SHIFT		7

#define MAIN_DEVSTAT_BACKUP_BOOTMODE_MASK		0x00001c00
#define MAIN_DEVSTAT_BACKUP_BOOTMODE_SHIFT		10

#define MAIN_DEVSTAT_BACKUP_BOOTMODE_CFG_MASK		0x00002000
#define MAIN_DEVSTAT_BACKUP_BOOTMODE_CFG_SHIFT		13

/* After the cfg mask and shifts have been applied */
#define MAIN_DEVSTAT_PRIMARY_MMC_PORT_SHIFT		2
#define MAIN_DEVSTAT_PRIMARY_MMC_PORT_MASK		0x04

#define MAIN_DEVSTAT_PRIMARY_USB_MODE_SHIFT		1
#define MAIN_DEVSTAT_PRIMARY_USB_MODE_MASK		0x02

#define MAIN_DEVSTAT_BACKUP_USB_MODE_MASK		0x01

#define ROM_EXTENDED_BOOT_DATA_INFO			0x701beb00

/* Use Last 2K as Scratch pad */
#define TI_SRAM_SCRATCH_BOARD_EEPROM_START		0x7019f800

static inline int k3_get_core_nr(void)
{
	u32 dev_id = readl(CTRLMMR_WKUP_JTAG_DEVICE_ID);

	return (dev_id & JTAG_DEV_CORE_NR_MASK) >> JTAG_DEV_CORE_NR_SHIFT;
}

static inline int k3_has_canfd(void)
{
	u32 dev_id = readl(CTRLMMR_WKUP_JTAG_DEVICE_ID);
	u32 feature_mask = (dev_id & JTAG_DEV_FEATURES_MASK) >>
				JTAG_DEV_FEATURES_SHIFT;

	return (feature_mask == JTAG_DEV_FEATURE_HAS_CAN_VAL1) |
				(feature_mask == JTAG_DEV_FEATURE_HAS_CAN_VAL2);
}

static inline int k3_has_icss(void)
{
	u32 dev_id = readl(CTRLMMR_WKUP_JTAG_DEVICE_ID);

	return (dev_id & JTAG_DEV_ICSS_MASK) >> JTAG_DEV_ICSS_SHIFT;
}

#if defined(CONFIG_SYS_K3_SPL_ATF) && !defined(__ASSEMBLY__)

#define AM64X_DEV_RTI8			127
#define AM64X_DEV_RTI9			128
#define AM64X_DEV_R5FSS0_CORE0		121
#define AM64X_DEV_R5FSS0_CORE1		122

static const u32 put_device_ids[] = {
	AM64X_DEV_RTI9,
	AM64X_DEV_RTI8,
};

static const u32 put_core_ids[] = {
	AM64X_DEV_R5FSS0_CORE1,
	AM64X_DEV_R5FSS0_CORE0, /* Handle CPU0 after CPU1 */
};

#endif

#endif /* __ASM_ARCH_DRA8_HARDWARE_H */
