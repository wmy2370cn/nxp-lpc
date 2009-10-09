 


#include <rtthread.h>			    
#include <LPC24xx.H> 
#include "applib.h"
#include "dm9161_def.h"
#include "nic_bsp.h"
#include "emac.h" 
 

/*********************************************************************************************************
** 函数名称: get_phy_autoneg_state
** 函数名称: get_phy_autoneg_state
**
** 功能描述：  Returns state of auto-negotiation
**
** 输　入:  void
**          
** 输　出:   rt_uint8_t  State of auto-negociation (FALSE = not completed, TRUE = completed).
**         
** 全局变量:  
** 调用模块:  init
**
** 作　者:  LiJin
** 日　期:  2009年8月3日
** 备  注:  If any error is encountered while reading the PHY, this function 
will return Auto Negotiation State = FALSE (incomplete).
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
rt_uint8_t  get_phy_autoneg_state(void)
{
	rt_uint16_t     err = 0;
	rt_uint32_t  reg_val;

	reg_val     = read_phy_ex(EMAC_CFG_PHY_ADDR, PHY_REG_BMSR, &err);
	reg_val     = read_phy_ex(EMAC_CFG_PHY_ADDR, PHY_REG_BMSR, &err);

	if (err   != NET_PHY_ERR_NONE) 
	{
		reg_val = 0;
	}
	/* DM9161AE register 0x01: Basic Status Register #1      */
	/* BIT 5 Signal the result of the auto negotiation   */  
	if ((reg_val & BMSR_ANEGCOMPLETE) == BMSR_ANEGCOMPLETE)
	{  
		return (RT_TRUE);                                     
	} 
	else 
	{   /* 1 = complete, 0 = incomplete                          */
		return (RT_FALSE);
	}
}
/*********************************************************************************************************
** 函数名称: get_phy_link_state
** 函数名称: get_phy_link_state
**
** 功能描述：   Returns state of ethernet link
**
** 输　入:  void
**          
** 输　出:   rt_uint8_t  State of ethernet link (FALSE = link down, TRUE = link up).
**         
** 全局变量:  
** 调用模块:  isr
**
** 作　者:  LiJin
** 日　期:  2009年8月3日
** 备  注:   If any error is encountered while reading the PHY, this function
will return link state = FALSE.
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
rt_uint8_t  get_phy_link_state (void)
{
	rt_uint16_t     err = 0;
	rt_uint32_t  reg_val;

	/* DM9161AE register 0x01: Basic Status Register #1      */
	/* BIT 2 , Link Status, 1 = linked, 0 = not linked.      */
	reg_val      = read_phy_ex(EMAC_CFG_PHY_ADDR, PHY_REG_BMSR, &err);
	reg_val      = read_phy_ex(EMAC_CFG_PHY_ADDR, PHY_REG_BMSR, &err);

	if (err   != NET_PHY_ERR_NONE)
	{
		reg_val  = 0;
	}

	reg_val     &= BMSR_LSTATUS;

	if (reg_val == BMSR_LSTATUS)
	{
		return (RT_TRUE);
	} 
	else 
	{
		return (RT_FALSE);
	}
}
/*********************************************************************************************************
** 函数名称: get_phy_link_speed
** 函数名称: get_phy_link_speed
**
** 功能描述： Returns the speed of the current Ethernet link 
**
** 输　入:  void
**          
** 输　出:   rt_uint32_t  0 = No Link, 10 = 10mbps, 100 = 100mbps
**         
** 全局变量:  
** 调用模块: init
**
** 作　者:  LiJin
** 日　期:  2009年8月3日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
rt_uint32_t  get_phy_link_speed (void)
{
	rt_uint32_t  bmsr;
	rt_uint32_t  bmcr;
	rt_uint32_t  lpa;
	rt_uint16_t   err;

	bmsr    = read_phy_ex(EMAC_CFG_PHY_ADDR, PHY_REG_BMSR, &err);       /* Get Link Status from PHY status reg. Requires 2 reads    */
	bmsr    = read_phy_ex(EMAC_CFG_PHY_ADDR, PHY_REG_BMSR, &err);       /* Get Link Status from PHY status reg. Requires 2 reads    */

	if ((bmsr & BMSR_LSTATUS) == 0)
	{
		return (NET_PHY_SPD_0);                                         /* No link                                                  */
	}

	bmcr    = read_phy_ex(EMAC_CFG_PHY_ADDR, PHY_REG_BMCR, &err);       /* Read the PHY Control Register                            */

	if ((bmcr & BMCR_ANENABLE) == BMCR_ANENABLE)
	{	/* If AutoNegotiation is enabled                            */
		if ((bmsr & BMSR_ANEGCOMPLETE) == 0)
		{                          /* If AutoNegotiation is not complete                       */
			return (NET_PHY_SPD_0);       					            /* AutoNegotitation in progress                             */
		}

		lpa = read_phy_ex(EMAC_CFG_PHY_ADDR, PHY_REG_ANLPAR, &err);     /* Read the Link Partner Ability Register                   */

		if (((lpa & ANLPAR_100FULL) == ANLPAR_100FULL) || ((lpa & ANLPAR_100HALF) == ANLPAR_100HALF))
		{
			return (NET_PHY_SPD_100);
		} 
		else 
		{
			return (NET_PHY_SPD_10);
		}
	} 
	else
	{  /* Auto-negotiation not enabled, get speed from BMCR        */
		if ((bmcr & BMCR_SPEED100) == BMCR_SPEED100) 
		{
			return (NET_PHY_SPD_100);
		}
		else 
		{
			return (NET_PHY_SPD_10);
		}
	}
}
/*********************************************************************************************************
** 函数名称: get_phy_link_duplex
** 函数名称: get_phy_link_duplex
**
** 功能描述：  Returns the duplex mode of the current Ethernet link
**            his probes the Davicom DM9161AE '3.3V Dual-Speed Fast Ethernet Transceiver'
** 输　入:  void
**          
** 输　出:   rt_uint32_t  0 = Unknown (Auto-Neg in progress), 1 = Half Duplex, 2 = Full Duplex
**         
** 全局变量:  
** 调用模块: EMAC_Init()
**
** 作　者:  LiJin
** 日　期:  2009年8月10日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
rt_uint32_t  get_phy_link_duplex (void)
{
	rt_uint32_t  bmcr;
	rt_uint32_t  bmsr;
	rt_uint32_t  lpa;
	rt_uint16_t     err;

	bmsr    = read_phy_ex(EMAC_CFG_PHY_ADDR, PHY_REG_BMSR, &err);       /* Get Link Status from PHY status reg. Requires 2 reads    */
	bmsr    = read_phy_ex(EMAC_CFG_PHY_ADDR, PHY_REG_BMSR, &err);       /* Get Link Status from PHY status reg. Requires 2 reads    */

	if ((bmsr & BMSR_LSTATUS) == 0)
	{
		return (NET_PHY_DUPLEX_UNKNOWN);   /* No link, return 'Duplex Uknown'                          */
	}

	bmcr    = read_phy_ex(EMAC_CFG_PHY_ADDR, PHY_REG_BMCR, &err);       /* Read the PHY Control Register      */

	if ((bmcr & BMCR_ANENABLE) == BMCR_ANENABLE)
	{	/* If AutoNegotiation is enabled   */
		if ((bmsr & BMSR_ANEGCOMPLETE) == 0) 
		{     /* If AutoNegotiation is not complete                       */
			return (NET_PHY_DUPLEX_UNKNOWN);   /* AutoNegotitation in progress       */
		}

		lpa = read_phy_ex(EMAC_CFG_PHY_ADDR, PHY_REG_ANLPAR, &err);     /* Read the Link Partner Ability Register                   */

		if (((lpa & ANLPAR_100FULL) == ANLPAR_100FULL) || ((lpa & ANLPAR_10FULL) == ANLPAR_10FULL)) 
		{
			return (NET_PHY_DUPLEX_FULL);
		}
		else 
		{
			return (NET_PHY_DUPLEX_HALF);
		}
	} 
	else
	{  /* Auto-negotiation not enabled, get duplex from BMCR       */
		if ((bmcr & BMCR_FULLDPLX) == BMCR_FULLDPLX)
		{
			return (NET_PHY_DUPLEX_FULL);
		} 
		else
		{
			return (NET_PHY_DUPLEX_HALF);
		}
	}
}

