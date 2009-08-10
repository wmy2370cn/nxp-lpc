#include <rtthread.h>
#include "dm9161.h"

#include <netif/ethernetif.h>
#include "lwipopts.h"
#include "emac.h"
#include "applib.h"
#include "iolpc23xx.h"
#include "emac.h"

#define  DM9161AE_INIT_AUTO_NEG_RETRIES        3

#define  DM9161AE_OUI                   0x00606E
#define  DM9161AE_VNDR_MDL                  0x08

INT16U PHYID;
 
#define MAX_ADDR_LEN 6
struct rt_lpc24xx_eth
{
	/* inherit from ethernet device */
	struct eth_device parent;

	/* interface address info. */
	rt_uint8_t  dev_addr[MAX_ADDR_LEN];			/* hw address	*/
};


//此处需要再确认下要不要??????
#define RB_BUFFER_SIZE		8			/* max number of receive buffers */
#define ETH_RX_BUF_SIZE		128

#define TB_BUFFER_SIZE		4
#define ETH_TX_BUF_SIZE		(PBUF_POOL_BUFSIZE)

typedef struct {                        /* RX Descriptor struct              */
	rt_uint32_t Packet;
	rt_uint32_t Ctrl;
} RX_Desc;

typedef struct {                        /* RX Status struct                  */
	rt_uint32_t Info;
	rt_uint32_t HashCRC;
} RX_Stat;

typedef struct {                        /* TX Descriptor struct              */
	rt_uint32_t Packet;
	rt_uint32_t Ctrl;
} TX_Desc;

typedef struct {                        /* TX Status struct                  */
	rt_uint32_t Info;
} TX_Stat;



static rt_uint32_t 	current_rb_index;						/* current receive buffer index */

 
//static            RX_Desc rb_descriptors[NUM_RX_FRAG];
//static            TX_Desc tb_descriptors[NUM_TX_FRAG];

static            RX_Desc *rb_descriptors ;
static            TX_Desc *tb_descriptors ;

// static  RX_Stat Rx_Stat[NUM_RX_FRAG]; /* Must be 8-Byte alligned   */
// static  TX_Stat Tx_Stat[NUM_TX_FRAG];
static  RX_Stat *Rx_Stat ; /* Must be 8-Byte alligned   */
static  TX_Stat *Tx_Stat ;
static  rt_uint8_t       *RxBufBaseAddr;
static  rt_uint8_t       *TxBufBaseAddr;

#define  EMAC_RX_DESC_BASE_ADDR                (EMAC_RAM_BASE_ADDR)
#define  EMAC_RX_STATUS_BASE_ADDR              (EMAC_RX_DESC_BASE_ADDR   + (NUM_RX_FRAG * sizeof(RX_Desc)))
#define  EMAC_TX_DESC_BASE_ADDR                (EMAC_RX_STATUS_BASE_ADDR + (NUM_RX_FRAG * sizeof(RX_Stat)))
#define  EMAC_TX_STATUS_BASE_ADDR              (EMAC_TX_DESC_BASE_ADDR   + (NUM_TX_FRAG * sizeof(TX_Desc)))
#define  EMAC_RX_BUFF_BASE_ADDR                (EMAC_TX_STATUS_BASE_ADDR + (NUM_TX_FRAG * sizeof(TX_Stat)))
#define  EMAC_TX_BUFF_BASE_ADDR                (EMAC_RX_BUFF_BASE_ADDR   + (NUM_RX_FRAG * ETH_FRAG_SIZE))

/* EMAC local DMA buffers. */
//static rt_uint32_t rx_buf[NUM_RX_FRAG][ETH_FRAG_SIZE>>2];
//static rt_uint32_t tx_buf[NUM_TX_FRAG][ETH_FRAG_SIZE>>2];


static struct rt_lpc24xx_eth lpc24xx_device;

static struct rt_semaphore tx_sem;


//  function added to initialize Rx Descriptors
void RxDescrInit (void)
{
	/* Initialize Receive Descriptor and Status array. */
	unsigned int i = 0;

	rb_descriptors      =  (RX_Desc *)(EMAC_RX_DESC_BASE_ADDR);
 	Rx_Stat            =  (RX_Stat  *)(EMAC_RX_STATUS_BASE_ADDR);
 	RxBufBaseAddr       =  (rt_uint8_t *)(EMAC_RX_BUFF_BASE_ADDR);
 
	for (i = 0; i < NUM_RX_FRAG; i++) 
	{ 
		rb_descriptors[i].Packet  = (rt_uint32_t)(RxBufBaseAddr + (i * ETH_FRAG_SIZE));
		rb_descriptors[i].Ctrl    = RCTRL_INT | (ETH_FRAG_SIZE-1);
		Rx_Stat[i].Info    = 0;
		Rx_Stat[i].HashCRC = 0;
	}

	/* Set EMAC Receive Descriptor Registers. */
	MAC_RXDESCRIPTOR    = (rt_uint32_t)&rb_descriptors[0];
	MAC_RXSTATUS        = (rt_uint32_t)&Rx_Stat[0];
 
	MAC_RXDESCRIPTORNUM = NUM_RX_FRAG-1;

	/* Rx Descriptors Point to 0 */
	MAC_RXCONSUMEINDEX  = 0;
}


