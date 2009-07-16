/****************************************Copyright (c)**************************************************

**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
********************************************************************************************************/

#ifndef __MODBUS_H
#define __MODBUS_H

#define  MAX_MB_ADDR   247 
#define	MAX_PDU_DATA_LENGTH		 253
#define	MAX_PDU_LENGTH		 	(MAX_PDU_DATA_LENGTH+1)
#define MAX_ADU_LENGTH  		(MAX_PDU_LENGTH+2)


//支持的Modbus功能码
typedef enum MBX_FUNC
{
	MB_FUNC_NONE                           = 0,
	MB_FUNC_READ_COILS		               = 1,		//01号，读开出
	MB_FUNC_READ_DISCRETE_INPUTS		   = 2,		//02号，读开入
	MB_FUNC_READ_HOLDING_REGISTER          = 3,		//03号，读寄存器
	MB_FUNC_READ_INPUT_REGISTER            = 4,     // 读输入寄存器
	MB_FUNC_WRITE_SINGLE_COIL              = 5,     // 写单个线圈
	MB_FUNC_WRITE_REGISTER                 = 6,     // 写单个寄存器
	MB_FUNC_DIAG_READ_EXCEPTION         	= 7,	//07号，读自检异常
	MB_FUNC_DIAG_DIAGNOSTIC                 = 8,
 	MB_FUNC_DIAG_GET_COM_EVENT_CNT        = 11 ,
	MB_FUNC_DIAG_GET_COM_EVENT_LOG        = 12 ,
    MB_FUNC_WRITE_MULTIPLE_COILS           =15,     // 写多个线圈
    MB_FUNC_WRITE_MULTIPLE_REGISTERS       =16 ,    // 写多个寄存器
	MB_FUNC_OTHER_REPORT_SLAVEID          =  17 ,
    MB_FUNC_READWRITE_MULTIPLE_REGISTERS   = 23 ,   //读/写多个寄存器

	MB_FUNC_READ_FILE_RECORD               = 20,    //读文件记录
	MB_FUNC_WRITE_FILE_RECORD              = 21,    //写文件记录


	MB_FUNC_MEI                            = 43,    //MEI接口

	MBX_FUNC_DEVICE_CTRL                     = 66,    //MBX扩展功能，设备控制

	MBX_FUNC_READ_FORCEMARK                = 101,  //读强制标志
	MBX_FUNC_WRITE_FORCEMARK               = 102,  //写强制标志


    MB_FUNC_ERROR                         = 128 
}MBX_FUNC;

typedef enum MBX_DEV_CTRL
{
	MBX_CTRL_SET_TIME  = 1,          //对时
	MBX_CTRL_RESET     = 2,          //重启
	MBX_CTRL_CLEAR_FLASH = 3,        //清除铁电
	MBX_CTRL_CLEAR_FRAM  = 4         //清除FRAM
}MBX_DEV_CTRL;

typedef enum MB_MEI_OBJ_ID
{
	MB_MEI_READ_DEV_VER = 0X80,
	MB_MEI_READ_DN_TM = 0X81,
	MB_MEI_READ_VER_TM = 0X82,
	MB_MEI_READ_CFG_VER = 0X83,
	MB_MEI_READ_FLASH_FCB = 0X84,
	MB_MEI_READ_RAM_FCB = 0X85,
	MB_MEI_READ_FLASHRAM_FCB = 0X86
}MB_MEI_OBJ_ID;

typedef enum MBException
{
	MB_EX_NONE = 0x00,
	MB_EX_ILLEGAL_FUNCTION = 0x01,
	MB_EX_ILLEGAL_DATA_ADDRESS = 0x02,
	MB_EX_ILLEGAL_DATA_VALUE = 0x03,
	MB_EX_SLAVE_DEVICE_FAILURE = 0x04,
	MB_EX_ACKNOWLEDGE = 0x05,
	MB_EX_SLAVE_BUSY = 0x06,
	MB_EX_MEMORY_PARITY_ERROR = 0x08,
	MB_EX_GATEWAY_PATH_FAILED = 0x0A,
	MB_EX_GATEWAY_TGT_FAILED = 0x0B
} MBException;

