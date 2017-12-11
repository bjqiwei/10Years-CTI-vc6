#ifndef __ISUP_DLL__EX__
#define __ISUP_DLL__EX__

enum ISUP_MSG_TYPE_EX
{
	IM_BACKWARD_CALL_INDICATORS_EX,// 3.5 后向呼叫指示码
	IM_EVENT_INFORMATION_EX,// 3.21 事件信息（必备字段）
	IM_FACILITY_INDICATOR_EX,// 3.22 功能指示码（必备字段）
	IM_INFORMATION_INDICATORS_EX,// 3.28 信息指示码
	IM_INFORMATION_REQUEST_INDICATORS_EX,// 3.29 信息请求指示码
	IM_NATURE_OF_CONNECTION_INDICATORS_EX,// 3.35 接续性质指示码
	IM_FORWARD_CALL_INDICATORS_EX,// 3.23 前向呼叫指示码
	IM_CALLING_PARTY_CATEGORY_EX,// 3.11 主叫用户类别
	IM_TRANSMISSION_MEDIUM_REQUIREMENT_EX,// 3.54 传输媒介要求
	IM_SUSPEND_RESUME_INDICATORS_EX,// 3.52 暂停/恢复指示码
	IM_CIRCUIT_GROUP_SUPERVISION_MESSAGE_TYPE_EX,// 3.13 电路群监视消息指示码
	IM_RANGE_AND_STATUS_EX,// 3.43 范围和状态
	IM_CIRCUIT_STATE_INDICATOR_EX,// 3.14 电路状态指示码
	IM_CAUSE_INDICATORS_EX,// 3.12 原因指示码
	IM_CALLED_PARTY_NUMBER_EX,// 3.9 被叫用户号码（只用于起始地址消息）
	IM_SUBSEQUENT_NUMBER_EX,// 3.51 后续号码
	IM_USER_TO_USER_INFORMATION_EX,// 3.61 用户至用户消息
	IM_CALLING_PARTY_NUMBER_EX,// 3.10 主叫用户号码
	IM_REDIRECTION_NUMBER_EX,// 3.46 改发号码（类被叫号码）
	IM_CALL_HISTORY_INFORMATION_EX,// 3.7 呼叫历史
	IM_PROPAGATION_DELAY_COUNTER_EX,// 3.42 传播时延（毫秒）
	IM_OPTIONAL_BACKWARD_CALL_INDICATORS_EX,// 3.37 任选后向呼叫指示码
	IM_OPTIONAL_FORWARD_CALL_INDICATORS_EX,// 3.38 任选前向呼叫指示码
	IM_ORA_CALLEE_PARTY_NUMBER_EX,// 3.39 原被叫号码
	IM_ACCESS_TRANSPORT_EX,// 接入传送
	IM_MESSAGE_COMPATIBILITY_INFORMATION_EX,// 3.33 消息兼容性信息
	IM_PARAM_COMPATIBILITY_INFORMATION_EX,// 3.41 参数兼容性信息
	IM_REDIRECTING_NUMBER_EX,// 3.44 改发号码
	IM_REDIRECTION_INFORMATION_EX, //3.45 改发信息

	// 内部转换标识，兼容老函数
	_IM_CALLING_PARTY_EX,
	_IM_FORWARD_CALL_INDICATORS_EX,
	_IM_REDIRECTION_INFORMATION_EX,
	_IM_EVENT_INFORMATION_EX,
	_IM_CALLED_PARTY_NUMBER_EX,
	
};

