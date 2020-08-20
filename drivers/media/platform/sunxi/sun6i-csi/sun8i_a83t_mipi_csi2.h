/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright Kévin L'hôpital (C) 2020
 */

#ifndef __SUN8I_A83T_MIPI_CSI2_H__
#define __SUN8I_A83T_MIPI_CSI2_H__
#include <linux/regmap.h>
#include "sun6i_csi.h"

void sun6i_mipi_csi_set_stream(struct sun6i_csi *csi, bool enable);
void sun6i_mipi_csi_setup_bus(struct sun6i_csi *csi);
int sun6i_mipi_csi_clk_enable(struct sun6i_csi *csi);
void sun6i_mipi_csi_clk_disable(struct sun6i_csi *csi);

#endif /* __SUN8I_A83T_MIPI_CSI2_H__ */
