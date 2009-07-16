//*------------------------------------------------------------------------------------------------
//* 文件名				: lib_emac.c
//* 功能描述			: EMAC外设的函数库
//* 作者    			: 焦海波
//* 版本				: 0.1
//* 建立日期、时间		: 2006/10/08 10:25
//* 最近修改日期、时间	: 
//* 修改原因			: 
//*------------------------------------------------------------------------------------------------
//*------------------------------------------ 头文件 ----------------------------------------------
#include "includes.h"
 
#include	"../include/lwip/opt.h"
#include	"emac.h"
//*------------------------------------- 常量、变量、宏 -------------------------------------------
__align(8) static volatile AT91S_RxBDescriptor __staRxBDescriptors[NB_RX_BUFS];	//* 接收缓冲区描述符数组
__align(4) static volatile INT8S baRxBufs[NB_RX_BUFS][ETH_RX_BUF_SIZE];			//* 接收缓冲区

__align(8) static volatile AT91S_TxBDescriptor __staTxBDescriptors[NB_TX_BUFS];	//* 发送缓冲区描述符数组
static INT8S baTxBufs[NB_TX_BUFS][ETH_TX_BUF_SIZE];								//* 发送缓冲区

static INT32S	__s32IdxToReset;												//* EMAC发送缓冲区队列发送位置索引

//* 保存要读取的接收缓冲区的索引位置
static INT16U	__u16CurRxBIdx;
static INT8S	*__pbFrom;

