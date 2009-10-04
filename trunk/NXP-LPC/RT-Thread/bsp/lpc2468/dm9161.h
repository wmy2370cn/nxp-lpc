

#pragma once

INT8U  get_phy_autoneg_state(void);
INT8U  get_phy_link_state (void);
INT32U  get_phy_link_speed (void);
INT32U  get_phy_link_duplex (void);
void  nic_phy_init   (INT16U *perr);
