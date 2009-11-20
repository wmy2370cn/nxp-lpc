/****************************************Copyright (c)**************************************************
**                                文件解析模块
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
#pragma  once

#ifndef _CFG_PARSER_H
#define	_CFG_PARSER_H


#define  MAX_GATAGE_CNT 64  //64段参数
#define  FCB_FLAG   0x4C49


 
#pragma pack(push)
#pragma pack(4)

typedef enum ENCODER_TYPE
{
	INTERFACE_NONE			=0,	//没有配置
	INTERFACE_PARALLEL		=1,	//绝对型并行接口
	INTERFACE_SSI			=2,	//绝对型SSI接口
	INTERFACE_INCREMENTAL	=3	//增量型
	//	INTERFACE_AI			=4	//模入
}ENCODER_TYPE;


typedef enum GATAGE_UINT
{
	UNIT_MM = 0,   //毫米
	UNIT_CM = 1,    //厘米
	UNIT_DEGREE = 2  //度
}GATAGE_UINT;

 
typedef enum GATAGE_CALC
{
	CALC_BY_LINE = 0 ,  //线性计算
	CALC_BY_TAB  = 1  //开度转换表
}GATAGE_CALC;

//旋转方向
typedef enum ENCODER_DIRECTION
{
	DIRE_CLOCKWISE =  0,      //顺时针方向
	DIRE_ANTI_CLOCKWISE =  1  //逆时针方向
}ENCODER_DIRECTION;

//绝对型编码器的编码类型
typedef enum CODE_TYPE
{
	CODE_GRAY		=0,	//格雷码
	CODE_BCD	    =1,	//BCD码
	CODE_BINARY		=2	//二进制码
}CODE_TYPE;

typedef enum STOP_BIT
{
	ONE_STOPBIT = 0,
	ONEPOINTFILE_STOPBIT = 1,
	TWO_STOPBIT = 2
}STOP_BIT;

typedef enum PARITY_BIT
{
	NONE_PARITY = 0,  //无校验
	ODD_PARITY = 1,  //奇校验
	EVEN_PARITY = 2  //偶校验  
}PARITY_BIT;

typedef __packed struct DEVICE_TIME
{
	INT16U  wYear;
	INT8U	ucMonth;
	INT8U	ucDay;
	INT8U   ucHour;
	INT8U   ucMinute;
	INT16U  wSecond;
	INT16U wMilliseconds;
	INT16U wPad;
}DEVICE_TIME;


//配置文件控制块
typedef  struct  FCB 
{
	INT16U  wHeadFlag; //文件头标志 0x4c 0x49

	//版本号，装置键盘修改数据时，自动增加wBuild，下载时，下载时由配置源文件生成，
	//上传时，将此版本号覆盖到配置文件的版本号中
	INT16U	wMajorVer; //主版本号  如：1.00 :wMajorVer = 1 wMinorVer= 0，2.01 = wMajorVer = 2 wMinorVer= 1 主版本号和应用程序有关 
	INT16U  wMinorVer; //从版本号，
	INT16U  wBuild;    //由文件保存时产生，每保存一次，版本号增加1	

	INT16U  wBinBuild;    //如果键盘修改配置，每修改一次，此版本号追加一次 	
	INT16U  wPad;

	//文件下载的时间，在下载时生成
	DEVICE_TIME            DownLoadTm;
	//原始文件最后保存时间
	DEVICE_TIME            SaveTm;		

	//源文件的长度  源文件是可选的
	INT32U    nCfgFileLen;	
	//编译后文件长度  BIN文件是必须的
	INT32U    nBinLen;     
	//源文件的crc校验
	INT16U  wConfCrc;

	INT16U  wBinCrc;

	//文件信息
	INT32U   nCtrlBit;       // bit 0  
	// bit 1	
	// bit 2

}FCB;

 

typedef struct  tagMB_PORT
{
	INT32U Baud;

	INT8U  StopBit;
	INT8U  DataBit;
	INT8U  ParityBit;

	INT8U  Addr;
} MB_PORT ;//液晶可修改


typedef struct  tagCPU
{
	MB_PORT   Port;
	INT32U    MWCnt;
	INT32U    MDCnt;
}CPU;

typedef struct HD_CFG 
{
	CPU CpuCfg;

}SEC_HD_CFG;

 //分段参数
typedef struct GATAGE_PARA
{
	INT32U EncoderVal;
	FP64 Gatage;
}GATAGE_PARA;

typedef struct GATAGE_CFG
{
	FP64  MaxGatage;    //最大闸门开度
	FP64  MinGatage;    //最小闸门开度

	INT32U  MaxEncoderVal;    //最大闸门开度对应的编码器值
	INT32U  MinEncoderVal;    //最小闸门开度

	INT8U   UnitType;      //单位
	INT8U	CalcMethod;    //计算方式
	INT8U   Direction;     //旋转方向
	INT8U   EncodeType;    //编码类型，只对绝对编码器有效

	INT32U    ParaCnt;  //分段参数个数
	GATAGE_PARA  pGatageTab[MAX_GATAGE_CNT];
} SEC_GATAGE_CFG;

typedef struct  BIN_DATA
{
	SEC_HD_CFG HdCfg;
	SEC_GATAGE_CFG  GatageCfg;
}BIN_DATA;


typedef struct UE_VARIANT
{
	INT16U Type;
	INT16U Reserved1;
	INT16U Reserved2;
	INT16U Reserved3;
	union
	{
		FP64   dValue;
		FP32   fValue;
		INT32U nValue;
		INT16U wValue;
		INT8U  bValue;

	} UNION_VALUE;

}UE_VARIANT;

#define V_UNION(X, Y)   ((X).UNION_VALUE.Y)
#define V_BOOL(X)         V_UNION(X, bValue) 
#define V_WORD(X)         V_UNION(X, wValue) 
#define V_UINT(X)         V_UNION(X, nValue) 



#pragma pack(pop)

//从flash加载配置信息
INT32U LoadBinData(INT8U *pBuf,BIN_DATA *pBinData);
//在加载flash上面的配置信息失败时，调用此接口完成默认的配置信息
void InitDefaultBinData(BIN_DATA *pBinData);



  
#endif
