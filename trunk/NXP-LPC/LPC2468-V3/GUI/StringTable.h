
//资源文件
#ifndef STR_TAB_H
#define STR_TAB_H

#define ID_STRING_NONE                                  	0   	//无

#define ID_STRING_SETVAL_EXIT                           	1   	//退出

#define ID_STRING_LOCAL                                 	2   	//本地

#define ID_STRING_REMOTE                                	3   	//远程

#define ID_STRING_DI_LOCALREMOTE                        	4   	//本地/远程

#define ID_STRING_HAVE                                  	5   	//有

#define ID_STRING_HAVENOT                               	6   	//无

#define ID_STRING_ENABLE                                	7   	//允许

#define ID_STRING_DISABLE                               	8   	//禁止

#define ID_STRING_PASSWORD                              	9   	//用户密码

#define ID_STRING_SC_COMM_485                           	10  	//485

#define ID_STRING_SC_COMM_ETH                           	11  	//ETH

#define ID_STRING_SC_COMM_ADDRESS                       	12  	//通讯地址

#define ID_STRING_SC_ETH_IP1                            	13  	//IP地址第1段

#define ID_STRING_SC_ETH_IP2                            	14  	//IP地址第2段

#define ID_STRING_SC_ETH_IP3                            	15  	//IP地址第3段

#define ID_STRING_SC_ETH_IP4                            	16  	//IP地址第4段

#define ID_STRING_SC_ETH_MASK1                          	17  	//Mask地址第1段

#define ID_STRING_SC_ETH_MASK2                          	18  	//Mask地址第2段

#define ID_STRING_SC_ETH_MASK3                          	19  	//Mask地址第3段

#define ID_STRING_SC_ETH_MASK4                          	20  	//Mask地址第4段

#define ID_STRING_SC_COM_BAUDRATE                       	21  	//波特率

#define ID_STRING_SC_LANGUAGE                           	22  	//语言选择

#define ID_STRING_SC_LANGUAGE_CN                        	23  	//中文

#define ID_STRING_SC_LANGUAGE_EN                        	24  	//English

#define ID_STRING_SC_SCREENSAVE                         	25  	//屏保时间

#define ID_STRING_SC_COMM_BAUDRATE_4800                 	26  	//485~4800

#define ID_STRING_SC_COMM_BAUDRATE_9600                 	27  	//485~9600

#define ID_STRING_SC_COMM_BAUDRATE_19200                	28  	//485~19200

#define ID_STRING_SC_COMM_BAUDRATE_38400                	29  	//485~38400

#define ID_STRING_SC_COMM_BAUDRATE_57600                	30  	//485~57600

#define ID_STRING_SC_COMM_BAUDRATE_115200               	31  	//485~115200

#define ID_STRING_SC_COM_DATAB                          	32  	//数据位

#define ID_STRING_SC_COM_STOPB                          	33  	//停止位

#define ID_STRING_SC_COM_PARITYB                        	34  	//校验位

#define ID_STRING_PRJ_RANGE                             	35  	//开度量程

#define ID_STRING_PRJ_HIGHEST                           	36  	//闸门最大开度

#define ID_STRING_PRJ_ZEROCODE                          	37  	//零点码值

#define ID_STRING_PRJ_HIGHCODE                          	38  	//最高点码值

#define ID_STRING_PRJ_PRESET                            	39  	//预设开度

#define ID_STRING_PRJ_ARCNUM                            	40  	//弧分段数

#define ID_STRING_PRJ_GATAGE                            	41  	//第 点开度值

#define ID_STRING_PRJ_CODERDATA                         	42  	//第 点编码值

#define ID_STRING_PRJ_SLOPE                             	43  	//第 段斜率

#define ID_STRING_DEV_DIRECTION                         	44  	//闸门转向

#define ID_STRING_DEV_CALCMETHOD                        	45  	//计算方式

#define ID_STRING_DEV_DATASOURSE                        	46  	//数据来源

#define ID_STRING_DEV_ENCODERTYPE                       	47  	//编码器接口

#define ID_STRING_DEV_CODETYPE                          	48  	//码字类型

#define ID_STRING_DEV_METHOD_LINE                       	49  	//线性算法

#define ID_STRING_DEV_METHOD_TABLE                      	50  	//查表算法

