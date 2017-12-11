#ifndef __StructO__h_
#define __StructO__h_

#define WORKBORDNUM			16     //系统最大卡数目
#define MAXEXCHLINE			2048   //系统最大线路数目
#define MAXVOCINDEX			10000  //系统最大内存索引放音的索引数目
#define USERMAXVOCINDEX		8192   //用户可使用最大内存索引放音的索引数目,用户使用Voc0--Voc8191
#define ECONFGROUPCHNUM		32 //每个会议组通道数目(每个DSP支持的会议通道)
#define ECONFGROUPCONFNUM	8  //每个会议组会议数目(每个DSP支持的会议)
#define ECONFGROUPNUM		32 //最多可能会议组数(4个DSP*8块卡)=1024通道

#define CARDHARDWARE_DT256           2  //30--240线数字卡
#define CARDHARDWARE_SLAT24          3  //4--24线模拟卡

/////////////////定义通道类型
#define CHCLASS_NONE    0x00  //空线路类型
#define CHCLASS_DT      0x08  //数字线
#define CHCLASS_AT      0x10  //模拟线

#define CHTYPE_DTNO1    0x09  //数字中继中国一号信令 
#define CHTYPE_DTNO7    0x0A  //数字中继中国七号信令(TUP)
#define CHTYPE_DTDSS1   0x0B  //数字中继ISDN PRI(30B+D)
#define CHTYPE_DTNO7ISUP 0x0C  //数字中继中国七号信令(ISUP)
#define CHTYPE_MTP3		0x0F  //七号信令MTP传输层通道

#define CHTYPE_TB       0x11  //模拟中继外线(普通市话线),接电信局
#define CHTYPE_AGENT    0x12  //模拟中继内线(座席线)，接电话机
#define CHTYPE_HB       0x13  //模拟中继高阻线(监控线)，与电话线/电话机相并联

/////////////////定义资源类型
#define RESTYPE_VOCPCM  0x01   //PCM语音资源
#define RESTYPE_DTMF    0x02   //DTMF资源
#define RESTYPE_FAX     0x03   //传真资源
#define RESTYPE_FSK     0x04   //fsk通讯资源
#define RESTYPE_MODEM   0x05   //modem通讯资源
#define RESTYPE_HDLC    0x06   //HDLC资源(内部使用)
#define RESTYPE_CAS     0x07   //cas资源

#define RESTYPE_VOIP    0x1D   //VOIP通讯资源
#define RESTYPE_BIG     0x1E   //大数通讯资源
#define RESTYPE_MFC     0x1F   //mfc资源(内部使用)

#define RESTYPE_VC      0x18   //变声(voice change)资源
#define RESTYPE_CONF    0x19   //电话会议室资源 
#define RESTYPE_CONFCH  0x1A   //电话会议通道资源 
#define RESTYPE_ECONF   0x1B   //增强电话会议室资源 
#define RESTYPE_ECONFCH 0x1C   //增强电话会议通道资源 

/////////////////定义通道状态
#define CHSTAT_IDLE     0x00  //空闲

#define CHSTAT_CALLOUT  0x10  //呼出状态
#define CHSTAT_CALLOSET 0x11  //呼出状态setup
#define CHSTAT_CALLOING 0x12  //呼出状态正在呼叫
#define CHSTAT_CALLORIN 0x1E  //呼出回铃
#define CHSTAT_CALLOCON 0x1F  //呼出通话

#define CHSTAT_CALLIN   0x20  //呼入状态
#define CHSTAT_CALLINTF 0x21  //呼入提醒
#define CHSTAT_CALLIPRO 0x22  //呼入状态处理
#define CHSTAT_CALLIRIN 0x2E  //呼入振铃
#define CHSTAT_CALLICON 0x2F  //呼入通话

/////////////////定义资源状态
#define RESTAT_IDLE     0x00  //空闲
#define RESTAT_OPENNING 0x01  //正在打开
#define RESTAT_INIT     0x02  //正在初始化
#define RESTAT_CLOSING  0x03  //正在关闭
#define RESTAT_STOP     0x04  //被驱动停止
#define RESTAT_CLEAR    0x05  //停止后清除相关资源
#define RESTAT_WORKING  0x0F  //正在使用
/////////////////定义资源参数
#define RESPAR_NONE     0x00  //
#define RESPAR_DTMFSTOP 0x01  //收到DTMF时停止录或放音
#define RESPAR_PLOOPMSG  0x20  //这是一个循环放音消息
			       //如果没有同时设置RESPAR_PLASTMSG,
                               //则放完后重新追加到队列尾
#define RESPAR_PFIRSTMSG 0x40  //这是第一个放音消息
#define RESPAR_PLASTMSG  0x80  //这是最后一个放音消息

#define RESPAR_RAGC     0x02  //录音使用自动增益控制
#define RESPAR_RWAITP   0x40  //等待放音结束后才开始录音
#define RESPAR_RAPPEND  0x80  //录音时追加到原文件末尾
                               
////////定义文件放音的文件格式
#define PFDATA_ALAW     0x00  //标准alaw pcm 无格式文件
#define PFDATA_WAVE     0x01  //windows .WAV文件
#define PFDATA_CLADPCM  0x02  //cirrus logic adpcm(32kbps) 无格式文件
#define PFDATA_G7231    0x03  //G.723.1(6.3Kbps) 无格式文件
#define PFDATA_DLGVOX6K 0x04  //dialogic vox 6K sample
#define PFDATA_DLGVOX8K 0x05  //dialogic vox 8K sample
#define PF_OFFSETPFILE	0x80  //是否指定播放文件的起始位置(毫秒)