// 3.5 后向呼叫指示码
typedef struct _BACKWARD_CALL_INDICATORS_PARAM_EX
{
	BYTE bChargeIndicator ;
		//计费指示码
		//00：无指示
		//01：不计费
		//10：计费
		//11：备用
	BYTE bCalledStatusIndicator ;
		//被叫用户状态指示码
		//00：无指示
		//01：用户空闲
		//10：空闲时接续
		//11：备用
	BYTE bCalledCategoryIndicator ;
		//被叫用户类别指示码
		//00：无指示
		//01：普通用户
		//10：投币电话
		//11：备用
	BYTE bEndToEndMethodIndicator ;
		//端到端方法指示码
		//00：无端到端方法可利用
		//01：传递方法可利用
		//10：SCCP方法可利用
		//11：传递和SCCP方法可利用
	BYTE bInterworkingIndicator ;
		//互通指示码
		//0：不会碰到互通
		//1：碰到互通
	BYTE bEndToEndInformationIndicator ;
		//端到端信息指示码
		//0：无端到端信息可利用
		//1：端到端信息可利用
	BYTE bISDNUserPartIndicator ;
		//ISDN用户部分指示码
		//0：终端接入非ISDN
		//1：终端接入ISDN
	BYTE bHoldingIndicator ;
		//保持指示码
		//0：保持未请求
		//1：保持请求
	BYTE bISDNAccessIndicator ;
		//ISDN接入指示码
		//0：终端接入非ISDN
		//0：终端接入ISDN
	BYTE bEchoControlDeviceIndicator ;
		//回音控制器件指示码
		//0：来话半回音控制器件未包括
		//1：来话半回音控制器件包括
	BYTE bSCCPMethodIndicator ;
		//SCCP方法指示码
		//00：无指示
		//01：无接续方法可利用
		//10：接续方法可利用
		//11：无接续和面向接续方法可利用
}BACKWARD_CALL_INDICATORS_PARAM_EX, *LPBACKWARD_CALL_INDICATORS_PARAM_EX;

// 3.21 事件信息（必备字段）
typedef struct _EVENT_INFORMATION_PARAM_EX
{
	BYTE bEventIndicator;
	//事件指示码
	//0000000：备用
	//0000001：通知
	//0000010：进行
	//0000011：带内信息或一个合适型式现可获得
	//0000100：遇忙呼叫前转
	//0000101：无应答呼叫前转
	//0000110：无条件呼叫前转
	BYTE bReserved ;
}EVENT_INFORMATION_PARAM_EX, *LPEVENT_INFORMATION_PARAM_EX;

// 3.18 导通指示码

// 3.22 功能指示码（必备字段）
typedef struct _FACILITY_INDICATOR_PARAM_EX
{
	BYTE bFacilityIndicator;
}FACILITY_INDICATOR_PARAM_EX, *LPFACILITY_INDICATOR_PARAM_EX;

// 3.28 信息指示码
typedef struct _INFORMATION_INDICATORS_PARAM_EX
{
	BYTE bCallingPartyAddressResponseIndicator ;
		//主叫用户地址响应指示码
		//00：主叫用户地址不包括
		//01：主叫用户地址不可获得
		//10：备用
		//11：主叫用户地址包括
	BYTE bHoldProvidedIndicator ;
		//保持提供指示码
		//0：保持不提供
		//1：保持提供
	BYTE bReserved1 ;
	BYTE bCallingPartyCategoryResponseIndicator ;
		//主叫用户类别响应指示码
		//0：主叫用户类别不包括
		//1：主叫用户类别包括
	BYTE bChargeInformationResponseIndicator ;
		//计费信息指示码
		//0：计费信息不包括
		//1：计费信息包括
	BYTE bSolicitedInformationIndicator ;
		//请求信息指示码
		//0：请求
		//1：未请求
	BYTE bReserved2;
}INFORMATION_INDICATORS_PARAM_EX, *LPINFORMATION_INDICATORS_PARAM_EX;

// 3.29 信息请求指示码
typedef struct _INFORMATION_REQUEST_INDICATORS_PARAM_EX
{
	BYTE bCallingPartyAddressRequestIndicator ;
	BYTE bHoldingIndicator ;
	BYTE bReserved1 ;
	BYTE bCallingPartyCategoryRequestIndicator ;
	BYTE bChargeInformationRequestIndicator ;
	BYTE bReserved2 ;
	BYTE bMaliciousCallIdentificationRequestIndicator ;
	BYTE bReserved3;
}INFORMATION_REQUEST_INDICATORS_PARAM_EX, *LPINFORMATION_REQUEST_INDICATORS_PARAM_EX;

