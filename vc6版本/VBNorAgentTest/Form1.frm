VERSION 5.00
Object = "{B11111E6-F45F-4328-9852-A9C5E4A194D4}#1.0#0"; "NorAgent.ocx"
Begin VB.Form Form1 
   Caption         =   "NorthMarchAgent"
   ClientHeight    =   5055
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   9270
   LinkTopic       =   "Form1"
   ScaleHeight     =   5055
   ScaleWidth      =   9270
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command1 
      Caption         =   "GetActiveCall By CallerID"
      Height          =   375
      Left            =   6720
      TabIndex        =   24
      Top             =   4080
      Width           =   2415
   End
   Begin VB.CommandButton CmdConferCallAgent 
      Caption         =   "ConferCall Agent"
      Height          =   375
      Left            =   6720
      TabIndex        =   23
      Top             =   3120
      Width           =   2415
   End
   Begin VB.CommandButton CmdPickUpHoldbyCallerID 
      Caption         =   "PickUpHold by CallerID"
      Height          =   375
      Left            =   6720
      TabIndex        =   22
      Top             =   2160
      Width           =   2415
   End
   Begin VB.TextBox TxtIP 
      Height          =   285
      Left            =   360
      TabIndex        =   20
      Text            =   "192.168.10.88"
      Top             =   240
      Width           =   3015
   End
   Begin NORAGENTLib.NorAgent NorAgent2 
      Height          =   375
      Left            =   120
      TabIndex        =   19
      Top             =   4440
      Width           =   615
      _Version        =   65536
      _ExtentX        =   1085
      _ExtentY        =   661
      _StockProps     =   0
   End
   Begin VB.CommandButton CmdJoinConference 
      Caption         =   "Join Conference"
      Height          =   375
      Left            =   5040
      TabIndex        =   18
      Top             =   1680
      Width           =   1455
   End
   Begin VB.CommandButton CmdGetActiveCall 
      Caption         =   "GetActiveCall FromAgent"
      Height          =   375
      Left            =   6720
      TabIndex        =   17
      Top             =   3600
      Width           =   2415
   End
   Begin VB.CommandButton CmdPickUpHold 
      Caption         =   "PickUpHold by Normal"
      Height          =   375
      Left            =   6720
      TabIndex        =   16
      Top             =   1680
      Width           =   2415
   End
   Begin VB.CommandButton CmdHold 
      Caption         =   "Hold"
      Height          =   375
      Left            =   5040
      TabIndex        =   15
      Top             =   3120
      Width           =   1455
   End
   Begin VB.CommandButton CmdCancelTransfer 
      Caption         =   "Cancel  Transfer"
      Height          =   375
      Left            =   5040
      TabIndex        =   14
      Top             =   4560
      Width           =   1455
   End
   Begin VB.CommandButton CmdConferCall 
      Caption         =   "ConferCall Out"
      Height          =   375
      Left            =   6720
      TabIndex        =   13
      Top             =   2640
      Width           =   2415
   End
   Begin VB.CommandButton CmdToAgentData 
      Caption         =   "To Agent Data"
      Height          =   375
      Left            =   5040
      TabIndex        =   12
      Top             =   3600
      Width           =   1455
   End
   Begin VB.CommandButton CmdAnswerCall 
      Caption         =   "Answer Call"
      Height          =   375
      Left            =   5040
      TabIndex        =   11
      Top             =   2160
      Width           =   1455
   End
   Begin VB.CommandButton CmdHangUp 
      Caption         =   "HangUp"
      Height          =   375
      Left            =   5040
      TabIndex        =   10
      Top             =   4080
      Width           =   1455
   End
   Begin VB.CommandButton CmdTransferToIVR 
      Caption         =   "Transfer To IVR"
      Height          =   375
      Left            =   5040
      TabIndex        =   9
      Top             =   2640
      Width           =   1455
   End
   Begin VB.CommandButton CmdTransferToOut 
      Caption         =   "TransferToOut"
      Height          =   375
      Left            =   6720
      TabIndex        =   8
      Top             =   1200
      Width           =   2415
   End
   Begin VB.CommandButton CmdTranToAgent 
      Caption         =   "TransferToAgent"
      Height          =   375
      Left            =   5040
      TabIndex        =   7
      Top             =   1200
      Width           =   1455
   End
   Begin VB.CommandButton CmdCallAgent 
      Caption         =   "Call Agent"
      Height          =   375
      Left            =   6720
      TabIndex        =   6
      Top             =   720
      Width           =   2415
   End
   Begin VB.CommandButton CmdOutCall 
      Caption         =   "OutCall"
      Height          =   375
      Left            =   5040
      TabIndex        =   5
      Top             =   720
      Width           =   1455
   End
   Begin VB.ListBox LstEvent 
      Height          =   3570
      Left            =   120
      TabIndex        =   4
      Top             =   600
      Width           =   4815
   End
   Begin VB.TextBox TxtDN 
      Height          =   285
      Left            =   3840
      TabIndex        =   2
      Top             =   240
      Width           =   1095
   End
   Begin VB.CommandButton CmdLogOut 
      Caption         =   "Logout"
      Height          =   375
      Left            =   6720
      TabIndex        =   1
      Top             =   240
      Width           =   2415
   End
   Begin VB.CommandButton Cmd 
      Caption         =   "Login"
      Height          =   375
      Left            =   5040
      TabIndex        =   0
      Top             =   240
      Width           =   1455
   End
   Begin VB.Label Label5 
      AutoSize        =   -1  'True
      Caption         =   "IP"
      Height          =   195
      Left            =   120
      TabIndex        =   21
      Top             =   240
      Width           =   150
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "DN:"
      Height          =   195
      Left            =   3480
      TabIndex        =   3
      Top             =   240
      Width           =   285
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Cmd_Click()
    If "" = TxtDN.Text Then
        MsgBox "please input DN ! "
        Exit Sub
    End If
    
    NorAgent2.LogIn TxtDN.Text, 7000, TxtIP.Text