//  function added to initialize Tx Descriptors
void TxDescrInit (void) 
{
	unsigned int i;

	tb_descriptors      =  (TX_Desc *)(EMAC_TX_DESC_BASE_ADDR);
	Tx_Stat            =  (TX_Stat  *)(EMAC_TX_STATUS_BASE_ADDR);
	TxBufBaseAddr       =  (rt_uint8_t *)(EMAC_TX_BUFF_BASE_ADDR);

	for (i = 0; i < NUM_TX_FRAG; i++) 
	{
		tb_descriptors[i].Packet =  (rt_uint32_t)(TxBufBaseAddr + (i * ETH_FRAG_SIZE));
		tb_descriptors[i].Ctrl   = 0;
		Tx_Stat[i].Info   = 0;
	}

	/* Set EMAC Transmit Descriptor Registers. */
	MAC_TXDESCRIPTOR    = (rt_uint32_t)&tb_descriptors[0];
	MAC_TXSTATUS        = (rt_uint32_t)&Tx_Stat[0];
	MAC_TXDESCRIPTORNUM = NUM_TX_FRAG-1;

	/* Tx Descriptors Point to 0 */
	MAC_TXPRODUCEINDEX  = 0;
} 
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
** 函数名称: ReadPHY
** 函数名称: ReadPHY
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
/******************************************************************************
** Function name:		EMAC_RxEnable/EMAC_RxDisable
**
** Descriptions:		EMAC RX API modules
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void EMAC_RxEnable( void )
{
	MAC_COMMAND |= 0x01;
	MAC_MAC1 |= 0x01;
	return;    
}

void EMAC_RxDisable( void )
{
	MAC_COMMAND &= ~0x01;
	MAC_MAC1 &= ~0x01;
	return;
}
/******************************************************************************
** Function name:		EMAC_TxEnable/EMAC_TxDisable
**
** Descriptions:		EMAC TX API modules
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void EMAC_TxEnable( void )
{
	MAC_COMMAND |= 0x02;
	return;
}

void EMAC_TxDisable( void )
{
	MAC_COMMAND &= ~0x02;
	return;
}
/* interrupt service routine */
void nic_isr_handler( void )
{
    rt_uint32_t status   =  (MAC_INTSTATUS & MAC_INTENABLE);

    if (status & INT_RX_DONE) // if receive packet
    {
        rt_err_t result;

        /* a frame has been received */
        result = eth_device_ready(&(lpc24xx_device.parent));
        RT_ASSERT(result == RT_EOK);
    }

	if ((status & (INT_RX_OVERRUN)) > 0) 
	{                           /* If a fator Overrun error has occured                     */
		MAC_INTCLEAR            = (INT_RX_OVERRUN);                         /* Clear the overrun interrupt flag                         */
		MAC_COMMAND            |=  COMMAND_RESET_RX;                        /* Soft reset the Rx datapath, this disables the receiver   */
		MAC_COMMAND            |=  COMMAND_RX_EN;                           /* Re-enable the reciever                                   */
		MAC_MAC1               |=  MAC1_REC_EN;                             /* Re-enable the reciever                                   */
	}

}

#define  NET_IF_ADDR_SIZE                                  6    /* 48-bit MAC/net addr size.                            */
#define  NET_IF_ADDR_SIZE_MAC                            NET_IF_ADDR_SIZE

#define  NET_IF_ADDR_BROADCAST                0xFFFFFFFFFFFF
#define  NET_IF_ADDR_BROADCAST_xx                       0xFF    /* ALL broadcast addr octet vals identical.             */
#define  NET_IF_ADDR_BROADCAST_00                       0xFF
#define  NET_IF_ADDR_BROADCAST_01                       0xFF
#define  NET_IF_ADDR_BROADCAST_02                       0xFF
#define  NET_IF_ADDR_BROADCAST_03                       0xFF
#define  NET_IF_ADDR_BROADCAST_04                       0xFF
#define  NET_IF_ADDR_BROADCAST_05                       0xFF
rt_uint8_t   NetIF_MAC_Addr[NET_IF_ADDR_SIZE];      /* NIC's MAC addr.                                      */
/*********************************************************************************************************
** 函数名称: SetMacID
** 函数名称: SetMacID
**
** 功能描述：  设置芯片物理地址,物理地址已经存储在程序空间内 
**
** 输　入:  INT8U * mac_ptr
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年7月30日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void SetMacID( )   
{
	MAC_SA0   =  (NetIF_MAC_Addr[5] << 8) |(NetIF_MAC_Addr[4]);                   /* Write the MAC Address, octect 2 and 1 to the EMAC        */
	MAC_SA1   =  (NetIF_MAC_Addr[3] << 8) |(NetIF_MAC_Addr[2]);                   /* Write the MAC Address, octect 4 and 3 to the EMAC        */
	MAC_SA2  =  (NetIF_MAC_Addr[1] << 8) | (NetIF_MAC_Addr[0]);                  /* Write the MAC Address, octect 6 and 5 to the EMAC        */

