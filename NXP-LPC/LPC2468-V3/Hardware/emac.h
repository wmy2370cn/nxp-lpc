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
// 
// 
// typedef struct 
// {/* RX Descriptor struct              */
// 	INT32U Packet;
// 	INT32U Ctrl;
// } RX_Desc;
// 
// typedef struct 
// {/* RX Status struct                  */
// 	INT32U Info;
// 	INT32U HashCRC;
// } RX_Stat;
// 
// typedef struct 
// {                        /* TX Descriptor struct              */
// 	INT32U Packet;
// 	INT32U Ctrl;
// } TX_Desc;
// 
// typedef struct 
// {                        /* TX Status struct                  */
// 	INT32U Info;
// } TX_Stat;
// 

#define MYMAC_1         0x00            /* our ethernet (MAC) address        */
#define MYMAC_2         0x30            /* (MUST be unique in LAN!)          */
#define MYMAC_3         0x6C
#define MYMAC_4         0x00
#define MYMAC_5         0x00
#define MYMAC_6         0x02

/* EMAC Memory Buffer configuration for 16K Ethernet RAM. */
#define NUM_RX_FRAG         4           /* Num.of RX Fragments 4*1536= 6.0kB */
#define NUM_TX_FRAG         2           /* Num.of TX Fragments 2*1536= 3.0kB */
#define ETH_FRAG_SIZE       1536        /* Packet Fragment size 1536 Bytes   */

#define ETH_MAX_FLEN        1536        /* Max. Ethernet Frame Size          */

/* EMAC variables located in 16K Ethernet SRAM */
#define RX_DESC_BASE        0x7FE00000
#define RX_STAT_BASE        (RX_DESC_BASE + NUM_RX_FRAG*8)
#define TX_DESC_BASE        (RX_STAT_BASE + NUM_RX_FRAG*8)
#define TX_STAT_BASE        (TX_DESC_BASE + NUM_TX_FRAG*8)
#define RX_BUF_BASE         (TX_STAT_BASE + NUM_TX_FRAG*4)
#define TX_BUF_BASE         (RX_BUF_BASE  + NUM_RX_FRAG*ETH_FRAG_SIZE)

/* RX and TX descriptor and status definitions. */
#define RX_DESC_PACKET(i)   (*(unsigned int *)(RX_DESC_BASE   + 8*i))
#define RX_DESC_CTRL(i)     (*(unsigned int *)(RX_DESC_BASE+4 + 8*i))
#define RX_STAT_INFO(i)     (*(unsigned int *)(RX_STAT_BASE   + 8*i))
#define RX_STAT_HASHCRC(i)  (*(unsigned int *)(RX_STAT_BASE+4 + 8*i))
#define TX_DESC_PACKET(i)   (*(unsigned int *)(TX_DESC_BASE   + 8*i))
#define TX_DESC_CTRL(i)     (*(unsigned int *)(TX_DESC_BASE+4 + 8*i))
#define TX_STAT_INFO(i)     (*(unsigned int *)(TX_STAT_BASE   + 4*i))
#define RX_BUF(i)           (RX_BUF_BASE + ETH_FRAG_SIZE*i)
#define TX_BUF(i)           (TX_BUF_BASE + ETH_FRAG_SIZE*i)

/* MAC Configuration Register 1 */
#define MAC1_REC_EN         0x00000001  /* Receive Enable                    */
#define MAC1_PASS_ALL       0x00000002  /* Pass All Receive Frames           */
#define MAC1_RX_FLOWC       0x00000004  /* RX Flow Control                   */
#define MAC1_TX_FLOWC       0x00000008  /* TX Flow Control                   */
#define MAC1_LOOPB          0x00000010  /* Loop Back Mode                    */
#define MAC1_RES_TX         0x00000100  /* Reset TX Logic                    */
#define MAC1_RES_MCS_TX     0x00000200  /* Reset MAC TX Control Sublayer     */
#define MAC1_RES_RX         0x00000400  /* Reset RX Logic                    */
#define MAC1_RES_MCS_RX     0x00000800  /* Reset MAC RX Control Sublayer     */
#define MAC1_SIM_RES        0x00004000  /* Simulation Reset                  */
#define MAC1_SOFT_RES       0x00008000  /* Soft Reset MAC                    */