//////////                                                             
////////定义传真文件的编码格式
#define FAXFILE_NOFORMAT 0x00  //无格式文件，普通分辨率，一维编码
#define FAXFILE_TIFF     0x01  //TIFF传真文件
////////定义传真数据的宽度
#define FAXVRES_LOW      0x00  //普通垂直分辨率(98/英寸)
#define FAXVRES_HIGH     0x20  //高垂直分辨率(196/英寸)

#define FAXWIDTH_A4      0x00  //1728点宽
#define FAXWIDTH_A3      0x40  //2432点宽
#define FAXWIDTH_B4      0x80  //2048点宽
                
/////////定义MODEM握手方向
#define MODEM_ANS		 0x00  //应答
#define MODEM_CALL		 0x01  //呼叫

/////////定义常数
#define MSGFROMECONF     0x01  //消息来源于增强型会议

//判断是否为数字线
inline unsigned short IsCLASS_DT(unsigned short chtype)
{  
	switch (chtype)
	{
		case CHTYPE_DTNO1://    0x09  //数字中继中国一号信令 
	  case CHTYPE_DTNO7://    0x0A  //数字中继中国七号信令(TUP)
		case CHTYPE_DTDSS1://   0x0B  //数字中继ISDN PRI(30B+D)
	  case CHTYPE_DTNO7ISUP:// 0x0C  //数字中继中国七号信令(ISUP);
	    return 1;
	  default:
	  	return 0;
	}
}

//判断是否为模拟线
inline unsigned short IsCLASS_AT(unsigned short chtype)
{  
	switch (chtype)
	{
		case CHTYPE_TB://       0x11  //模拟中继外线(普通市话线),接电信局
		case CHTYPE_AGENT://    0x12  //模拟中继内线(座席线)，接电话机
		case CHTYPE_HB:   //    0x13  //模拟中继高阻线(监控线)，与电话线/电话机相并联
	    return 1;
	  default:
	  	return 0;
	}
}

/////////////////定义消息,可用于下传也用于上传
#define MSG_NULL              0     //空消息,消息无意义,DataLen仍旧有意义
                                    
#define MSG_RELEASE           1     //通道释放
 #define F_R_NORMAL            0    //正常释放
 #define F_R_CALLOUTERR        1    //呼出失败释放
  #define P_R_NODIALTONE        1   //没有检测到拨号音 
  #define P_R_NOANSWER          2   //没有响应
  #define P_R_RINGTIMEOUT       3   //振铃超时 （无人接）
  #define P_R_LINEUSED          4  //线路正被对方占用或没有复位完成
  #define P_R_CONGESTION        12  //设备拥塞
  #define P_R_INCOMPLETE        15  //地址不全
  #define P_R_FAILURE           16  //呼叫故障
  #define P_R_CALLBARRING       18  //接入拒绝
  #define P_R_BUSYLOCAL         19  //市话忙
  #define P_R_BUSYDDD           20  //长话忙
  #define P_R_NULLDN            22  //空号
  #define P_R_NODIGITALPATH     26  //没有数字通路
 #define F_R_RESET	           2    //线路复原


#define MSG_CALL              2     //通道呼出/或有呼入
 #define F_C_NORMAL            0    //普通呼叫
  //对于TB 和 AGENT 呼出时
  #define P_C_NORMAL            0    //TB=检测dialtone,AGENT=标准振铃声+FSK主叫发送                     
  #define P_C_1                 1    //TB=不检测dialtone;AGENT=标准振铃声+DTMF主叫发送
  #define P_C_2                 2    //AGENT=第2种振铃声+DTMF主叫发送
  #define P_C_3                 3    //AGENT=第3种振铃声+DTMF主叫发送
  #define P_C_4                 4    //AGENT=第4种振铃声+DTMF主叫发送 
  //对于TB 和 AGENT 呼入时,Param=0
  //对于NO1信令,Param低4位为KA值(0代表默任值KA=1),Param高4位为KD值(0代表默任值KD=1)
  //对于NO7信令,Param 	 //bit5:bit4 表示原始被叫类别(00=普通，01=国内备用，10=国内有效，11=国际)
  			 //bit3:bit2 表示主叫类别(00=普通，01=国内备用，10=国内有效，11=国际)
					//bit1表示是否主叫号码显示限制(0=不限制,1=限制),
				    //bit0表示是否是不完全呼叫(0=完全呼叫，1=不完全呼叫)
  //对于DSS1信令,Param表示是否是不完全呼叫(0=完全呼叫，1=不完全呼叫)
 #define F_C_OVLP              1    //重叠呼叫
  
#define MSG_CALLEDACK         3     //被叫摘机应答
 #define  F_ACK_ANC            0    //计费
 #define  F_ACK_ANN            1    //不计费

#define MSG_FLASH             4     //拍叉簧

#define MSG_ACM               5    //上传消息本方呼出成功(对方已经振铃),有可能收不到该消息而直接收到Connect消息
					               //下传消息给对方(呼叫方)发地址全(特殊情况下用)
 #define F_ACM_IDLE            0   //空闲可用
 #define F_ACM_BUSY            1   //线路忙
 #define F_ACM_NULL            2   //线路空号
 #define F_ACM_FAULT           3   //线路呼叫失败


