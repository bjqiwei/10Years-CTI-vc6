#ifndef __PCICARD_H__
#define __PCICARD_H__

long  __stdcall PCIGETCARDNUM(); //检测目前实际存在并激活的卡的数目
long  __stdcall PCIGETCONFIGCARDNUM();//返回配置文件中设置使用的卡数目

/*  GETCARDHARDWARESERIAL 函数返回结果的位域意义
    bit[15.. 0] = CardHardwareSerialNo // 实际的本卡硬件序列号
    bit[19..16] = CardHardwareLine     // 硬件支持的通道数目
	                                   // 数字线:1--8 表示支持1--8个E1   
									   // 模拟线:1--6 表示支持4--24线(以4线为单位)
    bit[23..20] = CardHardwareRes      // 卡上带有的资源类型bit3=fsk,bit2=fax
    bit[27..24] = CardSwitch           //卡号(卡上跳线开关号0--15)
	bit[31..28] = CardHardwareFlag     //卡类型标志(1=4线模拟卡,2=数字卡,3=24线模拟卡)
*/

long  __stdcall PCIGETCARDHARDWARESERIAL(long CARDNO); //返回对应位置的卡的硬件序列号
long  __stdcall PCIGETCARDHARDWARETYPE(long CARDNO); //返回对应位置的卡的硬件类型
long  __stdcall PCIGETLOGICCHNUM(long CHTYPE); //获取指定类型线路配置的逻辑通道数目
long  __stdcall PCIGETTOTALLOGICCHNUM(); //获取系统中所有类型线路配置的逻辑通道数目总和 
long  __stdcall PCIGRABMSG(RMSG *PMSG,unsigned short MSGNUM);//不阻塞接收消息（不论是否收到消息，函数都返回）
long  __stdcall PCISENDMSG(RMSG *PMSG); //不阻塞发送消息（不论是否发送成功消息，函数都返回）
BOOL  __stdcall PCIINITCARD(long PARAM); //初始化硬件
BOOL  __stdcall PCIEXITCARD(); //关闭硬件
//long  __stdcall PCIGETSLOT(long CHTYPE,long CHANNEL,long PARAM);//取逻辑通道号的物理时隙
#endif