//	MAC_SA0 = mac_ptr[0]*256+mac_ptr[1];
//	MAC_SA1 = mac_ptr[2]*256+mac_ptr[3];
//	MAC_SA2 = mac_ptr[4]*256+mac_ptr[5];
	//把MAC地址写入MY——MAC——ID中
}

static  void  AppInitTCPIP (void)
{  
	NetIF_MAC_Addr[0] = 0x00;
	NetIF_MAC_Addr[1] = 0x50;
	NetIF_MAC_Addr[2] = 0xC2;
	NetIF_MAC_Addr[3] = 0x25;
	NetIF_MAC_Addr[4] = 0x61;
	NetIF_MAC_Addr[5] = 0x39;

// 	err             = Net_Init();                               /* Initialize uC/TCP-IP                                     */
// 
// 	AppNetIP        = NetASCII_Str_to_IP("10.10.1.129",  &err);
// 	AppNetMsk       = NetASCII_Str_to_IP("255.255.255.0", &err);
// 	AppNetGateway   = NetASCII_Str_to_IP("10.10.1.1",   &err);

//	err             = NetIP_CfgAddrThisHost(AppNetIP, AppNetMsk);
//	err             = NetIP_CfgAddrDfltGateway(AppNetGateway);
}

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
	 
	reg_val     = read_phy_ex(PHYID, PHY_REG_BMSR, &err);
	reg_val     = read_phy_ex(PHYID, PHY_REG_BMSR, &err);

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
	reg_val      = read_phy_ex(PHYID, PHY_REG_BMSR, &err);
	reg_val      = read_phy_ex(PHYID, PHY_REG_BMSR, &err);

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

	bmsr    = read_phy_ex(PHYID, PHY_REG_BMSR, &err);       /* Get Link Status from PHY status reg. Requires 2 reads    */
	bmsr    = read_phy_ex(PHYID, PHY_REG_BMSR, &err);       /* Get Link Status from PHY status reg. Requires 2 reads    */

	if ((bmsr & BMSR_LSTATUS) == 0)
	{
		return (NET_PHY_SPD_0);                                         /* No link                                                  */
	}

	bmcr    = read_phy_ex(PHYID, PHY_REG_BMCR, &err);       /* Read the PHY Control Register                            */

	if ((bmcr & BMCR_ANENABLE) == BMCR_ANENABLE)
	{	/* If AutoNegotiation is enabled                            */
		if ((bmsr & BMSR_ANEGCOMPLETE) == 0)
		{                          /* If AutoNegotiation is not complete                       */
			return (NET_PHY_SPD_0);       					            /* AutoNegotitation in progress                             */
		}

		lpa = read_phy_ex(PHYID, PHY_REG_ANLPAR, &err);     /* Read the Link Partner Ability Register                   */

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

	bmsr    = read_phy_ex(PHYID, PHY_REG_BMSR, &err);       /* Get Link Status from PHY status reg. Requires 2 reads    */
	bmsr    = read_phy_ex(PHYID, PHY_REG_BMSR, &err);       /* Get Link Status from PHY status reg. Requires 2 reads    */

	if ((bmsr & BMSR_LSTATUS) == 0)
	{
		return (NET_PHY_DUPLEX_UNKNOWN);   /* No link, return 'Duplex Uknown'                          */
	}

	bmcr    = read_phy_ex(PHYID, PHY_REG_BMCR, &err);       /* Read the PHY Control Register      */

	if ((bmcr & BMCR_ANENABLE) == BMCR_ANENABLE)
	{	/* If AutoNegotiation is enabled   */
		if ((bmsr & BMSR_ANEGCOMPLETE) == 0) 
		{     /* If AutoNegotiation is not complete                       */
			return (NET_PHY_DUPLEX_UNKNOWN);   /* AutoNegotitation in progress       */
		}

		lpa = read_phy_ex(PHYID, PHY_REG_ANLPAR, &err);     /* Read the Link Partner Ability Register                   */

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
			reg_val = read_phy_ex(PHYID, PHY_REG_BMSR, &err);   /* Read the Basic Mode Status Register          */
			reg_val = read_phy_ex(PHYID, PHY_REG_BMSR, &err);   /* Read the Basic Mode Status Register          */
			i--;
		} while (((reg_val & BMSR_LSTATUS) == 0) && (i > 0));           /* While link not established and retries remain    */
	}
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
		case NET_PHY_SPD_0:                                             /* Assume 10Mbps operation until linked                     */
		case NET_PHY_SPD_10:
			MAC_SUPP      &=  ~SUPP_SPEED;                                 /* Configure the RMII logic (if used) for 10MBps operation  */
			break;

		case NET_PHY_SPD_100:
			MAC_SUPP      |=   SUPP_SPEED;                                 /* Configure the RMII logic (if uses) for 100MBps operation */
			break;
	}

	switch (link_duplex) 
	{
		case NET_PHY_DUPLEX_UNKNOWN:                                    /* Assume half duplex until link duplex is known            */
		case NET_PHY_DUPLEX_HALF:
			MAC_MAC2      &=  ~MAC2_FULL_DUP;                           /* Configure the EMAC to run in HALF duplex mode            */
			MAC_COMMAND   &=  ~COMMAND_FULL_DUPLEX;                        /* Configure the MII logic for a Half Duplex PHY Link       */
			MAC_IPGT       =   IPGT_HALF_DUP;	                            /* Set inter packet gap to the recommended Half Duplex      */
			break;

		case NET_PHY_DUPLEX_FULL:
			MAC_MAC2      |=   MAC2_FULL_DUP;                           /* Configure the EMAC to run in FULL duplex mode            */
			MAC_COMMAND   |=   COMMAND_FULL_DUPLEX;                        /* Configure the MII logic for a Full Duplex PHY Link       */
			MAC_IPGT       =   IPGT_FULL_DUP;	                            /* Set inter packet gap to the recommended Full Duplex      */
			break;
	}
}