#define UPMSG_CALLOUTCHANNEL  6    //上传消息分配到的呼出通道
 #define F_CC_NORMAL           0    //分配到呼出通道(=ChIndex)
 #define F_CC_NOCHANNEL        1    //分配不到呼出通道(ChIndex)
  #define P_CC_CHBUSY           1   //通道使用中
  #define P_CC_CHCALLMASK       2   //在配置中通道呼出被禁止
  #define P_CC_CHNOTEXIST       3   //通道号不存在
  #define P_CC_PCMNOSYN         4   //数字线端口没有同步
  #define P_CC_CANNOTASSIGNCH   5   //不支持用户设置的呼出通道指定方式
  #define P_CC_REMOTENOANSWER   6   //对方没有响应
  #define P_CC_DPCUNAVAILABLE   7   //DPC不可到达
  #define P_CC_ADDRESSERR		8   //号码设置错误
 #define F_CC_HANDLE           2    // 分配到呼出句柄(ChIndex),用于重叠呼叫     
                              
#define DOWNMSG_ROUTER        6    //通道交换命令
 #define F_R_SILENCE           0   //通道输出静音
 #define F_R_MONITOR           1   //单向监听
 #define F_R_TALK              2   //双向通话
 #define F_R_TALK_RP		   3   //双向通话并通话中可混合录放语音	
 #define F_R_MONITOR_P		   4   //强制连接到卡内放音资源		   
 #define F_R_TRANSTO_CT        13  //直接发送到CT总线一个时隙 
 #define F_R_MONITOR_CT        14  //直接监听CT总线一个时隙 

#define MSG_CONF              7    //会议
 #define F_CF_CREATE           1   //请求电话会议资源，创建一个会议室        
 #define F_CF_CLOSE            2   //关闭电话会议资源，关闭一个会议室        
 #define F_CF_LINKCH           3   //连接电话会议通道到线路通道,连接逻辑通道到会议室
 #define F_CF_UNLINKCH         4   //断开电话会议通道到线路通道的连接,断开逻辑通道和会议室的连接
 #define F_CF_MONITOR          5   //单向旁听会议,逻辑通道单向联到会议室旁听(增强会议)
 #define F_CF_PARAM			   6   //设置会议参数(增强会议)

								   //所有旁听将会只占用一路会议通道资源(旁听整个会议)  
 #define D_ECF_USEAGC          0x80//会议室使用自动增益控制
 #define D_ECF_CHNUMASK        0x7F//会议室大小掩码	
 
 #define P_CF_OK               0  //指定的功能正确完成
 #define P_CF_FUNCTIONERR      1  //指定的FUNCTION错误
 #define P_CF_CONFNOTEXIST     2  //会议室号非法
 #define P_CF_LOGICHNOTEXIST   3  //会议人逻辑通道非法
 #define P_CF_CONFNOTCREATE    4  //关闭/连接/监听/设置会议参数时指定会议室没有被创建
 #define P_CF_NOCONFCHRES      5  //创建/连接/监听时会议通道资源不足
 #define P_CF_CR_CONFSIZE	   6  //创建时会议室大小错误	
 #define P_CF_CR_NOCONFRES     7  //创建时会议室资源不足或指定会议室已经被创建 
 #define P_CF_LK_LOGICHINCONF  8  //创建/连接时指定的逻辑通道已经在另外的会议室中了
 #define P_CF_UL_LOGICHNOTCONF 9  //断开连接时指定逻辑通道并没有在某个会议中
 #define P_CF_ST_NOTSAMEGROUP  10 //设置会议参数时指定的被监听会议与监听者会议不在同一个组内

#define MSG_ONOFF             8     //通道关闭/打开(不许使用/可以使用)
 #define F_OF_OFF              0   //关闭
 #define F_OF_ON               1   //打开
 #define P_OF_CALLIN           1   //允许呼入
 #define P_OF_CALLOUT          2   //允许呼出
 
#define DOWNMSG_SETPARAM      9    //下传消息设置参数 
 #define F_PM_DEVICE             0  //每个设备参数
 #define F_PM_GLOBAL	         1  //全局参数
 
#define UPMSG_PARAMERROR      9    //上传消息设置参数(或有错误)
 #define F_PM_SLOT				15  //物理时隙位置
 

#define UPMSG_ALARM           10    //上传消息线路告警(软故障)
 #define F_AM_CH				0   //通道告警
  #define P_AM_NO1_CHREOK        0   //线路正常（空闲）
  #define P_AM_NO1_CHREMOTEBLOCK 1   //远端闭塞
  #define P_AM_NO1_CHCONFUSE     2   //100:呼出线路收到对方占用
  #define P_AM_NO1_CHNOCONFIRM   3   //101:呼出时对方占用证实时间太长(>200ms),实际为>800ms
  #define P_AM_NO1_CHERRORREL1   4   //102:呼出时对方占用证实后并且在摘机前不正常地释放了线路(>2s)
  #define P_AM_NO1_CHERRORREL2   5   //103:呼出时对方摘机后不正常地释放了线路
  #define P_AM_TB_OK  		     0    //外线上直流电压恢复正常
  #define P_AM_TB_NOPOWER		 6    //外线上没有直流电压(掉线或短路)
 #define F_AM_PCMSYN			1   //PCM同步告警
  #define P_AM_SYNOK             0    //同步正常
  #define P_AM_SYNNOCARRY		 1    //接收无载波
  #define P_AM_SYNFAS		     2    //接收FAS同步错误
  #define P_AM_SYNCAS			 3    //接收CAS MF同步错误（只在CAS方式下）
  #define P_AM_SYNSLIP			 4    //弹性缓冲区滑动
  #define P_AM_SYNALARMA		 5    //远端A位告警
  #define P_AM_SYNALARMY		 6    //远端CAS MF Y位告警（只在CAS方式下）
  #define P_AM_SYNALLONE		 7    //接收全1错误
  #define P_AM_SYNJITTER		 8    //抖动消除电路告警
 #define F_AM_MTP				2    //七号信令MTP告警
  #define P_L3L4_STOP			 1    //所有链路中断
  #define P_L3L4_LINKOK			 2    //链路开通，开通链路号	  
  #define P_L3L4_LINKFAIL        3    //链路中断，失败链路号	  
  #define P_L3L4_DPCOK           4    //DPC 可到达	  
  #define P_L3L4_DPCNOLINK       5    //DPC 不可到达
	  
