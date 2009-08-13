#include <rtthread.h>
#include "applib.h"
#include "emac.h" 
#include "emac_def.h"
#include "dm9161_def.h"
#include "dm9161.h"
#include "net_bsp.h"
#include <LPC23xx.H>


/*********************************************************************************************************
** 函数名称: Write_PHY
** 函数名称: Write_PHY
**
** 功能描述：  
**
** 输　入:  INT32U phyadd   PHY address, normally 0.  
** 输　入:  INT32S PhyReg   PHY register.
** 输　入:  INT32S Value    Data to write to PHY register.
**          
** 输　出:   INT8U
**         
** 全局变量:  
** 调用模块: EMAC_Init().
**
** 作　者:  LiJin
** 日　期:  2009年7月31日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
rt_uint8_t write_phy (rt_uint32_t phyadd,rt_uint32_t PhyReg, rt_uint32_t Value)
{
	unsigned int tout;

	MAC_MCMD = MCMD_WRITE;			        // Issue a Write COMMAND     
	MAC_MADR = (phyadd<<8) | PhyReg;    //[12:8] == PHY addr, [4:0]=0x00(BMCR) register addr
	MAC_MWTD = Value;                   //Write the data to the Management Write Data register

	/* Wait utill operation completed */
	tout = 0;
	for (tout = 0; tout < MII_WR_TOUT; tout++)
	{
		if ((MAC_MIND & 1) == 0)
		{
			return TRUE;
		}
	}
	//超时
	return FALSE;
}
void  write_phy_ex (rt_uint8_t  phy, rt_uint8_t  reg, rt_uint16_t  val, rt_uint16_t  *perr)
{
	rt_uint8_t  retries;
	volatile  rt_uint32_t  i;


	retries     =   0;                                                  /* Initialize retries to 0                                  */

	MAC_MCMD        =   MCMD_WRITE;                                         /* Issue a Write COMMAND                                    */
	MAC_MADR        =  (phy << 8) | reg;                                    /* [12:8] == PHY addr, [4:0]=0x00(BMCR) register addr       */
	MAC_MWTD        =   val;                                                /* Write the data to the Management Write Data register     */

	while ((MAC_MIND != 0) && (retries < PHY_RDWR_RETRIES))
	{               /* Read the Management Indicator register, MII busy if > 0  */
		for (i = 0; i < 1000; i++)
		{                                    /* Delay while the read is in progress                      */
			;
		}
		retries++;
	}

	if (retries >= PHY_RDWR_RETRIES)
	{                                  /* If there are no retries remaining                        */
		*perr     = NET_PHY_ERR_REGWR_TIMEOUT;                           /* Return an error code if the PHY Read timed out           */
	}
	else 
	{
		*perr     = NET_PHY_ERR_NONE;
	}
}
/*********************************************************************************************************
** 函数名称: read_phy
** 函数名称: read_phy
**
** 功能描述：  从PHY端口读取数据
**
** 输　入:  INT16U phyadd      PHY address, normally 0.
** 输　入:  INT32U PHYReg      PHY register.
**          
** 输　出:   INT32U  PHY data  MRDD        PHY register data.
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年7月28日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/ 
rt_uint16_t read_phy ( rt_uint16_t phyadd ,rt_uint8_t  PhyReg) 
{
	rt_uint32_t tout = 0;

	MAC_MCMD =  0;            // Clear the Read COMMAND   
	MAC_MADR = (phyadd<<8) | PhyReg;  //[12:8] == PHY addr, [4:0]=0x00(BMCR) register addr 
	MAC_MCMD = MCMD_READ;             //   Issue a Read COMMAND 

	/* Wait until operation completed */
	for (tout = 0; tout < MII_RD_TOUT; tout++) 
	{
		if ((MAC_MIND & MIND_BUSY) == 0) 
		{
			break;
		}
	}
	return (MAC_MRDD);
}

rt_uint16_t read_phy_ex ( rt_uint16_t phyadd ,rt_uint8_t  PhyReg,rt_uint16_t *err)
{
	rt_uint32_t tout = 0;

	MAC_MCMD = 0;                     // Clear the Read COMMAND    
	MAC_MADR = (phyadd<<8) | PhyReg;  //[12:8] == PHY addr, [4:0]=0x00(BMCR) register addr 
	MAC_MCMD =  MCMD_READ;            // Issue a Read COMMAND 

	/* Wait until operation completed */
	for (tout = 0; tout < MII_RD_TOUT; tout++) 
	{
		if ((MAC_MIND & MIND_BUSY) == 0) 
		{
			break;
		}
	}
	if (tout == MII_RD_TOUT)
	{
		*err = NET_PHY_ERR_REGRD_TIMEOUT;
	}
	else
	{
		*err = NET_PHY_ERR_NONE;
	}
	return (MAC_MRDD);
}


