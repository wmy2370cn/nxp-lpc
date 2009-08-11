
#pragma  once

  
/* ------------------- PHYTER FNCTS ------------------ */

rt_uint16_t read_phy ( rt_uint16_t phyadd ,rt_uint8_t  PhyReg) ;
rt_uint8_t write_phy (rt_uint32_t phyadd,rt_uint32_t PhyReg, rt_uint32_t Value);

rt_uint16_t read_phy_ex ( rt_uint16_t phyadd ,rt_uint8_t  PhyReg,rt_uint16_t *err);
void  write_phy_ex (rt_uint8_t  phy, rt_uint8_t  reg, rt_uint16_t  val, rt_uint16_t  *perr);

rt_err_t rt_emac_init(rt_device_t dev);

void  nic_link_change(rt_uint32_t link_speed, rt_uint32_t link_duplex);


