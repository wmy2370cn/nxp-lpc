

#pragma once

rt_uint8_t  get_phy_autoneg_state(void);
rt_uint8_t  get_phy_link_state (void);
rt_uint32_t  get_phy_link_speed (void);
rt_uint32_t  get_phy_link_duplex (void);
void  nic_phy_init   (rt_uint16_t *perr);