static void  phy_auto_nego  (void)
{
	rt_uint16_t   i;
	rt_uint16_t   reg_val;
	rt_uint8_t  link;
	rt_uint16_t      err;
	rt_uint32_t  tout = 0;

	i               = DM9161AE_INIT_AUTO_NEG_RETRIES;                   /* Set the # of retries before declaring a timeout  */
	link            = get_phy_link_state();                            /* Get the current link state. 1=linked, 0=no link  */

	if (link == RT_FALSE) 
	{
		// #if (!defined(EMAC_CFG_RMII)) || (EMAC_CFG_RMII <= 0)
		// 		reg_val     = NetNIC_PhyRegRd(EMAC_CFG_PHY_ADDR, MII_BMCR, &err);   /* Get current control register value           */
		// 		reg_val    |= DEF_BIT_09;                                       /* Set the auto-negotiation start bit               */
		// 
		// 		NetNIC_PhyRegWr(AT91C_PHY_ADDR, MII_BMCR, reg_val, &err);       /* Initiate auto-negotiation                        */
		// #endif

		do {                                                            /* Do while auto-neg incomplete, or retries expired */
			for (tout = 5000; tout; tout--);
			/* Wait for a while auto-neg to proceed (net_bsp.c) */
			reg_val = read_phy_ex(EMAC_CFG_PHY_ADDR, PHY_REG_BMSR, &err);   /* Read the Basic Mode Status Register          */
			reg_val = read_phy_ex(EMAC_CFG_PHY_ADDR, PHY_REG_BMSR, &err);   /* Read the Basic Mode Status Register          */
			i--;
		} while (((reg_val & BMSR_LSTATUS) == 0) && (i > 0));           /* While link not established and retries remain    */
	}
} 
static void  nic_phy_int_enter (void)
{

} 
static void nic_phy_int_clr(void)
{
	IO2_INT_CLR               =  DEF_BIT_11;
}
/*
*********************************************************************************************************
*                                        nic_phy_isr_handler()
*
* Description : (1) Update NetNIC_ConnStatus according to link state
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/
rt_uint16_t  NetNIC_ConnStatus;                            /* NIC's connection status : DEF_ON/DEF_OFF.            */