// 3.35 接续性质指示码
typedef struct _NATURE_OF_CONNECTION_INDICATORS_PARAM_EX
{
	BYTE bSatelliteIndicator ;
		//卫星指示码
		//00：接续中无卫星电路
		//01：接续中有一条卫星电路
		//10：接续中有两条卫星电路
		//01：备用
	BYTE bContinuityCheckIndicator ;
		//导通检验指示码
		//00：不需要导通检验
		//01：在这条电路上需要导通检验
		//10：在前面电路上已做导通检验
		//11：备用
	BYTE bEchoControlDeviceIndicator ;
		//回声控制器件指示码
		//0：去话半回声控制器件不包括
		//1：去话半回声控制器件包括

	BYTE bReserved ;
}NATURE_OF_CONNECTION_INDICATORS_PARAM_EX, *LPNATURE_OF_CONNECTION_INDICATORS_PARAM_EX;

// 3.23 前向呼叫指示码
typedef struct _FORWARD_CALL_INDICATORS_PARAM_EX
{
	BYTE bNationalInternationalCallIndicator ;
		//国内/国际呼叫指示码
		//0：呼叫作为一个国内呼叫处理
		//1：呼叫作为一个国际呼叫处理
	BYTE bEndToEndMethodIndicator ;
		//端到端方法指示码
		//00：端到端方法不可获得（仅可获得逐段链路转接方法）
		//01：传递（pass along）方法可获得
		//10：SCCP方法可获得
		//11：传递及SCCP方法可获得
	BYTE bInterworkingIndicator ; 
		//互通指示码
		//0：不会碰到互通
		//1：会碰到互通
	BYTE bEndToEndInformationIndicator ;
		//端到端信息指示码
		//0：无端到端信息可利用
		//1：端到端信息可利用
	BYTE bISDNUserPartIndicator ;
		//ISDN用户部分指示码
		//0：非全程应用ISDN用户部分
		//1：全程应用ISDN用户部分
	BYTE bISDNUserPartPreferenceIndicator ;
		//ISDN用户部分优先指示码
		//00：ISDN用户部分全程优先
		//01：非全程需要ISDN用户部分
		//10：全程需要ISDN用户部分
		//11：备用
	BYTE bISDNAccessIndicator ;
		//ISDN接入指示码
		//0：始发端接入非ISDN
		//1：始发端接入ISDN
	BYTE bSCCPMethodIndicator ;
		//SCCP方法指示码
		//00：无指示
		//01：无接续方法可获得
		//10：接续方法可获得
		//11：无接续及面向接续方法可获得
	BYTE bReserved1 ;
	BYTE bReserved2 ;
}FORWARD_CALL_INDICATORS_PARAM_EX, *LPFORWARD_CALL_INDICATORS_PARAM_EX;

// 3.11 主叫用户类别
typedef struct _CALLING_PARTY_CATEGORY_PARAM_EX
{
	BYTE bCallingPartyCategory;
		//主叫用户类别
		//00000000：类别未知
		//00000001：话务员，法语
		//...
		//00001010：普通主叫用户
		//00001011：优先主叫用户
		//00001100：数据呼叫
		//00001101：测试呼叫
		//00001110：备用
		//00001111：公用（投币）电话
		//00001001：可用于指明主叫用户是一个国内话务员
}CALLING_PARTY_CATEGORY_PARAM_EX, *LPCALLING_PARTY_CATEGORY_PARAM_EX;

// 3.54 传输媒介要求
typedef struct _TRANSMISSION_MEDIUM_REQUIREMENT_PARAM_EX
{
	BYTE bTransmissionMediumRequirement;
}TRANSMISSION_MEDIUM_REQUIREMENT_PARAM_EX, *LPTRANSMISSION_MEDIUM_REQUIREMENT_PARAM_EX;

