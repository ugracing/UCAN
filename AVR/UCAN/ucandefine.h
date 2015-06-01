#define UCAN_h

#define UCAN_MCPRXPin					10 //NOT FINAL

//   AVR-LOWMem specific stuff. Tweak this at your own risk of breaking UCAN.

//4 messages in queue should be enough for everyone at 8 bytes a pop on a 2K system.
#define UCAN_MSGStack_Size				4

//Allow tracking of 16 different parameters (4 bytes each)
#define UCAN_FeedStack_Size				8

//The amount of time in ms that can elapse before UCAN gives up on an operation
#define UCAN_UCANOperationTimeout		5


#define UCAN_CallMode_FullAuto			0
#define UCAN_CallMode_ContAuto			1
#define UCAN_CallMode_FullManual		2
#define UCAN_CallMode_ContManual		3

#define UCAN_FeedMode_Auto				0

#define UCAN_StackMode_Auto				0

#define UCAN_Version_Family				1
#define UCAN_Version_Major				1
#define UCAN_Version_Minor				0
#define UCAN_Version_Revision			0

#define UCAN_ChanType_0_Announce		1

//Define the packet types for channel 1 data - DO NOT MODIFY.
#define UCAN_ChanType_1_nul				0

#define UCAN_ChanType_1_b8				10

#define UCAN_ChanType_1_i16				20
#define UCAN_ChanType_1_l16				21

#define UCAN_ChanType_1_i32				30
#define UCAN_ChanType_1_l32				32
#define UCAN_ChanType_1_f32				33

#define UCAN_ChanType_1_str1			40
#define UCAN_ChanType_1_str2			41
#define UCAN_ChanType_1_str3			42
#define UCAN_ChanType_1_str4			43
#define UCAN_ChanType_1_str5			44
#define UCAN_ChanType_1_str6			45

#define UCAN_ChanType_1_bin1			50
#define UCAN_ChanType_1_bin2			51
#define UCAN_ChanType_1_bin3			52
#define UCAN_ChanType_1_bin4			53
#define UCAN_ChanType_1_bin5			54
#define UCAN_ChanType_1_bin6			55

//Define the packet types for channel 2 stream data - DO NOT MODIFY
#define UCAN_ChanType_2_DUIDQRY			1
#define UCAN_ChanType_2_DUIDRPY			2
#define UCAN_ChanType_2_DUIDANC			3
#define UCAN_ChanType_2_DUIDEND			3

#define UCAN_ChanType_2_ACKQRY			4
#define UCAN_ChanType_2_ACKRPY			5

#define UCAN_ChanType_2_ACKPKT			10
#define UCAN_ChanType_2_RQRSND			11

#define UCAN_ChanType_2_STDATA			20
#define UCAN_ChanType_2_STSTOP			21
#define UCAN_ChanType_2_STCKSM			29

//UCAN Operation descriptors:
#define UCAN_ChanOp_2_WaitForDUIDRPY	1
