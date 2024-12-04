// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright 2023 Toradex - https://www.toradex.com/
 */

#include <asm/hardware.h>
#include <fdt_support.h>
#include <fdtdec.h>

#include "../common_fdt.h"

static void fdt_fixup_cores_nodes_am642(void *blob, int core_nr)
{
	char node_path[32];

	for (; core_nr < 2; core_nr++) {
		snprintf(node_path, sizeof(node_path), "/cpus/cpu@%d", core_nr);
		fdt_del_node_path(blob, node_path);
		snprintf(node_path, sizeof(node_path), "/cpus/cpu-map/cluster0/core%d", core_nr);
		fdt_del_node_path(blob, node_path);
		snprintf(node_path, sizeof(node_path), "/bus@f4000/watchdog@e0%d0000", core_nr);
		fdt_del_node_path(blob, node_path);
	}
}

static void fdt_fixup_canfd_nodes_am642(void *blob, bool has_canfd)
{
	if (!has_canfd) {
		fdt_del_node_path(blob, "/bus@f4000/can@20701000");
		fdt_del_node_path(blob, "/bus@f4000/can@20711000");
	}
}

static void fdt_fixup_icss_nodes_am642(void *blob, bool has_icss)
{
	if (!has_icss) {
		fdt_del_node_path(blob, "/bus@f4000/icssg@30000000");
		fdt_del_node_path(blob, "/bus@f4000/icssg@30080000");
	}
}

int ft_system_setup(void *blob, struct bd_info *bd)
{
	fdt_fixup_cores_nodes_am642(blob, k3_get_core_nr());
	fdt_fixup_canfd_nodes_am642(blob, k3_has_canfd());
	fdt_fixup_icss_nodes_am642(blob, k3_has_icss());
	fdt_fixup_reserved(blob, "tfa", CONFIG_K3_ATF_LOAD_ADDR, 0x80000);
	fdt_fixup_reserved(blob, "optee", CONFIG_K3_OPTEE_LOAD_ADDR, 0x1800000);

	return 0;
}