#define UPMSG_DAMAGE          11    //上传消息硬件板故障(硬故障)

/////////////////热插拔消息
#define MSG_PnP               12    //板插入拔除消息
 #define F_PnP_PLUG            1    //上传消息板插入
 #define F_PnP_QUERYPULL       2    //上传消息希望拔除
 #define F_PnP_PULL            3    //上传消息板拔除
 #define F_PnP_USE             4    //下传消息使用板
 #define F_PnP_UNUSE           5    //下传消息停止使用板(允许拔除)

#define MSG_USERDATA		  13    //用户直接收发信令相关的消息包
 #define F_UD_ITU			    0   //ITU格式
 #define F_UD_USER			    1	//用户自定义格式

#define UPMSG_SYNREF          14    //同步参考
#define DOWNMSG_ADDINDEX      14    //添加一条内存索引(文件形式)

/////////////////多媒体消息
#define MSG_MEDIA             15    //多媒体消息(VOICE,DTMF,FAX,FSK,MODEM,CAS,HDLC)
 #define F_MEDIA_None          0    //空
 #define F_MEDIA_Tx_Release    1    //释放发送资源 
 #define F_MEDIA_Rx_Release    2    //释放接收资源 
 #define F_MEDIA_Tx_File       4    //发送文件
 #define F_MEDIA_Tx_Memory     5    //发送内存 
 #define F_MEDIA_Tx_Index      6    //发送内存索引 
 #define F_MEDIA_Tx_TTStr      7    //文本串
 #define F_MEDIA_Tx_TTSFile    8    //文本文件
 #define F_MEDIA_Tx_DTMF       9    //发送 dtmf
 #define F_MEDIA_Tx_CPSign     10   //播放信号音

 #define F_MEDIA_Rx_File       12    //接收到文件
 #define F_MEDIA_Rx_Memory     13    //分配资源并以内存方式接收 
 #define F_MEDIA_Rx_Pause      14    //接收暂停
 #define F_MEDIA_Rx_Resume     15    //接收重新开始

 #define F_MEDIA_SetParam      11    //设置指定通道的媒体参数(下发)

 #define F_MEDIA_Error         11    //资源操作分配结果(上传)
  #define P_MEDIA_AllocOk        0   //分配成功
  #define P_MEDIA_AllocErr       1   //分配错误
  #define P_MEDIA_BusyErr        2   //使用中
  #define P_MEDIA_FullErr        3   //资源满，无法接受新命令(当前命令被丢掉)
  #define P_MEDIA_TypeErr        4   //错误的资源类型号(可能通道操作的前一种资源还没有释放)
  #define P_MEDIA_FuncErr        5   //不支持的资源操作方式
  #define P_MEDIA_FileErr        6   //文件错误



////////////////定义停止原因 when
#define RESTOP_DTMF     0x01  //收到DTMF停止
#define RESTOP_USER     0x02  //用户停止
#define RESTOP_FINISH   0x03  //自然结束停止
#define RESTOP_COMMERR  0x04  //握手或通讯失败停止
#define RESTOP_CHREL    0x05  //对应的线路通道释放
#define RESTOP_FILERR   0x06  //文件写错误



#ifndef _min_
  #define _min_(a,b) (((unsigned long)a < (unsigned long)b) ? a : b)
#endif

#define MAXCALLLEN     32  //最大主叫被叫号码长度
/////////////////定义消息结构
struct RMSG  //用于传送消息,分短结构(8字节长)和长结构(264字节长).
{   
  unsigned short MsgType     : 4;  //消息类型      
  unsigned short Function    : 4;  //功能
  unsigned short Param       : 8;  //参数
  
  								   
  unsigned short ChIndex     :11;  //通道逻辑号(0--2047) 
  unsigned short ChType      : 5;  //通道类型(见CHTYPE定义)	       
  
  unsigned short LinkChIndex :11; //相关的另一个通道逻辑号(0--2047)      
  unsigned short LinkChType  : 5; //相关的另一个通道的线路类型      
  
  unsigned short DataLen     : 8;  //0表示没有后面的大union部分,否则表示union的实际有效长度
				                   //1--255表示有后面的大union部分			  
  unsigned short DataSpec    : 8; //数据描述
// DataSpec低5bit;     //被叫长度(0--31)	  
// 主叫长度(0--31)= DataLen>MAXCALLLEN?DataLen-MAXCALLLEN:0;	  


  union  //只有当 DataLen >0  时有效
  {
    struct
	{
      char  Called[MAXCALLLEN];	//被叫号码(0--31)	    	  
      char  Caller[MAXCALLLEN];    //主叫号码(0--31)(可能包括逗号用于隔开主叫和原始被叫)	    	  
	};	
	unsigned char  DataB[256];  
	unsigned short DataW[128];	
	unsigned long  DataDW[64];	
  };

public:
  RMSG() //构造函数
  {Clear();};
  
  void Clear() 
  { ((long *)this)[0]=((long *)this)[1]=0;  };

