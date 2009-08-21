
#pragma  once

  
/* ------------------- PHYTER FNCTS ------------------ */

INT16U read_phy ( INT16U phyadd ,INT8U  PhyReg) ;
INT8U write_phy (INT32U phyadd,INT32U PhyReg, INT32U Value);

INT16U read_phy_ex ( INT16U phyadd ,INT8U  PhyReg,INT16U *err);
void  write_phy_ex (INT8U  phy, INT8U  reg, INT16U  val, INT16U  *perr);

//INT8U lpc24xxether_init(rt_device_t dev);

void  nic_link_change(INT32U link_speed, INT32U link_duplex);


