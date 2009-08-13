
#pragma  once 
#include "applib.h"

/*
*********************************************************************************************************
*                                     PHYSICAL LAYER DEFINITIONS
*********************************************************************************************************
*/

#define  NET_PHY_SPD_0                                     0    /* Link speed unknown, or link down                     */
#define  NET_PHY_SPD_10                                   10    /* Link speed = 10mbps                                  */
#define  NET_PHY_SPD_100                                 100    /* Link speed = 100mbps                                 */
#define  NET_PHY_SPD_1000                               1000    /* Link speed = 1000mbps                                */

#define  NET_PHY_DUPLEX_UNKNOWN                            0    /* Duplex uknown or auto-neg incomplete                 */
#define  NET_PHY_DUPLEX_HALF                               1    /* Duplex = Half Duplex                                 */
#define  NET_PHY_DUPLEX_FULL                               2    /* Duplex = Full Duplex                                 */



/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/
 

#define  DM9161AE_INIT_AUTO_NEG_RETRIES        3

#define  DM9161AE_OUI                   0x00606E
#define  DM9161AE_VNDR_MDL                  0x08

/*
*********************************************************************************************************
*                                       DM9161AE REGISTER DEFINES
*********************************************************************************************************
*/
/* ------- Generic MII registers ---------- */
#define  PHY_REG_BMCR                       0x00                /* Basic mode control register              */
#define  PHY_REG_BMSR                       0x01                /* Basic mode status register               */
#define  PHY_REG_PHYID1                     0x02                /* PHYS ID 1                                */
#define  PHY_REG_PHYID2                     0x03                /* PHYS ID 2                                */
#define  PHY_REG_ANAR                       0x04                /* Advertisement control reg                */
#define  PHY_REG_ANLPAR                     0x05                /* Link partner ability reg                 */
#define  PHY_REG_ANER                       0x06                /* Expansion register                       */

/* ---------- Extended registers ---------- */
#define  DM9161_DSCR                        0x10                /* DAVICOM specified configuration          */
#define  DM9161_DSCSR                       0x11                /* DAVICOM specified configuration & status */
#define  DM9161_PWDOR                       0x13                /* Power down control register              */
#define  DM9161_SCR                         0x14                /* Specified configuration                  */
#define  DM9161_MDINTR                      0x15                /* DAVICOM Specified interrupt register     */
#define  DM9161_RECR                        0x16                /* DAVICOM specified RX error counter reg.  */
#define  DM9161_DISCR                       0x17                /* DAVICOM specified disconnect cntr. reg.  */
#define  DM9161_RLSR                        0x18                /* DAVICOM hardware reset latch state reg.  */

/*
*********************************************************************************************************
*                                         DM9161AE REGISTER BITS
*********************************************************************************************************
*/
/* -------- PHY_REG_BMCR Register Bits -------- */
#define  BMCR_RESV                         0x007F               /* Unused...                                */
#define  BMCR_CTST                       DEF_BIT_07             /* Collision test                           */
#define  BMCR_FULLDPLX                   DEF_BIT_08             /* Full duplex                              */
#define  BMCR_ANRESTART                  DEF_BIT_09             /* Auto negotiation restart                 */
#define  BMCR_ISOLATE                    DEF_BIT_10             /* Disconnect DM9161 from MII              */
#define  BMCR_PDOWN                      DEF_BIT_11             /* Powerdown the DM9161                    */
#define  BMCR_ANENABLE                   DEF_BIT_12             /* Enable auto negotiation                  */
#define  BMCR_SPEED100                   DEF_BIT_13             /* Select 100Mbps                           */
#define  BMCR_LOOPBACK                   DEF_BIT_14             /* TXD loopback bits                        */
#define  BMCR_RESET                      DEF_BIT_15             /* Reset the DM9161                        */

/* -------- PHY_REG_BMSR Register Bits -------- */
#define  BMSR_ERCAP                      DEF_BIT_00             /* Ext-reg capability                       */
#define  BMSR_JCD                        DEF_BIT_01             /* Jabber detected                          */
#define  BMSR_LSTATUS                    DEF_BIT_02             /* Link status                              */
#define  BMSR_ANEGCAPABLE                DEF_BIT_03             /* Able to do auto-negotiation              */
#define  BMSR_RFAULT                     DEF_BIT_04             /* Remote fault detected                    */
#define  BMSR_ANEGCOMPLETE               DEF_BIT_05             /* Auto-negotiation complete                */
#define  BMSR_RESV                         0x07C0               /* Unused...                                */
#define  BMSR_10HALF                     DEF_BIT_11             /* Can do 10mbps, half-duplex               */
#define  BMSR_10FULL                     DEF_BIT_12             /* Can do 10mbps, full-duplex               */
#define  BMSR_100HALF                    DEF_BIT_13             /* Can do 100mbps, half-duplex              */
#define  BMSR_100FULL                    DEF_BIT_14             /* Can do 100mbps, full-duplex              */
#define  BMSR_100BASE4                   DEF_BIT_15             /* Can do 100mbps, 4k packets               */

