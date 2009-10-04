#include <rtthread.h> 

#include <netif/ethernetif.h>
#include "lwipopts.h" 
#include "applib.h"
#include "LPC24xx.h" 

#include "ioLPC24xx.H" 

#include "emac.h"
#include "emac_def.h"
#include "dm9161.h"	
#include "dm9161_def.h"

#define  DM9161AE_INIT_AUTO_NEG_RETRIES        3

#define  DM9161AE_OUI                   0x00606E
#define  DM9161AE_VNDR_MDL                  0x08

 
#define MAX_ADDR_LEN 6
struct rt_lpc24xx_eth
{
	/* inherit from ethernet device */
	struct eth_device parent;

	/* interface address info. */
	rt_uint8_t  dev_addr[MAX_ADDR_LEN];			/* hw address	*/
};

#define RB_BUFFER_SIZE		8			/* max number of receive buffers */
#define ETH_RX_BUF_SIZE		128

#define TB_BUFFER_SIZE		4
#define ETH_TX_BUF_SIZE		(PBUF_POOL_BUFSIZE)

typedef struct {                        /* RX Descriptor struct              */
	INT32U Packet;
	INT32U Ctrl;
} RX_Desc;

typedef struct {                        /* RX Status struct                  */
	INT32U Info;
	INT32U HashCRC;
} RX_Stat;

typedef struct {                        /* TX Descriptor struct              */
	INT32U Packet;
	INT32U Ctrl;
} TX_Desc;

typedef struct {                        /* TX Status struct                  */
	INT32U Info;
} TX_Stat;

static            RX_Desc *rb_descriptors ;
static            TX_Desc *tb_descriptors ;

static  RX_Stat *Rx_Stat ; /* Must be 8-Byte alligned   */
static  TX_Stat *Tx_Stat ;
static  INT8U       *RxBufBaseAddr;
static  INT8U       *TxBufBaseAddr;
 
static struct rt_lpc24xx_eth lpc24xx_device;

static struct rt_semaphore tx_sem;


#define  EMAC_RX_DESC_BASE_ADDR                (EMAC_RAM_BASE_ADDR)
#define  EMAC_RX_STATUS_BASE_ADDR              (EMAC_RX_DESC_BASE_ADDR   + (NUM_RX_FRAG * sizeof(RX_Desc)))
#define  EMAC_TX_DESC_BASE_ADDR                (EMAC_RX_STATUS_BASE_ADDR + (NUM_RX_FRAG * sizeof(RX_Stat)))
#define  EMAC_TX_STATUS_BASE_ADDR              (EMAC_TX_DESC_BASE_ADDR   + (NUM_TX_FRAG * sizeof(TX_Desc)))
#define  EMAC_RX_BUFF_BASE_ADDR                (EMAC_TX_STATUS_BASE_ADDR + (NUM_TX_FRAG * sizeof(TX_Stat)))
#define  EMAC_TX_BUFF_BASE_ADDR                (EMAC_RX_BUFF_BASE_ADDR   + (NUM_RX_FRAG * ETH_FRAG_SIZE))

//  function added to initialize Rx Descriptors
void rx_descr_init (void)
{
	/* Initialize Receive Descriptor and Status array. */
	unsigned int i = 0;

	rb_descriptors      =  (RX_Desc *)(EMAC_RX_DESC_BASE_ADDR);
	Rx_Stat            =  (RX_Stat  *)(EMAC_RX_STATUS_BASE_ADDR);
	RxBufBaseAddr       =  (INT8U *)(EMAC_RX_BUFF_BASE_ADDR);

	for (i = 0; i < NUM_RX_FRAG; i++) 
	{ 
		rb_descriptors[i].Packet  = (INT32U)(RxBufBaseAddr + (i * ETH_FRAG_SIZE));
		rb_descriptors[i].Ctrl    = RCTRL_INT | (ETH_FRAG_SIZE-1);
		Rx_Stat[i].Info    = 0;
		Rx_Stat[i].HashCRC = 0;
	}

	/* Set EMAC Receive Descriptor Registers. */
	MAC_RXDESCRIPTOR    = (INT32U)&rb_descriptors[0];
	MAC_RXSTATUS        = (INT32U)&Rx_Stat[0];

	MAC_RXDESCRIPTORNUM = NUM_RX_FRAG-1;

	/* Rx Descriptors Point to 0 */
	MAC_RXCONSUMEINDEX  = 0;
}


