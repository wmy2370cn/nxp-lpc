///////////////////////////////////////////////////////////////
//	文 件 名 : Resource.c
//	文件功能 : 资源文件
//	作    者 : 仲伟
//	创建时间 : 2008-03-06 14:50:04
//	项目名称 : 
//	操作系统 : 
//	备    注 :
//	历史记录 : 
///////////////////////////////////////////////////////////////
#include "applib.h"
#include "StringTable.h"
//字符串资源

#define CN_WORD_STOCK

#ifdef CN_WORD_STOCK
#define _PER_SIZE sizeof(INT16U)
#else
#define _PER_SIZE sizeof(char)
#endif
 
//------ ID_STRING_NONE --------
//无
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_NONE[]={10062};
#else
const char str_cn_ID_STRING_NONE[]="无";
#endif
//None
const char str_en_ID_STRING_NONE[]="None";

//------ ID_STRING_LOGO --------
//UITC2000闸门测控装置
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_LOGO[]={85,73,84,67,50,48,48,48,10002,10036,10064,10057,10017,10034};
//const INT16U str_cn_ID_STRING_LOGO[]={85,73,84,67,50,10002,10036,10064,10057,10017,10034};
#else
const char str_cn_ID_STRING_LOGO[]="UITC2000闸门测控装置";
#endif
// 
const char str_en_ID_STRING_LOGO[]=" ";

//------ ID_STRING_MAIN_MENU --------
//主菜单
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_MAIN_MENU[]={10080,10041,10007};
#else
const char str_cn_ID_STRING_MAIN_MENU[]="主菜单";
#endif
// 
const char str_en_ID_STRING_MAIN_MENU[]=" ";

//------ ID_STRING_VALUE_TAB --------
//实时测值
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_VALUE_TAB[]={10024,10019,10064,10025};
#else
const char str_cn_ID_STRING_VALUE_TAB[]="实时测值";
#endif
// 
const char str_en_ID_STRING_VALUE_TAB[]=" ";

//------ ID_STRING_GATE_CTRL --------
//闸门操作
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_GATE_CTRL[]={10002,10036,10059,10081};
#else
const char str_cn_ID_STRING_GATE_CTRL[]="闸门操作";
#endif
// 
const char str_en_ID_STRING_GATE_CTRL[]=" ";

//------ ID_STRING_DEVICE_CFG --------
//装置配置
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_DEVICE_CFG[]={10017,10034,10066,10034};
#else
const char str_cn_ID_STRING_DEVICE_CFG[]="装置配置";
#endif
// 
const char str_en_ID_STRING_DEVICE_CFG[]=" ";

//------ ID_STRING_RUN_STAT --------
//运行统计
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_RUN_STAT[]={10042,10048,10021,10038};
#else
const char str_cn_ID_STRING_RUN_STAT[]="运行统计";
#endif
// 
const char str_en_ID_STRING_RUN_STAT[]=" ";

//------ ID_STRING_DEVICE_ADDR --------
//装置地址
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_DEVICE_ADDR[]={10017,10034,10056,10026};
#else
const char str_cn_ID_STRING_DEVICE_ADDR[]="装置地址";
#endif
// 
const char str_en_ID_STRING_DEVICE_ADDR[]=" ";

//------ ID_STRING_TIME --------
//时间
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_TIME[]={10019,10065};
#else
const char str_cn_ID_STRING_TIME[]="时间";
#endif
// 
const char str_en_ID_STRING_TIME[]=" ";

//------ ID_STRING_GATAGE --------
//闸门开度
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_GATAGE[]={10002,10036,10012,10039};
#else
const char str_cn_ID_STRING_GATAGE[]="闸门开度";
#endif
// 
const char str_en_ID_STRING_GATAGE[]=" ";

//------ ID_STRING_M --------
//米
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_M[]={10055};
#else
const char str_cn_ID_STRING_M[]="米";
#endif
// 
const char str_en_ID_STRING_M[]=" ";

//------ ID_STRING_ENCODER_VAL --------
//编码器输入
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_ENCODER_VAL[]={10063,10073,10079,10067,10074};
#else
const char str_cn_ID_STRING_ENCODER_VAL[]="编码器输入";
#endif
// 
const char str_en_ID_STRING_ENCODER_VAL[]=" ";

//------ ID_STRING_UP_LEVEL --------
//上游水位
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_UP_LEVEL[]={10047,10046,10014,10028};
#else
const char str_cn_ID_STRING_UP_LEVEL[]="上游水位";
#endif
// 
const char str_en_ID_STRING_UP_LEVEL[]=" ";