static INT16U	__u16TotalLenInRxBToRead;										//* EMAC接收缓冲区中已经读了多少字节的数据
static INT16U 	__u16TxBIndex;		                                            //* 对已使用的发送缓冲区进行索引计数
//*-------------------------------------- 函数原型声明 --------------------------------------------
//*================================================================================================
//*　　　　　　　　　　　　　　　　　　　　　函　数　区
//*================================================================================================
//*------------------------------------------------------------------------------------------------
//* 函数名称 : __ResetPHY
//* 功能描述 : 复位PHY芯片以使PHY芯片进入UTP模式
//* 入口参数 : 无
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
static void __ResetPHY(void)
{
	//* PHY在上电或复位期间需要根据RXER/FXEN引脚（24脚）的锁存输入状态来选择是UTP模式还是光纤模式。每个PIO
	//*	口线都被内置了一个上拉电阻，所以整机上电后该引脚的逻辑电平为高，这样就会使得PHY进入了光纤模式。我们
	//* 需要UTP模式，也就是PHY上电或复位期间的锁存输入状态为低，所以在这里必须禁止该口线的内部上拉电阻，然后
	//* 再复位PHY芯片，使其进入UTP模式
//	AT91C_BASE_PIOB->PIO_PPUDR = AT91C_PB7_ERXER;
		
	//* 复位PHY芯片，RTL8201BL的数据手册要求必须维持至少10ms的低电平，这里设置为2的(8 + 1)次方个SCK周期，
	//* 时间为15.621ms
//	AT91C_BASE_RSTC->RSTC_RMR = 0xA5000000 | (0x08 << 8);
//	AT91C_BASE_RSTC->RSTC_RCR = 0xA5000000 | AT91C_RSTC_EXTRST;

	//* 等待NRST引脚恢复为高电平
//	while(!(AT91C_BASE_RSTC->RSTC_RSR & AT91C_RSTC_NRSTL));
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : __HandlePHY
//* 功能描述 : 对PHY进行读写操作
//* 入口参数 : <u8RegAddr>[in]     指定对PHY的哪个寄存器进行读写
//*			 :  <pu16Data>[in/out] 指向操作数据的指针。对于读，这个地址保存读取的数据；对于写则是要写入
//*			 :                     寄存器的数据
//*			 :  <blIsRead>[in]     是否是读操作，如果不是则是写操作
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
static void __HandlePHY(INT8U u8RegAddr, INT16U *pu16Data, BOOLEAN blIsRead)
{
	INT32U	__u32HandleVal;	
#if 0
	
	if(blIsRead)
	{
		__u32HandleVal = (0x01 << 30) 
						| (0x02 << 28) 
						| (PHY_ADDR << 23) 
						| (u8RegAddr << 18) 
						| (0x02 << 16);
	}
	else
	{
		__u32HandleVal = (0x01 << 30) 
						| (0x01 << 28) 
						| (PHY_ADDR << 23) 
						| (u8RegAddr << 18) 
						| (0x02 << 16) 
						| (*pu16Data & 0xFFFF);
	}
		
	AT91C_BASE_EMAC->EMAC_MAN = __u32HandleVal;
	while(!(AT91C_BASE_EMAC->EMAC_NSR & AT91C_EMAC_IDLE));		
	if(blIsRead)
		*pu16Data = AT91C_BASE_EMAC->EMAC_MAN & 0x0000FFFF;

#endif
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : __CheckPHYID
//* 功能描述 : 检查PHY ID是否为0x82010000，如果不是则表明PHY还没有就绪或者出现故障，函数将一直查询直至正确
//* 入口参数 : 无
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
static void __CheckPHYID(void)
{
	INT32U		__u32PHYID;
	INT8U		__u8DelaySeconds = 0;
 #if 0
	macEnableMDI()
	{
		while(OS_TRUE)	
		{
			__HandlePHY(PHY_REG_ID1, (INT16U*)&__u32PHYID, OS_TRUE);
			__HandlePHY(PHY_REG_ID2, ((INT16U*)&__u32PHYID)+1, OS_TRUE);
			if(__u32PHYID == PHY_ID_RTL8201)
				break;
			else
			{
				__ResetPHY();
				if(__u8DelaySeconds < 15)
					__u8DelaySeconds++;
				OSTimeDlyHMSM(0, 0, __u8DelaySeconds, 0);
			}
		}
	}	
	macDisableMDI()
  #endif
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : __ilWaitLinkEstablished
//* 功能描述 : 等待PHY建立实际的物理链路
//* 入口参数 : 无
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
__inline void __ilWaitLinkEstablished(void)
{
	INT16U		__u16HandleData;
	INT8U		__u8DelaySeconds = 0;
#if 0	
	while(OS_TRUE)
	{
		__HandlePHY(PHY_REG_BMSR, &__u16HandleData, OS_TRUE);
		if(__u16HandleData & PHY_BMSR_LINKESTABLISHED)
			break;
		else
		{
			__ResetPHY();
			if(__u8DelaySeconds < 15)
				__u8DelaySeconds++;
			OSTimeDlyHMSM(0, 0, __u8DelaySeconds, 0);
		}
	}
#endif
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : __ilWaitAutoNegEnd
//* 功能描述 : 等待PHY自动协商结束
//* 入口参数 : 无
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
__inline void __ilWaitAutoNegEnd(void)
{
	INT16U		__u16HandleData = 0;
#if 0
	do{
		if(__u16HandleData & PHY_BMSR_AUTONEGEND)
			break;
		else
		{
			__HandlePHY(PHY_REG_BMSR, &__u16HandleData, OS_TRUE);
			OSTimeDlyHMSM(0, 0, 1, 0);
		}
	}while(OS_TRUE);
#endif
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : __SetupLinkSpeedAndDuplex
//* 功能描述 : 从PHY获取自动协商的结果，设置EMAC自身的链路速度和单双工方式。注意，该函数会阻塞所在任务的正
//*          : 常执行直至设置成功
//* 入口参数 : 无
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
static void __SetupLinkSpeedAndDuplex(void)
{
	INT16U		__u16HandleData;
	INT32U		__u32SpdAndFD = 0x00000000;
#if 0
	macEnableMDI()
	{
		__ilWaitLinkEstablished();
		
		__ilWaitAutoNegEnd();
		
		//* 获得协商的结果
		__HandlePHY(PHY_REG_ANLPAR, &__u16HandleData, OS_TRUE);
		//* 决定线速
		if((__u16HandleData & PHY_ANLPAR_100TX) || (__u16HandleData & PHY_ANLPAR_100TXFD))
			__u32SpdAndFD = AT91C_EMAC_SPD;
		//* 决定单双工方式
		if((__u16HandleData & PHY_ANLPAR_100TXFD) || (__u16HandleData & PHY_ANLPAR_10TFD))
			__u32SpdAndFD |= AT91C_EMAC_FD;
		
		//* 将链路速度和单双工方式设置进EMAC的网络配置寄存器
		AT91C_BASE_EMAC->EMAC_NCFGR = (AT91C_BASE_EMAC->EMAC_NCFGR & EMAC_NCFGR_SPD_FD_MASK) | __u32SpdAndFD;
	}
	macDisableMDI()
#endif
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : __InitDescriptorsForRxBAndTxB
//* 功能描述 : 初始化接收和发送缓冲区描述符，使每个描述符指向正确的缓冲区地址，然后将描述符首地址写入队列指
//*			 : 针寄存器
//* 入口参数 : 无
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
static void __InitDescriptorsForRxBAndTxB(void)
{
	INT32S		i;
#if 0	
	//* 将接收缓冲区地址填充到缓冲区描述符
	for(i=0; i<NB_RX_BUFS; i++)
		__staRxBDescriptors[i].u32RxBAddrAndFlag = (int)baRxBufs[i];
	//* 置位最后一个缓冲区描述符的Wrap位
	__staRxBDescriptors[NB_RX_BUFS - 1].u32RxBAddrAndFlag |= RxDESC_FLAG_WARP;
	
	
	//* 将发送缓冲区地址填充到缓冲区描述符
	for(i=0; i<NB_TX_BUFS; i++)
	{
		__staTxBDescriptors[i].u32TxBAddr = (int)baTxBufs[i];
		//* 标记这个缓冲区为程序所有，根据数据手册，该位为0表示这个缓冲区为EMAC所有
		__staTxBDescriptors[i].uStatus.bstStatus.bitIsUsed = 1;
	}
	//* 置位发送缓冲区的结束位（Wrap）
	__staTxBDescriptors[NB_TX_BUFS - 1].uStatus.bstStatus.bitIsWrap = 1;
	
	//* 将描述符队列首地址写入接收和发送队列指针寄存器
	AT91C_BASE_EMAC->EMAC_RBQP = (INT32U)__staRxBDescriptors;
	AT91C_BASE_EMAC->EMAC_TBQP = (INT32U)__staTxBDescriptors;
	
	//* 初始化与缓冲区队列相关的私有静态变量
	__s32IdxToReset = 0;
	__u16CurRxBIdx = 0;
	__u16TotalLenInRxBToRead = 0;
	__u16TxBIndex = 0;
#endif
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : __ilResetTxBDescriptors
//* 功能描述 : 复位发送缓冲区描述符的Used位，使其能够继续被使用
//* 入口参数 : 无
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
__inline void __ilResetTxBDescriptors(void)
{	
	//* 根据EMAC数据手册，EMAC在发送完毕后会置位帧的第一个缓冲区描述符的Used位，所以在这里只需置位其它描
	//* 述符即可
#if 0
	if(__staTxBDescriptors[__s32IdxToReset].uStatus.bstStatus.bitIsUsed)
	{
		while(!__staTxBDescriptors[__s32IdxToReset].uStatus.bstStatus.bitIsLastBuf)
		{
			__s32IdxToReset++;
			if(__s32IdxToReset >= NB_TX_BUFS)
				__s32IdxToReset = 0;
				
			__staTxBDescriptors[__s32IdxToReset].uStatus.bstStatus.bitIsUsed = 1;
		}
		
		__s32IdxToReset++;
		
		if(__s32IdxToReset >= NB_TX_BUFS)
			__s32IdxToReset = 0;
	}
	else;
 #endif 
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : irqEMACISR
//* 功能描述 : EMAC发送和接收结束中断处理函数
//* 入口参数 : 无
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
void irqEMACISR(void)
{
#if 0
	extern HANDLER hEthernetInput;
	INT32U	__u32IntStatus, __u32ReceiveStatus;

	//* 在读取时中断状态寄存器位会被清除
	__u32IntStatus = AT91C_BASE_EMAC->EMAC_ISR;
	
	//* 这个是容易忽略的地方，只有读取RSR寄存器，EMAC中断处理才能在写EOICR寄存器之后真正结束
	__u32ReceiveStatus = AT91C_BASE_EMAC->EMAC_RSR;
		
	if((__u32IntStatus & AT91C_EMAC_RCOMP) || (__u32ReceiveStatus & AT91C_EMAC_REC))
	{
		//* 向EMAC接收线程发送信号
		OSSemPost(hEthernetInput);
			
		//* 清除REC（Frame Received）位
		AT91C_BASE_EMAC->EMAC_RSR = AT91C_EMAC_REC;
	}

	if(__u32IntStatus & AT91C_EMAC_TCOMP)
	{
		//* 复位发送缓冲区描述符的Used位使其为程序所有
		__ilResetTxBDescriptors();
		AT91C_BASE_EMAC->EMAC_TSR = AT91C_EMAC_COMP;
	}
#endif
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : EMACInit
//* 功能描述 : 初始化EMAC：对PHY、MII口线、EMAC操作模式进行配置，设置接收和发送缓冲区描述符。设置
//*			 : EMAC接收和发送中断
//* 入口参数 : 无
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
void EMACInit(void)
{
	extern HANDLER hEthernetInput;
	
#if OS_CRITICAL_METHOD == 3                     
    OS_CPU_SR  		cpu_sr = 0;
#endif	

	//* 复位PHY芯片，使其进入UTP模式
	__ResetPHY();
	
	//* 等待一段指定的时间，使PHY就绪
	OSTimeDlyHMSM(0, 0, 3, 0);
#if 0
	//* 设置PIOB引脚为外设A引脚（即EMAC引脚），禁止PIOB控制，改为外设控制
	AT91C_BASE_PIOB->PIO_ASR = EMAC_MII_PINS;
	AT91C_BASE_PIOB->PIO_PDR = EMAC_MII_PINS;
	
	//* 因为没有使用ETXER，所以这里将其配置为由PIO控制
	AT91C_BASE_PIOB->PIO_PER = AT91C_PB12_ETXER;
	AT91C_BASE_PIOB->PIO_ODR = AT91C_PB12_ETXER;
	
	//* 设置MDC时钟分频数
	AT91C_BASE_EMAC->EMAC_NCFGR |= AT91C_EMAC_CLK_HCLK_32;
	
	//* 检查PHY ID是否为0x82010000，如果不是则表明PHY还没有就绪或者出现故障，函数将一直查询直至正确
	__CheckPHYID();
	
	//* 从PHY获取自动协商的结果，设置EMAC自身的链路速度和单双工方式。注意，该函数会阻塞所在任务的正常执行
	//* 直至设置成功
	__SetupLinkSpeedAndDuplex();
	
	//* 建立接收任务使用的信号量，对uCOS的配置保证信号量在软件逻辑上能够百分百建立成功
	hEthernetInput = OSSemCreate(0);
	
	//* 初始化接收和发送缓冲区描述符，使每个描述符指向正确的缓冲区地址
	__InitDescriptorsForRxBAndTxB();
	
	//* 设置EMAC为MII模式，使能EMAC时钟
	AT91C_BASE_EMAC->EMAC_USRIO = AT91C_EMAC_CLKEN;
	
	//* 清除接收状态寄存器
	AT91C_BASE_EMAC->EMAC_RSR = AT91C_EMAC_OVR | AT91C_EMAC_REC | AT91C_EMAC_BNA;
	
	//* 复制所有有效帧到接收缓冲区，不接收广播帧，不复制FCS字段
	AT91C_BASE_EMAC->EMAC_NCFGR |= AT91C_EMAC_CAF | AT91C_EMAC_NBC | AT91C_EMAC_DRFCS;
	
	//* 设置EMAC地址
	AT91C_BASE_EMAC->EMAC_SA1L = 0xBC | ((INT32U)(0x20 << 8)) | ((INT32U)(0x06 << 16)) | ((INT32U)(0x09 << 24));
	AT91C_BASE_EMAC->EMAC_SA1H = 0x30 | ((INT32U)(0x11 << 8));
	
	//* 设置EMAC中断
	OS_ENTER_CRITICAL()
	{
		//* 发送和接收结束中断使能
		AT91C_BASE_EMAC->EMAC_IER = AT91C_EMAC_RCOMP | AT91C_EMAC_TCOMP;
		
		AT91F_AIC_ConfigureIt(AT91C_BASE_AIC, AT91C_ID_EMAC, AT91C_AIC_PRIOR_HIGHEST, AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, irqEMACISR);
	    AT91C_BASE_AIC->AIC_IECR = 0x01 << AT91C_ID_EMAC;
	}
	OS_EXIT_CRITICAL()
	
	//* 最后，接收、发送使能
	AT91C_BASE_EMAC->EMAC_NCR |= AT91C_EMAC_TE | AT91C_EMAC_RE;
 #endif
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : GetInputPacketLen
//* 功能描述 : 获取到达的信息包的长度
//* 入口参数 : 无
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
INT16U GetInputPacketLen(void)
{
	INT16U		__u16Idx, __u16Len = 0;
#if 0	
	//* 跳过碎片帧，判断依据是帧头位未被置位
	while((__staRxBDescriptors[__u16CurRxBIdx].u32RxBAddrAndFlag & RxDESC_FLAG_OWNSHIP) 
			&& !__staRxBDescriptors[__u16CurRxBIdx].uStatus.bstStatus.bitStartOfFrm)
	{
		//* 释放这个缓冲区
		__staRxBDescriptors[__u16CurRxBIdx].u32RxBAddrAndFlag &= (~RxDESC_FLAG_OWNSHIP);		
		__u16CurRxBIdx++;
		if(__u16CurRxBIdx >= NB_RX_BUFS)
		{
			__u16CurRxBIdx = 0;
		}
	}
	
	__u16Idx = __u16CurRxBIdx;
	
	//* 只有最后一个缓冲区才保存帧的长度，其它均为0
	while((__staRxBDescriptors[__u16Idx].u32RxBAddrAndFlag & RxDESC_FLAG_OWNSHIP))
	{
		__u16Len = __staRxBDescriptors[__u16Idx].uStatus.bstStatus.bitLen;
		if(__u16Len > 0)
			break;
			
		__u16Idx++;
		if(__u16Idx >= NB_RX_BUFS)
			__u16Idx = 0;
	}
	
	//* 保存信息包读取位置
	__pbFrom = (INT8S*)(__staRxBDescriptors[__u16CurRxBIdx].u32RxBAddrAndFlag & EMAC_RxB_ADDR_MASK);
#endif 	
	return __u16Len;
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : EMACReadPacket
//* 功能描述 : 从EMAC读取信息包到申请的pbuf链，该函数要求PBUF_POOL_BUFSIZE大于或等于接收缓冲区，最好是整
//*			 : 数倍，这样函数处理最简单。注意，在调用该函数之前必须先调用GetInputPacketLen()函数，这样才
//*			 : 能获取正确的读取位置
//* 入口参数 :          <pbTo>[in] 指向pbuf的指针
//*			 : <u16SegmentLen>[in] pbuf中需要存储的实际数据长度，因为在申请pbuf时，pbuf_alloc()已经根据
//*			 :                     实际的帧长将其分割进了多个pbuf组成的pbuf链中，而pstPbuf->len则保存了
//*			 :                     每个pbuf需要存储的数据。换句话说pbuf链中的最后一个pbuf的len字段长度应
//*			 :                     该小于或等于PBUF_POOL_BUFSIZE，而前面的pbuf则等于PBUF_POOL_BUFSIZE
//*			 :  <blIsLastPbuf>[in] 是否是最后一个Pbuf
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
void EMACReadPacket(INT8S *pbTo, INT16U u16SegmentLen, BOOLEAN blIsLastPbuf)
{		
	INT16U			__u16TotalLenToRead = 0, 					//* 已经读取到pbuf的总字节数
					__u16RemainLenToRead,						//* 还剩多少字节没有读取到pbuf中
					__u16RemainLenInRxBToRead;					//* EMAC接收缓冲区中还剩下多少字节没有读取					
	BOOLEAN			__blIsNotRelease;							//* 是否已经主动释放给EMAC
	
	
	/* 注意，必须保证PBUF_POOL_BUFSIZE大于或等于接收缓冲区 */
#if 0	
	while(__u16TotalLenToRead < u16SegmentLen)
	{
		__u16RemainLenInRxBToRead = ETH_RX_BUF_SIZE - __u16TotalLenInRxBToRead;
		__u16RemainLenToRead = u16SegmentLen - __u16TotalLenToRead;
		if(__u16RemainLenToRead >= __u16RemainLenInRxBToRead)
		{
			memcpy(pbTo + __u16TotalLenToRead, __pbFrom, __u16RemainLenInRxBToRead);
			__u16TotalLenToRead += __u16RemainLenInRxBToRead;
			__u16TotalLenInRxBToRead = 0;
			
			//* 将接收缓冲区归还给EMAC
			__staRxBDescriptors[__u16CurRxBIdx].u32RxBAddrAndFlag &= (~RxDESC_FLAG_OWNSHIP);
			__blIsNotRelease = OS_FALSE;
			
			//* 调整描述符索引与读取指针
			__u16CurRxBIdx++;
			if(__u16CurRxBIdx >= NB_RX_BUFS)
				__u16CurRxBIdx = 0;
				
			__pbFrom = (INT8S*)(__staRxBDescriptors[__u16CurRxBIdx].u32RxBAddrAndFlag & EMAC_RxB_ADDR_MASK);
		}
		else
		{
			memcpy(pbTo + __u16TotalLenToRead, __pbFrom, __u16RemainLenToRead);
			__u16TotalLenToRead += __u16RemainLenToRead;
			__u16TotalLenInRxBToRead += __u16RemainLenToRead;
			
			__pbFrom = __pbFrom + __u16TotalLenInRxBToRead;						
			__blIsNotRelease = OS_TRUE;
		}					
	}
	
	if(blIsLastPbuf)
	{
		//* 将接收缓冲区归还给EMAC，如果存在恰好是PBUF_POOL_BUFSIZE的整数倍的数据包，则没有必要再一次释放，因为它已经在
		//* 上面被释放
		if(__blIsNotRelease)
		{
			__staRxBDescriptors[__u16CurRxBIdx].u32RxBAddrAndFlag &= (~RxDESC_FLAG_OWNSHIP);
			__u16CurRxBIdx++;
			if(__u16CurRxBIdx >= NB_RX_BUFS)
				__u16CurRxBIdx = 0;
		}
			
		__u16TotalLenInRxBToRead = 0;
	}
#endif			
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : EMACSendPacket
//* 功能描述 : 由low_level_output()函数调用，完成实际的数据发送。
//* 入口参数 :         <pbFrom>[in] 指向pbuf中数据的指针
//*			 :      <u32Length>[in] pbuf中的数据长度
//*			 : <blIsEndOfFrame>[in] 是否是pbuf链中的最后一个，也就是帧尾
//* 出口参数 : 如果无法申请下内存则返回ERR_MEM，成功则返回ERR_OK
//*------------------------------------------------------------------------------------------------
BOOLEAN EMACSendPacket(INT8S *pbFrom, INT32U u32Length, BOOLEAN blIsEndOfFrame)
{
	INT32U 			__u32TotalLenToWrite = 0, __u32CurLenToWrite, __u32LenRemainToWrite, __u32IsLastBuf;
	
	INT32S 			i;
	INT8S			*__pbBuf;
	
#if OS_CRITICAL_METHOD == 3                     
    OS_CPU_SR  		cpu_sr = 0;
#endif	

#if 0
	//* 如果要发送的数据长度大于一个发送缓冲区，则需要将这些数据分割进多个缓冲区进行发送
	while(__u32TotalLenToWrite < u32Length)
	{
		//* 等待缓冲区可用，最长等待3秒钟
		i = 0;
		while(!__staTxBDescriptors[__u16TxBIndex].uStatus.bstStatus.bitIsUsed)
		{
			//* 如果已经到达等待时间仍然没有可用缓冲区，则立即返回
			if(i > 300)
				return OS_FALSE;
			
			OSTimeDly(1);
			i++;
		}
	
		OS_ENTER_CRITICAL()
		{
			//* 从描述符中获得缓冲区地址，然后把数据复制到缓冲区
			__pbBuf = (INT8S*)__staTxBDescriptors[__u16TxBIndex].u32TxBAddr;
			
			//* 计算向缓冲区写入的数据长度
			__u32LenRemainToWrite = u32Length - __u32TotalLenToWrite;
			if(__u32LenRemainToWrite > ETH_TX_BUF_SIZE)
				__u32CurLenToWrite = ETH_TX_BUF_SIZE;
			else
				__u32CurLenToWrite = __u32LenRemainToWrite;
			
			//* 将pbuf中的数据复制到发送缓冲区
			memcpy(__pbBuf, &(pbFrom[__u32TotalLenToWrite]), __u32CurLenToWrite);
			__u32TotalLenToWrite += __u32CurLenToWrite;
			
			//* 看看是否是已经到达pbuf链的末尾，如果是则标记当前使用的缓冲区为最后一个缓冲区
			if(blIsEndOfFrame && (__u32TotalLenToWrite >= u32Length))
			{
				__u32IsLastBuf = TxDESC_STATUS_LAST_BUF;
			}
			else
				__u32IsLastBuf = 0;
	
			//* 填充当前的描述符：缓冲区中的数据实际长度、最后一个缓冲区标记、WRAP位（如果确实是最后一个描述符）
			if(__u16TxBIndex >= (NB_TX_BUFS-1))
			{				
				__staTxBDescriptors[__u16TxBIndex].uStatus.u32Status = (__u32CurLenToWrite & TxDESC_STATUS_BUF_SIZE)
																		| __u32IsLastBuf
																		| TxDESC_STATUS_WRAP;
				__u16TxBIndex = 0;
			}
			else
			{
				__staTxBDescriptors[__u16TxBIndex].uStatus.u32Status = (__u32CurLenToWrite & TxDESC_STATUS_BUF_SIZE)
																		| __u32IsLastBuf;
				__u16TxBIndex++;
			}
	
			//* 如果已经到达pbuf链的末尾则立即发送
			if(__u32IsLastBuf)
				AT91C_BASE_EMAC->EMAC_NCR |= AT91C_EMAC_TSTART;
		}
		OS_EXIT_CRITICAL()
	}
#endif
	return OS_TRUE;
}