/*********************************************************************************************************
** 函数名称: nic_linkup
** 函数名称: nic_linkup
**
** 功能描述：  Message from NIC that the ethernet link is up.
**
** 输　入:  void
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年8月10日
** 备  注:  WARNING: Called in interruption context most of the time.
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void  nic_linkup (void)
{
	rt_uint32_t  link_speed;
	rt_uint32_t  link_duplex;

	link_speed              =   get_phy_link_speed();                  /* Read the PHY's current link speed                    */
	link_duplex             =   get_phy_link_duplex();                 /* Read the PHY's current link duplex mode              */

	if (link_speed == NET_PHY_SPD_0 || link_duplex == NET_PHY_DUPLEX_UNKNOWN) 
	{   /* Inform the EMAC about the current PHY settings       */
		nic_link_change(NET_PHY_SPD_0, NET_PHY_DUPLEX_UNKNOWN);    
	}
	else
	{                /* Inform the EMAC about the current PHY settings       */
		nic_link_change(link_speed,    link_duplex);  
	}
}
/*********************************************************************************************************
** 函数名称: nic_linkdown
** 函数名称: nic_linkdown
**
** 功能描述：  Message from NIC that the ethernet link is down.
**
** 输　入:  void
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年8月10日
** 备  注:  WARNING: Called in interruption context most of the time.
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void  nic_linkdown (void)
{
	rt_uint32_t  link_speed;
	rt_uint32_t  link_duplex;

	link_speed              =   get_phy_link_speed();                  /* Read the PHY's current link speed                    */
	link_duplex             =   get_phy_link_duplex();                 /* Read the PHY's current link duplex mode              */

	if (link_speed == NET_PHY_SPD_0 || link_duplex == NET_PHY_DUPLEX_UNKNOWN)
	{    /* Inform the EMAC about the current PHY settings       */
		nic_link_change(NET_PHY_SPD_0, NET_PHY_DUPLEX_UNKNOWN);   
	} 
	else
	{  /* Inform the EMAC about the current PHY settings       */
		nic_link_change(link_speed,    link_duplex);                
	}
}

/* RT-Thread Device Interface */
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


#define  MAIN_OSC_FRQ              11059200L
#define  IRC_OSC_FRQ               11059200L
#define  RTC_OSC_FRQ                  32768L


