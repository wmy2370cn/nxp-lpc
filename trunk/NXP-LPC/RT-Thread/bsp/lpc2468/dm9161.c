#include <rtthread.h>
#include "dm9161.h"

#include <netif/ethernetif.h>
#include "lwipopts.h"
#include "emac.h"
#include "applib.h"
#include "LPC24xx.h"

INT16U PHYID;
 
#define MAX_ADDR_LEN 6
struct rt_dm9161_eth
{
	/* inherit from ethernet device */
	struct eth_device parent;

	/* interface address info. */
	rt_uint8_t  dev_addr[MAX_ADDR_LEN];			/* hw address	*/
};
static struct rt_dm9161_eth dm9161_device;


//  function added to initialize Rx Descriptors
void RxDescrInit (void)
{
	unsigned int i;

	for (i = 0; i < NUM_RX_FRAG; i++)
	{
		RX_DESC_PACKET(i)  = RX_BUF(i);
		RX_DESC_CTRL(i)    = RCTRL_INT | (ETH_FRAG_SIZE-1);
		RX_STAT_INFO(i)    = 0;
		RX_STAT_HASHCRC(i) = 0;
	}

	/* Set EMAC Receive Descriptor Registers. */
	MAC_RXDESCRIPTOR    = RX_DESC_BASE;
	MAC_RXSTATUS        = RX_STAT_BASE;
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
		TX_DESC_PACKET(i) = TX_BUF(i);
		TX_DESC_CTRL(i)   = 0;
		TX_STAT_INFO(i)   = 0;
	}

	/* Set EMAC Transmit Descriptor Registers. */
	MAC_TXDESCRIPTOR    = TX_DESC_BASE;
	MAC_TXSTATUS        = TX_STAT_BASE;
	MAC_TXDESCRIPTORNUM = NUM_TX_FRAG-1;

	/* Tx Descriptors Point to 0 */
	MAC_TXPRODUCEINDEX  = 0;
}
/*********************************************************************************************************
** 函数名称: WritePHY
** 函数名称: WritePHY
**
** 功能描述：  写PHY端口
**
** 输　入:  INT32U PHYReg
** 输　入:  INT32U PHYData
**          
** 输　出:   void
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
void WritePHY( INT32U PHYReg, INT32U PHYData )
{
	MAC_MCMD = 0x0000;			/* write command */
	MAC_MADR = 0X0300 | PHYReg;	/* [12:8] == PHY addr, [4:0]=0x00(BMCR) register addr */
	MAC_MWTD = PHYData;
	while ( MAC_MIND != 0 );
	return;
}