/* MAC Configuration Register 2 */
#define MAC2_FULL_DUP       0x00000001  /* Full Duplex Mode                  */
#define MAC2_FRM_LEN_CHK    0x00000002  /* Frame Length Checking             */
#define MAC2_HUGE_FRM_EN    0x00000004  /* Huge Frame Enable                 */
#define MAC2_DLY_CRC        0x00000008  /* Delayed CRC Mode                  */
#define MAC2_CRC_EN         0x00000010  /* Append CRC to every Frame         */
#define MAC2_PAD_EN         0x00000020  /* Pad all Short Frames              */
#define MAC2_VLAN_PAD_EN    0x00000040  /* VLAN Pad Enable                   */
#define MAC2_ADET_PAD_EN    0x00000080  /* Auto Detect Pad Enable            */
#define MAC2_PPREAM_ENF     0x00000100  /* Pure Preamble Enforcement         */
#define MAC2_LPREAM_ENF     0x00000200  /* Long Preamble Enforcement         */
#define MAC2_NO_BACKOFF     0x00001000  /* No Backoff Algorithm              */
#define MAC2_BACK_PRESSURE  0x00002000  /* Backoff Presurre / No Backoff     */
#define MAC2_EXCESS_DEF     0x00004000  /* Excess Defer                      */

/* Back-to-Back Inter-Packet-Gap Register */
#define IPGT_FULL_DUP       0x00000015  /* Recommended value for Full Duplex */
#define IPGT_HALF_DUP       0x00000012  /* Recommended value for Half Duplex */

/* Non Back-to-Back Inter-Packet-Gap Register */
#define IPGR_DEF            0x00000012  /* Recommended value                 */

/* Collision Window/Retry Register */
#define CLRT_DEF            0x0000370F  /* Default value                     */

/* PHY Support Register */
#define SUPP_SPEED          0x00000100  /* Reduced MII Logic Current Speed   */
#define SUPP_RES_RMII       0x00000800  /* Reset Reduced MII Logic           */

/* Test Register */
#define TEST_SHCUT_PQUANTA  0x00000001  /* Shortcut Pause Quanta             */
#define TEST_TST_PAUSE      0x00000002  /* Test Pause                        */
#define TEST_TST_BACKP      0x00000004  /* Test Back Pressure                */

/* MII Management Configuration Register */
#define MCFG_SCAN_INC       0x00000001  /* Scan Increment PHY Address        */
#define MCFG_SUPP_PREAM     0x00000002  /* Suppress Preamble                 */
#define MCFG_CLK_SEL        0x0000001C  /* Clock Select Mask                 */
#define MCFG_RES_MII        0x00008000  /* Reset MII Management Hardware     */

/* MII Management Command Register */
#define MCMD_READ           0x00000001  /* MII Read                          */
#define MCMD_SCAN           0x00000002  /* MII Scan continuously             */

#define MII_WR_TOUT         0x00050000  /* MII Write timeout count           */
#define MII_RD_TOUT         0x00050000  /* MII Read timeout count            */

/* MII Management Address Register */
#define MADR_REG_ADR        0x0000001F  /* MII Register Address Mask         */
#define MADR_PHY_ADR        0x00001F00  /* PHY Address Mask                  */

/* MII Management Indicators Register */
#define MIND_BUSY           0x00000001  /* MII is Busy                       */
#define MIND_SCAN           0x00000002  /* MII Scanning in Progress          */
#define MIND_NOT_VAL        0x00000004  /* MII Read Data not valid           */
#define MIND_MII_LINK_FAIL  0x00000008  /* MII Link Failed                   */

/* Command Register */
#define CR_RX_EN            0x00000001  /* Enable Receive                    */
#define CR_TX_EN            0x00000002  /* Enable Transmit                   */
#define CR_REG_RES          0x00000008  /* Reset Host Registers              */
#define CR_TX_RES           0x00000010  /* Reset Transmit Datapath           */
#define CR_RX_RES           0x00000020  /* Reset Receive Datapath            */
#define CR_PASS_RUNT_FRM    0x00000040  /* Pass Runt Frames                  */
#define CR_PASS_RX_FILT     0x00000080  /* Pass RX Filter                    */
#define CR_TX_FLOW_CTRL     0x00000100  /* TX Flow Control                   */
#define CR_RMII             0x00000200  /* Reduced MII Interface             */
#define CR_FULL_DUP         0x00000400  /* Full Duplex                       */

/* Status Register */
#define SR_RX_EN            0x00000001  /* Enable Receive                    */
#define SR_TX_EN            0x00000002  /* Enable Transmit                   */