/*********************************************************************************************************
** 函数名称: nic_link_change
** 函数名称: nic_link_change
**
** 功能描述：  This function is called by NetNIC_Init and the PHY ISR in order to update the
**             speed and duplex settings for the EMAC. 
** 输　入:  rt_uint32_t link_speed
** 输　入:  rt_uint32_t link_duplex
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
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
void  nic_link_change(rt_uint32_t link_speed, rt_uint32_t link_duplex)
{
	switch (link_speed)
	{
	case NET_PHY_SPD_0:                    /* Assume 10Mbps operation until linked                     */
	case NET_PHY_SPD_10:
		MAC_SUPP      &=  ~SUPP_SPEED;    /* Configure the RMII logic (if used) for 10MBps operation  */
		break;

	case NET_PHY_SPD_100:
		MAC_SUPP      |=   SUPP_SPEED;    /* Configure the RMII logic (if uses) for 100MBps operation */
		break;
	}

	switch (link_duplex) 
	{
	case NET_PHY_DUPLEX_UNKNOWN:         /* Assume half duplex until link duplex is known            */
	case NET_PHY_DUPLEX_HALF:
		MAC_MAC2      &=  ~MAC2_FULL_DUP;  /* Configure the EMAC to run in HALF duplex mode            */
		MAC_COMMAND   &=  ~CR_FULL_DUP;                        /* Configure the MII logic for a Half Duplex PHY Link       */
		MAC_IPGT       =   IPGT_HALF_DUP;	                            /* Set inter packet gap to the recommended Half Duplex      */
		break;

	case NET_PHY_DUPLEX_FULL:
		MAC_MAC2      |=   MAC2_FULL_DUP;                           /* Configure the EMAC to run in FULL duplex mode            */
		MAC_COMMAND   |=   CR_FULL_DUP;                        /* Configure the MII logic for a Full Duplex PHY Link       */
		MAC_IPGT       =   IPGT_FULL_DUP;	                            /* Set inter packet gap to the recommended Full Duplex      */
		break;
	}
}
/******************************************************************************
** Function name:		emac_rx_enanble/emac_rx_disable
**
** Descriptions:		EMAC RX API modules
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void emac_rx_enanble( void )
{
	MAC_COMMAND |= CR_RX_EN;
	MAC_MAC1    |=  CR_RX_EN;   
    return;    
}

void emac_rx_disable( void )
{
	MAC_COMMAND &= ~CR_RX_EN;
	MAC_MAC1    &= ~ CR_RX_EN;   

	 return;
}
/******************************************************************************
** Function name:		emac_tx_enable/emac_tx_disable
**
** Descriptions:		EMAC TX API modules
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void emac_tx_enable( void )
{
	MAC_COMMAND |= CR_TX_EN;
	return;
}

void emac_tx_disable( void )
{
	MAC_COMMAND &= ~CR_TX_EN;
	return;
}
/* ----------------- MCFG bits ---------------- */
#define  MCFG_CLKSEL_DIV4                             0x0000
#define  MCFG_CLKSEL_DIV6                             0x0008
#define  MCFG_CLKSEL_DIV8                             0x000C
#define  MCFG_CLKSEL_DIV10                            0x0010
#define  MCFG_CLKSEL_DIV14                            0x0014
#define  MCFG_CLKSEL_DIV20                            0x0018
#define  MCFG_CLKSEL_DIV28                            0x001C

