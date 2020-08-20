/* SPDX-License-Identifier: GPL-2.0 */
/*
 * sun6i_dphy.h
 * Copyright Kévin L'hôpital (C) 2020
 */

#ifndef __SUN8I_A83T_DPHY_H__
#define __SUN8I_A83T_DPHY_H__

#include <linux/regmap.h>
#include "sun6i_csi.h"

void sun6i_dphy_first_init(struct sun6i_csi_dev *sdev);
void sun6i_dphy_second_init(struct sun6i_csi_dev *sdev);

#endif /* __SUN8I_A83T_DPHY_H__ */