rt_uint32_t  BSP_CPU_ClkFreq (void)
{
	rt_uint32_t  msel;
	rt_uint32_t  nsel;
	rt_uint32_t  fin;
	rt_uint32_t  pll_clk_feq;                                    /* When the PLL is enabled, this is Fcco                    */
	rt_uint32_t  clk_div;
	rt_uint32_t  clk_freq;

	switch (CLKSRCSEL & 0x03)
	{                                 /* Determine the current clock source                       */
		case 0:
			fin        =  IRC_OSC_FRQ;
			break;
		case 1:
			fin        =  MAIN_OSC_FRQ;
			break;
		case 2:
			fin        =  RTC_OSC_FRQ;
			break;
		default:
			fin        =  IRC_OSC_FRQ;
			break;
	}

	if ((PLLSTAT & (1 << 25)) > 0) 
	{                                                              /* If the PLL is currently enabled and connected        */
		msel        = (rt_uint32_t)(PLLSTAT & 0x3FFF) + 1;           /* Obtain the PLL multiplier                            */
		nsel        = (rt_uint32_t)((PLLSTAT >>   16) & 0x0F) + 1;   /* Obtain the PLL divider                               */
		pll_clk_feq = (2 * msel * fin / nsel);                      /* Compute the PLL output frequency                     */
	} 
	else
	{
		pll_clk_feq = (fin);                                        /* The PLL is bypassed                                  */
	}

	clk_div         = (rt_uint8_t)(CCLKCFG & 0x0F) + 1;             /* Obtain the CPU core clock divider                    */
	clk_freq        = (rt_uint32_t)(pll_clk_feq / clk_div);          /* Compute the ARM Core clock frequency                 */

	return (clk_freq);
}
 
static void hd_DelayMS(rt_uint32_t ms)
{
	//执行11059200次语句，花时3.570S，差不多每ms计算3097条语句
	rt_uint32_t count=3339*ms;
	rt_uint32_t i;
	for (i=0;i<count;i++)
	{
		__asm{ NOP };
	}
}

/*********************************************************************************************************
** 函数名称: phy_hw_init
** 函数名称: phy_hw_init
**
** 功能描述：  
**
** 输　入:  void
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年8月8日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void  phy_hw_init (void)
{ /* Configure I/O and the RMII / MII interface pins          */
 	PINSEL2             =   0x50150105;	                                /* Selects P1[0,1,4,8,9,10,14,15]                           */
 	PINSEL3             =   0x00000005;	                                /* Selects P1[17:16]                                        */
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
rt_uint16_t  NetNIC_ConnStatus;                            /* NIC's connection status : DEF_ON/DEF_OFF.            */
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
//	NetNIC_PhyIntInit();

// 	reg_val     = NetNIC_PhyRegRd(EMAC_CFG_PHY_ADDR, DM9161_MDINTR, perr); /* Clear interrupts                                         */
// 	reg_val    |= MDINTER_FDX_MSK | MDINTER_SPD_MSK | MDINTER_LINK_MSK | MDINTER_INTR_MSK;
// 	reg_val    &= ~(MDINTER_SPD_MSK | MDINTER_LINK_MSK | MDINTER_INTR_MSK);

//	NetNIC_PhyRegWr(EMAC_CFG_PHY_ADDR, DM9161_MDINTR, reg_val, perr);   /* Enable link change interrupt                             */
 }