void  nic_phy_isr_handler   (int vector)
{
	volatile rt_uint16_t reg_val;
	rt_uint16_t     err;


 	nic_phy_int_enter();

	NetNIC_ConnStatus = get_phy_link_state()  ;                          /* Set NetNIC_ConnStatus according to link state            */

	if (NetNIC_ConnStatus == RT_TRUE)
	{
		nic_linkup();
	} 
	else 
	{
		nic_linkdown();
	}

	reg_val     = read_phy_ex(EMAC_CFG_PHY_ADDR, DM9161_MDINTR, &err);  /* Clear interrupts                                     */
	nic_phy_int_clr();
} 

void  nic_phy_int_init   (void)
{
	IO2_INT_CLR              |=  DEF_BIT_11;
	IO2_INT_EN_R              |=  DEF_BIT_11;
//	rt_hw_interrupt_install(VIC_EINT3, nic_phy_isr_handler, RT_NULL);
//	rt_hw_interrupt_umask(VIC_EINT3);

//	VICIntSelect           &= ~(1 << VIC_EINT3);                        /* Enable interrupts                                        */
//	VICVectAddr17           = (CPU_rt_uint32_t)NetNIC_PhyISR_Handler;        /* Set the vector address                                   */
//	VICIntEnable            =  (1 << VIC_EINT3);                        /* Enable Interrupts                                        */
}
/*
*********************************************************************************************************
*                                         NetNIC_PhyInit()
*
* Description : Initialize phyter (ethernet link controller)
*               This instance configures the Davicom DM9161AE PHY
*
* Argument(s) : none.
*
* Return(s)   : 1 for OK, 0 for error
*
* Caller(s)   : EMAC_Init()
*
* Note(s)     : Assumes the MDI port as already been enabled for the PHY.
*********************************************************************************************************
*/
void  nic_phy_init   (rt_uint16_t *perr)
{
	volatile  rt_uint32_t  reg_val;
 
	phy_auto_nego();                                                /* Perform auto-negotiation                                 */

	NetNIC_ConnStatus = get_phy_link_state();      /* Set NetNIC_ConnStatus according to link state            */

	if (NetNIC_ConnStatus == RT_TRUE)
	{
		nic_linkup();
	} 
	else
	{
		nic_linkdown();
	}
	//挂接中断 
	nic_phy_int_init();

	reg_val     = read_phy_ex(EMAC_CFG_PHY_ADDR, DM9161_MDINTR, perr); /* Clear interrupts                                         */
	reg_val    |= MDINTER_FDX_MSK | MDINTER_SPD_MSK | MDINTER_LINK_MSK | MDINTER_INTR_MSK;
	reg_val    &= ~(MDINTER_SPD_MSK | MDINTER_LINK_MSK | MDINTER_INTR_MSK);

	write_phy_ex(EMAC_CFG_PHY_ADDR, DM9161_MDINTR, reg_val, perr);   /* Enable link change interrupt                             */
}
