#define UCAN_Debug_DebugSTRID			"UDBG "

//define UCAN debug codes
#define UCAN_Debug_Unknown				0
#define UCAN_Debug_Stub					1
#define UCAN_Debug_RxMSG				2
#define UCAN_Debug_TxMSG				3
#define UCAN_Debug_Boot					4
#define UCAN_Debug_BootSecret			5

#define UCAN_Debug_ERROR_BadID			10
#define UCAN_Debug_ERROR_BadCANInit		11

#define UCAN_Debug_Chan0Announce		20
#define UCAN_Debug_Chan0Request			21

#define UCAN_Debug_Call_Init			100
#define UCAN_Debug_WatchCall_f32		101
#define UCAN_Debug_CANPending			102

#define UCAN_Debug_StackCallW_ADD		110
#define UCAN_Debug_StackCallW_DEL		111
#define UCAN_Debug_StackCallW_FEMPT		112
#define UCAN_Debug_StackCallW_FDUP		113
#define UCAN_Debug_StackCallW_UPDAT		114

#define UCAN_Debug_Call_SetID			120
#define UCAN_Debug_Call_HandlerMode		121
#define UCAN_Debug_Call_StackMode		122
#define UCAN_Debug_Call_FeedMode		123

#define UCAN_Debug_Call_Main			130

#define UCAN_Debug_StoreMSG				131
#define UCAN_Debug_NoMSG				132
#define UCAN_Debug_UpdateFeedStack		133
#define UCAN_Debug_CANFetchMSG			134
#define UCAN_Debug_Process_Data			135
#define UCAN_Debug_MSGDispatch			136
#define UCAN_Debug_BadPKTFormat			137
#define UCAN_Debug_Call_GetNext_Clear	138

#define UCAN_Debug_SendCall_b8			140
#define UCAN_Debug_SendCall_i16			141
#define UCAN_Debug_SendCall_l16			142
#define UCAN_Debug_SendCall_i32			143
#define UCAN_Debug_SendCall_l32			144
#define UCAN_Debug_SendCall_f32			145

#define UCAN_Debug_RECV_Ch1_DServ_i16	150
#define UCAN_Debug_RECV_Ch1_DServ_l16	151
#define UCAN_Debug_RECV_Ch1_DServ_i32	152
#define UCAN_Debug_RECV_Ch1_DServ_l32	153
#define UCAN_Debug_RECV_Ch1_DServ_f32	154

#define UCAN_Debug_StackCallM_ADD		160
#define UCAN_Debug_StackCallM_DEL		161
#define UCAN_Debug_StackCallM_FEMPT		162
#define UCAN_Debug_StackCallM_FDUP		163
#define UCAN_Debug_StackCallM_UPDAT		164

#define UCAN_Debug_UserDebug_1			999
#define UCAN_Debug_UserDebug_2			998
#define UCAN_Debug_UserDebug_3			997
#define UCAN_Debug_UserDebug_4			996
#define UCAN_Debug_UserDebug_5			995
