
//资源文件
#ifndef STR_TAB_H
#define STR_TAB_H
#define ID_STRING_NONE                                  	0   	//无

#define ID_STRING_LOGO                                  	1   	//UITC2000闸门测控装置

#define ID_STRING_MAIN_MENU                             	2   	//主菜单

#define ID_STRING_VALUE_TAB                             	3   	//实时测值

#define ID_STRING_GATE_CTRL                             	4   	//闸门操作

#define ID_STRING_DEVICE_CFG                            	5   	//装置配置

#define ID_STRING_RUN_STAT                              	6   	//运行统计

#define ID_STRING_DEVICE_ADDR                           	7   	//装置地址

#define ID_STRING_TIME                                  	8   	//时间

#define ID_STRING_GATAGE                                	9   	//闸门开度

#define ID_STRING_M                                     	10  	//米

#define ID_STRING_ENCODER_VAL                           	11  	//编码器输入

#define ID_STRING_UP_LEVEL                              	12  	//上游水位

#define ID_STRING_DN_LEVEL                              	13  	//下游水位

#define ID_STRING_CUR_FLUX                              	14  	//当前过闸流量

#define ID_STRING_STATE                                 	15  	//状态

#define ID_STRING_GATE_STATE                            	16  	//闸门状态

#define ID_STRING_RAISE                                 	17  	//上升

#define ID_STRING_DROP                                  	18  	//下降

#define ID_STRING_STOP                                  	19  	//停止

#define ID_STRING_DEST_GATAGE                           	20  	//设定目标开度

#define ID_STRING_START                                 	21  	//启动

#define ID_STRING_RAW_OP                                	22  	//直接操作

#define ID_STRING_CUR_GATE_STATE                        	23  	//当前闸门状态

#define ID_STRING_COMM_CFG                              	24  	//通信参数配置

#define ID_STRING_COMM_STATE                            	25  	//通信状态信息

#define ID_STRING_VER_INFO                              	26  	//版本信息

#define ID_STRING_DIAG                                  	27  	//装置自检调试

#define ID_STRING_BAUD                                  	28  	//波特率

#define ID_STRING_DATA_BIT                              	29  	//数据位

#define ID_STRING_STOP_BIT                              	30  	//停止位

#define ID_STRING_PARITY_BIT                            	31  	//校验位

#define ID_STRING_RCV_CNT                               	32  	//接收报文包数量

#define ID_STRING_SND_CNT                               	33  	//发送报文包数量

#define ID_STRING_RCV_LEN                               	34  	//接收报文字节长度

#define ID_STRING_SND_LEN                               	35  	//发送报文字节长度

#define ID_STRING_FM_VER                                	36  	//固件版本

#define ID_STRING_FM_TIME                               	37  	//固件烧录时间

#define ID_STRING_FILE_VER                              	38  	//配置文件版本

#define ID_STRING_FILE_TIME                             	39  	//配置文件保存时间


//字符串资源
typedef struct STR_RES
{
/**中文**/
	const INT16U * cn;
	INT16U cnlen;
/**英文**/
	const char * en;
	INT16U enlen;
}STR_RES;

extern const STR_RES StringRes[];

extern const INT16U NUM_STRINGRESOURCE ;
 
#endif	//_RESOURCE_H