/* Transmit Status Vector 0 Register */
#define TSV0_CRC_ERR        0x00000001  /* CRC error                         */
#define TSV0_LEN_CHKERR     0x00000002  /* Length Check Error                */
#define TSV0_LEN_OUTRNG     0x00000004  /* Length Out of Range               */
#define TSV0_DONE           0x00000008  /* Tramsmission Completed            */
#define TSV0_MCAST          0x00000010  /* Multicast Destination             */
#define TSV0_BCAST          0x00000020  /* Broadcast Destination             */
#define TSV0_PKT_DEFER      0x00000040  /* Packet Deferred                   */
#define TSV0_EXC_DEFER      0x00000080  /* Excessive Packet Deferral         */
#define TSV0_EXC_COLL       0x00000100  /* Excessive Collision               */
#define TSV0_LATE_COLL      0x00000200  /* Late Collision Occured            */
#define TSV0_GIANT          0x00000400  /* Giant Frame                       */
#define TSV0_UNDERRUN       0x00000800  /* Buffer Underrun                   */
#define TSV0_BYTES          0x0FFFF000  /* Total Bytes Transferred           */
#define TSV0_CTRL_FRAME     0x10000000  /* Control Frame                     */
#define TSV0_PAUSE          0x20000000  /* Pause Frame                       */
#define TSV0_BACK_PRESS     0x40000000  /* Backpressure Method Applied       */
#define TSV0_VLAN           0x80000000  /* VLAN Frame                        */

/* Transmit Status Vector 1 Register */
#define TSV1_BYTE_CNT       0x0000FFFF  /* Transmit Byte Count               */
#define TSV1_COLL_CNT       0x000F0000  /* Transmit Collision Count          */

/* Receive Status Vector Register */
#define RSV_BYTE_CNT        0x0000FFFF  /* Receive Byte Count                */
#define RSV_PKT_IGNORED     0x00010000  /* Packet Previously Ignored         */
#define RSV_RXDV_SEEN       0x00020000  /* RXDV Event Previously Seen        */
#define RSV_CARR_SEEN       0x00040000  /* Carrier Event Previously Seen     */
#define RSV_REC_CODEV       0x00080000  /* Receive Code Violation            */
#define RSV_CRC_ERR         0x00100000  /* CRC Error                         */
#define RSV_LEN_CHKERR      0x00200000  /* Length Check Error                */
#define RSV_LEN_OUTRNG      0x00400000  /* Length Out of Range               */
#define RSV_REC_OK          0x00800000  /* Frame Received OK                 */
#define RSV_MCAST           0x01000000  /* Multicast Frame                   */
#define RSV_BCAST           0x02000000  /* Broadcast Frame                   */
#define RSV_DRIB_NIBB       0x04000000  /* Dribble Nibble                    */
#define RSV_CTRL_FRAME      0x08000000  /* Control Frame                     */
#define RSV_PAUSE           0x10000000  /* Pause Frame                       */
#define RSV_UNSUPP_OPC      0x20000000  /* Unsupported Opcode                */
#define RSV_VLAN            0x40000000  /* VLAN Frame                        */

/* Flow Control Counter Register */
#define FCC_MIRR_CNT        0x0000FFFF  /* Mirror Counter                    */
#define FCC_PAUSE_TIM       0xFFFF0000  /* Pause Timer                       */

/* Flow Control Status Register */
#define FCS_MIRR_CNT        0x0000FFFF  /* Mirror Counter Current            */

/* Receive Filter Control Register */
#define RFC_UCAST_EN        0x00000001  /* Accept Unicast Frames Enable      */
#define RFC_BCAST_EN        0x00000002  /* Accept Broadcast Frames Enable    */
#define RFC_MCAST_EN        0x00000004  /* Accept Multicast Frames Enable    */
#define RFC_UCAST_HASH_EN   0x00000008  /* Accept Unicast Hash Filter Frames */
#define RFC_MCAST_HASH_EN   0x00000010  /* Accept Multicast Hash Filter Fram.*/
#define RFC_PERFECT_EN      0x00000020  /* Accept Perfect Match Enable       */
#define RFC_MAGP_WOL_EN     0x00001000  /* Magic Packet Filter WoL Enable    */
#define RFC_PFILT_WOL_EN    0x00002000  /* Perfect Filter WoL Enable         */

/* Receive Filter WoL Status/Clear Registers */
#define WOL_UCAST           0x00000001  /* Unicast Frame caused WoL          */
#define WOL_BCAST           0x00000002  /* Broadcast Frame caused WoL        */
#define WOL_MCAST           0x00000004  /* Multicast Frame caused WoL        */
#define WOL_UCAST_HASH      0x00000008  /* Unicast Hash Filter Frame WoL     */
#define WOL_MCAST_HASH      0x00000010  /* Multicast Hash Filter Frame WoL   */
#define WOL_PERFECT         0x00000020  /* Perfect Filter WoL                */
#define WOL_RX_FILTER       0x00000080  /* RX Filter caused WoL              */
#define WOL_MAG_PACKET      0x00000100  /* Magic Packet Filter caused WoL    */