// 3.52 暂停/恢复指示码
typedef struct _SUSPEND_RESUME_INDICATORS_PARAM_EX
{
	BYTE bSuspendResumeIndicator ;
		// 0：ISDN用户发起
		// 1：网络发起
	BYTE bReserved ;
}SUSPEND_RESUME_INDICATORS_PARAM_EX, *LPSUSPEND_RESUME_INDICATORS_PARAM_EX;

// 3.13 电路群监视消息指示码
typedef struct _CIRCUIT_GROUP_SUPERVISION_MESSAGE_TYPE_PARAM_EX
{
	BYTE bCircuitGroupSupervisionMessageTypeIndicator ;
	//类型指示码
	//00：面向维护
	//01：面向硬件故障
	//10：保留
	//11：备用
	BYTE bReserved ;	
}CIRCUIT_GROUP_SUPERVISION_MESSAGE_TYPE_PARAM_EX, *LPCIRCUIT_GROUP_SUPERVISION_MESSAGE_TYPE_PARAM_EX;

// 3.43 范围和状态
typedef struct _RANGE_AND_STATUS_PARAM_EX
{
	BYTE bRange;
	BYTE bStatus[32];
}RANGE_AND_STATUS_PARAM_EX, *LPRANGE_AND_STATUS_PARAM_EX;

// 3.14 电路状态指示码
typedef struct _CIRCUIT_STATE_INDICATOR_PARAM_EX
{
	BYTE bCircuitState[32];
}CIRCUIT_STATE_INDICATOR_PARAM_EX, *LPCIRCUIT_STATE_INDICATOR_PARAM_EX;

// 3.12 原因指示码
typedef struct _CAUSE_INDICATORS_PARAM_EX
{ 
	BYTE bLocation ; 
		//位置
		//0 0 0 0		user (U)
		//0 0 0 1		private network serving the local user (LPN)
		//0 0 1 0		public network serving the local user (LN)
		//0 0 1 1		transit network (TN)
		//0 1 0 0		public network serving the remote user (RLN)
		//0 1 0 1		private network serving the remote user (RPN)
		//0 1 1 1		international network (INTL)
		//1 0 1 0		network beyond interworking point (BI)
		//1 1 0 0		reserved for national use
		//1 1 0 1		reserved for national use
		//1 1 1 0		reserved for national use
		//1 1 1 1		reserved for national use
	BYTE bReserved ;
	BYTE bCodingStandard ;		
		//编码标准
		//0 0		ITU-T standardized coding, as described below
		//0 1		ISO/IEC standard (see Note)
		//1 0		national standard (see Note)
		//1 1		standard specific to identified location (see Note)
	BYTE bExtensionIndicator1 ;	
		//扩充
		//0		octet continues through the next octet (e.g. octet 1 to 1a)
		//1		last octet

	BYTE bRecommendation ;		//原因值
	BYTE bExtensionIndicator2 ;	//扩充
	BYTE bDiagnosis[10];
}CAUSE_INDICATORS_PARAM_EX, *LPCAUSE_INDICATORS_PARAM_EX;

// 3.9 被叫用户号码（只用于起始地址消息）
typedef struct _CALLED_PARTY_NUMBER_PARAM_EX
{
	BYTE bNatureOfAddressIndicator ;
		//地址性质指示码
		//000000：备用
		//000001：用户号码
		//000010：不知
		//000011：国内（有效）号码
		//000100：国际号码
	BYTE bOddEvenIndicator ;
		//奇/偶指示码
		//0：偶数个地址号码
		//1：奇数个地址号码
	BYTE bReserved ;
	BYTE bNumberingPlanIndicator ;
		//号码计划指示码
		//000：备用
		//001：ISDN（电话）号码计划（E.164）
		//010：备用
		//011：数据号码计划（X.121）
		//100：用户电报号码计划
		//101：保留用于国内应用
		//110：保留用于国内应用
		//111：备用
	BYTE InternalNetworkNumberIndicator ;
		//内部网络号码指示码
		//0：编路至内部网络号码允许
		//1：编路至内部网络号码不允许
	BYTE bAddressSignal[17];
}CALLED_PARTY_NUMBER_PARAM_EX, *LPCALLED_PARTY_NUMBER_PARAM_EX;

