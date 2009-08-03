#include <rtthread.h>
#include "dm9161.h"

#include <netif/ethernetif.h>
#include "lwipopts.h"
#include "emac.h"
#include "applib.h"
#include "LPC24xx.h"


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


/* EMAC local DMA Descriptors. */ 
/* EMAC local DMA Descriptors. */
static            RX_Desc rb_descriptors[NUM_RX_FRAG];
static            TX_Desc tb_descriptors[NUM_TX_FRAG];
 
static  RX_Stat Rx_Stat[NUM_RX_FRAG]; /* Must be 8-Byte alligned   */
static  TX_Stat Tx_Stat[NUM_TX_FRAG];


/* EMAC local DMA buffers. */
static rt_uint32_t rx_buf[NUM_RX_FRAG][ETH_FRAG_SIZE>>2];
static rt_uint32_t tx_buf[NUM_TX_FRAG][ETH_FRAG_SIZE>>2];


static struct rt_lpc24xx_eth lpc24xx_device;

static struct rt_semaphore tx_sem;


//  function added to initialize Rx Descriptors
void RxDescrInit (void)
{
	/* Initialize Receive Descriptor and Status array. */
	unsigned int i = 0;

	for (i = 0; i < NUM_RX_FRAG; i++) 
	{
		rb_descriptors[i].Packet  = (rt_uint32_t)&rx_buf[i];
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

	for (i = 0; i < NUM_TX_FRAG; i++) 
	{
		tb_descriptors[i].Packet = (rt_uint32_t)&tx_buf[i];
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

void  NetBSP_DlyMs (INT32U ms)
{
	rt_thread_delay(ms/10);
//	OSTimeDlyHMSM(0, 0, ms / 1000, ms % 1000);
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
INT8U Write_PHY (INT32U phyadd,INT32S PhyReg, INT32S Value)
{
	unsigned int tout;

	MAC_MCMD = 0x0000;			        // Issue a Write COMMAND     
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
INT16U Read_PHY ( INT16U phyadd ,INT8U  PhyReg) 
{
	INT32U tout = 0;

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
	MAC_MCMD = 0;
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
void rt_dm9000_isr(int irqno)
{
    rt_uint32_t status;

    if (status) // if receive packet
    {
        rt_err_t result;

        /* a frame has been received */
        result = eth_device_ready(&(lpc24xx_device.parent));
        RT_ASSERT(result == RT_EOK);
    }

    if (status) // if finished packet transmission
    {
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
static rt_err_t rt_dm9161_init(rt_device_t dev)
{
	unsigned int regv,tout,id1,id2 ,i = 0;
	INT32U  tempreg = 0;

	/* Power Up the EMAC controller. */
	PCONP |= 0x40000000;
 	/* Set the PIN to RMII */
	// PINSEL2 &= 0x0fc0ccf0;
	//PINSEL2 |= 0X50151105; //PINSEL2 = 0x50151105;	/* selects P1[0,1,4,8,9,10,14,15] */
	//PINSEL3 &= 0xfffffff0;
	//PINSEL3 |= 0X00000005; //PINSEL3 = 0x00000005;	/* selects P1[17:16] */

// 	i = rMAC_MODULEID;
// 	if(i == OLD_EMAC_MODULE_ID)
// 		PINSEL2 = 0x50151105;	/* selects P1[0,1,4,8,9,10,14,15] */
// 	else

	//是否需要设置?需要硬件部分确定?
//	PINSEL2 = 0x50150105;
// 	PINSEL3 = 0x00000005;	/* selects P1[17:16] */

	/* Reset all EMAC internal modules. */
	MAC_MAC1 = MAC1_RES_TX | MAC1_RES_MCS_TX | MAC1_RES_RX | MAC1_RES_MCS_RX | MAC1_SIM_RES | MAC1_SOFT_RES;
	MAC_COMMAND = CR_REG_RES | CR_TX_RES | CR_RX_RES;
	/* A short delay after reset. */
	for (tout = 500; tout; tout--);

	/* Initialize MAC control registers. */
	MAC_MAC1 = MAC1_PASS_ALL;
	MAC_MAC2 = MAC2_CRC_EN | MAC2_PAD_EN;
	MAC_MAXF = ETH_MAX_FLEN;
	MAC_CLRT = CLRT_DEF;
	MAC_IPGR = IPGR_DEF;

	//????
	/* Enable Reduced MII interface. */
	MAC_MCFG = MCFG_CLK_DIV20 | MCFG_RES_MII;
	for (tout = 100; tout; tout--);
	MAC_MCFG = MCFG_CLK_DIV20;

	MAC_COMMAND = CR_RMII | CR_PASS_RUNT_FRM;

	/* Enable Reduced MII interface. */
	MAC_COMMAND = CR_RMII | CR_PASS_RUNT_FRM;

	/* Reset Reduced MII Logic. */
	MAC_SUPP = SUPP_RES_RMII| SUPP_SPEED;
	for (tout = 100; tout; tout--);
	MAC_SUPP = SUPP_SPEED;

	//下面开始PHY设置
	//  复位PHY芯片
	#define EMAC_CFG_PHY_ADDR 0 
	Write_PHY(EMAC_CFG_PHY_ADDR, MII_BMCR, 0x9200 );

	// probe phy address
	for(i=0;i<32;i++)
	{
		PHYID = Read_PHY(i , 2 );
		if(PHYID == 0X0181)
			break;
	}
//	if(i >= 32)
//		while(1);
	//  等待一段指定的时间，使PHY就绪




	tempreg = Read_PHY(PHYID, 17 );

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
	}
	else
	{//出错啦

	}



	//设置MAC地址
//	SetMacID();
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


    return RT_EOK;
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

	/* enable interrupt */
//	AT91C_BASE_EMAC->EMAC_IER = AT91C_EMAC_RCOMP;


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