/* Interrupt Status/Enable/Clear/Set Registers */
#define INT_RX_OVERRUN      0x00000001  /* Overrun Error in RX Queue         */
#define INT_RX_ERR          0x00000002  /* Receive Error                     */
#define INT_RX_FIN          0x00000004  /* RX Finished Process Descriptors   */
#define INT_RX_DONE         0x00000008  /* Receive Done                      */
#define INT_TX_UNDERRUN     0x00000010  /* Transmit Underrun                 */
#define INT_TX_ERR          0x00000020  /* Transmit Error                    */
#define INT_TX_FIN          0x00000040  /* TX Finished Process Descriptors   */
#define INT_TX_DONE         0x00000080  /* Transmit Done                     */
#define INT_SOFT_INT        0x00001000  /* Software Triggered Interrupt      */
#define INT_WAKEUP          0x00002000  /* Wakeup Event Interrupt            */

/* Power Down Register */
#define PD_POWER_DOWN       0x80000000  /* Power Down MAC                    */

/* RX Descriptor Control Word */
#define RCTRL_SIZE          0x000007FF  /* Buffer size mask                  */
#define RCTRL_INT           0x80000000  /* Generate RxDone Interrupt         */

/* RX Status Hash CRC Word */
#define RHASH_SA            0x000001FF  /* Hash CRC for Source Address       */
#define RHASH_DA            0x001FF000  /* Hash CRC for Destination Address  */

/* RX Status Information Word */
#define RINFO_SIZE          0x000007FF  /* Data size in bytes                */
#define RINFO_CTRL_FRAME    0x00040000  /* Control Frame                     */
#define RINFO_VLAN          0x00080000  /* VLAN Frame                        */
#define RINFO_FAIL_FILT     0x00100000  /* RX Filter Failed                  */
#define RINFO_MCAST         0x00200000  /* Multicast Frame                   */
#define RINFO_BCAST         0x00400000  /* Broadcast Frame                   */
#define RINFO_CRC_ERR       0x00800000  /* CRC Error in Frame                */
#define RINFO_SYM_ERR       0x01000000  /* Symbol Error from PHY             */
#define RINFO_LEN_ERR       0x02000000  /* Length Error                      */
#define RINFO_RANGE_ERR     0x04000000  /* Range Error (exceeded max. size)  */
#define RINFO_ALIGN_ERR     0x08000000  /* Alignment Error                   */
#define RINFO_OVERRUN       0x10000000  /* Receive overrun                   */
#define RINFO_NO_DESCR      0x20000000  /* No new Descriptor available       */
#define RINFO_LAST_FLAG     0x40000000  /* Last Fragment in Frame            */
#define RINFO_ERR           0x80000000  /* Error Occured (OR of all errors)  */

#define RINFO_ERR_MASK     (RINFO_FAIL_FILT | RINFO_CRC_ERR   | RINFO_SYM_ERR | \
	RINFO_LEN_ERR   | RINFO_ALIGN_ERR | RINFO_OVERRUN)

/* TX Descriptor Control Word */
#define TCTRL_SIZE          0x000007FF  /* Size of data buffer in bytes      */
#define TCTRL_OVERRIDE      0x04000000  /* Override Default MAC Registers    */
#define TCTRL_HUGE          0x08000000  /* Enable Huge Frame                 */
#define TCTRL_PAD           0x10000000  /* Pad short Frames to 64 bytes      */
#define TCTRL_CRC           0x20000000  /* Append a hardware CRC to Frame    */
#define TCTRL_LAST          0x40000000  /* Last Descriptor for TX Frame      */
#define TCTRL_INT           0x80000000  /* Generate TxDone Interrupt         */

/* TX Status Information Word */
#define TINFO_COL_CNT       0x01E00000  /* Collision Count                   */
#define TINFO_DEFER         0x02000000  /* Packet Deferred (not an error)    */
#define TINFO_EXCESS_DEF    0x04000000  /* Excessive Deferral                */
#define TINFO_EXCESS_COL    0x08000000  /* Excessive Collision               */
#define TINFO_LATE_COL      0x10000000  /* Late Collision Occured            */
#define TINFO_UNDERRUN      0x20000000  /* Transmit Underrun                 */
#define TINFO_NO_DESCR      0x40000000  /* No new Descriptor available       */
#define TINFO_ERR           0x80000000  /* Error Occured (OR of all errors)  */










//*-------------------------------------------- 原型声明 -----------------------------------------
extern void EMACInit(void);
extern INT16U GetInputPacketLen(void);
extern void EMACReadPacket(INT8S *pbTo, INT16U u16SegmentLen, BOOLEAN blIsLastPbuf);
extern BOOLEAN EMACSendPacket(INT8S *pbFrom, INT32U u32Length, BOOLEAN blIsEndOfFrame);
#endif