// 3.51 后续号码
typedef struct _SUBSEQUENT_NUMBER_PARAM_EX
{
	BYTE bReserved ;
	BYTE bOddEvenIndicator ;
	BYTE bAddressSignal[7];
}SUBSEQUENT_NUMBER_PARAM_EX, *LPSUBSEQUENT_NUMBER_PARAM_EX;

// 3.61 用户至用户消息
typedef struct _USER_TO_USER_INFORMATION_PARAM_EX
{
	BYTE bUserToUserInformation[ MAX_DATALEN - 18 ];//129];
}USER_TO_USER_INFORMATION_PARAM_EX, *LPUSER_TO_USER_INFORMATION_PARAM_EX;

// 3.10 主叫用户号码
typedef struct _CALLING_PARTY_NUMBER_PARAM_EX
{
	BYTE bNatureOfAddressIndicator ;
		//地址性质指示码
		//000000：备用
		//000001：用户号码
		//000010：不知
		//000011：国内（有效）号码
		//000100：国际号码
	BYTE bOddEvenIndicator ;
		//奇/偶指示码
		//0：偶数个地址号码
		//1：奇数个地址号码
	BYTE bScreeningIndicator ;  //屏蔽
		//屏蔽指示码
		//00：备用
		//01：用户提供，验证和通过
		//10：保留
		//11：网络提供
	BYTE bAddressPresentationRestrictedIndicator ;  
		//地址显示限制指示码
		//00：显示允许
		//01：显示限制
		//10：地址不可利用
		//11：备用
	BYTE bNumberingPlanIndicator ;
		//号码计划指示码
		//000：备用
		//001：ISDN（电话）号码计划（E.164）
		//010：备用
		//011：数据号码计划（X.121）
		//100：用户电报号码计划
		//101：保留用于国内应用
		//110：保留用于国内应用
		//111：备用
	BYTE bNumberIncompleteIndicator ;
		//主叫用户号码不全指示码
		//0：完全
		//1：不完全
	BYTE bAddressSignal[17];
}CALLING_PARTY_NUMBER_PARAM_EX, *LPCALLING_PARTY_NUMBER_PARAM_EX;

// 3.46 改发号码（类被叫号码）
typedef struct _REDIRECTION_NUMBER_PARAM_EX
{
	BYTE bNatureOfAddressIndicator ;
		//地址性质指示码
		//000000：备用
		//000001：用户号码
		//000010：不知
		//000011：国内（有效）号码
		//000100：国际号码
	BYTE bOddEvenIndicator ;
		//奇/偶指示码
		//0：偶数个地址号码
		//1：奇数个地址号码
	BYTE bReserved ;
	BYTE bNumberingPlanIndicator ;
		//号码计划指示码
		//000：备用
		//001：ISDN（电话）号码计划（E.164）
		//010：备用
		//011：数据号码计划（X.121）
		//100：用户电报号码计划
		//101：保留用于国内应用
		//110：保留用于国内应用
		//111：备用
	BYTE InternalNetworkNumberIndicator ;
		//内部网络号码指示码
		//0：编路至内部网络号码允许
		//1：编路至内部网络号码不允许
	BYTE bAddressSignal[17];
}REDIRECTION_NUMBER_PARAM_EX, *LPREDIRECTION_NUMBER_PARAM_EX;

// 3.7 呼叫历史
typedef struct _CALL_HISTORY_INFORMATION_PARAM_EX
{
	BYTE bPropagationValue[2];
}CALL_HISTORY_INFORMATION_PARAM_EX, *LPCALL_HISTORY_INFORMATION_PARAM_EX;


// 3.42 传播时延（毫秒）
typedef struct _PROPAGATION_DELAY_COUNTER_PARAM_EX
{
	BYTE bPropagationValue[2];
}PROPAGATION_DELAY_COUNTER_PARAM_EX, *LPPROPAGATION_DELAY_COUNTER_PARAM_EX;