#define ID_STRING_DEV_DATA_ENCODER                      	51  	//来自编码值

#define ID_STRING_DEV_DATA_AD                           	52  	//来自模入

#define ID_STRING_DEV_GATE_COROTATION                   	53  	//正转

#define ID_STRING_DEV_GATE_ROLLBACK                     	54  	//反转

#define ID_STRING_DEV_ENCODE_NONE                       	55  	//无编码器

#define ID_STRING_DEV_ENCODE_PARALLEL                   	56  	//绝对型并口

#define ID_STRING_DEV_ENCODE_SSI                        	57  	//绝对型SSI

#define ID_STRING_DEV_ENCODE_INCREMENTAL                	58  	//增量型脉冲

#define ID_STRING_DEV_CODE_GRAY                         	59  	//格雷码

#define ID_STRING_DEV_CODE_BCD                          	60  	//BCD码

#define ID_STRING_DEV_CODE_BINARY                       	61  	//二进制码

#define ID_STRING_DEV_ADDACHOICE                        	62  	//AIAO通道选择

#define ID_STRING_DEV_NONE_ADDA                         	63  	//无模入模出

#define ID_STRING_DEV_ADDA                              	64  	//1路AI1路AO

#define ID_STRING_DEV_TWOAD                             	65  	//2路AI

#define ID_STRING_DEV_ONEDA                             	66  	//1路AO

#define ID_STRING_DEV_DIDOCONFIG                        	67  	//开入开出定义

#define ID_STRING_DEV_DIGITALTIME                       	68  	//去抖时间

#define ID_STRING_DC_DIDOCONFIG_DEFAULT                 	69  	//默认

#define ID_STRING_DC_DIDOCONFIG_PREDEFINE               	70  	//预定义

#define ID_STRING_DC_DIDOCONFIG_CUSTOM                  	71  	//自定义

#define ID_STRING_K_IN                                  	72  	//4~20mA Kin

#define ID_STRING_K_OUT                                 	73  	//4~20mA K

#define ID_STRING_B_OUT                                 	74  	//4~20mA B

#define ID_STRING_ABNORMAL_FRAM                         	75  	//FRAM

#define ID_STRING_ABNORMAL_RAM                          	76  	//RAM

#define ID_STRING_ABNORMAL_CLOCK                        	77  	//时钟

#define ID_STRING_ABNORMAL_MEAS_PRECISION               	78  	//测量精度系数

#define ID_STRING_ABNORMAL_SYSCFG                       	79  	//系统设置

#define ID_STRING_ABNORMAL_DBGCFG                       	80  	//工程配置

#define ID_STRING_ABNORMAL_DIDOCONFIG                   	81  	//开入开出配置

#define ID_STRING_ABNORMAL_DO_POWER                     	82  	//开出24V电源

#define ID_STRING_ABNORMAL_CONNECTION                   	83  	//外部接线

#define ID_STRING_ABNORMAL_EXECODE                      	84  	//可执行代码

#define ID_STRING_RUN_COOL_STARTUP                      	85  	//冷启动

#define ID_STRING_RUN_SETTING                           	86  	//修改定值

#define ID_STRING_RUN_FRAM_INIT                         	87  	//FRAM初始化

#define ID_STRING_RUN_INTO_DEBUGING                     	88  	//进入调试态中..

#define ID_STRING_RUN_OUTOF_DEBUGING                    	89  	//退出调试态中..

#define ID_STRING_RUN_INTO_DEBUG                        	90  	//进入调试态

#define ID_STRING_RUN_OUTOF_DEBUG                       	91  	//退出调试态

#define ID_STRING_MEASTYPE_ALL                          	92  	//显示所有量

#define ID_STRING_MEASTYPE_SIGNAL                       	93  	//显示信号量

#define ID_STRING_MEASTYPE_STATUS                       	94  	//显示状态量

#define ID_STRING_MEASTYPE_MEAS                         	95  	//显示测量值

#define ID_STRING_MEAS_GATAGE                           	96  	//闸门开度

#define ID_STRING_MEASTYPE_CODEDATA                     	97  	//编码值

#define ID_STRING_MEAS_CODEDATA_L                       	98  	//编码值低位

