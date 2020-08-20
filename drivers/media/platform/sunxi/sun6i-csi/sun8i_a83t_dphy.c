// SPDX-License-Identifier: GPL-2.0
/*
 * sun6i_dphy.c
 * Copyright Kévin L'hôpital (C) 2020
 */

#include "sun8i_a83t_dphy.h"
#include "sun8i_a83t_dphy_reg.h"

/* First initialization to turn on the dphy for the MIPI CSI2 controller
 * initialization.
 */

void sun6i_dphy_first_init(struct sun6i_csi_dev *sdev)
{
	regmap_update_bits(sdev->regmap, DPHY_CTRL_REG, DPHY_CTRL_REG_DBG,
			   DPHY_CTRL_REG_DBG);
	regmap_update_bits(sdev->regmap, DPHY_CTRL_REG, DPHY_CTRL_REG_SHUT, 0);
	regmap_update_bits(sdev->regmap, DPHY_CTRL_REG, DPHY_CTRL_REG_RSTN,
			   DPHY_CTRL_REG_RSTN);
}

/* Second initialization to turn off the dphy and do its initialization. */
void sun6i_dphy_second_init(struct sun6i_csi_dev *sdev)
{
	regmap_update_bits(sdev->regmap, DPHY_CTRL_REG, DPHY_CTRL_REG_DBG, 0);
	regmap_update_bits(sdev->regmap, DPHY_CTRL_REG, DPHY_CTRL_REG_SHUT,
			   DPHY_CTRL_REG_SHUT);
	regmap_update_bits(sdev->regmap, DPHY_CTRL_REG, DPHY_CTRL_REG_RSTN,
			   DPHY_CTRL_REG_RSTN);
	regmap_update_bits(sdev->regmap, DPHY_ANA0_REG, DPHY_ANA0_REG_SNK_MASK,
			   DPHY_ANA0_REG_SNK(0x02));
	regmap_update_bits(sdev->regmap, DPHY_ANA0_REG, DPHY_ANA0_REG_RINT_MASK,
			   DPHY_ANA0_REG_RINT(0x02));
	regmap_update_bits(sdev->regmap, DPHY_ANA0_REG, DPHY_ANA0_REG_REXT, 0);
	regmap_update_bits(sdev->regmap, DPHY_ANA0_REG, DPHY_ANA0_REG_ENREXT,
			   DPHY_ANA0_REG_ENREXT);
}