// 3.37 任选后向呼叫指示码
typedef struct _OPTIONAL_BACKWARD_CALL_INDICATORS_PARAM_EX
{
	BYTE bInBandInformationIndicator ;			//带内信息指示码
		//0：无指示
		//1：带内信息或一个适合的型式现可获得
	BYTE bCallTransferPossibleIndicator ;		//呼叫转移可能发生指示码
		//0：无指示
		//1：呼叫转移可能发生
	BYTE bSimpleSegmentationIndicator ;		//简单分段指示码
		//0：无额外信息将要送出
		//1：额外信息将在一个分段消息中送出
	BYTE bMLPPUserIndicator ;					//MLPP用户指示码
		//0：无指示
		//1：MLPP用户
	BYTE bReserved  ;							//保留
}OPTIONAL_BACKWARD_CALL_INDICATORS_PARAM_EX, *LPOPTIONAL_BACKWARD_CALL_INDICATORS_PARAM_EX;

// 3.38 任选前向呼叫指示码
typedef struct _OPTIONAL_FORWARD_CALL_INDICATORS_PARAM_EX
{
	BYTE bCUGCallIndicator ;					//（CUG）呼叫指示码
		//00：非CUG呼叫
		//01：备用
		//10：CUG呼叫，去话呼出允许
		//11：CUG呼叫，去话呼出不允许
	BYTE bSimpleSegmentationIndicator ;		//简单分段指示码
		//0：无额外信息将要送出
		//1：额外信息将在一个分段消息中送出
	BYTE bReserved ;							//保留		
		//保留
	BYTE bJointingUserIdentifyIndicator ;		//接续的用户识别请求指示码
		//0：未请求
		//1：请求
}OPTIONAL_FORWARD_CALL_INDICATORS_PARAM_EX, *LPOPTIONAL_FORWARD_CALL_INDICATORS_PARAM_EX;

// 3.39 原被叫号码
typedef struct _ORA_CALLEE_PARTY_NUMBER_PARAM_EX
{ 
	BYTE bNatureOfAddressIndicator ;
		//地址性质指示码
		//000000：备用
		//000001：用户号码
		//000010：不知
		//000011：国内（有效）号码
		//000100：国际号码
	BYTE bOddEvenIndicator ;
		//奇/偶指示码
		//0：偶数个地址号码
		//1：奇数个地址号码
	BYTE bReserved1 ;  //备用
	BYTE bAddressPresentationRestrictedIndicator ;  
		//地址显示限制指示码
		//00：显示允许
		//01：显示限制
		//10：地址不可利用
		//11：备用
	BYTE bNumberingPlanIndicator ;
		//号码计划指示码
		//000：备用
		//001：ISDN（电话）号码计划（E.164）
		//010：备用
		//011：数据号码计划（X.121）
		//100：用户电报号码计划
		//101：保留用于国内应用
		//110：保留用于国内应用
		//111：备用
	BYTE bReserved2 ;  //备用
	BYTE bAddressSignal[17];
}ORA_CALLEE_PARTY_NUMBER_PARAM_EX, *LPORA_CALLEE_PARTY_NUMBER_PARAM_EX;

// 接入传送
typedef struct _ACCESS_TRANSPORT_PARAM_EX
{
	BYTE bInfoUnit[ MAX_DATALEN - 18 ];//129];
}ACCESS_TRANSPORT_PARAM_EX, *LPACCESS_TRANSPORT_PARAM_EX;

