// SPDX-License-Identifier: GPL-2.0
/*
 * Allwinner A83t MIPI Camera Sensor Interface 2 driver
 * Copyright Kévin L'hôpital (C) 2020
 */

#include <linux/clk.h>
#include "sun8i_a83t_mipi_csi2.h"
#include "sun8i_a83t_mipi_csi2_reg.h"
#include "sun8i_a83t_dphy.h"
#include <linux/delay.h>

#define IS_FLAG(x, y) (((x) & (y)) == y)

enum mipi_csi2_pkt_fmt {
	MIPI_FS           = 0X00,
	MIPI_FE           = 0X01,
	MIPI_LS           = 0X02,
	MIPI_LE           = 0X03,
	MIPI_SDAT0          = 0X08,
	MIPI_SDAT1          = 0X09,
	MIPI_SDAT2          = 0X0A,
	MIPI_SDAT3          = 0X0B,
	MIPI_SDAT4          = 0X0C,
	MIPI_SDAT5          = 0X0D,
	MIPI_SDAT6          = 0X0E,
	MIPI_SDAT7          = 0X0F,
	MIPI_BLK            = 0X11,
	MIPI_EMBD         = 0X12,
	MIPI_YUV420       = 0X18,
	MIPI_YUV420_10    = 0X19,
	MIPI_YUV420_CSP   = 0X1C,
	MIPI_YUV420_CSP_10 =  0X1D,
	MIPI_YUV422       = 0X1E,
	MIPI_YUV422_10    = 0X1F,
	MIPI_RGB565       = 0X22,
	MIPI_RGB888       = 0X24,
	MIPI_RAW8         = 0X2A,
	MIPI_RAW10          = 0X2B,
	MIPI_RAW12          = 0X2C,
	MIPI_USR_DAT0     = 0X30,
	MIPI_USR_DAT1     = 0X31,
	MIPI_USR_DAT2     = 0X32,
	MIPI_USR_DAT3     = 0X33,
	MIPI_USR_DAT4     = 0X34,
	MIPI_USR_DAT5     = 0X35,
	MIPI_USR_DAT6     = 0X36,
	MIPI_USR_DAT7     = 0X37,
};

static inline struct sun6i_csi_dev *sun6i_csi_to_dev(struct sun6i_csi *csi)
{
	return container_of(csi, struct sun6i_csi_dev, csi);
}

static enum mipi_csi2_pkt_fmt get_pkt_fmt(u16 bus_pix_code)
{
	switch (bus_pix_code) {
	case MEDIA_BUS_FMT_RGB565_1X16:
		return MIPI_RGB565;
	case MEDIA_BUS_FMT_UYVY8_2X8:
	case MEDIA_BUS_FMT_UYVY8_1X16:
		return MIPI_YUV422;
	case MEDIA_BUS_FMT_UYVY10_2X10:
		return MIPI_YUV422_10;
	case MEDIA_BUS_FMT_RGB888_1X24:
		return MIPI_RGB888;
	case MEDIA_BUS_FMT_SBGGR8_1X8:
	case MEDIA_BUS_FMT_SGBRG8_1X8:
	case MEDIA_BUS_FMT_SGRBG8_1X8:
	case MEDIA_BUS_FMT_SRGGB8_1X8:
		return MIPI_RAW8;
	case MEDIA_BUS_FMT_SBGGR10_1X10:
	case MEDIA_BUS_FMT_SGBRG10_1X10:
	case MEDIA_BUS_FMT_SGRBG10_1X10:
	case MEDIA_BUS_FMT_SRGGB10_1X10:
		return MIPI_RAW10;
	case MEDIA_BUS_FMT_SBGGR12_1X12:
	case MEDIA_BUS_FMT_SGBRG12_1X12:
	case MEDIA_BUS_FMT_SGRBG12_1X12:
	case MEDIA_BUS_FMT_SRGGB12_1X12:
		return MIPI_RAW12;
	default:
		return MIPI_RAW8;
	}
}

void sun6i_mipi_csi_set_stream(struct sun6i_csi *csi, bool enable)
{
	struct sun6i_csi_dev *sdev = sun6i_csi_to_dev(csi);

	if (enable)
		regmap_update_bits(sdev->regmap, MIPI_CSI2_CFG_REG,
				   MIPI_CSI2_CFG_REG_SYNC_EN,
				   MIPI_CSI2_CFG_REG_SYNC_EN);
	else
		regmap_update_bits(sdev->regmap, MIPI_CSI2_CFG_REG,
				   MIPI_CSI2_CFG_REG_SYNC_EN, 0);
}