void Write_PHY (INT32U phyadd,INT32S PhyReg, INT32S Value)
{
	unsigned int tout;

	MAC_MADR = (phyadd<<8) | PhyReg;
	MAC_MWTD = Value;

	/* Wait utill operation completed */
	tout = 0;
	for (tout = 0; tout < MII_WR_TOUT; tout++)
	{
		if ((MAC_MIND & 1) == 0)
		{
			break;
		}
	}
}
/*********************************************************************************************************
** 函数名称: ReadPHY
** 函数名称: ReadPHY
**
** 功能描述：  从PHY端口读取数据
**
** 输　入:  INT16U phyadd   
** 输　入:  INT32U PHYReg
**          
** 输　出:   INT32U  PHY data
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
INT32U ReadPHY( INT16U phyadd,INT32U PHYReg )
{
	INT32U i32;
	MAC_MCMD = 0x0001;			/* read command */
	i32 = (phyadd<<8) | PHYReg;	/* [12:8] == PHY addr, [4:0]=0x00(BMCR) register addr */ 
	MAC_MADR = i32;
	while ( MAC_MIND != 0 );
	MAC_MCMD = 0x0000;
	return( MAC_MRDD );
}
INT16U Read_PHY ( INT16U phyadd ,INT8U  PhyReg) 
{
	INT32U tout = 0;

	MAC_MADR = (phyadd<<8) | PhyReg;
	MAC_MCMD = 1;

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
        result = eth_device_ready(&(dm9161_device.parent));
        RT_ASSERT(result == RT_EOK);
    }

    if (status) // if finished packet transmission
    {
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
static rt_err_t rt_dm9161_init(rt_device_t dev)
{
	unsigned int regv,tout,id1,id2 ,i = 0;

	/* Power Up the EMAC controller. */
	PCONP |= 0x40000000;

	/* Reset all EMAC internal modules. */
	MAC_MAC1 = MAC1_RES_TX | MAC1_RES_MCS_TX | MAC1_RES_RX | MAC1_RES_MCS_RX | MAC1_SIM_RES | MAC1_SOFT_RES;
	MAC_COMMAND = CR_REG_RES | CR_TX_RES | CR_RX_RES;
	/* A short delay after reset. */
	for (tout = 100; tout; tout--);

	/* Initialize MAC control registers. */
	MAC_MAC1 = MAC1_PASS_ALL;
	MAC_MAC2 = MAC2_CRC_EN | MAC2_PAD_EN;
	MAC_MAXF = ETH_MAX_FLEN;
	MAC_CLRT = CLRT_DEF;
	MAC_IPGR = IPGR_DEF;

	/* Enable Reduced MII interface. */
	MAC_COMMAND = CR_RMII | CR_PASS_RUNT_FRM;

	/* Reset Reduced MII Logic. */
	MAC_SUPP = SUPP_RES_RMII;
	for (tout = 100; tout; tout--);
	MAC_SUPP = 0;

	// probe phy address
	for(i=0;i<32;i++)
	{
		PHYID = Read_PHY(i , 2 );
		if(PHYID == 0X0181)
			break;
	}
	if(i >= 32)
		while(1);
	//  复位PHY芯片
	Write_PHY(PHYID, 0, 0x9200 );
	//  等待一段指定的时间，使PHY就绪



	//判断工作在10/100 半双工/全双工


	 // Initialize Tx and Rx DMA Descriptors 
	TxDescrInit();
	RxDescrInit();

	/* Receive Broadcast and Perfect Match Packets */
	MAC_RXFILTERCTRL = RFC_UCAST_EN | RFC_BCAST_EN | RFC_PERFECT_EN;

	/* Set up RX filter, accept broadcast and perfect station */
	MAC_RXFILTERCTRL = 0x0022;	/* [1]-accept broadcast, [5]accept perfect */
	MAC_RXFILTERCTRL |= 0x0005;//MULTICAST_UNICAST
	MAC_RXFILTERCTRL |= 0x0018;//ENABLE_HASH


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
		if(args) rt_memcpy(args, dm9161_device.dev_addr, 6);
		else return -RT_ERROR;
		break;

	default :
		break;
	}

	return RT_EOK;
}

/* ethernet device interface */
/* transmit packet. */
rt_err_t rt_dm9000_tx( rt_device_t dev, struct pbuf* p)
{
	struct pbuf* q;
	rt_uint32_t len;
	rt_uint8_t* ptr;

	for (q = p; q != NULL; q = q->next)
	{
		len = q->len;
		ptr = q->payload;

		/* write data to device */
	}

	return RT_EOK;
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

void rt_hw_dm9161_init()
{
	dm9161_device.parent.parent.init       = rt_dm9161_init;
	dm9161_device.parent.parent.open       = rt_dm9000_open;
	dm9161_device.parent.parent.close      = rt_dm9000_close;
	dm9161_device.parent.parent.read       = rt_dm9000_read;
	dm9161_device.parent.parent.write      = rt_dm9000_write;
	dm9161_device.parent.parent.control    = rt_dm9000_control;
	dm9161_device.parent.parent.private    = RT_NULL;

	dm9161_device.parent.eth_rx     = rt_dm9000_rx;
	dm9161_device.parent.eth_tx     = rt_dm9000_tx;

	rt_device_register((rt_device_t)&dm9161_device,"E0", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX);
}
