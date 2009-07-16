//*------------------------------------------------------------------------------------------------
//* 文件名				: lib_emac.h
//* 功能描述			: EMAC外设的函数库头文件
//* 作者    			: 焦海波
//* 版本				: 0.1
//* 建立日期、时间		: 2006/06/07 10:40
//* 最近修改日期、时间	: 
//* 修改原因			: 
//*------------------------------------------------------------------------------------------------

#ifndef	__lib_emac_h
#define	__lib_emac_h

//*---------------------------------------- 结构体及宏定义 -----------------------------------------

#define	EMAC_MII_PINS		(AT91C_PB8_EMDC         | \
							AT91C_PB9_EMDIO         | \
							AT91C_PB2_ETX0          | \
							AT91C_PB3_ETX1          | \
							AT91C_PB10_ETX2         | \
							AT91C_PB11_ETX3         | \
							AT91C_PB1_ETXEN         | \
							AT91C_PB0_ETXCK_EREFCK  | \
							AT91C_PB15_ERXDV_ECRSDV | \
							AT91C_PB5_ERX0			| \
							AT91C_PB6_ERX1			| \
							AT91C_PB13_ERX2			| \
							AT91C_PB14_ERX3			| \
							AT91C_PB17_ERXCK		| \
							AT91C_PB16_ECOL			| \
							AT91C_PB4_ECRS			| \
							AT91C_PB7_ERXER)
							
//* PHY芯片地址
#define	PHY_ADDR			0x01

//* PHY芯片RTL8201的ID号
#define	PHY_ID_RTL8201		0x82010000
										
#define	PHY_REG_ID1			0x02		//* PHY标识寄存器1
#define	PHY_REG_ID2			0x03		//* PHY标识寄存器2

#define	PHY_REG_BMSR		0x01		//* 基本模式状态寄存器
#define	PHY_REG_ANLPAR		0x05		//* 自动协商连接方能力寄存器

//* PHY寄存器位定义
#define	PHY_BMSR_LINKESTABLISHED	((INT16U)(1 << 2))	//* 已经建立了有效连接（基本模式状态寄存器）
#define	PHY_BMSR_AUTONEGEND			((INT16U)(1 << 5))	//* 自动协商已经结束（基本模式状态寄存器）
#define	PHY_ANLPAR_100TXFD			((INT16U)(1 << 8))	//* 连接方支持100Base-TX全双工
#define	PHY_ANLPAR_100TX			((INT16U)(1 << 7))	//* 连接方支持100Base-TX
#define	PHY_ANLPAR_10TFD			((INT16U)(1 << 6))	//* 连接方支持10Base-T全双工

#define	EMAC_NCFGR_SPD_FD_MASK		0xFFFFFFFC			//* 网络配置寄存器的速度和双工位掩码数

#define	NB_RX_BUFS					32					//* 接收缓冲区个数
#define	ETH_RX_BUF_SIZE				128					//* EMAC数据手册规定接收缓冲区仅占128字节大小，128 * 32接收缓冲区共4096字节

#define	NB_TX_BUFS					16					//* 发送缓冲区个数
#define	ETH_TX_BUF_SIZE				256					//* 发送缓冲区大小

/* 接收缓冲区描述符标志位及状态位宏定义 */
#define	RxDESC_FLAG_WARP			0x00000002			//* 标记接收缓冲区描述符列表中的最后一个描述符
#define	RxDESC_FLAG_OWNSHIP			0x00000001			//* 标记接收缓冲区为程序所有

/* 发送缓冲区描述符状态信息宏定义 */
#define	TxDESC_STATUS_LAST_BUF		((INT32U)(1 << 15))	//* 帧的最后一个缓冲区
#define	TxDESC_STATUS_BUF_SIZE		((INT32U)0x000007FF)//* 缓冲区长度掩码
#define	TxDESC_STATUS_WRAP			((INT32U)(1 << 30))	//* 最后一个缓冲区

//* 接收缓冲区描述符
#define	EMAC_RxB_ADDR_MASK			0xFFFFFFFC			//* 接收缓冲区描述符地址掩码
typedef struct{
	INT32U u32RxBAddrAndFlag;
	union
	{
		INT32U u32Status;
		struct {
			INT32U bitLen					:12;
			INT32U bitRxBOffset				:2;
			INT32U bitStartOfFrm			:1;
			INT32U bitEndOfFrm				:1;
			INT32U bitCFI					:1;
			INT32U bitVLANPrio				:3;
			INT32U bitPrioTag				:1;
			INT32U bitVLANTag				:1;
			INT32U bitTypeID				:1;
			INT32U bitSA4Match				:1;
			INT32U bitSA3Match				:1;
			INT32U bitSA2Match				:1;
			INT32U bitSA1Match				:1;
			INT32U bitRsrved0				:1;
			INT32U bitExtAddrMatch			:1;
			INT32U bitUniCastHashMatch		:1;
			INT32U bitMultiCastHashMatch	:1;
			INT32U bitIsBroadCastAddr		:1;
		}bstStatus;		
	}uStatus;
}AT91S_RxBDescriptor, *AT91PS_RxBDescriptor;

//* 发送缓冲区描述符 
typedef struct{
	INT32U u32TxBAddr;
	union
	{
		INT32U u32Status;
		struct {
			INT32U bitLen			:11;
			INT32U bitRsrved0		:4;
			INT32U bitIsLastBuf		:1;
			INT32U bitIsNoCRC		:1;
			INT32U bitRsrved1		:10;
			INT32U bitIsBExhausted	:1;
			INT32U bitIsTxUnderrun	:1;
			INT32U bitTxError		:1;
			INT32U bitIsWrap		:1;
			INT32U bitIsUsed		:1;
		}bstStatus;		
	}uStatus;
}AT91S_TxBDescriptor, *AT91PS_TxBDescriptor;
										
//* 使能EMAC管理数据接口
#define	macEnableMDI()								\
{													\
	AT91C_BASE_EMAC->EMAC_NCR |= AT91C_EMAC_MPE;	\
}

//* 禁止EMAC管理数据接口
#define	macDisableMDI()								\
{													\
	AT91C_BASE_EMAC->EMAC_NCR &= ~AT91C_EMAC_MPE;	\
}
//*-------------------------------------------- 原型声明 -----------------------------------------
extern void EMACInit(void);
extern INT16U GetInputPacketLen(void);
extern void EMACReadPacket(INT8S *pbTo, INT16U u16SegmentLen, BOOLEAN blIsLastPbuf);
extern BOOLEAN EMACSendPacket(INT8S *pbFrom, INT32U u32Length, BOOLEAN blIsEndOfFrame);
#endif