  void InitDataLen(unsigned short datalen=0)
  {	DataLen=datalen; }

  void InitCallNull()
  {DataLen=DataSpec=0;};

  void SetToNullMsg(unsigned short datalen) //设置为空消息
  {Clear();InitDataLen(datalen);};

  unsigned short AddByte(unsigned char data)
  {if (DataLen==255) return 0; DataB[DataLen++]=data;
       return 1;};

  unsigned short AddWord(unsigned short data)
  {if (DataLen>=254) return 0; DataB[DataLen++]=data&0xFF;
       DataB[DataLen++]=data>>8;return 2;};

  unsigned short AddStr(char *data)
  {
     unsigned short len=strlen(data);
	 if(len+DataLen>=255) 
		 return 0; //不能添加
	 RtlCopyMemory(&DataB[DataLen],data,len+1);//加0	 
	 DataLen+=len+1;	 
	 return len+1;
  }
///////used for call msg
  
  unsigned short GetCalledLen()
  {return DataSpec % MAXCALLLEN;};

  unsigned short GetCallerLen()
  {return DataLen > MAXCALLLEN ? DataLen-MAXCALLLEN:0;};

  void SetCalledLen(unsigned short len)
  {
	 DataSpec = len % MAXCALLLEN;
	
	 if(0==GetCallerLen())//原来无主叫号码,调整总长度
	    DataLen=DataSpec ;	
  };

  void SetCallerLen(unsigned short len)
  {
    if(len>0)
      DataLen=MAXCALLLEN+(len % MAXCALLLEN);
    else
      DataLen=DataSpec;
  }

  void ClearCalled()
  {SetCalledLen(0);};

  void ClearCaller()
  {SetCallerLen(0);};

  void AppendCaller(char code)
  {
	  unsigned short len=GetCallerLen();
	  if(len<MAXCALLLEN-1) 
	  {
	    Caller[len++]=code; 
        SetCallerLen(len);
		Caller[len]=0; 
	  }
  }

  void AppendCalled(char code)
  {
	  unsigned short len=GetCalledLen();
	  if(len<MAXCALLLEN-1)
	  {
		  Called[len++]=code; 
		  SetCalledLen(len);
		  Called[len]=0; 
	  }
  }

  void AppendCalledTerm() //添加结尾0
  {
	unsigned short len=GetCalledLen();
	if(len) //当长度为0时，不能使用Called
	  if(len<MAXCALLLEN)
  	    Called[len]=0; 
  }

  void AppendCallerTerm() //添加结尾0
  {
    unsigned short len=GetCallerLen();
	if(len) //当长度为0时，不能使用Caller
	  if(len<MAXCALLLEN)
  	    Caller[len]=0; 
  }
  
  void AppendCaller(const char *phone,unsigned short len) 
  {     
	 if(len>0)
	 {
       unsigned short l=GetCallerLen();
       if(l+len>(MAXCALLLEN-1))
		 len=MAXCALLLEN-1-l;
       RtlCopyMemory(&Caller[l], phone,len);	 
	   SetCallerLen(len+l);
	   //AppendCallerTerm();
	 }
  }

  void AppendCaller(const char *phone) 
  {
    unsigned short len=strlen(phone);
	AppendCaller(phone,len); 
  }
  
  
  void AppendCalled(const char *phone,unsigned short len) 
  {
	 if(len>0)
	 {
       unsigned short l=GetCalledLen();
       if(l+len>(MAXCALLLEN-1))
		 len=MAXCALLLEN-1-l;
       RtlCopyMemory(&Called[l], phone,len);	 
	   SetCalledLen(len+l);
	   //AppendCalledTerm();
	 }
  }

  void AppendCalled(const char *phone) 
  {
    unsigned short len=strlen(phone);
	AppendCalled(phone,len); 
  }

  const char *GetCalled()
  {	  return GetCalledLen()?Called:"";  }//注意当长度为0时，不能使用called
  
  const char *GetCaller()
  {	  return GetCallerLen()?Caller:"";  }//注意当长度为0时，不能使用caller
///////used for call msg

  
  unsigned short IsNullMsg(){return (MsgType==MSG_NULL);};

  unsigned short DsOfMsg()//消息的大小(双字表示)
  {return 2+((unsigned short)DataLen+3)/4;};

  const RMSG &operator =(const RMSG &m) //定义结构赋值操作
  {
    unsigned int i;
    ((long *)this)[0]=((long *)&m)[0];     //copy first 4 bytes
	((long *)this)[1]=((long *)&m)[1]; //copy second 4 bytes
		
	if(DataLen) //m is long struct,so copy union data
	{
      for(i=0;i<((unsigned int)DataLen+3)/4;i++)
	    ((long *)this)[2+i]=((long *)&m)[2+i];
	}
    return *this;
  };
};

void inline CV8bitTo4bit(unsigned char *b8,unsigned char *b4,unsigned short num)//转换8bit川到4bit川
{//注意b4 和b8可以重叠
  for(unsigned short i=0;i<num;i++)
  {
    if(i&1)
      b4[i/2]|=b8[i]<<4;  
    else
	  b4[i/2]=b8[i] & 0x0F;     
  }
}

void inline CV4bitTo8bit(unsigned char *b4,unsigned char *b8,unsigned short num)//转换8bit川到4bit川
{//注意b4 和b8可以重叠
  for(short i=num-1;i>=0;i--)
  {    
    if(i&1)
      b8[i]=b4[i>>1]>>4;  
    else
	  b8[i]=b4[i>>1] & 0x0F;     
  }
}