#define ID_STRING_MEAS_CODEDATA_H                       	99  	//编码值高位

#define ID_STRING_MEAS_AI1                              	100 	//输入电流1

#define ID_STRING_MEAS_AI2                              	101 	//输入电流2

#define ID_STRING_MEAS_AO                               	102 	//输出电流

#define ID_STRING_MEAS_ADCODE                           	103 	//AD码

#define ID_STRING_MEAS_LED                              	104 	//信 灯

#define ID_STRING_MEAS_GATESTATUS                       	105 	//闸门状态

#define ID_STRING_MEAS_SIG_DI                           	106 	//开入

#define ID_STRING_MEAS_SIG_DO                           	107 	//开出

#define ID_STRING_MEAS_SIG_SELFCHK                      	108 	//信 自检

#define ID_STRING_MEAS_SIG_SELFCHKEX                    	109 	//信 自检 

#define ID_STRING_MEAS_SIG_TOTAL                        	110 	//信 总信号

#define ID_STRING_CONTROL                               	111 	//默认画面

#define ID_STRING_STATUS                                	112 	//测量显示

#define ID_STRING_SYSINFO                               	113 	//系统信息

#define ID_STRING_SYSSET                                	114 	//系统设置

#define ID_STRING_PRJSET                                	115 	//工程设置

#define ID_STRING_DEVICEMODEL                           	116 	//装置配置

#define ID_STRING_DEBUG                                 	117 	//装置调试

#define ID_STRING_OTHER                                 	118 	//其他设置

#define ID_STRING_STA_MEAS                              	119 	//测量量显示

#define ID_STRING_STA_LOOP                              	120 	//循环显示

#define ID_STRING_STA_PROT                              	121 	//保护量显示

#define ID_STRING_STA_STAS                              	122 	//状态量显示

#define ID_STRING_SYSSET_PWVALUE                        	123 	//用户密码

#define ID_STRING_SYSSET_ETH                            	124 	//以太网口设置

#define ID_STRING_SYSSET_COM                            	125 	//485端口设置

#define ID_STRING_SYSSET_AO                             	126 	//4~20mA输出

#define ID_STRING_PRJSET_RANGE                          	127 	//量程设置

#define ID_STRING_PRJSET_PRESET                         	128 	//预设开度设置

#define ID_STRING_PRJSET_ARCSET                         	129 	//弧分段设置

#define ID_STRING_PRJSET_SLOPESET                       	130 	//各段斜率设置

#define ID_STRING_CLOCK                                 	131 	//时钟设置

#define ID_STRING_SYSSET_LANG                           	132 	//语言选择

#define ID_STRING_DEVICEMODEL_MEASPRECIS                	133 	//测量精度手动

#define ID_STRING_DEVICEMODEL_MODEL                     	134 	//装置选配

#define ID_STRING_DEVICEMODEL_DI                        	135 	//DI预定义

#define ID_STRING_DEVICEMODEL_DO                        	136 	//DO预定义

#define ID_STRING_INIT_DEFAULT                          	137 	//恢复出厂设置

#define ID_STRING_SCREEN                                	138 	//界面设置

#define ID_STRING_VERTTYPE                              	139 	//垂直滚动条

#define ID_STRING_VERTTYPE_NUM                          	140 	//显示数字

#define ID_STRING_VERTTYPE_GRAP1                        	141 	//显示图标

#define ID_STRING_VERTTYPE_GRAP2                        	142 	//显示滚动条

#define ID_STRING_MENU_LEFTRIGHT                        	143 	//菜单水平滚动

#define ID_STRING_DEFAULT_SCREEN                        	144 	//默认显示界面

#define ID_STRING_MAIN_MENU_STYLE                       	145 	//主菜单样式

#define ID_STRING_MAIN_MENU_STYLE_NORMAL                	146 	//菜单样式

#define ID_STRING_MAIN_MENU_STYLE_ICON                  	147 	//图标样式

#define ID_STRING_DO_CHECK                              	148 	//开出检查

#define ID_STRING_DI_CHECK                              	149 	//开入检查

#define ID_STRING_AO_CHECK                              	150 	//4~20mA输出

#define ID_STRING_CLEAR_CODE                            	151 	//编码值归零

#define ID_STRING_COMM_MONITOR                          	152 	//报文监视

