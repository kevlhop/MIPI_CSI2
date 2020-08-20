/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Allwinner A83t DPHY register description
 * Copyright Kévin L'hôpital (C) 2020
 */

#ifndef __SUN8I_A83T_DPHY_REG_H__
#define __SUN8I_A83T_DPHY_REG_H__


#define DPHY_OFFSET			0x1000

#define DPHY_CTRL_REG			(DPHY_OFFSET + 0x010)
#define DPHY_CTRL_REG_RSTN		BIT(31)
#define DPHY_CTRL_REG_SHUT              BIT(15)
#define DPHY_CTRL_REG_DBG               BIT(8)

#define DPHY_STATUS_REG			(DPHY_OFFSET + 0x014)
#define DPHY_STATUS_REG_CLK_STOP	BIT(10)
#define DPHY_STATUS_REG_CLK_UPLS        BIT(9)
#define DPHY_STATUS_REG_HSCLK           BIT(8)
#define DPHY_STATUS_REG_D3_STOP         BIT(7)
#define DPHY_STATUS_REG_D2_STOP         BIT(6)
#define DPHY_STATUS_REG_D1_STOP         BIT(5)
#define DPHY_STATUS_REG_D0_STOP         BIT(4)
#define DPHY_STATUS_REG_D3_UPLS         BIT(3)
#define DPHY_STATUS_REG_D2_UPLS         BIT(2)
#define DPHY_STATUS_REG_D1_UPLS         BIT(1)
#define DPHY_STATUS_REG_D0_UPLS         BIT(0)

#define DPHY_ANA0_REG			(DPHY_OFFSET + 0x030)
#define DPHY_ANA0_REG_ENREXT		BIT(31)
#define DPHY_ANA0_REG_REXT              BIT(30)
#define DPHY_ANA0_REG_RINT_MASK         GENMASK(29, 28)
#define DPHY_ANA0_REG_RINT(v)           ((v) << 28)
#define DPHY_ANA0_REG_SNK_MASK          GENMASK(22, 20)
#define DPHY_ANA0_REG_SNK(v)            ((v) << 20)

#endif /* __SUN8I_A83T_DPHY_REG_H__ */