//------ ID_STRING_DN_LEVEL --------
//下游水位
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_DN_LEVEL[]={10033,10046,10014,10028};
#else
const char str_cn_ID_STRING_DN_LEVEL[]="下游水位";
#endif
// 
const char str_en_ID_STRING_DN_LEVEL[]=" ";

//------ ID_STRING_CUR_FLUX --------
//当前过闸流量
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_CUR_FLUX[]={10018,10016,10083,10002,10078,10031};
#else
const char str_cn_ID_STRING_CUR_FLUX[]="当前过闸流量";
#endif
// 
const char str_en_ID_STRING_CUR_FLUX[]=" ";

//------ ID_STRING_STATE --------
//状态
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_STATE[]={10022,10013};
#else
const char str_cn_ID_STRING_STATE[]="状态";
#endif
//
const char str_en_ID_STRING_STATE[]="";

//------ ID_STRING_GATE_STATE --------
//闸门状态
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_GATE_STATE[]={10002,10036,10022,10013};
#else
const char str_cn_ID_STRING_GATE_STATE[]="闸门状态";
#endif
// 
const char str_en_ID_STRING_GATE_STATE[]=" ";

//------ ID_STRING_RAISE --------
//上升
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_RAISE[]={10047,10084};
#else
const char str_cn_ID_STRING_RAISE[]="上升";
#endif
// 
const char str_en_ID_STRING_RAISE[]=" ";

//------ ID_STRING_DROP --------
//下降
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_DROP[]={10033,10023};
#else
const char str_cn_ID_STRING_DROP[]="下降";
#endif
// 
const char str_en_ID_STRING_DROP[]=" ";

//------ ID_STRING_STOP --------
//停止
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_STOP[]={10004,10027};
#else
const char str_cn_ID_STRING_STOP[]="停止";
#endif
// 
const char str_en_ID_STRING_STOP[]=" ";

//------ ID_STRING_DEST_GATAGE --------
//设定目标开度
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_DEST_GATAGE[]={10070,10010,10032,10072,10012,10039};
#else
const char str_cn_ID_STRING_DEST_GATAGE[]="设定目标开度";
#endif
// 
const char str_en_ID_STRING_DEST_GATAGE[]=" ";

//------ ID_STRING_START --------
//启动
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_START[]={10076,10015};
#else
const char str_cn_ID_STRING_START[]="启动";
#endif
// 
const char str_en_ID_STRING_START[]=" ";

//------ ID_STRING_RAW_OP --------
//直接操作
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_RAW_OP[]={10020,10049,10059,10081};
#else
const char str_cn_ID_STRING_RAW_OP[]="直接操作";
#endif
// 
const char str_en_ID_STRING_RAW_OP[]=" ";

//------ ID_STRING_CUR_GATE_STATE --------
//当前闸门状态
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_CUR_GATE_STATE[]={10018,10016,10002,10036,10022,10013};
#else
const char str_cn_ID_STRING_CUR_GATE_STATE[]="当前闸门状态";
#endif
// 
const char str_en_ID_STRING_CUR_GATE_STATE[]=" ";

//------ ID_STRING_COMM_CFG --------
//通信参数配置
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_COMM_CFG[]={10011,10037,10045,10085,10066,10034};
#else
const char str_cn_ID_STRING_COMM_CFG[]="通信参数配置";
#endif
// 
const char str_en_ID_STRING_COMM_CFG[]=" ";

//------ ID_STRING_COMM_STATE --------
//通信状态信息
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_COMM_STATE[]={10011,10037,10022,10013,10037,10001};
#else
const char str_cn_ID_STRING_COMM_STATE[]="通信状态信息";
#endif
// 
const char str_en_ID_STRING_COMM_STATE[]=" ";

//------ ID_STRING_VER_INFO --------
//版本信息
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_VER_INFO[]={10068,10030,10037,10001};
#else
const char str_cn_ID_STRING_VER_INFO[]="版本信息";
#endif
// 
const char str_en_ID_STRING_VER_INFO[]=" ";

//------ ID_STRING_DIAG --------
//装置自检调试
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_DIAG[]={10017,10034,10051,10075,10077,10050};
#else
const char str_cn_ID_STRING_DIAG[]="装置自检调试";
#endif
// 
const char str_en_ID_STRING_DIAG[]=" ";

//------ ID_STRING_BAUD --------
//波特率
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_BAUD[]={10009,10058,10040};
#else
const char str_cn_ID_STRING_BAUD[]="波特率";
#endif
// 
const char str_en_ID_STRING_BAUD[]=" ";