void sun6i_mipi_csi_init(struct sun6i_csi_dev *sdev)
{
	regmap_update_bits(sdev->regmap, MIPI_CSI2_CTRL_REG,
			   MIPI_CSI2_CTRL_REG_RSTN, MIPI_CSI2_CTRL_REG_RSTN);
	regmap_write(sdev->regmap, MIPI_CSI2_RX_PKT_NUM_REG, 0xb8d257f8);
	sun6i_dphy_first_init(sdev);
	regmap_write(sdev->regmap, MIPI_CSI2_RSVD1_REG,
		     HW_LOCK_REGISTER_VALUE_1);
	regmap_write(sdev->regmap, MIPI_CSI2_RSVD2_REG,
		     HW_LOCK_REGISTER_VALUE_2);
	regmap_write(sdev->regmap, MIPI_CSI2_RX_PKT_NUM_REG, 0);
	regmap_write(sdev->regmap, MIPI_CSI2_VCDT0_REG, 0);
	regmap_update_bits(sdev->regmap, MIPI_CSI2_CFG_REG,
			   MIPI_CSI2_CFG_REG_SYNC_DLY_CYCLE_MASK,
			   MIPI_CSI2_CFG_REG_SYNC_DLY_CYCLE(0x11));
	regmap_update_bits(sdev->regmap, MIPI_CSI2_CFG_REG,
			   MIPI_CSI2_CFG_REG_N_BYTE, 0);
	regmap_update_bits(sdev->regmap, MIPI_CSI2_CFG_REG,
			   MIPI_CSI2_CFG_REG_YC_SWAB, 0);
	regmap_update_bits(sdev->regmap, MIPI_CSI2_CFG_REG,
			   MIPI_CSI2_CFG_REG_NONE_UNPKT_RX_MODE,
			   MIPI_CSI2_CFG_REG_NONE_UNPKT_RX_MODE);
	regmap_update_bits(sdev->regmap, MIPI_CSI2_CFG_REG,
			   MIPI_CSI2_CFG_REG_UNPKT_EN,
			   MIPI_CSI2_CFG_REG_UNPKT_EN);
	regmap_update_bits(sdev->regmap, MIPI_CSI2_CFG_REG,
			   MIPI_CSI2_CFG_REG_BYPASS_ECC_EN,
			   MIPI_CSI2_CFG_REG_BYPASS_ECC_EN);
	regmap_update_bits(sdev->regmap, MIPI_CSI2_CFG_REG,
			   MIPI_CSI2_CFG_REG_SYNC_EN,
			   MIPI_CSI2_CFG_REG_SYNC_EN);
	sun6i_dphy_second_init(sdev);
	regmap_update_bits(sdev->regmap, MIPI_CSI2_CTRL_REG,
			   MIPI_CSI2_CTRL_REG_RSTN, MIPI_CSI2_CTRL_REG_RSTN);
	regmap_update_bits(sdev->regmap, MIPI_CSI2_CFG_REG,
			   MIPI_CSI2_CFG_REG_SYNC_DLY_CYCLE_MASK,
			   MIPI_CSI2_CFG_REG_SYNC_DLY_CYCLE(0x08));
	regmap_update_bits(sdev->regmap, MIPI_CSI2_CFG_REG,
			   MIPI_CSI2_CFG_REG_NONE_UNPKT_RX_MODE, 0);
	regmap_update_bits(sdev->regmap, MIPI_CSI2_CFG_REG,
			   MIPI_CSI2_CFG_REG_BYPASS_ECC_EN, 0);
	regmap_update_bits(sdev->regmap, MIPI_CSI2_CFG_REG,
			   MIPI_CSI2_CFG_REG_SYNC_EN, 0);
}

void sun6i_mipi_csi_setup_bus(struct sun6i_csi *csi)
{
	struct v4l2_fwnode_endpoint *endpoint = &csi->v4l2_ep;
	struct sun6i_csi_dev *sdev = sun6i_csi_to_dev(csi);
	int lane_num = endpoint->bus.mipi_csi2.num_data_lanes;
	int flags = endpoint->bus.mipi_csi2.flags;
	int total_rx_ch = 0;
	int vc;

	sun6i_mipi_csi_init(sdev);

	if (IS_FLAG(flags, V4L2_MBUS_CSI2_CHANNEL_0)) {
		vc = 0;
		total_rx_ch++;
	}

	if (!total_rx_ch) {
		dev_dbg(sdev->dev,
			 "No receive channel assigned, using channel 0.\n");
		vc = 0;
		total_rx_ch++;
	}
	/* Set lane. */
	regmap_write_bits(sdev->regmap, MIPI_CSI2_CFG_REG,
			  MIPI_CSI2_CFG_REG_N_LANE_MASK, (lane_num - 1) <<
			  MIPI_CSI2_CFG_REG_N_LANE_SHIFT);
	/* Set total channels. */
	regmap_write_bits(sdev->regmap, MIPI_CSI2_CFG_REG,
			  MIPI_CSI2_CFG_REG_N_CHANNEL_MASK, (total_rx_ch - 1) <<
			  MIPI_CSI2_CFG_REG_N_CHANNEL_SHIFT);

	regmap_write_bits(sdev->regmap, MIPI_CSI2_VCDT0_REG,
			  MIPI_CSI2_VCDT0_REG_CH0_DT_MASK,
			  get_pkt_fmt(csi->config.code));
	regmap_write_bits(sdev->regmap, MIPI_CSI2_VCDT0_REG,
			  MIPI_CSI2_VCDT0_REG_CH0_VC_MASK,
			  vc << MIPI_CSI2_VCDT0_REG_CH0_VC_SHIFT);
}

int sun6i_mipi_csi_clk_enable(struct sun6i_csi *csi)
{
	struct sun6i_csi_dev *sdev = sun6i_csi_to_dev(csi);
	int ret;

	ret = clk_prepare_enable(sdev->clk_mipi);
	if (ret) {
		dev_err(sdev->dev, "Enable clk_mipi clk err %d\n", ret);
		return ret;
	}

	ret = clk_prepare_enable(sdev->clk_misc);
	if (ret) {
		dev_err(sdev->dev, "Enable clk_misc clk err %d\n", ret);
		goto clk_mipi_disable;
	}

	return 0;

clk_mipi_disable:
	clk_disable_unprepare(sdev->clk_mipi);
	return ret;
}

void sun6i_mipi_csi_clk_disable(struct sun6i_csi *csi)
{
	struct sun6i_csi_dev *sdev = sun6i_csi_to_dev(csi);

	clk_disable_unprepare(sdev->clk_misc);
	clk_disable_unprepare(sdev->clk_mipi);
}