/*! \ingroup modbus
* \brief Errorcodes used by all function in the protocol stack.
*/
typedef enum MBErrorCode
{
	MB_ENOERR,                  /*!< no error. */
	MB_ENOREG,                  /*!< illegal register address. */
	MB_EINVAL,                  /*!< illegal argument. */
	MB_EPORTERR,                /*!< porting layer error. */
	MB_ENORES,                  /*!< insufficient resources. */
	MB_EIO,                     /*!< I/O error. */
	MB_EILLSTATE,               /*!< protocol stack in illegal state. */
	MB_ETIMEDOUT                /*!< timeout error occurred. */
} MBErrorCode;

/*! \ingroup modbus
* \brief If register should be written or read.
*
* This value is passed to the callback functions which support either
* reading or writing register values. Writing means that the application
* registers should be updated and reading means that the modbus protocol
* stack needs to know the current register values.
*
* \see eMBRegHoldingCB( ), eMBRegCoilsCB( ), eMBRegDiscreteCB( ) and 
*   eMBRegInputCB( ).
*/
typedef enum MbxRWMode
{
	MBX_READ,                /*!< Read register values and pass to protocol stack. */
	MBX_WRITE                /*!< Update register values. */
} MbxRWMode;

 
#define RX_OVERTIME	10000			//1000		接受1S超时
#define TX_OVERTIME 10000			//1000      发送1S超时
//通讯超时时间
#define COMM_OVER_TIME			200
#define COMM_OVER_LTIME			600
 

#define	BROADCAST_ADDRESS	0

 
//允许发送中断
//typedef void (*RxEnableFunc)(void);

//允许接受中断
//typedef void (*TxEnableFunc)(void);

//当前是发送还是接受中断 返回TRUE - 发送态
//typedef INT8U (*IsTxEnableFunc)(void);

//发送数据的函数
// INT8U * pData 要发送的数据 
// INT16U nLen 要发送数据的长度
// 返回，实际发送出去的数据长度
//typedef INT16U (*CommSend)(INT8U * pData, INT16U nLen);


typedef struct MBX
{
	INT8U  Address;
	INT8U  FuncCode;			//报文特征码	

	INT8U  SubFuncCode;        //子功能码
	INT8U  UartId;              //串口号

	INT8U Broadcast;           //广播报文  
	INT8U Pad;


	//接受
	INT16U ModbusRxCRC;			//ModbusCRC校验

	INT8U  TempBuf[MAX_ADU_LENGTH];
	
	/**接受**/
	INT8U RxBuff[MAX_ADU_LENGTH];	//接受缓冲区
	INT16U RxLen;					//接受长度(PDU+地址的长度)，不含CRC段
	INT16U RxCount;					//接受计数

	/**发送**/
	INT8U TxBuff[MAX_ADU_LENGTH];	//发送缓冲区
	INT16U TxLen;					//发送长度(PDU+地址的长度)，不含CRC段
	INT16U TxCount;					//发送计数

	INT32U RxTotalLen;             //接收到的报文的总长度
	INT32U RxTotoalCnt;            //接收到报文的帧数量

	INT32U TxTotalLen;
	INT32U TxTotoalCnt;

	INT32U RxCrcErr;      //CRC错误的帧数量

	OS_EVENT *pUartRecvSem;   //有报文

	volatile INT32U RxTimer;					//接受时间计数器
	volatile INT32U TxTimer;					//发送超时计数器
  
	/** 通讯硬件接口函数 **/
//	RxEnableFunc pfnRxEnable;
//	TxEnableFunc pfnTxEnable;
//	IsTxEnableFunc pfnIsTxEnable;
//	CommSend pfnCommSend;
}MBX;

extern INT8U InitMBXTsk( void );
  
//在毫秒中断里面调用，主要用来判断超时
extern void CommTimerHandle( void  );

//设置装置地址
void SetDeviceAddr( INT8U Addr );

//中断里面调用
// void RcvOneByteInDbg(INT8U c);
// void RcvOneByteInComm(INT8U c);

#endif