#define ALARMSYNOK        0    //同步正常
#define ALARMSYNNOCARRY   1    //接收无载波
#define ALARMSYNFAS       2    //接收FAS同步错误
#define ALARMSYNCAS       3    //接收CAS MF同步错误（只在CAS方式下）
#define ALARMSYNSLIP      4    //弹性缓冲区滑动
#define ALARMSYNALARMA    5    //远端A位告警
#define ALARMSYNALARMY    6    //远端CAS MF Y位告警（只在CAS方式下）
#define ALARMSYNALLONE    7    //接收全1错误
#define ALARMSYNJITTER    8    //抖动消除电路告警



inline const char *GetChTypeNameStr(unsigned short chtype)//取通道类型的名字
{
 switch (chtype)
 {
  case CHTYPE_DTNO1:	return "一号通道";
  case CHTYPE_DTNO7:	return "七号通道";
  case CHTYPE_DTDSS1:	return "ISDN通道";
  case CHTYPE_DTNO7ISUP:return "ISUP通道";
  case CHTYPE_MTP3:		return "MTP3传输层通道";
  case CHTYPE_TB:		return "外线通道";
  case CHTYPE_AGENT:	return "内线通道";
  case CHTYPE_HB:		return "高阻通道";
  
  case RESTYPE_VOCPCM:  return "语音资源";
  case RESTYPE_DTMF  :  return "DTMF资源";
  case RESTYPE_FAX   :  return "传真资源";
  case RESTYPE_FSK   :  return "fsk资源";
  case RESTYPE_MODEM :  return "modem资源";
  case RESTYPE_HDLC  :  return "HDLC资源";
  case RESTYPE_CAS   :  return "cas资源";
  case RESTYPE_VOIP  :  return "VOIP资源";
  case RESTYPE_BIG   :  return "大数通讯资源";
  case RESTYPE_MFC   :  return "mfc资源";
  case RESTYPE_VC    :  return "变声资源";
  case RESTYPE_CONF  :  return "会议室资源"; 
  case RESTYPE_CONFCH:  return "会议通道资源"; 
  case RESTYPE_ECONF  : return "增强会议室资源"; 
  case RESTYPE_ECONFCH: return "增强会议通道资源"; 
  default:				return "未知类型";
 }
}
 
inline const char *GetResErrReason(unsigned char rsn)//返回媒体分配结果
{
 switch(rsn)
  {
  case P_MEDIA_AllocOk: return "分配媒体资源成功";
  case P_MEDIA_AllocErr: return "分配媒体资源失败";
  case P_MEDIA_BusyErr: return "资源占用中";
  case P_MEDIA_FullErr: return "资源满，丢弃";
  case P_MEDIA_TypeErr: return "媒体资源类型错误";
  case P_MEDIA_FuncErr: return "不支持的资源操作方式";
  case P_MEDIA_FileErr: return "文件操作错误";
  default:return "未知错误";
  }
}


inline const char *GetMsgTypeNameStr(unsigned short msgtype, int rt)
{
	if(rt == 0)		//发送消息
	{
		switch(msgtype)
		{
		case(MSG_RELEASE):		return "释放通道";
		case(MSG_CALL):			return "通道呼出";
		case(MSG_ACM):			return "地址收全";
		case(MSG_CALLEDACK):	return "我方应答";
		case(MSG_FLASH):		return "拍叉簧";
		case(DOWNMSG_ROUTER):	return "通道交换";
		case(MSG_CONF):			return "电话会议";
		case(MSG_ONOFF):		return "通道开关";
		case(DOWNMSG_SETPARAM):	return "设置参数";
		case(MSG_USERDATA):		return "信令消息包";
		case(DOWNMSG_ADDINDEX): return "添加语音索引";
		case(MSG_MEDIA):		return "多媒体";
		default:				return "未知消息"; 
		}
	}
	else	//接收消息
	{
		switch(msgtype)
		{
		case(MSG_RELEASE):			return "通道释放";
		case(MSG_CALL):				return "通道呼入";
		case(MSG_ACM):				return "地址收全";
		case(MSG_CALLEDACK):		return "对方应答";
		case(MSG_FLASH):			return "拍叉簧";
		case(UPMSG_CALLOUTCHANNEL):	return "分配呼出通道";
		case(MSG_CONF):				return "电话会议结果";
		case(MSG_ONOFF):			return "通道开关";
		case(UPMSG_PARAMERROR):		return "参数设置错误";
		case(UPMSG_ALARM):			return "线路告警";
		case(UPMSG_DAMAGE):			return "硬件故障";
		case(MSG_USERDATA):		    return "信令消息包";
		case(MSG_MEDIA):			return "多媒体";
		default:					return "未知消息"; 
		}
	}
}

inline const char *GetReleaseFuncNameStr(unsigned short func)
{
	switch(func)
	{
	case(F_R_NORMAL):		return "普通释放";
	case(F_R_CALLOUTERR):	return "呼出失败释放";
	default:				return "未知";
	}
}

inline const char *GetReleaseParamNameStr(unsigned short param)
{
	switch(param)
	{
	case(P_R_NODIALTONE):	return "无拨号音";
	case(P_R_NOANSWER):		return "没有响应";
	case(P_R_RINGTIMEOUT):	return "振铃超时";
	case(P_R_LINEUSED):		return "线路占用";
	case(P_R_CONGESTION):	return "设备拥塞";
	case(P_R_INCOMPLETE):	return "地址不全";
	case(P_R_FAILURE):		return "呼叫故障";
	case(P_R_CALLBARRING):	return "接入拒绝";
	case(P_R_BUSYLOCAL):	return "市话忙";
	case(P_R_BUSYDDD):		return "长话忙";
	case(P_R_NULLDN):		return "空号";
	case(P_R_NODIGITALPATH):return "无数字通路";
	default:				return "未知";
	}
}

