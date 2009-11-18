 /****************************************Copyright (c)**************************************************
**                                         
**                                      
**                                      
**
**                           
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名:  
**创   建   人:  
**最后修改日期: 
**描        述:   
**   File: $Id: ,v 1.0 2009/11/15 15:27:16   $
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人:  
** 版  本:  
** 日　期:  
** 描　述: 
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 描　述:  
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"
 

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbproto.h"

/* ----------------------- Defines ------------------------------------------*/
#define BITS_UCHAR      8U

/* ----------------------- Start implementation -----------------------------*/
void MBUtilSetBits( UCHAR * ucByteBuf, USHORT usBitOffset, UCHAR ucNBits,UCHAR ucValue )
{
    USHORT          usWordBuf;
    USHORT          usMask;
    USHORT          usByteOffset;
    USHORT          usNPreBits;
    USHORT          usValue = ucValue;

    assert_param( ucNBits <= 8 );
    assert_param( ( size_t )BITS_UCHAR == sizeof( UCHAR ) * 8 );

    /* Calculate byte offset for first byte containing the bit values starting
     * at usBitOffset. */
    usByteOffset = ( USHORT )( ( usBitOffset ) / BITS_UCHAR );

    /* How many bits precede our bits to set. */
    usNPreBits = ( USHORT )( usBitOffset - usByteOffset * BITS_UCHAR );

    /* Move bit field into position over bits to set */
    usValue <<= usNPreBits;

    /* Prepare a mask for setting the new bits. */
    usMask = ( USHORT )( ( 1 << ( USHORT ) ucNBits ) - 1 );
    usMask <<= usBitOffset - usByteOffset * BITS_UCHAR;

    /* copy bits into temporary storage. */
    usWordBuf = ucByteBuf[usByteOffset];
    usWordBuf |= ucByteBuf[usByteOffset + 1] << BITS_UCHAR;

    /* Zero out bit field bits and then or value bits into them. */
    usWordBuf = ( USHORT )( ( usWordBuf & ( ~usMask ) ) | usValue );

    /* move bits back into storage */
    ucByteBuf[usByteOffset] = ( UCHAR )( usWordBuf & 0xFF );
    ucByteBuf[usByteOffset + 1] = ( UCHAR )( usWordBuf >> BITS_UCHAR );
}

UCHAR MBUtilGetBits( UCHAR * ucByteBuf, USHORT usBitOffset, UCHAR ucNBits )
{
    USHORT          usWordBuf;
    USHORT          usMask;
    USHORT          usByteOffset;
    USHORT          usNPreBits;

    /* Calculate byte offset for first byte containing the bit values starting
     * at usBitOffset. */
    usByteOffset = ( USHORT )( ( usBitOffset ) / BITS_UCHAR );

    /* How many bits precede our bits to set. */
    usNPreBits = ( USHORT )( usBitOffset - usByteOffset * BITS_UCHAR );

    /* Prepare a mask for setting the new bits. */
    usMask = ( USHORT )( ( 1 << ( USHORT ) ucNBits ) - 1 );

    /* copy bits into temporary storage. */
    usWordBuf = ucByteBuf[usByteOffset];
    usWordBuf |= ucByteBuf[usByteOffset + 1] << BITS_UCHAR;

    /* throw away unneeded bits. */
    usWordBuf >>= usNPreBits;

    /* mask away bits above the requested bitfield. */
    usWordBuf &= usMask;

    return ( UCHAR ) usWordBuf;
}

MBException MBError2Exception( MBErrorCode eErrorCode )
{
    MBException    eStatus;

    switch ( eErrorCode )
    {
        case MB_ENOERR:
            eStatus = MB_EX_NONE;
            break;

        case MB_ENOREG:
            eStatus = MB_EX_ILLEGAL_DATA_ADDRESS;
            break;

        case MB_ETIMEDOUT:
            eStatus = MB_EX_SLAVE_BUSY;
            break;

        default:
            eStatus = MB_EX_SLAVE_DEVICE_FAILURE;
            break;
    }

    return eStatus;
}