static rt_err_t rt_dm9161_init(rt_device_t dev)
{
	unsigned int regv,tout,id1,id2 ,i = 0;
	rt_uint32_t  tempreg = 0;
	rt_uint16_t  ret = 0;

	rt_uint32_t clk_freq            =   BSP_CPU_ClkFreq();  

 	clk_freq           /=   100000;        
	/* Power Up the EMAC controller. */
	PCONP |= 0x40000000;
	phy_hw_init();

	/* Reset all EMAC internal modules. */
 	MAC_MAC1 = MAC1_RES_TX | MAC1_RES_MCS_TX | MAC1_RES_RX | MAC1_RES_MCS_RX | MAC1_SIM_RES | MAC1_SOFT_RES;
 
	MAC_COMMAND = CR_REG_RES | CR_TX_RES | CR_RX_RES;
 
	/* A short delay after reset. */
 	for (tout = 0; tout <5000; tout++);

	//Deassert all prior resets
 	MAC_MAC1 = 0;
	EMAC_RxDisable();
	EMAC_TxDisable();
 /* Configure EMAC / PHY communication to RMII mode          */
  	MAC_COMMAND            |=   COMMAND_RMII;  
 /* Assume and configure RMII link speed logic for 10Mbit    */
  	MAC_SUPP = 0;
 	for (tout = 0; tout <5000; tout++);

 	MAC_TEST                =   0;     
  	/* Initialize MAC control registers. */
  	MAC_MAC1 |= MAC1_PASS_ALL;
  	MAC_MAC2 = MAC2_CRC_EN | MAC2_PAD_EN;
 	MAC_MAXF = ETH_MAX_FLEN;
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

	if (PHYID == 0 || PHYID > 32)
	{//出错

	}
 
    //	PHYID = 0;
	//  复位PHY芯片
	//  等待一段指定的时间，使PHY就绪 
	write_phy(PHYID, PHY_REG_BMCR,  BMCR_RESET|BMCR_ANRESTART|BMCR_ANENABLE  );
//	write_phy (EMAC_CFG_PHY_ADDR, PHY_REG_BMCR, PHY_AUTO_NEG);
	for ( i = 0; i < 5; i++ )
	{
		hd_DelayMS(1000);
	}
	 
	for(i=0;i<32;i++)
	{
		PHYREG[i] = read_phy_ex(PHYID ,i ,&ret);
	}

	nic_phy_init(&ret); 

	tempreg = read_phy(PHYID, DM9161_DSCSR );
	

	MAC_CLRT = CLRT_DEF;
	MAC_IPGR = IPGR_DEF;


  	for(i=0;i<32;i++)
	{
		PHYREG[i] = read_phy_ex(PHYID ,i ,&ret);
	}
	//判断工作在10/100 半双工/全双工
	if(tempreg & 0x8000)//100fdx
	{

	}
	else if(tempreg & 0x4000)//100hdx
	{

	}
	else if(tempreg & 0x2000)//10fdx
	{

	}
	else if(tempreg & 0x1000)//10hdx
	{
//     	MAC_MAC2 = 0x30;		/* half duplex, CRC and PAD enabled. */
// 		MAC_SUPP = 0;	/* RMII Support Reg. speed is set to 10M */
// 		MAC_COMMAND |= 0x0240;
// 		/* back to back int-packet gap */
// 		MAC_IPGT = 0x0012;		/* IPG setting in half duplex mode */ 
	}
	else
	{//出错啦

	}



	//设置MAC地址
 	SetMacID();
	 // Initialize Tx and Rx DMA Descriptors 
  	TxDescrInit();
 	RxDescrInit();
	/* Receive Broadcast, Unicast ,Multicast and Perfect Match Packets */
	MAC_RXFILTERCTRL = RFC_UCAST_EN |RFC_MCAST_EN | RFC_BCAST_EN | RFC_PERFECT_EN;

	/* Enable EMAC interrupts. */
	MAC_INTENABLE = INT_RX_DONE | INT_TX_DONE;

	/* Reset all interrupts */
	MAC_INTCLEAR  = 0xFFFF;

	/* Enable receive and transmit mode of MAC Ethernet core */
	MAC_COMMAND  |= (CR_RX_EN | CR_TX_EN);
	MAC_MAC1     |= MAC1_REC_EN;

	EMAC_RxEnable();
	EMAC_TxEnable();
    return RT_EOK;
}

static  void  rt_emac_init (rt_uint16_t *perr)
{

}

static rt_err_t rt_dm9000_open(rt_device_t dev, rt_uint16_t oflag)
{
	return RT_EOK;
}

static rt_err_t rt_dm9000_close(rt_device_t dev)
{
	return RT_EOK;
}

static rt_size_t rt_dm9000_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
	rt_set_errno(-RT_ENOSYS);
	return 0;
}

static rt_size_t rt_dm9000_write (rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
	rt_set_errno(-RT_ENOSYS);
	return 0;
}

static rt_err_t rt_dm9000_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
	switch(cmd)
	{
	case NIOCTL_GADDR:
		/* get mac address */
		if(args) rt_memcpy(args, lpc24xx_device.dev_addr, 6);
		else return -RT_ERROR;
		break;

	default :
		break;
	}

	return RT_EOK;
}