// 3.33 消息兼容性信息
typedef struct _MESSAGE_COMPATIBILITY_INFORMATION_PARAM_EX
{ 
	BYTE bMiddleSwitchingExchaneEandemIndicator ;	//在中间交换局汇接指示码
		//0:汇接说明
		//1:终端节点说明
	BYTE bReleaseCallIndicator ;					//释放呼叫指示码
		//0:不释放呼叫
		//1:释放呼叫
	BYTE bSendAnnounceIndicator ;					//发送通知指示码
		//0:不发通知
		//1:发送通知
	BYTE bObsoleteMessageIndicator ;				//舍弃消息指示码
		//0:不舍弃消息
		//1:舍弃消息
	BYTE bPassPossibleIndicator ;					//“通过”不可能指示码
		//0:释放呼叫
		//1:舍弃消息
	BYTE bReserved ;								//保留
	BYTE bExtendIndicator ;						//扩充
		//0:下一个八位组存在
		//1:最后的八位组
	struct _MORE_DIRECTIVE_INDICATOR
	{
		BYTE bMoreDirectiveIndicatior  ;					//更多的指导指示码
		BYTE bExtendIndicator ;							//扩充
	}	m_bMoreDirectiveIndicator[30];					
}MESSAGE_COMPATIBILITY_INFORMATION_PARAM_EX, *LPMESSAGE_COMPATIBILITY_INFORMATION_PARAM_EX;

// 3.41 参数兼容性信息
typedef struct _PARAM_COMPATIBILITY_INFORMATION_PARAM_EX
{
	BYTE bDateBuf[ MAX_DATALEN - 18 ];
}PARAM_COMPATIBILITY_INFORMATION_PARAM_EX, *LPPARAM_COMPATIBILITY_INFORMATION_PARAM_EX;

// 3.44 改发号码
typedef struct _REDIRECTING_NUMBER_PARAM_EX
{
	BYTE bNatureOfAddressIndicator ;
		//地址性质指示码
		//000000：备用
		//000001：用户号码
		//000010：不知
		//000011：国内（有效）号码
		//000100：国际号码
	BYTE bOddEvenIndicator ;
		//奇/偶指示码
		//0：偶数个地址号码
		//1：奇数个地址号码
	BYTE bReserved1 ;  //备用
	BYTE bAddressPresentationRestrictedIndicator ;  
		//地址显示限制指示码
		//00：显示允许
		//01：显示限制
		//10：地址不可利用
		//11：备用
	BYTE bNumberingPlanIndicator ;
		//号码计划指示码
		//000：备用
		//001：ISDN（电话）号码计划（E.164）
		//010：备用
		//011：数据号码计划（X.121）
		//100：用户电报号码计划
		//101：保留用于国内应用
		//110：保留用于国内应用
		//111：备用
	BYTE bReserved2 ;  //备用
	BYTE bAddressSignal[17];
}REDIRECTING_NUMBER_PARAM_EX, *LPREDIRECTING_NUMBER_PARAM_EX;

//3.45 改发信息 
typedef struct _REDIRECTION_INFORMATION_PARAM_EX
{
	BYTE bRedirectingIndicator ;
		//改发指示码
		//000：无改发
		//001：呼叫重新编路
		//010：呼叫重新编路，所有改发信息显示限制
		//011：呼叫转移
		//100：呼叫转移，所有改发信息显示限制
		//101：呼叫重新编路，改发号码显示限制
		//110：呼叫转移，改发号码显示限制
		//111：备用
	BYTE bReserved1 ;  //备用
	BYTE bOriginalRedirectionReason ;
		//原来的改发原因
		//0000：不知/不可获得
		//0001：用户忙
		//0010：无应答
		//0011：无条件
		//0100-1111：备用
	BYTE bRedirectionCounter ;
	BYTE bReserved2 ;  //备用
	BYTE bRedirectingReason ;
		//原来的改发原因
		//0000：不知/不可获得
		//0001：用户忙
		//0010：无应答
		//0011：无条件
		//0100：通知时偏转
		//0101：偏转立即响应
		//0110：移动用户不可达到
		//0111-1111：备用
}REDIRECTION_INFORMATION_PARAM_EX, *LPREDIRECTION_INFORMATION_PARAM_EX;

#ifdef	__cplusplus
extern "C" {
#endif

int DJISUP_SetISUPParam(BYTE nPCM, BYTE nCHN, ISUP_MSG_TYPE_EX MsgType, void *pParam);
int DJISUP_GetISUPParam(BYTE nPCM, BYTE nCHN, ISUP_MSG_TYPE_EX MsgType, void *pParam);

#ifdef	__cplusplus
}
#endif

#endif