//------ ID_STRING_DATA_BIT --------
//数据位
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_DATA_BIT[]={10085,10061,10028};
#else
const char str_cn_ID_STRING_DATA_BIT[]="数据位";
#endif
// 
const char str_en_ID_STRING_DATA_BIT[]=" ";

//------ ID_STRING_STOP_BIT --------
//停止位
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_STOP_BIT[]={10004,10027,10028};
#else
const char str_cn_ID_STRING_STOP_BIT[]="停止位";
#endif
// 
const char str_en_ID_STRING_STOP_BIT[]=" ";

//------ ID_STRING_PARITY_BIT --------
//校验位
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_PARITY_BIT[]={10005,10071,10028};
#else
const char str_cn_ID_STRING_PARITY_BIT[]="校验位";
#endif
// 
const char str_en_ID_STRING_PARITY_BIT[]=" ";

//------ ID_STRING_RCV_CNT --------
//接收报文包数量
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_RCV_CNT[]={10049,10053,10008,10035,10082,10085,10031};
#else
const char str_cn_ID_STRING_RCV_CNT[]="接收报文包数量";
#endif
// 
const char str_en_ID_STRING_RCV_CNT[]=" ";

//------ ID_STRING_SND_CNT --------
//发送报文包数量
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_SND_CNT[]={10000,10044,10008,10035,10082,10085,10031};
#else
const char str_cn_ID_STRING_SND_CNT[]="发送报文包数量";
#endif
// 
const char str_en_ID_STRING_SND_CNT[]=" ";

//------ ID_STRING_RCV_LEN --------
//接收报文字节长度
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_RCV_LEN[]={10049,10053,10008,10035,10054,10060,10006,10039};
#else
const char str_cn_ID_STRING_RCV_LEN[]="接收报文字节长度";
#endif
// 
const char str_en_ID_STRING_RCV_LEN[]=" ";

//------ ID_STRING_SND_LEN --------
//发送报文字节长度
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_SND_LEN[]={10000,10044,10008,10035,10054,10060,10006,10039};
#else
const char str_cn_ID_STRING_SND_LEN[]="发送报文字节长度";
#endif
// 
const char str_en_ID_STRING_SND_LEN[]=" ";

//------ ID_STRING_FM_VER --------
//固件版本
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_FM_VER[]={10043,10086,10068,10030};
#else
const char str_cn_ID_STRING_FM_VER[]="固件版本";
#endif
// 
const char str_en_ID_STRING_FM_VER[]=" ";

//------ ID_STRING_FM_TIME --------
//固件烧录时间
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_FM_TIME[]={10043,10086,10052,10029,10019,10065};
#else
const char str_cn_ID_STRING_FM_TIME[]="固件烧录时间";
#endif
// 
const char str_en_ID_STRING_FM_TIME[]=" ";

//------ ID_STRING_FILE_VER --------
//配置文件版本
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_FILE_VER[]={10066,10034,10035,10086,10068,10030};
#else
const char str_cn_ID_STRING_FILE_VER[]="配置文件版本";
#endif
// 
const char str_en_ID_STRING_FILE_VER[]=" ";

//------ ID_STRING_FILE_TIME --------
//配置文件保存时间
#ifdef CN_WORD_STOCK
const INT16U str_cn_ID_STRING_FILE_TIME[]={10066,10034,10035,10086,10003,10069,10019,10065};
#else
const char str_cn_ID_STRING_FILE_TIME[]="配置文件保存时间";
#endif
// 
const char str_en_ID_STRING_FILE_TIME[]=" ";

