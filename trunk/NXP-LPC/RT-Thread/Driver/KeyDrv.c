/****************************************Copyright (c)**************************************************
**                                   
**                             键盘扫描模块         
**                             [          ]         
**
**                           
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名:  
**创   建   人:  
**最后修改日期: 
**描        述:  
**
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
#include "applib.h"
#include "KeyDrv.h" 
#include <rtthread.h>
#include <rthw.h>
/*
*********************************************************************************************************
*                                            LOCAL CONSTANTS
*********************************************************************************************************
*/

#define KEY_STATE_UP                 1      /* Key scanning states used in KeyScan()                   */
#define KEY_STATE_DEBOUNCE           2
#define KEY_STATE_RPT_START_DLY      3
#define KEY_STATE_RPT_DLY            4

#define  KEY_BUF_SIZE               0X20      /* Size of the KEYBOARD buffer                             */

#define  KEY_SCAN_TASK_DLY          50      /* Number of milliseconds between keyboard scans           */
#define  KEY_RPT_START_DLY          10      /* Number of scan times before auto repeat function engages*/
#define  KEY_RPT_DLY                 2      /* Number of scan times before auto repeat executes again  */

static  INT8U     KeyBuf[KEY_BUF_SIZE];     /* Keyboard buffer                                         */
static  INT8U     KeyBufInIx;               /* Index into key buf where next scan code will be inserted*/
static  INT8U     KeyBufOutIx;              /* Index into key buf where next scan code will be removed */
static  INT16U    KeyDownTmr;               /* Counts how long key has been pressed                    */
static  INT8U     KeyNRead;                 /* Number of keys read from the keyboard                   */

static  INT8U     KeyRptStartDlyCtr;        /* Number of scan times before auto repeat is started      */
static  INT8U     KeyRptDlyCtr;             /* Number of scan times before auto repeat executes again  */

static  INT8U     KeyScanState;             /* Current state of key scanning function                  */

#define  KEY_SCAN_TASK_STK_SIZE  200
#define  PRIO_KEY_SCAN_TASK   60

static  char  KeyScanTaskStk[KEY_SCAN_TASK_STK_SIZE];  /* Keyboard scanning task stack             */

static struct rt_thread key_scan_thread;

//static  OS_EVENT *KeySemPtr;                               /* Pointer to keyboard semaphore            */


/******************* 按键地址 ************************************************/ 

/*按键部分 CS2  */
#define   KEY1_8 (*((volatile unsigned short *) 0x82000004))
#define   KEY9 (*((volatile unsigned short *) 0x82000008))


/*********************************************************************************************************
** 函数名称: GetKeyHdVal
** 函数名称: GetKeyHdVal
**
** 功能描述：  读取键盘硬件的值
**
** 输　入:  void
**          
** 输　出:   INT16U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月3日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static INT16U GetKeyHdVal(void)
{
	INT16U CurrentKey,temp;
	INT16U KeyValue = 0;
	CurrentKey = KEY1_8;
	CurrentKey &= 0xff;
	temp = KEY9;
	CurrentKey |= (temp&0x80)<<8;       

	switch(CurrentKey)
	{
	case 0x80fe: KeyValue = 0x1;    break;
	case 0x80fd: KeyValue = 0x2;    break;
	case 0x80fb: KeyValue = 0x3;    break;
	case 0x80f7: KeyValue = 0x4;    break;
	case 0x80ef: KeyValue = 0x5;    break;
	case 0x80df: KeyValue = 0x6;    break;
	case 0x80bf: KeyValue = 0x7;    break;
	case 0x807f: KeyValue = 0x8;    break;
	case 0x00ff: KeyValue = 0x9;    break;
	default:  break;
	}
	return KeyValue;
}

/*********************************************************************************************************
** 函数名称: AddKey
** 函数名称: AddKey
**
** 功能描述： 插入一个键盘值到缓冲中
**
** 输　入:  INT8U code
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月23日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static  void  AddKey (INT8U code)
{
	OS_CPU_SR  cpu_sr;

	OS_ENTER_CRITICAL();                      
	if (KeyNRead < KEY_BUF_SIZE)
	{               /* Make sure that we don't overflow the buffer        */
		KeyNRead++;                              /* Increment the number of keys read                  */
		KeyBuf[KeyBufInIx++] = code;             /* Store the scan code into the buffer                */
		if (KeyBufInIx >= KEY_BUF_SIZE)
		{        /* Adjust index to the next scan code to put in buffer*/
			KeyBufInIx = 0;
		}
		OS_EXIT_CRITICAL();                       
