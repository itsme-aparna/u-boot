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

static int fdt_fixup_trips_node(void *blob, int zoneoffset, int maxc)
{
	int node, trip;

	node = fdt_subnode_offset(blob, zoneoffset, "trips");
	if (node < 0)
		return -1;

	fdt_for_each_subnode(trip, blob, node) {
		const char *type = fdt_getprop(blob, trip, "type", NULL);

		if (!type || (strncmp(type, "critical", 8) != 0))
			continue;

		if (fdt_setprop_u32(blob, trip, "temperature", 1000 * maxc) < 0)
			return -1;
	}

	return 0;
}

static void fdt_fixup_thermal_zone_nodes_am642(void *blob, int maxc)
{
	int node, zone;

	node = fdt_path_offset(blob, "/thermal-zones");
	if (node < 0)
		return;

	fdt_for_each_subnode(zone, blob, node) {
		if (fdt_fixup_trips_node(blob, zone, maxc) < 0)
			printf("Failed to set temperature in %s critical trips\n",
			       fdt_get_name(blob, zone, NULL));
	}
}

int ft_system_setup(void *blob, struct bd_info *bd)
{
	fdt_fixup_cores_nodes_am642(blob, k3_get_core_nr());
	fdt_fixup_canfd_nodes_am642(blob, k3_has_canfd());
	fdt_fixup_icss_nodes_am642(blob, k3_has_icss());
	fdt_fixup_thermal_zone_nodes_am642(blob, k3_get_max_temp());
	fdt_fixup_reserved(blob, "tfa", CONFIG_K3_ATF_LOAD_ADDR, 0x80000);
	fdt_fixup_reserved(blob, "optee", CONFIG_K3_OPTEE_LOAD_ADDR, 0x1800000);

	return 0;
}