inline const char *GetCallFuncNameStr(unsigned short func)
{
	switch(func)
	{
	case(F_C_NORMAL):		return "普通呼叫";
	case(F_C_OVLP):			return "重叠呼叫";
	default:				return "未知功能";
	}
}

inline const char *GetCallParamNameStr(unsigned short chtype, unsigned short param)
{
	switch(param)
	{
	case(P_C_NORMAL):
		switch(chtype)
		{
			case(CHTYPE_AGENT):		return "标准振铃声+FSK主叫发送";
			case(CHTYPE_TB):		return "检测拨号音";
			case(CHTYPE_DTNO1):
			case(CHTYPE_DTNO7):
			case(CHTYPE_DTNO7ISUP):
			case(CHTYPE_DTDSS1):	return "标准呼叫";
		}
		break;
	case(P_C_1):
		switch(chtype)
		{
			case(CHTYPE_AGENT):		return "DTMF主叫发送+标准振铃声";
			case(CHTYPE_TB):		return "不检测拨号音";
			case(CHTYPE_DTNO1):		return "KA(1),主叫为普通,定期";
			case(CHTYPE_DTNO7):
			case(CHTYPE_DTNO7ISUP):
			case(CHTYPE_DTDSS1):	return "不完全呼出";
		}
		break;
	case(P_C_2):
		switch(chtype)
		{
			case(CHTYPE_AGENT):		return "DTMF主叫发送+第2种振铃声";
			case(CHTYPE_DTNO1):		return "KA(2),主叫为普通,立即";
			case(CHTYPE_DTNO7):		
			case(CHTYPE_DTNO7ISUP):	return "主叫号码显示限制";
		}
		break;
	case(P_C_3):
		switch(chtype)
		{
		case(CHTYPE_AGENT):			return "DTMF主叫发送+第3种振铃声";
		case(CHTYPE_DTNO1):			return "KA(3),主叫为普通,营业处";
		case(CHTYPE_DTNO7):
		case(CHTYPE_DTNO7ISUP):		return "不完全呼出+主叫号码显示限制";
		}
		break;
	case(P_C_4):
		switch(chtype)
		{
		case(CHTYPE_AGENT):			return "DTMF主叫发送+第4种振铃声";
		case(CHTYPE_DTNO1):			return "KA(2),备用";
		}
		break;
/*				
	case(P_C_5):
		switch(chtype)
		{
		case(CHTYPE_DTDSS1):		return "KA(5),主叫为普通,免费";	
		}
		break;
*/				
	//default:					
	}
	return "未知参数";

}

inline const char *GetAcmFuncNameStr(unsigned short func)
{
	switch(func)
	{
	case(F_ACM_IDLE):		return "空闲可用";
	case(F_ACM_BUSY):		return "线路忙";
	case(F_ACM_NULL):		return "线路空号";
	case(F_ACM_FAULT):		return "呼叫失败";
	default:				return "未知功能";
	}
}

inline const char *GetCallOutChannelFuncNameStr(unsigned short func)
{
	switch(func)
	{
	case(F_CC_HANDLE):		return "分配到句柄";
	case(F_CC_NORMAL):		return "分配到通道";
	case(F_CC_NOCHANNEL):	return "分配不到通道";
	default:				return "未知功能";
	}
}

inline const char *GetCallOutChannelParamNameStr(unsigned short param)
{
	switch(param)
	{
	case(P_CC_CHBUSY):			return "通道使用中";
	case(P_CC_CHCALLMASK):		return "配置中通道呼出禁止";
	case(P_CC_CHNOTEXIST):		return "通道号不存在";
	case(P_CC_PCMNOSYN):		return "数字线端口没有同步";
	case(P_CC_CANNOTASSIGNCH):	return "不支持用户设置的呼出通道指定方式";
	case(P_CC_REMOTENOANSWER):	return "对方没有响应";
	case(P_CC_DPCUNAVAILABLE):	return "DPC不可到达";
	case(P_CC_ADDRESSERR):		return "号码设置错误";
	default:					return "未知参数";
	}
}

inline const char *GetConfFuncNameStr(unsigned short func)
{
	switch(func)
	{
	case(F_CF_CREATE):		return "创建会议";
	case(F_CF_CLOSE):		return "关闭会议";
	case(F_CF_LINKCH):		return "参加会议";
	case(F_CF_UNLINKCH):	return "退出会议";
	case(F_CF_MONITOR):		return "监听会议";
	case(F_CF_PARAM):		return "会议参数";
	default:				return "未知功能";
	}
}

inline const char *GetAlarmFuncNameStr(unsigned short func)
{
	switch(func)
	{
	case(F_AM_CH):		return "通道告警";
	case(F_AM_PCMSYN):	return "PCM同步告警";
	case(F_AM_MTP):     return "七号信令MTP告警";
	default:			return "未知功能";
	}
}