const STR_RES StringRes[]=
{
	{str_cn_ID_STRING_NONE,sizeof(str_cn_ID_STRING_NONE)/_PER_SIZE,str_en_ID_STRING_NONE,sizeof(str_en_ID_STRING_NONE)},
	{str_cn_ID_STRING_LOGO,sizeof(str_cn_ID_STRING_LOGO)/_PER_SIZE,str_en_ID_STRING_LOGO,sizeof(str_en_ID_STRING_LOGO)},
	{str_cn_ID_STRING_MAIN_MENU,sizeof(str_cn_ID_STRING_MAIN_MENU)/_PER_SIZE,str_en_ID_STRING_MAIN_MENU,sizeof(str_en_ID_STRING_MAIN_MENU)},
	{str_cn_ID_STRING_VALUE_TAB,sizeof(str_cn_ID_STRING_VALUE_TAB)/_PER_SIZE,str_en_ID_STRING_VALUE_TAB,sizeof(str_en_ID_STRING_VALUE_TAB)},
	{str_cn_ID_STRING_GATE_CTRL,sizeof(str_cn_ID_STRING_GATE_CTRL)/_PER_SIZE,str_en_ID_STRING_GATE_CTRL,sizeof(str_en_ID_STRING_GATE_CTRL)},
	{str_cn_ID_STRING_DEVICE_CFG,sizeof(str_cn_ID_STRING_DEVICE_CFG)/_PER_SIZE,str_en_ID_STRING_DEVICE_CFG,sizeof(str_en_ID_STRING_DEVICE_CFG)},
	{str_cn_ID_STRING_RUN_STAT,sizeof(str_cn_ID_STRING_RUN_STAT)/_PER_SIZE,str_en_ID_STRING_RUN_STAT,sizeof(str_en_ID_STRING_RUN_STAT)},
	{str_cn_ID_STRING_DEVICE_ADDR,sizeof(str_cn_ID_STRING_DEVICE_ADDR)/_PER_SIZE,str_en_ID_STRING_DEVICE_ADDR,sizeof(str_en_ID_STRING_DEVICE_ADDR)},
	{str_cn_ID_STRING_TIME,sizeof(str_cn_ID_STRING_TIME)/_PER_SIZE,str_en_ID_STRING_TIME,sizeof(str_en_ID_STRING_TIME)},
	{str_cn_ID_STRING_GATAGE,sizeof(str_cn_ID_STRING_GATAGE)/_PER_SIZE,str_en_ID_STRING_GATAGE,sizeof(str_en_ID_STRING_GATAGE)},
	{str_cn_ID_STRING_M,sizeof(str_cn_ID_STRING_M)/_PER_SIZE,str_en_ID_STRING_M,sizeof(str_en_ID_STRING_M)},
	{str_cn_ID_STRING_ENCODER_VAL,sizeof(str_cn_ID_STRING_ENCODER_VAL)/_PER_SIZE,str_en_ID_STRING_ENCODER_VAL,sizeof(str_en_ID_STRING_ENCODER_VAL)},
	{str_cn_ID_STRING_UP_LEVEL,sizeof(str_cn_ID_STRING_UP_LEVEL)/_PER_SIZE,str_en_ID_STRING_UP_LEVEL,sizeof(str_en_ID_STRING_UP_LEVEL)},
	{str_cn_ID_STRING_DN_LEVEL,sizeof(str_cn_ID_STRING_DN_LEVEL)/_PER_SIZE,str_en_ID_STRING_DN_LEVEL,sizeof(str_en_ID_STRING_DN_LEVEL)},
	{str_cn_ID_STRING_CUR_FLUX,sizeof(str_cn_ID_STRING_CUR_FLUX)/_PER_SIZE,str_en_ID_STRING_CUR_FLUX,sizeof(str_en_ID_STRING_CUR_FLUX)},
	{str_cn_ID_STRING_STATE,sizeof(str_cn_ID_STRING_STATE)/_PER_SIZE,str_en_ID_STRING_STATE,sizeof(str_en_ID_STRING_STATE)},
	{str_cn_ID_STRING_GATE_STATE,sizeof(str_cn_ID_STRING_GATE_STATE)/_PER_SIZE,str_en_ID_STRING_GATE_STATE,sizeof(str_en_ID_STRING_GATE_STATE)},
	{str_cn_ID_STRING_RAISE,sizeof(str_cn_ID_STRING_RAISE)/_PER_SIZE,str_en_ID_STRING_RAISE,sizeof(str_en_ID_STRING_RAISE)},
	{str_cn_ID_STRING_DROP,sizeof(str_cn_ID_STRING_DROP)/_PER_SIZE,str_en_ID_STRING_DROP,sizeof(str_en_ID_STRING_DROP)},
	{str_cn_ID_STRING_STOP,sizeof(str_cn_ID_STRING_STOP)/_PER_SIZE,str_en_ID_STRING_STOP,sizeof(str_en_ID_STRING_STOP)},
	{str_cn_ID_STRING_DEST_GATAGE,sizeof(str_cn_ID_STRING_DEST_GATAGE)/_PER_SIZE,str_en_ID_STRING_DEST_GATAGE,sizeof(str_en_ID_STRING_DEST_GATAGE)},
	{str_cn_ID_STRING_START,sizeof(str_cn_ID_STRING_START)/_PER_SIZE,str_en_ID_STRING_START,sizeof(str_en_ID_STRING_START)},
	{str_cn_ID_STRING_RAW_OP,sizeof(str_cn_ID_STRING_RAW_OP)/_PER_SIZE,str_en_ID_STRING_RAW_OP,sizeof(str_en_ID_STRING_RAW_OP)},
	{str_cn_ID_STRING_CUR_GATE_STATE,sizeof(str_cn_ID_STRING_CUR_GATE_STATE)/_PER_SIZE,str_en_ID_STRING_CUR_GATE_STATE,sizeof(str_en_ID_STRING_CUR_GATE_STATE)},
	{str_cn_ID_STRING_COMM_CFG,sizeof(str_cn_ID_STRING_COMM_CFG)/_PER_SIZE,str_en_ID_STRING_COMM_CFG,sizeof(str_en_ID_STRING_COMM_CFG)},
	{str_cn_ID_STRING_COMM_STATE,sizeof(str_cn_ID_STRING_COMM_STATE)/_PER_SIZE,str_en_ID_STRING_COMM_STATE,sizeof(str_en_ID_STRING_COMM_STATE)},
	{str_cn_ID_STRING_VER_INFO,sizeof(str_cn_ID_STRING_VER_INFO)/_PER_SIZE,str_en_ID_STRING_VER_INFO,sizeof(str_en_ID_STRING_VER_INFO)},
	{str_cn_ID_STRING_DIAG,sizeof(str_cn_ID_STRING_DIAG)/_PER_SIZE,str_en_ID_STRING_DIAG,sizeof(str_en_ID_STRING_DIAG)},
	{str_cn_ID_STRING_BAUD,sizeof(str_cn_ID_STRING_BAUD)/_PER_SIZE,str_en_ID_STRING_BAUD,sizeof(str_en_ID_STRING_BAUD)},
	{str_cn_ID_STRING_DATA_BIT,sizeof(str_cn_ID_STRING_DATA_BIT)/_PER_SIZE,str_en_ID_STRING_DATA_BIT,sizeof(str_en_ID_STRING_DATA_BIT)},
	{str_cn_ID_STRING_STOP_BIT,sizeof(str_cn_ID_STRING_STOP_BIT)/_PER_SIZE,str_en_ID_STRING_STOP_BIT,sizeof(str_en_ID_STRING_STOP_BIT)},
	{str_cn_ID_STRING_PARITY_BIT,sizeof(str_cn_ID_STRING_PARITY_BIT)/_PER_SIZE,str_en_ID_STRING_PARITY_BIT,sizeof(str_en_ID_STRING_PARITY_BIT)},
	{str_cn_ID_STRING_RCV_CNT,sizeof(str_cn_ID_STRING_RCV_CNT)/_PER_SIZE,str_en_ID_STRING_RCV_CNT,sizeof(str_en_ID_STRING_RCV_CNT)},
	{str_cn_ID_STRING_SND_CNT,sizeof(str_cn_ID_STRING_SND_CNT)/_PER_SIZE,str_en_ID_STRING_SND_CNT,sizeof(str_en_ID_STRING_SND_CNT)},
	{str_cn_ID_STRING_RCV_LEN,sizeof(str_cn_ID_STRING_RCV_LEN)/_PER_SIZE,str_en_ID_STRING_RCV_LEN,sizeof(str_en_ID_STRING_RCV_LEN)},
	{str_cn_ID_STRING_SND_LEN,sizeof(str_cn_ID_STRING_SND_LEN)/_PER_SIZE,str_en_ID_STRING_SND_LEN,sizeof(str_en_ID_STRING_SND_LEN)},
	{str_cn_ID_STRING_FM_VER,sizeof(str_cn_ID_STRING_FM_VER)/_PER_SIZE,str_en_ID_STRING_FM_VER,sizeof(str_en_ID_STRING_FM_VER)},
	{str_cn_ID_STRING_FM_TIME,sizeof(str_cn_ID_STRING_FM_TIME)/_PER_SIZE,str_en_ID_STRING_FM_TIME,sizeof(str_en_ID_STRING_FM_TIME)},
	{str_cn_ID_STRING_FILE_VER,sizeof(str_cn_ID_STRING_FILE_VER)/_PER_SIZE,str_en_ID_STRING_FILE_VER,sizeof(str_en_ID_STRING_FILE_VER)},
	{str_cn_ID_STRING_FILE_TIME,sizeof(str_cn_ID_STRING_FILE_TIME)/_PER_SIZE,str_en_ID_STRING_FILE_TIME,sizeof(str_en_ID_STRING_FILE_TIME)},

};

const INT16U NUM_STRINGRESOURCE = sizeof(StringRes)/sizeof(STR_RES);