/* -------- PHY_REG_ANAR Register Bits -------- */
#define  ANAR_SLCT                         0x001F               /* Selector bits                            */
#define  ANAR_CSMA                       DEF_BIT_04             /* Only selector supported                  */
#define  ANAR_10HALF                     DEF_BIT_05             /* Try for 10mbps half-duplex               */
#define  ANAR_10FULL                     DEF_BIT_06             /* Try for 10mbps full-duplex               */
#define  ANAR_100HALF                    DEF_BIT_07             /* Try for 100mbps half-duplex              */
#define  ANAR_100FULL                    DEF_BIT_08             /* Try for 100mbps full-duplex              */
#define  ANAR_100BASE4                   DEF_BIT_09             /* Try for 100mbps 4k packets               */
#define  ANAR_RESV                         0x1C00               /* Unused...                                */
#define  ANAR_RFAULT                     DEF_BIT_13             /* Say we can detect faults                 */
#define  ANAR_LPACK                      DEF_BIT_14             /* Ack link partners response               */
#define  ANAR_NPAGE                      DEF_BIT_15             /* Next page bit                            */

#define  ANAR_FULL       (ANAR_100FULL | ANAR_10FULL | ANAR_CSMA)
#define  ANAR_ALL        (ANAR_100FULL | ANAR_10FULL | ANAR_100HALF | ANAR_10HALF)

/* ------- PHY_REG_ANLPAR Register Bits ------- */
#define  ANLPAR_SLCT                       0x001F               /* Same as advertise selector               */
#define  ANLPAR_10HALF                   DEF_BIT_05             /* Can do 10mbps half-duplex                */
#define  ANLPAR_10FULL                   DEF_BIT_06             /* Can do 10mbps full-duplex                */
#define  ANLPAR_100HALF                  DEF_BIT_07             /* Can do 100mbps half-duplex               */
#define  ANLPAR_100FULL                  DEF_BIT_08             /* Can do 100mbps full-duplex               */
#define  ANLPAR_100BASE4                 DEF_BIT_09             /* Can do 100mbps 4k packets                */
#define  ANLPAR_RESV                       0x1C00               /* Unused...                                */
#define  ANLPAR_RFAULT                   DEF_BIT_13             /* Link partner faulted                     */
#define  ANLPAR_LPACK                    DEF_BIT_14             /* Link partner acked us                    */
#define  ANLPAR_NPAGE                    DEF_BIT_15             /* Next page bit                            */

#define  ANLPAR_DUPLEX   (ANLPAR_10FULL  | ANLPAR_100FULL)
#define  ANLPAR_100	     (ANLPAR_100FULL | ANLPAR_100HALF | ANLPAR_100BASE4)

/* -------- PHY_REG_ANER Register Bits -------- */
#define  ANER_NWAY                       DEF_BIT_00             /* Can do N-way auto-nego                   */
#define  ANER_LCWP                       DEF_BIT_01             /* Got new RX page code word                */
#define  ANER_ENABLENPAGE                DEF_BIT_02             /* This enables npage words                 */
#define  ANER_NPCAPABLE                  DEF_BIT_03             /* Link partner supports npage              */
#define  ANER_MFAULTS                    DEF_BIT_04             /* Multiple faults detected                 */
#define  ANER_RESV                         0xFFE0               /* Unused...                                */

/* ------ DM9161_MDINTR Register Bits ----- */
#define  MDINTR_INTR_PEND                DEF_BIT_15             /* Interrupt pending                        */
#define  MDINTER_FDX_MSK                 DEF_BIT_11             /* Full-duplex interrupt mask               */
#define  MDINTER_SPD_MSK                 DEF_BIT_10             /* Speed       interrupt mask               */
#define  MDINTER_LINK_MSK                DEF_BIT_09             /* Link        interrupt mask               */
#define  MDINTER_INTR_MSK                DEF_BIT_08             /* Master      interrupt mask               */
#define  MDINTER_FDX_CHG                 DEF_BIT_04             /* Full-duplex change                       */
#define  MDINTER_SPD_CHG                 DEF_BIT_03             /* Speed       change                       */
#define  MDINTER_LINK_CHG                DEF_BIT_02             /* Link        change                       */
#define  MDINTER_INTR_STAT               DEF_BIT_00             /* Interrupt status                         */

/*
*********************************************************************************************************
*                                   PHY ERROR CODES 12,000 -> 13,000
*********************************************************************************************************
*/
#define  NET_PHY_ERR_NONE                  12000
#define  NET_PHY_ERR_REGRD_TIMEOUT         12010
#define  NET_PHY_ERR_REGWR_TIMEOUT         12020
#define  NET_PHY_ERR_AUTONEG_TIMEOUT       12030
#define  NET_PHY_ERR_RESET_TIMEOUT         12040