static  rt_uint8_t        MII_Dividers [7][2] =  {{4,  MCFG_CLKSEL_DIV4},
{6,  MCFG_CLKSEL_DIV6},
{8,  MCFG_CLKSEL_DIV8},
{10, MCFG_CLKSEL_DIV10},
{14, MCFG_CLKSEL_DIV14},
{20, MCFG_CLKSEL_DIV20},
{28, MCFG_CLKSEL_DIV28}};
/*********************************************************************************************************
** 函数名称: rt_dm9161_init
** 函数名称: rt_dm9161_init
**
** 功能描述：  对芯片的工作寄存器进行设置,各个寄存器的用法可参考文档和络芯片的数据手册
**
** 输　入:  rt_device_t dev
**          
** 输　出:   rt_err_t
**         
** 全局变量:            
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年7月29日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/ 
rt_uint16_t PHYREG[80];
INT16U PHYID;
rt_err_t lpc24xxether_init(rt_device_t dev)
{
	unsigned int  tout, i = 0;
	rt_uint32_t  tempreg = 0;
	rt_uint16_t  ret = 0;

	rt_uint32_t clk_freq            =   bsp_cpu_clk_freq();  
	
	clk_freq           /=   100000;     
	nic_linkup();                                                    /* Set NetNIC_ConnStatus to TRUE by default (for uC/TCP-IP) */

	/* Power Up the EMAC controller. */
	PCONP |= (1 << 30);  
	phy_hw_init();

	/* Reset all EMAC internal modules. */
	MAC_MAC1 = MAC1_RES_TX | MAC1_RES_MCS_TX | MAC1_RES_RX | MAC1_RES_MCS_RX | MAC1_SIM_RES | MAC1_SOFT_RES;

	MAC_COMMAND = CR_REG_RES | CR_TX_RES | CR_RX_RES;

	/* A short delay after reset. */
	for (tout = 0; tout <5000; tout++);

	//Deassert all prior resets
	MAC_MAC1 = 0;
	emac_tx_disable();
	emac_rx_disable();
	/* Configure EMAC / PHY communication to RMII mode          */
	MAC_COMMAND            |=   CR_RMII;  
	/* Assume and configure RMII link speed logic for 10Mbit    */
	MAC_SUPP = 0;
	for (tout = 0; tout <5000; tout++);

	MAC_TEST                =   0;     
	MAC_MAXF = ETH_MAX_FLEN;

	/* Initialize MAC control registers. */
	MAC_MAC1 |= MAC1_PASS_ALL;
	MAC_MAC2 = MAC2_CRC_EN | MAC2_PAD_EN;
	MAC_RXFILTERCTRL =   RFC_BCAST_EN | RFC_PERFECT_EN;          /* Accept Broadcast and Perfect Address frames              */

	// 	/* Enable Reduced MII interface. */
	MAC_MCFG |= MCFG_CLK_DIV20 | MCFG_RES_MII;
	MAC_MCMD                =   0;                                          /* Clear MII command register                               */

	for (tout = 0; tout <5000; tout++);
	for (i = 0; i < 7; i++) 
	{                                           /* Check dividers to yield MII frequency ~2.5 MHz           */
		if ((clk_freq / MII_Dividers[i][0]) <=  25) 
		{                   /* Index [i][0] = decimal div value, [i][1] = MCFG reg val  */
			MAC_MCFG        =   MII_Dividers[i][1];                         /* Remove reset, set proper MIIM divider                    */
			break;
		}
	}
	/* Enable Reduced MII interface. */
	//CR_PASS_RUNT_FRM 为“1”时，将小于64字节的短帧传递到存储器中，除非该短帧的CRC有误为“0”，则将短帧被滤除。
	//	COMMAND |=  CR_PASS_RUNT_FRM;

	/* Reset Reduced MII Logic. */
	//PHY支持寄存器???

	//下面开始PHY设置
	// probe phy address
	for(i=0;i<32;i++)
	{
		ret = read_phy(i , PHY_REG_PHYID1 );
		if(ret == 0X0181)
		{
			PHYID = i;
			break;
		}
	}

	
	//	PHYID = 0;
	//  复位PHY芯片
	//  等待一段指定的时间，使PHY就绪 
	//	write_phy(PHYID, PHY_REG_BMCR,  BMCR_RESET|BMCR_ANRESTART|BMCR_ANENABLE  );
	for ( i = 0; i < 5; i++ )
	{
		rt_delayms(1000);
	}

	for(i=0;i<32;i++)
	{
		PHYREG[i] = read_phy_ex(EMAC_CFG_PHY_ADDR ,i ,&ret);
	}

	nic_phy_init(&ret); 

	tempreg = read_phy(PHYID, DM9161_DSCSR );

	MAC_IPGR = IPGR_DEF;
	MAC_CLRT = CLRT_DEF;

	//设置MAC地址
	set_mac_id();
	
	/* Receive Broadcast, Unicast ,Multicast and Perfect Match Packets */
//	MAC_RXFILTERCTRL = RFC_UCAST_EN |RFC_MCAST_EN | RFC_BCAST_EN | RFC_PERFECT_EN;



	for(i=0;i<32;i++)
	{
		PHYREG[i] = read_phy_ex(EMAC_CFG_PHY_ADDR ,i ,&ret);
	}
	// Initialize Tx and Rx DMA Descriptors 
	tx_descr_init();
	rx_descr_init();
	/* Enable EMAC interrupts. */
//	MAC_INTENABLE = INT_RX_DONE | INT_TX_DONE;


	/* Reset all interrupts */

	MAC_INTCLEAR            =  (INT_RX_OVERRUN   |                          /* Clear all EMAC interrupt sources                         */
		INT_RX_ERR       |
		INT_RX_FIN       |
		INT_RX_DONE      |
		INT_TX_UNDERRUN  |
		INT_TX_ERR       |
		INT_TX_FIN       |
		INT_TX_DONE      |
		INT_SOFT_INT     |
		INT_WAKEUP);
	/* update mac address */
//	update_mac_address(&lpc24xx_device);

	nic_int_init();
	/* Enable receive and transmit mode of MAC Ethernet core */
	emac_rx_enanble();
	emac_tx_enable();
	return RT_EOK;
}