#define ID_STRING_MAIN_VERSION                          	153 	//装置版本

#define ID_STRING_YEAR                                  	154 	//年

#define ID_STRING_MONTH                                 	155 	//月

#define ID_STRING_DAY                                   	156 	//日

#define ID_STRING_HOUR                                  	157 	//时

#define ID_STRING_MIN                                   	158 	//分

#define ID_STRING_SEC                                   	159 	//秒

#define ID_STRING_COMMFLT                               	160 	//通讯故障

#define ID_STRING_STATUS_STOP                           	161 	//停止

#define ID_STRING_STATUS_RUN                            	162 	//运动

#define ID_STRING_BRANDING                              	163 	//科技无限

#define ID_STRING_WISCOM                                	164 	//金色智慧

#define ID_STRING_NOMEAS                                	165 	//无测量量

#define ID_STRING_ACT_RESULT_WARNING                    	166 	//告警

#define ID_STRING_ERR_UNKNOWN                           	167 	//通讯出错

#define ID_STRING_ERR_CHECKSUM                          	168 	//校验和

#define ID_STRING_ERR_SETRANGE                          	169 	//定值范围错

#define ID_STRING_ERR_TIME_ILLEGAL                      	170 	//时间不合法

#define ID_STRING_ERR_UNREMOTE                          	171 	//装置不在远方

#define ID_STRING_ERR_MODIFYSET_CANCEL                  	172 	//修改定值\n非正常取消

#define ID_STRING_ERR_UNKNOW_CODE                       	173 	//未知指令

#define ID_STRING_ERR_RECHECK                           	174 	//返校错误

#define ID_STRING_ERR_UNDEBUG                           	175 	//装置不在\n调试位置

#define ID_STRING_ERR_SEQUENCE                          	176 	//报文序列错误

#define ID_STRING_ERR_UNLOCAL                           	177 	// 装置不在\n就地位置

#define ID_STRING_ERR_UNKNOW_SETID                      	178 	//未知的设置序号

#define ID_STRING_ERR_MESSAGE_CRASH                     	179 	//报文结构不完整

#define ID_STRING_ERR_CONTROL_ILLEGAL                   	180 	//控制非法\n不允许执行

#define ID_STRING_ERR_DISABLE_CODE                      	181 	//指令不允许

#define ID_STRING_ERR_OPERATE                           	182 	//操作失败

#define ID_STRING_OPERATING                             	183 	//操作执行中...

#define ID_STRING_ERR_OVER_RANGE                        	184 	//范围越界

#define ID_STRING_ERR_CTRLCMD_CHECK_CONTROLMODE         	185 	//不允许该\n控制命令

#define ID_STRING_CTRLCMD_CHECK_HAVEOPERA               	186 	//当前正在\n执行指令

#define ID_STRING_SEARCHING                             	187 	//搜索中...

#define ID_STRING_STARTING_TIME                         	188 	//启动用时

#define ID_STRING_OP_CONFIRM                            	189 	//操作执行？

#define ID_STRING_ERROR_PASWORD                         	190 	//密码错误！

#define ID_STRING_DI_DI1                                	191 	//开入1

#define ID_STRING_DI_DI2                                	192 	//开入2

#define ID_STRING_DI_DI3                                	193 	//开入3

#define ID_STRING_DI_DI4                                	194 	//开入4

#define ID_STRING_DI_DI5                                	195 	//开入5

#define ID_STRING_DI_DI6                                	196 	//开入6

#define ID_STRING_DI_DI7                                	197 	//开入7

#define ID_STRING_DI_DI8                                	198 	//开入8

#define ID_STRING_DI_DI9                                	199 	//开入9

#define ID_STRING_DI_DI10                               	200 	//开入10

#define ID_STRING_DI_DI11                               	201 	//开入11

#define ID_STRING_DI_DI12                               	202 	//开入12

#define ID_STRING_DI_UPPER_LIMIT                        	203 	//闸位上限

#define ID_STRING_DI_LOWER_LIMIT                        	204 	//闸位下限

#define ID_STRING_DI_OVER_UPPER                         	205 	//闸位上上限

#define ID_STRING_DI_BELOW_LOWER                        	206 	//闸位下下限

#define ID_STRING_DI_GATE_RISE                          	207 	//闸门升