//		OSSemPost(KeySemPtr);                    /* Signal sem if scan code inserted in the buffer     */
	}
	else
	{// 数据区是否已经满了，如果满了，则抛弃该按键        
		OS_EXIT_CRITICAL();                  
	}
}


/*
*********************************************************************************************************
*                                        SEE IF ANY KEY IN BUFFER
*
* Description : This function checks to see if a key was pressed
* Arguments   : none
* Returns     : TRUE   if a key has been pressed
*               FALSE  if no key pressed
*********************************************************************************************************
*/

/*********************************************************************************************************
** 函数名称: KeyHit
** 函数名称: KeyHit
**
** 功能描述：由外部调用检查缓冲中是否有键盘按下 
**
** 输　入:  void
**          
** 输　出:  BOOLEAN
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月23日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
BOOLEAN  KeyHit (void)
{
	BOOLEAN hit;
	hit = (BOOLEAN)(KeyNRead > 0) ? TRUE : FALSE;
	return (hit);
}

/*********************************************************************************************************
** 函数名称: GetKey
** 函数名称: GetKey
**
** 功能描述： 获取键盘的按键
**
** 输　入:  INT16U to  键盘的输入等待时间(时间单位：ticks) ，0表示一直等待
**          
** 输　出:  INT8U    != 0xFF  键盘值有效，返回值即键盘值 == 0xFF 在规定的时间内，没有键盘被按下
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月23日
** 备  注: 外部调用
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U  GetKey (INT16U to)
{
	OS_CPU_SR  cpu_sr;

	INT8U code;
	INT8U err;


//	OSSemPend(KeySemPtr, to, &err);              /* Wait for a key to be pressed                       */
	OS_ENTER_CRITICAL();                        
	if (KeyNRead > 0)
	{                          /* See if we have keys in the buffer                  */
		KeyNRead--;                              /* Decrement the number of keys read                  */
		code = KeyBuf[KeyBufOutIx];              /* Get scan code from the buffer                      */
		KeyBufOutIx++;
		if (KeyBufOutIx >= KEY_BUF_SIZE) 
		{       /* Adjust index into the keyboard buffer              */
			KeyBufOutIx = 0;
		}
		OS_EXIT_CRITICAL();                  
		return (code);                           /* Return the scan code of the key pressed            */
	}
	else
	{
		OS_EXIT_CRITICAL();                     
		return (0xFF);                           /* No scan codes in the buffer, return -1             */
	}
} 
/*********************************************************************************************************
** 函数名称: FlushKeyBuffer
** 函数名称: FlushKeyBuffer
**
** 功能描述： 清空
**
** 输　入:  void
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月23日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void  FlushKeyBuffer (void)
{
	while (KeyHit())
	{                           /* While there are keys in the buffer...              */
		GetKey(0);              /* ... extract the next key from the buffer           */
	}
}
/*********************************************************************************************************
** 函数名称: GetKeyDownTime
** 函数名称: GetKeyDownTime
**
** 功能描述： 键盘被按下去多长时间
**
** 输　入:  void
**          
** 输　出:  INT32U  键盘被按下去多长时间（单位：毫秒）
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月23日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT32U GetKeyDownTime (void)
{                            
	OS_CPU_SR  cpu_sr;
 
	INT16U tmr;

	OS_ENTER_CRITICAL();
	tmr = KeyDownTmr;
	OS_EXIT_CRITICAL();
	return (tmr * KEY_SCAN_TASK_DLY);
}


static  BOOLEAN   IsKeyDown (void)
{                              
	OS_CPU_SR  cpu_sr;
 
	INT16U key_now; 
	key_now = GetKeyHdVal();
	if (KEY_NONE != key_now)
	{         /* Key not pressed if 0                     */
		OS_ENTER_CRITICAL();
		KeyDownTmr++;                                      /* Update key down counter                  */
		OS_EXIT_CRITICAL();
		return (TRUE);
	} 
	else
	{
		return (FALSE);
	}
}
/*********************************************************************************************************
** 函数名称: ScanKeyTask
** 函数名称: ScanKeyTask
**
** 功能描述： 键盘扫描任务
**
** 输　入:  void * data
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月23日
** 备  注: 这里有问题，
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static  void  ScanKeyTask (void *data)
{
	INT16U code;

	data = data;                                         
	for (;;)
	{
		rt_thread_delay_hmsm(0, 0, 0, KEY_SCAN_TASK_DLY);         /* Delay between keyboard scans             */
		switch (KeyScanState)
		{
		case KEY_STATE_UP:                             /* See if need to look for a key pressed    */
			if (IsKeyDown())
			{                     /* See if key is pressed                    */
				KeyScanState = KEY_STATE_DEBOUNCE;    /* Next call we will have debounced the key */
				KeyDownTmr   = 0;                     /* Reset key down timer                     */
			}
			break;

		case KEY_STATE_DEBOUNCE:                       /* Key pressed, get scan code and buffer    */
			if (IsKeyDown())
			{                     /* See if key is pressed                    */
				code              = GetKeyHdVal();      /* Determine the key scan code              */
				AddKey(code);                       /* Input scan code in buffer                */
				KeyRptStartDlyCtr = KEY_RPT_START_DLY;/* Start delay to auto-repeat function      */
				KeyScanState      = KEY_STATE_RPT_START_DLY;
			} 
			else
			{
				//			KeySelRow(KEY_ALL_ROWS);              /* Select all row                           */
				KeyScanState      = KEY_STATE_UP;     /* Key was not pressed after all!           */
			}
			break;

		case KEY_STATE_RPT_START_DLY:
			if (IsKeyDown())
			{                     /* See if key is still pressed              */
				if (KeyRptStartDlyCtr > 0)
				{          /* See if we need to delay before auto rpt  */
					KeyRptStartDlyCtr--;              /* Yes, decrement counter to start of rpt   */
					if (KeyRptStartDlyCtr == 0)
					{     /* If delay to auto repeat is completed ... */
						code         = GetKeyHdVal();   /* Determine the key scan code              */
						AddKey(code);               /* Input scan code in buffer                */
						KeyRptDlyCtr = KEY_RPT_DLY;   /* Load delay before next repeat            */
						KeyScanState = KEY_STATE_RPT_DLY;
					}
				}
			} 
			else
			{
				KeyScanState = KEY_STATE_DEBOUNCE;    /* Key was not pressed after all            */
			}
			break;

		case KEY_STATE_RPT_DLY:
			if (IsKeyDown())
			{                     /* See if key is still pressed              */
				if (KeyRptDlyCtr > 0)
				{               /* See if we need to wait before repeat key */
					KeyRptDlyCtr--;                   /* Yes, dec. wait time to next key repeat   */
					if (KeyRptDlyCtr == 0)
					{          /* See if it's time to repeat key           */
						code         = GetKeyHdVal();   /* Determine the key scan code              */
						AddKey(code);               /* Input scan code in buffer                */
						KeyRptDlyCtr = KEY_RPT_DLY;   /* Reload delay counter before auto repeat  */
					}
				}
			} 
			else
			{
				KeyScanState = KEY_STATE_DEBOUNCE;    /* Key was not pressed after all            */
			}
			break;
		}
	}
}

extern void  InitKeyDriver (void)
{
	KeyScanState = KEY_STATE_UP;                 /* Keyboard should not have a key pressed             */
	KeyNRead     = 0;                            /* Clear the number of keys read                      */
	KeyDownTmr   = 0;
	KeyBufInIx   = 0;                            /* Key codes inserted at  the beginning of the buffer */
	KeyBufOutIx  = 0;                            /* Key codes removed from the beginning of the buffer */
// 	KeySemPtr    = OSSemCreate(0);               /* Initialize the keyboard semaphore                  */
// 
// 	OSTaskCreate(ScanKeyTask , (void *)0, &KeyScanTaskStk[KEY_SCAN_TASK_STK_SIZE-1], PRIO_KEY_SCAN_TASK);
}
