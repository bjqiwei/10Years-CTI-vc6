Attribute VB_Name = "Module4"
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'DJLanMon.DLL可以支持通过局域网进行录音监听。
'当D081A模拟语音卡上某路通道在录音时，
'该接口可以提供用户远程或本机监听D081A模拟语音卡上某路通道的录音?
'头文件DJLanMon.h , Lib文件DJLanMon.lib, Dll文件DJLanMon.Dll?
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'修改时间: 2003-07-21


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'初始化监听服务端( ServerIp = 服务器IP地址; BuffSize = IP地址字符串长度)
'返回值:
'  0  =  成功,启动被监听功能
'  -1 =  不成功，原因：获取缓冲区大小失败
'  -2 =  不成功，原因：初始化失败
'  -3 =  不成功，原因：启动被监听失败
Declare Function DJLM_Svr_Init Lib "DJLanMon.DLL" (ByVal ServerIp As String, ByVal BuffSize As Long) As Long


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'初始化监听客户端( ServerIp = 服务器IP地址; BuffSize1 = 服务器IP地址字符串长度; ClientIp = 客户端IP地址; BuffSize = 客户端IP地址字符串长度; )
'返回值:
'  0  = 成功
'  -1 = 不成功，原因：初始化资源失败
'  -2 = 不成功，原因：初始化声卡失败
Declare Function DJLM_Clt_Init Lib "DJLanMon.DLL" (ByVal ServerIp As String, ByVal BuffSize1 As Long, ByVal ClientIp As String, ByVal BuffSize As Long) As Long


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'关闭释放监听服务端
Declare Sub DJLM_Svr_Quit Lib "DJLanMon.DLL" ()


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'关闭释放监听客户端
Declare Sub DJLM_Clt_Quit Lib "DJLanMon.DLL" ()


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'监听端向被监听端发送某通道的监听标志。参数sign的值为1表示监听，0表示取消监听
'返回值:
'   0  = 成功
'   -1 = 不成功，原因：准备资源失败
'   -2 = 不成功，原因：向远端发送命令失败
'   -3 = 不成功，原因：目前有一条通道正在监听
'   -4 = 不成功，原因：启动放音失败
Declare Function DJLM_Clt_SetMonSign Lib "DJLanMon.DLL" (ByVal Channel As Long, ByVal sign As Long) As Long


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'获得监听通道Channel的客户端个数
'返回值:监听客户端个数
Declare Function DJLM_Svr_GetMonInfo_MoniterClientCount Lib "DJLanMon.DLL" (ByVal Channel As Long) As Long


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'获得监听通道Channel的客户端序列号为ClientIndex的IP地址( ClientIndex 的取值范围[1~16] )
'返回值:IP地址字符串
Declare Function DJLM_Svr_GetMonInfo_MoniterClientIP Lib "DJLanMon.DLL" (ByVal Channel As Long, ByVal ClientIndex As Long) As String


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'获得客户端序列号为ClientIndex的监听通道Channel开始时间( ClientIndex 的取值范围[1~16] )
'返回值:监听开始时间字符串
Declare Function DJLM_Svr_GetMonInfo_MonStartTime Lib "DJLanMon.DLL" (ByVal Channel As Long, ByVal ClientIndex As Long) As String


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'获得客户端序列号为ClientIndex的监听通道Channel的持续时间( ClientIndex 的取值范围[1~16] )
'返回值:监听持续时间单位(秒)
Declare Function DJLM_Svr_GetMonInfo_MonDura Lib "DJLanMon.DLL" (ByVal Channel As Long, ByVal ClientIndex As Long) As Long


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'获得序列号为ClientIndex的客户端是否正在监听通道Channel( ClientIndex 的取值范围[1~16] )
'返回值:
'    -1 = 当前此通道无客户端监听
'    0  = 序列号为ClientIndex的客户端没有监听此通道
'    1  = 序列号为ClientIndex的客户端正在监听此通道
Declare Function DJLM_Svr_GetMonInfo_IsMonStatus Lib "DJLanMon.DLL" (ByVal Channel As Long, ByVal ClientIndex As Long) As Long






