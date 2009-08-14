#pragma  once

/* PHY Address */
#define  EMAC_CFG_PHY_ADDR                              0x13           
#define ETH_MTU         1514      /* Ethernet Frame Max Transfer Unit        */

void  nic_linkup (void);
void  nic_linkdown (void);

void  phy_hw_init (void);
void rt_delayms(rt_uint32_t ms);

void tx_descr_init (void) ;
void rx_descr_init(void);

rt_uint32_t  bsp_cpu_clk_freq (void);
void  nic_int_init  (void);
 
void set_mac_id( void )   ;