/* See the header file for descriptions of public functions. */
void lpc24xxether_write_frame(rt_uint8_t *ptr, rt_uint32_t length, rt_bool_t eof)
{
	rt_uint8_t *buf_ptr;
	static rt_uint32_t current_tb_index = 0;
	rt_uint32_t is_last, tx_offset = 0, remain, pdu_length;

	rt_uint32_t TxProduceIndex = MAC_TXPRODUCEINDEX;
	rt_uint32_t TxConsumeIndex = MAC_TXCONSUMEINDEX;	 

	while(tx_offset < length)
	{
		/* check whether buffer is available */
		while( TxConsumeIndex == (TxProduceIndex+1)%NUM_TX_FRAG)
		{	/* no buffer */
			rt_thread_delay(5);
			TxProduceIndex = MAC_TXPRODUCEINDEX;
			TxConsumeIndex = MAC_TXCONSUMEINDEX;
		}

		/* Get the address of the buffer from the descriptor, then copy
		the data into the buffer. */
		current_tb_index = MAC_TXPRODUCEINDEX;
		buf_ptr = (rt_uint8_t *)tb_descriptors[current_tb_index].Packet;

		/* How much can we write to the buffer? */
		remain = length - tx_offset;
		pdu_length = (remain <= ETH_TX_BUF_SIZE)? remain : ETH_TX_BUF_SIZE;

		/* Copy the data into the buffer. */
		rt_memcpy(buf_ptr, &ptr[tx_offset], pdu_length );
		tx_offset += pdu_length;

		/* Is this the last data for the frame? */
		if((eof == RT_TRUE) && ( tx_offset >= length ))
			is_last = RT_TRUE;
		else 
			is_last = RT_FALSE;

		/* Fill out the necessary in the descriptor to get the data sent,
		then move to the next descriptor, wrapping if necessary. */

		if(is_last)
		tb_descriptors[current_tb_index].Ctrl =  (EMAC_TX_DESC_OVERRIDE   |        /* Override the defaults from the MAC internal registers    */
			EMAC_TX_DESC_PAD        |        /* Add padding for frames < 64 bytes                        */
			EMAC_TX_DESC_LAST       |        /* No additional descriptors to follow, this is the last    */
			EMAC_TX_DESC_CRC)       |        /* Append the CRC automatically                             */
			(pdu_length - 1);                       /* Write the size of the frame, starting from 0             */
		else
			tb_descriptors[current_tb_index].Ctrl =  (EMAC_TX_DESC_OVERRIDE   |        /* Override the defaults from the MAC internal registers    */
			EMAC_TX_DESC_PAD        |        /* Add padding for frames < 64 bytes                        */
			EMAC_TX_DESC_CRC)       |        /* Append the CRC automatically                             */
			(pdu_length - 1);               /* Write the size of the frame, starting from 0             */


		MAC_TXPRODUCEINDEX      =   (MAC_TXPRODUCEINDEX + 1) % NUM_TX_FRAG;    /* Increment the produce Ix register, initiate Tx of frame  */
	}
}
/* ethernet device interface */
/*
* Transmit packet.
*/
rt_err_t lpc24xxether_tx( rt_device_t dev, struct pbuf* p)
{
	struct pbuf* q;

	/* lock tx operation */
	rt_sem_take(&tx_sem, RT_WAITING_FOREVER);

	for (q = p; q != NULL; q = q->next)
	{
		if (q->next == RT_NULL)
			lpc24xxether_write_frame(q->payload, q->len, RT_TRUE);
		else
			lpc24xxether_write_frame(q->payload, q->len, RT_FALSE);
	}

	rt_sem_release(&tx_sem);

	return 0;
}
void lpc24xxether_read_frame(rt_uint8_t* ptr, rt_uint32_t section_length, rt_uint32_t total)
{
	static rt_uint8_t* src_ptr;
	register rt_uint32_t buf_remain, section_remain;
	static rt_uint32_t section_read = 0, buf_offset = 0, frame_read = 0;
#if 0
	if(ptr == RT_NULL)
	{
		/* Reset our state variables ready for the next read from this buffer. */
		src_ptr = (rt_uint8_t *)(rb_descriptors[current_rb_index].Packet & RxDESC_FLAG_ADDR_MASK);
		frame_read = (rt_uint32_t)0;
		buf_offset = (rt_uint32_t)0;
	}
	else
	{
		/* Loop until we have obtained the required amount of data. */
		section_read = 0;
		while( section_read < section_length )
		{
			buf_remain = (ETH_RX_BUF_SIZE - buf_offset);
			section_remain = section_length - section_read;

			if( section_remain > buf_remain )
			{
				/* more data on section than buffer size */
				rt_memcpy(&ptr[ section_read ], &src_ptr[buf_offset], buf_remain);
				section_read += buf_remain;
				frame_read += buf_remain;

				/* free buffer */
				rb_descriptors[current_rb_index].Packet &= ~RxDESC_FLAG_OWNSHIP;

				/* move to the next frame. */
				current_rb_index++;
				if(current_rb_index >= RB_BUFFER_SIZE) current_rb_index = 0;

				/* Reset the variables for the new buffer. */
				src_ptr = (rt_uint8_t *)(rb_descriptors[current_rb_index].Packet & RxDESC_FLAG_ADDR_MASK);
				buf_offset = 0;
			}
			else
			{
				/* more data on buffer than section size */
				rt_memcpy(&ptr[section_read], &src_ptr[buf_offset], section_remain);
				buf_offset += section_remain;
				section_read += section_remain;
				frame_read += section_remain;

				/* finish this read */
				if((buf_offset >= ETH_RX_BUF_SIZE) || (frame_read >= total))
				{
					/* free buffer */
					rb_descriptors[current_rb_index].Packet &= ~(RxDESC_FLAG_OWNSHIP);

					/* move to the next frame. */
					current_rb_index++;
					if( current_rb_index >= RB_BUFFER_SIZE ) current_rb_index = 0;

					src_ptr = (rt_uint8_t*)(rb_descriptors[current_rb_index].Packet & RxDESC_FLAG_ADDR_MASK);
					buf_offset = 0;
				}
			}
		}
	}
#endif
}