//  function added to initialize Tx Descriptors
void tx_descr_init (void) 
{
	unsigned int i;

	tb_descriptors      =  (TX_Desc *)(EMAC_TX_DESC_BASE_ADDR);
	Tx_Stat            =  (TX_Stat  *)(EMAC_TX_STATUS_BASE_ADDR);
	TxBufBaseAddr       =  (INT8U *)(EMAC_TX_BUFF_BASE_ADDR);

	for (i = 0; i < NUM_TX_FRAG; i++) 
	{
		tb_descriptors[i].Packet =  (INT32U)(TxBufBaseAddr + (i * ETH_FRAG_SIZE));
		tb_descriptors[i].Ctrl   = 0;
		Tx_Stat[i].Info   = 0;
	}

	/* Set EMAC Transmit Descriptor Registers. */
	MAC_TXDESCRIPTOR    = (INT32U)&tb_descriptors[0];
	MAC_TXSTATUS        = (INT32U)&Tx_Stat[0];
	MAC_TXDESCRIPTORNUM = NUM_TX_FRAG-1;

	/* Tx Descriptors Point to 0 */
	MAC_TXPRODUCEINDEX  = 0;
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
static  void  EMAC_RxPktDiscard ( )
{
	MAC_RXCONSUMEINDEX      = (MAC_RXCONSUMEINDEX + 1) % NUM_RX_FRAG;
}
/* interrupt service routine */
void nic_isr_handler(int irqno)
{
//     rt_uint32_t status;
// 
//     if (status) // if receive packet
//     {
//         rt_err_t result;
// 
//         /* a frame has been received */
//         result = eth_device_ready(&(lpc24xx_device.parent));
//         RT_ASSERT(result == RT_EOK);
//     }
// 
//     if (status) // if finished packet transmission
//     {
//     }
	INT32U status               =  MAC_INTSTATUS;    
	INT16U  n_new;
	INT32U  nRsv =  MAC_RSV;
	INT32U  nSta =  MAC_STATUS;

	if (status & INT_RX_DONE) // if receive packet
	{
		INT8U result;
		MAC_INTCLEAR = status;
		/* a frame has been received */
		result = eth_device_ready(&(lpc24xx_device.parent));
		if (result != RT_TRUE)
		{//如果发送失败,那么说明队列满了,处理不过来,那么就把这封信扔掉
			EMAC_RxPktDiscard();
		}
		MAC_INTCLEAR            = (INT_RX_DONE);                            /* Clear the interrupt flags        */
	}

	/* check for receive overrun */
	if (status & INT_RX_OVERRUN)
	{
		MAC_INTCLEAR = INT_RX_OVERRUN;
		//	eth->rx_overrun++;
		MAC_COMMAND |= CR_RX_RES;
		/* usleep(?); */
		MAC_COMMAND |= CR_RX_EN;
		MAC_MAC1 |= MAC1_REC_EN;
	}
	/* check for transmit underrun */
	if (status & INT_TX_UNDERRUN)
	{
		MAC_INTCLEAR = INT_TX_UNDERRUN;
		//	eth->tx_underrun++;
		MAC_COMMAND |= CR_TX_RES;;
		/* usleep(?); */
		MAC_COMMAND |= CR_TX_EN;
	}

	//???????????
	VICVectAddr = 0;            //interrupt close 通知中断控制器中断结束
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

INT8U   NetIF_MAC_Addr[NET_IF_ADDR_SIZE];      /* NIC's MAC addr.                                      */
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
void  phy_hw_init (void)
{ /* Configure I/O and the RMII / MII interface pins          */
  //	PINSEL2             =   0x50150105;	                                /* Selects P1[0,1,4,8,9,10,14,15]                           */
  //	PINSEL3             =   0x00000005;	                                /* Selects P1[17:16]                                        */
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
	INT32U  link_speed;
	INT32U  link_duplex;

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
	INT32U  link_speed;
	INT32U  link_duplex;

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

#define  MAIN_OSC_FRQ              11059200L
#define  IRC_OSC_FRQ               11059200L
#define  RTC_OSC_FRQ                  32768L


INT32U  bsp_cpu_clk_freq (void)
{
	INT32U  msel;
	INT32U  nsel;
	INT32U  fin;
	INT32U  pll_clk_feq;                                    /* When the PLL is enabled, this is Fcco                    */
	INT32U  clk_div;
	INT32U  clk_freq;

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
		msel        = (INT32U)(PLLSTAT & 0x3FFF) + 1;           /* Obtain the PLL multiplier                            */
		nsel        = (INT32U)((PLLSTAT >>   16) & 0x0F) + 1;   /* Obtain the PLL divider                               */
		pll_clk_feq = (2 * msel * fin / nsel);                      /* Compute the PLL output frequency                     */
	} 
	else
	{
		pll_clk_feq = (fin);                                        /* The PLL is bypassed                                  */
	}

	clk_div         = (INT8U)(CCLKCFG & 0x0F) + 1;             /* Obtain the CPU core clock divider                    */
	clk_freq        = (INT32U)(pll_clk_feq / clk_div);          /* Compute the ARM Core clock frequency                 */

	return (clk_freq);
}
 
void rt_delayms(INT32U ms)
{
	//执行11059200次语句，花时3.570S，差不多每ms计算3097条语句
	INT32U count=3339*ms;
	INT32U i;
	for (i=0;i<count;i++)
	{
		__asm{ NOP };
	}
}


void  nic_int_init  (void)
{
	//	rt_hw_interrupt_install(VIC_ETHERNET, nic_isr_handler, RT_NULL);
	//	rt_hw_interrupt_umask(VIC_ETHERNET);

	VICIntSelect       &= ~(1 << VIC_ETHERNET);                         /* Configure the Ethernet VIC interrupt source for IRQ      */
	VICVectAddr21       =  (INT32U)nic_isr_handler;              /* Set the vector address                                   */
	VICIntEnable        =  (1 << VIC_ETHERNET);                         /* Enable the VIC interrupt source, but no local sources    */
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

static rt_err_t rt_lpc24xxether_open(rt_device_t dev, rt_uint16_t oflag)
{
	return RT_EOK;
}

static rt_err_t rt_lpc24xxether_close(rt_device_t dev)
{
	return RT_EOK;
}

static rt_size_t rt_lpc24xxether_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
	rt_set_errno(-RT_ENOSYS);
	return 0;
}

static rt_size_t rt_lpc24xxether_write (rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
	rt_set_errno(-RT_ENOSYS);
	return 0;
}
 
static rt_err_t rt_lpc24xxether_control(rt_device_t dev, rt_uint8_t cmd, void *args)
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

#if 0
/* See the header file for descriptions of public functions. */
void lpc24xxether_write_frame(rt_uint8_t *ptr, rt_uint32_t length, rt_bool_t eof)
{
	rt_uint8_t *buf_ptr;
	static rt_uint32_t current_tb_index = 0;
	rt_uint32_t is_last, tx_offset = 0, remain, pdu_length;

	while(tx_offset < length)
	{
		/* check whether buffer is available */
		while(!(tb_descriptors[current_tb_index].Ctrl & TxDESC_STATUS_USED))
		{
			/* no buffer */
			rt_thread_delay(5);
		}

		/* Get the address of the buffer from the descriptor, then copy
		the data into the buffer. */
		buf_ptr = (rt_uint8_t *)tb_descriptors[current_tb_index].Packet;

		/* How much can we write to the buffer? */
		remain = length - tx_offset;
		pdu_length = (remain <= ETH_TX_BUF_SIZE)? remain : ETH_TX_BUF_SIZE;

		/* Copy the data into the buffer. */
		rt_memcpy(buf_ptr, &ptr[tx_offset], pdu_length );
		tx_offset += pdu_length;

		/* Is this the last data for the frame? */
		if((eof == RT_TRUE) && ( tx_offset >= length )) is_last = TxDESC_STATUS_LAST_BUF;
		else is_last = 0;

		/* Fill out the necessary in the descriptor to get the data sent,
		then move to the next descriptor, wrapping if necessary. */
		if(current_tb_index >= (TB_BUFFER_SIZE - 1))
		{
			tb_descriptors[current_tb_index].Ctrl = ( pdu_length & TxDESC_STATUS_BUF_SIZE )
				| is_last
				| TxDESC_STATUS_WRAP;
			current_tb_index = 0;
		}
		else
		{
			tb_descriptors[current_tb_index].Ctrl = ( pdu_length & TxDESC_STATUS_BUF_SIZE )
				| is_last;
			current_tb_index++;
		}

		/* If this is the last buffer to be sent for this frame we can start the transmission. */
		if(is_last)
		{
			//AT91C_BASE_EMAC->EMAC_NCR |= AT91C_EMAC_TSTART;
		}
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
#endif

#define   MIN(x, y)   ((x)   >   (y)   ?   (y)   :   (x)) 
INT16U lpc24xxether_write_frame( struct pbuf* p )
{
	struct pbuf* q;
	INT8U *pDescBuf = NULL;
	INT8U *pSrcBuf = NULL;
	INT16U nTxBufOffset = 0,nDescOffset = 0,nPduLen = 0,nSendLen = 0;

	INT32U TxProduceIndex = MAC_TXPRODUCEINDEX;
	INT32U TxConsumeIndex = MAC_TXCONSUMEINDEX;	 

	if (p == NULL)
		return 0;
 
	pDescBuf = (INT8U *)tb_descriptors[TxProduceIndex].Packet;

	for (q = p; q != NULL; q = q->next)
	{
		nTxBufOffset = 0;
		if (q && q->len)
		{
			pSrcBuf = (INT8U*) q->payload;

			while( nTxBufOffset < q->len )
			{
				nPduLen = MIN(ETH_FRAG_SIZE-nDescOffset,q->len-nTxBufOffset);
				if (nPduLen)
				{
					memcpy(&pDescBuf[nDescOffset], &pSrcBuf[nTxBufOffset], nPduLen );
					nSendLen += nPduLen;

					if (nSendLen >= p->tot_len)
					{
						tb_descriptors[TxProduceIndex].Ctrl =  (EMAC_TX_DESC_OVERRIDE   |        /* Override the defaults from the MAC internal registers    */
							EMAC_TX_DESC_PAD        |        /* Add padding for frames < 64 bytes                        */
							EMAC_TX_DESC_LAST       |        /* No additional descriptors to follow, this is the last    */
							EMAC_TX_DESC_CRC)       |        /* Append the CRC automatically                             */
							(nPduLen + nDescOffset- 1);                       /* Write the size of the frame, starting from 0             */
					}
					else
					{
						tb_descriptors[TxProduceIndex].Ctrl =  (EMAC_TX_DESC_OVERRIDE   |        /* Override the defaults from the MAC internal registers    */
							EMAC_TX_DESC_PAD        |        /* Add padding for frames < 64 bytes                        */
							EMAC_TX_DESC_CRC)       |        /* Append the CRC automatically                             */
							(nPduLen + nDescOffset- 1);                       /* Write the size of the frame, starting from 0             */
					}

					nTxBufOffset += nPduLen;
					nDescOffset += nPduLen;
				
					//更新
					if (nDescOffset >= ETH_FRAG_SIZE )
					{
						TxProduceIndex ++;
						if (TxProduceIndex >= NUM_TX_FRAG)
						{
							TxProduceIndex -= NUM_TX_FRAG;
						}
						pDescBuf = (INT8U *)tb_descriptors[TxProduceIndex].Packet;
					}
					else if (nSendLen >= p->tot_len)
					{
						TxProduceIndex ++;
						if (TxProduceIndex >= NUM_TX_FRAG)
						{
							TxProduceIndex -= NUM_TX_FRAG;
						}
						break;
					}
				}
			}
		}							   
	}
	MAC_TXPRODUCEINDEX = TxProduceIndex;
}
/*
* Transmit packet.
*/ 
 
rt_err_t lpc24xxether_tx( rt_device_t dev, struct pbuf* p)
{
	struct pbuf* q; 
// 	err = IsTxDescEmpty();
// 	if (err == FALSE)
// 	{
// 		return 0;
// 	}
// 	while(!IsTxDescEmpty())
// 	{
// 		OSTimeDly(5);
// 	}

	/* lock tx operation */
//	rt_sem_take(&tx_sem, RT_WAITING_FOREVER);
	rt_sem_take(&tx_sem, RT_WAITING_FOREVER);

//	OSSemPend(tx_sem,0,&err);
	lpc24xxether_write_frame (p);
//	OSSemPost(tx_sem);
	rt_sem_release(&tx_sem);

	return 0;
}

INT8U lpc24xxether_read_frame(INT8U* ptr, INT32U section_length, INT32U total)
{
	register INT32U buf_remain, section_remain;
	static INT32U section_read = 0, buf_offset = 0, frame_read = 0;
	static INT8U* src_ptr = RT_NULL;

	INT32U RxProduceIndex = MAC_RXPRODUCEINDEX;
	INT32U RxConsumeIndex = MAC_RXCONSUMEINDEX;	 

	src_ptr = (INT8U *)(rb_descriptors[RxConsumeIndex].Packet );
	/* Loop until we have obtained the required amount of data. */
	section_read = 0;//
	while( section_read < section_length )
	{
		buf_remain = (total - buf_offset);
		section_remain = section_length - section_read;

		if( section_remain > buf_remain )
		{//目标空间够大
			/* more data on section than buffer size */
			memcpy(&ptr[ section_read ], &src_ptr[buf_offset], buf_remain);
			section_read += buf_remain;
			frame_read += buf_remain;

			/* free buffer */
			//rb_descriptors[current_rb_index].Packet &= ~RxDESC_FLAG_OWNSHIP;
			Rx_Stat[MAC_RXCONSUMEINDEX].Info     = 0;                       //Clear status for debugging purposes                      */

			/* move to the next frame. */
			MAC_RXCONSUMEINDEX      = (MAC_RXCONSUMEINDEX + 1) % NUM_RX_FRAG;     

			/* Reset the variables for the new buffer. */
			src_ptr = (INT8U *)(rb_descriptors[MAC_RXCONSUMEINDEX].Packet );
			buf_offset = 0;
		}
		else
		{
			/* more data on buffer than section size */
			memcpy(&ptr[section_read], &src_ptr[buf_offset], section_remain);
			buf_offset += section_remain;
			section_read += section_remain;
			frame_read += section_remain;

			/* finish this read */
			if((frame_read >= total) || (buf_offset >= ETH_FRAG_SIZE))
			{
				/* free buffer */
				Rx_Stat[MAC_RXCONSUMEINDEX].Info     = 0;                       //Clear status for debugging purposes                      */

				/* move to the next frame. */
				MAC_RXCONSUMEINDEX      = (MAC_RXCONSUMEINDEX + 1) % NUM_RX_FRAG;     

				src_ptr = (INT8U*)(rb_descriptors[MAC_RXCONSUMEINDEX].Packet );
				buf_offset = 0;
			}
		}
	}
} 

#if 0
void lpc24xxether_read_frame(rt_uint8_t* ptr, rt_uint32_t section_length, rt_uint32_t total)
{
	static rt_uint8_t* src_ptr;
	register rt_uint32_t buf_remain, section_remain;
	static rt_uint32_t section_read = 0, buf_offset = 0, frame_read = 0;

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
}

struct pbuf *lpc24xxether_rx(rt_device_t dev)
{
	struct pbuf *p = RT_NULL;
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
}
#endif

INT16U get_nic_rx_frame_size (void)
{
	INT16U     rxconsumeix;
	INT16U     rxproduceix;
	INT16U     desc_cnt=0 , i =0;
	INT16U     rx_frame_size = 0;
	INT32U   rxstatus;
	INT8U   flag = RT_FALSE;

	INT32U  nRsv =  MAC_RSV;
	INT32U  nSta =  MAC_STATUS;


	rxconsumeix =   MAC_RXCONSUMEINDEX;
	rxproduceix =   MAC_RXPRODUCEINDEX;
	if (rxconsumeix == rxproduceix )
	{
		return rx_frame_size;
	}

	if (rxproduceix < rxconsumeix) 
	{ /* If the produce index has wrapped around                  */
		desc_cnt   =   NUM_RX_FRAG - rxconsumeix + rxproduceix;
	}
	else
	{ /* If consumeix is < produceix, then no wrap around occured */
		desc_cnt   =   rxproduceix - rxconsumeix;	
	}

	for (i = rxconsumeix;  ; i++)
	{
		rxstatus   =  Rx_Stat[i].Info;   
		rxstatus       &= (RINFO_NO_DESCR    |                       /* Obtain the descriptor error bits                         */
			RINFO_OVERRUN    |
			RINFO_ALIGN_ERR  |
			RINFO_LEN_ERR    |
			RINFO_SYM_ERR    |
			RINFO_CRC_ERR    |
			RINFO_FAIL_FILT);

		if (rxstatus > 0)
			//	if (0)
			break;			 
		else
		{
			rx_frame_size  =  Rx_Stat[i].Info & RINFO_SIZE;
			rx_frame_size  -= 3;

			if (rxstatus | RINFO_LAST_FLAG)
			{//最后一封
				break;
			}				 
		}
		if (rxproduceix > rxconsumeix)
		{
			if (i >= rxproduceix) //到头了
				break;
		}
		else
		{
			if (i >= NUM_RX_FRAG-1)
			{
				i = 0;
				flag = RT_TRUE;
			}
			if (flag && i >= rxproduceix)
				break;		
		}					
	}
	return rx_frame_size;
}

INT8U  NetIF_IsValidPktSize (INT16U  size)
{
	INT8U  valid;

	valid = RT_TRUE;

	if (size  < 60)
	{
		valid = RT_FALSE;
	}

	if (size  > 1514+4) 
	{
		valid = RT_FALSE;
	}

	return (valid);
}

struct pbuf *lpc24xxether_rx(rt_device_t dev)
{
	
	struct pbuf *p = RT_NULL;

	INT32U RxProduceIndex = MAC_RXPRODUCEINDEX;
	INT32U RxConsumeIndex = MAC_RXCONSUMEINDEX;	 

	struct pbuf* q;
	INT16U  pkt_len = 0;
	INT16U  pkt_cnt = 0;
	INT8U   ret = RT_FALSE;

	if (RxProduceIndex == RxConsumeIndex)
		return RT_NULL;


	pkt_len = get_nic_rx_frame_size();
	//判断一下 pkt_len 是否有效，如果无效，则丢弃
	ret = NetIF_IsValidPktSize(pkt_len);
	if (ret == RT_FALSE)
	{
		EMAC_RxPktDiscard();
		return NULL;
	}

	//
	if (pkt_len)
	{
		p = pbuf_alloc(PBUF_RAW, pkt_len, PBUF_RAM);
		if(p != RT_NULL)
		{
			for(q = p; q != RT_NULL; q= q->next)
				lpc24xxether_read_frame(q->payload, q->len, pkt_len);
		}
		else
		{//如果内存申请不到，那么需要对描述符进行处理，扔掉部分数据包
			//rt_kprintf("no memory in pbuf\n");
			EMAC_RxPktDiscard();		 
		}
	}

	return p;
}
 
void set_mac_id( )   
{
//	MAC_SA0   =  (NetIF_MAC_Addr[5] << 8) |(NetIF_MAC_Addr[4]);                   /* Write the MAC Address, octect 2 and 1 to the EMAC        */
//	MAC_SA1   =  (NetIF_MAC_Addr[3] << 8) |(NetIF_MAC_Addr[2]);                   /* Write the MAC Address, octect 4 and 3 to the EMAC        */
//	MAC_SA2  =  (NetIF_MAC_Addr[1] << 8) | (NetIF_MAC_Addr[0]);                  /* Write the MAC Address, octect 6 and 5 to the EMAC        */
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
	lpc24xx_device.parent.parent.init		= rt_lpc24xxether_init;
	lpc24xx_device.parent.parent.open		= rt_lpc24xxether_open;
	lpc24xx_device.parent.parent.close		= rt_lpc24xxether_close;
	lpc24xx_device.parent.parent.read		= rt_lpc24xxether_read;
	lpc24xx_device.parent.parent.write		= rt_lpc24xxether_write;
	lpc24xx_device.parent.parent.control	= rt_lpc24xxether_control;
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