#define ID_STRING_DI_GATE_FALL                          	208 	//闸门降

#define ID_STRING_DI_BREAK_STATE                        	209 	//断路器状态

#define ID_STRING_DI_OVER_LOAD1                         	210 	//过负荷信号1

#define ID_STRING_DI_OVER_LOAD2                         	211 	//过负荷信号2

#define ID_STRING_DI_OVER_LOAD3                         	212 	//过负荷信号3

#define ID_STRING_DI_OVER_LOAD4                         	213 	//过负荷信号4

#define ID_STRING_DI_REMOTE_LOCAL                       	214 	//远方/现地

#define ID_STRING_DO_DO1                                	215 	//开出1

#define ID_STRING_DO_DO2                                	216 	//开出2

#define ID_STRING_DO_DO3                                	217 	//开出3

#define ID_STRING_DO_DO4                                	218 	//开出4

#define ID_STRING_DO_DO5                                	219 	//开出5

#define ID_STRING_DO_DO6                                	220 	//开出6

#define ID_STRING_DO_RISE_GATE                          	221 	//升闸门

#define ID_STRING_DO_FALL_GATE                          	222 	//降闸门

#define ID_STRING_DO_STOP_GATE                          	223 	//停闸门

#define ID_STRING_DO_CLOSE_BREAK                        	224 	//合闸

#define ID_STRING_DO_TRIP_BREAK                         	225 	//分闸

#define ID_STRING_OPEN                                  	226 	//分

#define ID_STRING_CLOSE                                 	227 	//合

#define ID_STRING_NOMENUITEM                            	228 	//无菜单项目

#define ID_STRING_COMPILETIME                           	229 	//编译时间

#define ID_STRING_CFGLOADTIME                           	230 	//配置加载时间

#define ID_STRING_LOADTIME                              	231 	//系统加载时间

#define ID_STRING_OLD_PW                                	232 	//原密码

#define ID_STRING_NEW_PW2                               	233 	//确认新密码

#define ID_STRING_NEW_PW                                	234 	//输入新密码

#define ID_STRING_DO1_PULSEWIDTH                        	235 	//DO1时间定义

#define ID_STRING_DO2_PULSEWIDTH                        	236 	//DO2时间定义

#define ID_STRING_DO3_PULSEWIDTH                        	237 	//DO3时间定义

#define ID_STRING_DO4_PULSEWIDTH                        	238 	//DO4时间定义

#define ID_STRING_DI_SWITCHOFF                          	239 	//分闸状态

#define ID_STRING_DI_SWITCHON                           	240 	//合闸状态

#define ID_STRING_DI_CMDSWITCHOFF                       	241 	//分闸命令

#define ID_STRING_DI_CMDSWITCHON                        	242 	//合闸命令

#define ID_STRING_CONTROL_SWITCHOFF_COMM                	243 	//通信分闸

#define ID_STRING_CONTROL_SWITCHON_COMM                 	244 	//通信合闸

#define ID_STRING_READING_SYSINIT_INFO                  	245 	//读初始化信息..

#define ID_STRING_ERR_PASSWORD                          	246 	//密码错误！

#define ID_STRING_ERR_OVERRANGE                         	247 	//数值越界

#define ID_STRING_CONFIRM                               	248 	//确认修改 

#define ID_STRING_ERR_TIME                              	249 	//时钟信息错误

#define ID_STRING_NO_ABNORMAL                           	250 	//没有异常

#define ID_STRING_OPERATION_SUCCESS                     	251 	//操作成功

#define ID_STRING_DO_OFF2ON                             	252 	//分->合

#define ID_STRING_DO_ON2OFF                             	253 	//合->分

#define ID_STRING_RESET_CONFIRM                         	254 	//确认复归？

#define ID_STRING_RESET                                 	255 	//装置复归

#define ID_STRING_RESTART                               	256 	//装置重启

#define ID_STRING_RESTART_CONFIRM                       	257 	//确认重启？

#define ID_STRING_CONFIG_VERSION                        	258 	//配置文件版本

#define ID_STRING_NOW_TIME                              	259 	//当前时间

#define ID_STRING_DOWNLOAD                              	260 	//文件下载中...

 
//字符串资源
typedef struct
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