struct pbuf *lpc24xxether_rx(rt_device_t dev)
{
	struct pbuf *p = RT_NULL;
#if 0
	/* skip fragment frame */
	while((rb_descriptors[current_rb_index].Packet & RxDESC_FLAG_OWNSHIP)&& !(rb_descriptors[current_rb_index].Ctrl & RxDESC_STATUS_FRAME_START))
	{
		rb_descriptors[current_rb_index].Packet &= (~RxDESC_FLAG_OWNSHIP);

		current_rb_index++;
		if(current_rb_index >= RB_BUFFER_SIZE) current_rb_index = 0;
	}
	if ((rb_descriptors[current_rb_index].Packet & RxDESC_FLAG_OWNSHIP))
	{
		struct pbuf* q;
		rt_uint32_t index, pkt_len = 0;

		/* first of all, find the frame length */
		index = current_rb_index;
		while (rb_descriptors[index].Packet & RxDESC_FLAG_OWNSHIP)
		{
			pkt_len = rb_descriptors[index].Ctrl & RxDESC_STATUS_BUF_SIZE;
			if (pkt_len > 0) 
				break;

			index ++;
			if (index > RB_BUFFER_SIZE) index = 0;
		}

		if (pkt_len)
		{
			p = pbuf_alloc(PBUF_LINK, pkt_len, PBUF_RAM);
			if(p != RT_NULL)
			{
				lpc24xxether_read_frame(RT_NULL, 0, pkt_len);
				for(q = p; q != RT_NULL; q= q->next)
					lpc24xxether_read_frame(q->payload, q->len, pkt_len);
			}
			else
			{
				rt_kprintf("no memory in pbuf\n");
			}
		}
	}

	/* enable interrupt */
//	AT91C_BASE_EMAC->EMAC_IER = AT91C_EMAC_RCOMP;


#endif
}

/* reception packet. */
struct pbuf *rt_dm9000_rx(rt_device_t dev)
{
    struct pbuf* p;
	rt_uint32_t len;

    /* init p pointer */
    p = RT_NULL;

    if (1) // if there is packet in device
    {
        /* get one packet length */
        len = 0; // packet length

        /* allocate buffer */
        p = pbuf_alloc(PBUF_LINK, len, PBUF_RAM);

        if (p != RT_NULL)
        {
            rt_uint8_t* data;
            struct pbuf* q;

            for (q = p; q != RT_NULL; q= q->next)
            {
                data = q->payload;
                len = q->len;

                /* read data from device */
            }
        }
    }
    else
    {
        /* restore interrupt */
    }

    return p;
}

/*********************************************************************************************************
** 函数名称: lpc24xxether_register
** 函数名称: lpc24xxether_register
**
** 功能描述：  注册MAC设备
**
** 输　入:  char * name
**          
** 输　出:   int
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年7月30日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
int lpc24xxether_register(char *name)
{
	rt_err_t result;

	/* init rt-thread device interface */
  	lpc24xx_device.parent.parent.init		= rt_dm9161_init;
  	lpc24xx_device.parent.parent.open		= rt_dm9000_open;
  	lpc24xx_device.parent.parent.close		= rt_dm9000_close;
  	lpc24xx_device.parent.parent.read		= rt_dm9000_read;
	lpc24xx_device.parent.parent.write		= rt_dm9000_write;
	lpc24xx_device.parent.parent.control	= rt_dm9000_control;
	lpc24xx_device.parent.parent.private    = RT_NULL;

  	lpc24xx_device.parent.eth_rx			= lpc24xxether_rx;
  	lpc24xx_device.parent.eth_tx			= lpc24xxether_tx;

	/* Update MAC address */
// 	lpc24xx_device.dev_addr[0] = 0x1e;
// 	lpc24xx_device.dev_addr[1] = 0x30;
// 	lpc24xx_device.dev_addr[2] = 0x6c;
// 	lpc24xx_device.dev_addr[3] = 0xa2;
// 	lpc24xx_device.dev_addr[4] = 0x45;
// 	lpc24xx_device.dev_addr[5] = 0x5e;
// 	/* update mac address */
// 	update_mac_address(lpc24xx_device);

	rt_sem_init(&tx_sem, "emac", 1, RT_IPC_FLAG_FIFO);
	result = eth_device_init(&(lpc24xx_device.parent), (char*)name);
	RT_ASSERT(result == RT_EOK);
	return RT_EOK;
}


void rt_hw_eth_init()
{
	AppInitTCPIP();
	lpc24xxether_register("E0");	 
}