inline const char *GetAlarmParamNameStr(unsigned short func, unsigned short param)
{
	switch(func)
	{
	case(F_AM_CH):
		switch(param)
		{
		case(P_AM_NO1_CHREOK):			return "线路正常";
		case(P_AM_NO1_CHREMOTEBLOCK):	return "远端闭塞";
		case(P_AM_NO1_CHCONFUSE):		return "收到对方占用";
		case(P_AM_NO1_CHNOCONFIRM):		return "占用证实时间太长";
		case(P_AM_NO1_CHERRORREL1):		return "占用证实后且在摘机前异常释放";
		case(P_AM_NO1_CHERRORREL2):		return "对方摘机后异常释放";
		case(P_AM_TB_NOPOWER):			return "外线无直流电压";
		}
		break;

	case(F_AM_PCMSYN):
		switch(param)
		{
		case(P_AM_SYNOK):		return "同步正常";
		case(P_AM_SYNNOCARRY):	return "接收无载波";
		case(P_AM_SYNFAS):		return "接收FAS同步错误";
		case(P_AM_SYNCAS):		return "接收CAS MF同步错误";
		case(P_AM_SYNSLIP):		return "弹性缓冲区滑动";
		case(P_AM_SYNALARMA):	return "远端A位告警";
		case(P_AM_SYNALARMY):	return "远端CAS MF Y位告警";
		case(P_AM_SYNALLONE):	return "接收全1错误";
		case(P_AM_SYNJITTER):	return "抖动消除电路告警";
		}
		break;

	case (F_AM_MTP):
		switch(param)
		{
		case P_L3L4_STOP:		return "所有链路中断";
		case P_L3L4_LINKOK:		return "链路开通，开通链路号";	  
		case P_L3L4_LINKFAIL:   return "链路中断，失败链路号";	  
		case P_L3L4_DPCOK:      return "DPC 可到达";	  
		case P_L3L4_DPCNOLINK:  return "DPC 不可到达";
		}
		break;
	}
	return "未知参数";
}

inline const char *GetMediaFuncNameStr(unsigned short func)
{
	switch(func)
	{
	case(F_MEDIA_Tx_Release):	return "释放发送资源";
	case(F_MEDIA_Rx_Release):	return "释放接收资源";
	case(F_MEDIA_Tx_File):		return "发送文件";
	case(F_MEDIA_Tx_Memory):	return "发送内存";
	case(F_MEDIA_Tx_Index):		return "发送内存索引";
	case(F_MEDIA_Tx_TTStr):		return "文本串";
	case(F_MEDIA_Tx_TTSFile):	return "文本文件";
	case(F_MEDIA_Tx_DTMF):		return "发送DTMF";
	case(F_MEDIA_Tx_CPSign):	return "播放信号音";
	case(F_MEDIA_Rx_File):		return "接收到文件";
	case(F_MEDIA_Rx_Memory):	return "内存方式接收";
	case(F_MEDIA_Rx_Pause):		return "接收暂停";
	case(F_MEDIA_Rx_Resume):	return "接收重新开始";
	case(F_MEDIA_Error):		return "资源参数/结果";//(下发为参数，上传为结果)
	default:					return "未知功能";
	}
}


inline const char *GetStopReason(unsigned char rsn)//返回媒体停止原因
{
  switch(rsn)
  {
  case RESTOP_DTMF: return "被DTMF打断";
  case RESTOP_USER: return "被程序终止";
  case RESTOP_FINISH: return "成功完成";
  case RESTOP_COMMERR: return "通讯错误";
  case RESTOP_CHREL: return "相关线路释放";
  case RESTOP_FILERR: return "写入文件时失败";
  default:return "未知错误";
  }
}

inline const char *GetRouterFuncNameStr(unsigned short func)
{
	switch(func)
	{
	case(F_R_SILENCE):		return "输出静音";
	case(F_R_MONITOR):		return "单向监听";
	case(F_R_TALK):			return "双向通话";
	case(F_R_TALK_RP):      return "双向通话并混合语音";
	case(F_R_MONITOR_P):    return "连接放音资源";	
	case(F_R_TRANSTO_CT):	return "发送到CT总线一个时隙";
	case(F_R_MONITOR_CT):	return "监听CT总线一个时隙";
	default:				return "未知类型";
	}
}

inline const char *GetCPToneNameStr(unsigned short cp)
{
	switch(cp)
	{
	case 1:return "拨号音";
	case 2:return "回铃音";
	case 3:return "忙音";
	default:return "未知信号音";
	}
}

inline const char *GetVocFileFormatNameStr(unsigned short fmt)
{
	switch(fmt)
	{
    case 0:return "ALAW PCM";
	case 1:return "WAVE";
	case 2:return "CIRRUS ADPCM";
	case 3:return "G.723.1";
	case 4:return "Dialogic 6K";
	case 5:return "Dialogic 8K";
	default:return "未知格式";
	}
}

inline const char *GetConfResultStr(unsigned char result)
{
	switch(result)
	{		
	case P_CF_OK               :return "正确完成";
	case P_CF_FUNCTIONERR      :return "FUNCTION 错误";
	case P_CF_CONFNOTEXIST     :return "会议室号非法";
	case P_CF_LOGICHNOTEXIST   :return "会议人逻辑通道非法";
	case P_CF_CONFNOTCREATE    :return "指定会议室没有被创建";
	case P_CF_NOCONFCHRES      :return "会议通道资源不足";
	case P_CF_CR_CONFSIZE	   :return "创建会议室大小错误";	
	case P_CF_CR_NOCONFRES     :return "创建会议室资源不足或已被创建"; 
	case P_CF_LK_LOGICHINCONF  :return "会议人逻辑通道已在另外会议室中";
	case P_CF_UL_LOGICHNOTCONF :return "会议人逻辑通道没有在会议中";
	case P_CF_ST_NOTSAMEGROUP  :return "被监听会议与监听者会议不在同一个组";
	default:return "未知结果";
	}
}
#endif