End Sub

Private Sub CmdAnswerCall_Click()
    NorAgent2.AnswerCall
End Sub

Private Sub CmdCallAgent_Click()
    tel = InputBox("please input the aim DN", "Call Agent")
    NorAgent2.CallAgent tel
End Sub

Private Sub CmdCancelTransfer_Click()
    NorAgent2.CancelTransfer
End Sub

Private Sub CmdConferCall_Click()
    tel = InputBox("please input the tel", "Conference Call Out")
    NorAgent2.ConferCall 1, tel
End Sub

Private Sub CmdConferCallAgent_Click()
    tel = InputBox("please input the aim DN", "Conference Call Agent")
    NorAgent2.ConferCall 0, tel
End Sub

Private Sub CmdGetActiveCall_Click()
    tel = InputBox("please input the aim DN", "GetActiveCall from Agent")
    NorAgent2.GetActiveCall 0, tel

End Sub

Private Sub CmdHangUp_Click()
    NorAgent2.HangUp
End Sub

Private Sub CmdHold_Click()
    NorAgent2.Hold
End Sub

Private Sub CmdJoinConference_Click()
    tel = InputBox("please input the aim DN", "Join Conference")
    NorAgent2.JoinConference tel

End Sub

Private Sub CmdLogOut_Click()
    NorAgent2.LogOut
End Sub

Private Sub CmdOutCall_Click()
    tel = InputBox("please input the tel number", "Out Call")
    NorAgent2.OutCall tel
    
End Sub



Private Sub CmdPickUpHold_Click()
    NorAgent2.PickupHold 0, ""

End Sub

Private Sub CmdPickUpHoldbyCallerID_Click()
    CallerID = InputBox("please input the CallerID", "PickUpHold by CallerID")
    NorAgent2.PickupHold 1, CallerID
End Sub

Private Sub CmdToAgentData_Click()
    tel = InputBox("please input the Aim DN", "To Agent Data")
    msg = InputBox("please input the data", "To Agent Data")
    NorAgent2.AgentToAgentData tel, msg
End Sub

Private Sub CmdTransferToIVR_Click()
    NorAgent2.TransferToIVR
End Sub

Private Sub CmdTransferToOut_Click()
    tel = InputBox("please input the tel number", "Transfer To Out")
    NorAgent2.TransferToOut tel
End Sub

Private Sub CmdTranToAgent_Click()
    tel = InputBox("please input the aim DN", "Transfer To Agent")
    NorAgent2.TransferToAgent tel
End Sub

Private Sub Command1_Click()
    tel = InputBox("please input the CallerID", "GetActiveCall by CallerID")
    NorAgent2.GetActiveCall 1, tel
End Sub

Private Sub NorAgent2_OnHangup()
    LstEvent.AddItem "OnHangup"
End Sub

Private Sub NorAgent2_OnNetAbend()
    LstEvent.AddItem "NetAbend"
End Sub

Private Sub NorAgent2_OnOffHook()
    LstEvent.AddItem "OnOffHook"
End Sub

Private Sub NorAgent2_OnReciveAgentData(ByVal nDN As Integer, ByVal Buf As String)
    LstEvent.AddItem "ReciveAgentData   " + Buf + Str(nDN)
End Sub

Private Sub NorAgent2_OnResponesMakeCall(ByVal bResult As Long)
    LstEvent.AddItem "ResponesMakeCall"
End Sub

Private Sub NorAgent2_OnResponseAnswerCall(ByVal bResult As Long)
    LstEvent.AddItem "ResponseAnswerCall"
End Sub

Private Sub NorAgent2_OnResponseCancelTransfer(ByVal bResult As Long)
    LstEvent.AddItem "ResponseCancelTransfer"
End Sub

Private Sub NorAgent2_OnResponseConferCall(ByVal bResult As Long)
    LstEvent.AddItem "ResponseCancelTransfer"
End Sub

Private Sub NorAgent2_OnResponseDisMonitor(ByVal bResult As Long)
    LstEvent.AddItem "ResponseDisMonitor"
End Sub


Private Sub NorAgent2_OnResponseGetActive(ByVal bResult As Long, ByVal CallerID As String, ByVal CalledID As String)
 LstEvent.AddItem "ResponseGetActive" & "  " & CallerID & "  " & CalledID
End Sub

Private Sub NorAgent2_OnResponseHold(ByVal bResult As Long)
    LstEvent.AddItem "ResponseHold"
End Sub

Private Sub NorAgent2_OnResponseJoinConfer(ByVal bResult As Long)
    LstEvent.AddItem "ResponseJoinConfer"
End Sub

Private Sub NorAgent2_OnResponseLogin(ByVal bResult As Long)
    LstEvent.AddItem "ResponseLogin  "
End Sub

Private Sub NorAgent2_OnResponseMonitor(ByVal bResult As Long)
    LstEvent.AddItem "ResponseMonitor"
End Sub

Private Sub NorAgent2_OnResponsePickUpHold(ByVal bResult As Long, ByVal CallerID As String, ByVal CalledID As String)
 LstEvent.AddItem "OnResponsePickUpHold" & "  " & CallerID & "  " & CalledID
End Sub

Private Sub NorAgent2_OnResponseTransfer(ByVal bResult As Long)
    LstEvent.AddItem "ResponseTransfer  "
End Sub

Private Sub NorAgent2_OnRinging(ByVal CallerID As String, ByVal CalledID As String)
    LstEvent.AddItem "OnRinging  " + CallerID + "  " + CalledID
End Sub
