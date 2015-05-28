/*				AVR-LOWMEM implementation of UCAN.
 * 
 *                    UGRacing CAN API [UCAN]
 *      Technology demonstrator - not for production or release
 *
 *  Author: Benjamin Gregg-Smith (bgreggsmith@live.co.uk)
 * 
 * Note: Should you make any modifications to this source, and use it
 * 	in your project, which have not been ratified in their entirety by
 * 	the UCAN maintainer(s) as well as tested in their ENTIRETY will
 * 	automatically bump your device/project to ring 9 on the bus.
 * 	Ie. MODIFY THIS AND YOUR STUFF JOINS THE "DEV/NEW CRAP ZONE".
 * 
 * Credit: I make no claims to be a good programmer and this
 * 	code is provided without any warranty etc... UCAN for AVR
 * 	relies heavily on the MCP_CAN library by coryjfowler. For more
 * 	information, please see https://github.com/coryjfowler/MCP_CAN_lib
 * 
 * UCAN Information:
 * 	Standard			:			0A0
 * 	Development name	:			UCAN-AVR/Kryolith
 * 	Revision			:			0
 * 
 * This code is not currently placed under any license and therefore
 * 	not for public release. It is intended for internal use by UGRacing
 * 	only unless prioir written permission has been obtained for outside
 * 	use.
*/


	#include "Arduino.h"
	#include <SPI.h>
	#include <inttypes.h>
	
	//The MCP interface code
	#include "mcp_can_dfs.h"
	#define MAX_CHAR_IN_MESSAGE 8

	class MCP_CAN
	{
		private:
			INT8U   m_nExtFlg;                                                  /* identifier xxxID             */
																				/* either extended (the 29 LSB) */
																				/* or standard (the 11 LSB)     */
			INT32U  m_nID;                                                      /* can id                       */
			INT8U   m_nDlc;                                                     /* data length:                 */
			INT8U   m_nDta[MAX_CHAR_IN_MESSAGE];                            	/* data                         */
			INT8U   m_nRtr;                                                     /* rtr                          */
			INT8U   m_nfilhit;
			INT8U   SPICS;

	/*
	*  mcp2515 driver function 
	*/
	   // private:
	   private:

			void mcp2515_reset(void);                                           /* reset mcp2515                */

			INT8U mcp2515_readRegister(const INT8U address);                    /* read mcp2515's register      */
			
			void mcp2515_readRegisterS(const INT8U address, INT8U values[], const INT8U n);
			void mcp2515_setRegister(const INT8U address, const INT8U value);

			void mcp2515_setRegisterS(const INT8U address, const INT8U values[],const INT8U n);
			
			void mcp2515_initCANBuffers(void);
			
			void mcp2515_modifyRegister(const INT8U address, const INT8U mask, const INT8U data);

			INT8U mcp2515_readStatus(void);                                     /* read mcp2515's Status        */
			INT8U mcp2515_setCANCTRL_Mode(const INT8U newmode);                 /* set mode                     */
			INT8U mcp2515_configRate(const INT8U canSpeed);                     /* set boadrate                 */
			INT8U mcp2515_init(const INT8U canSpeed);                           /* mcp2515init                  */

			void mcp2515_write_id( const INT8U mcp_addr, const INT8U ext, const INT32U id );
			void mcp2515_read_id( const INT8U mcp_addr, INT8U* ext, INT32U* id );

			void mcp2515_write_canMsg( const INT8U buffer_sidh_addr );          /* write can msg                */
			void mcp2515_read_canMsg( const INT8U buffer_sidh_addr);            /* read can msg                 */
			void mcp2515_start_transmit(const INT8U mcp_addr);                  /* start transmit               */
			INT8U mcp2515_getNextFreeTXBuf(INT8U *txbuf_n);                     /* get Next free txbuf          */

			/*
			*  can operator function
			*/    
			INT8U setMsg(INT32U id, INT8U ext, INT8U len, INT8U *pData);    /* set message                  */  
			INT8U clearMsg();                                               /* clear all message to zero    */
			INT8U readMsg();                                                /* read message                 */
			INT8U sendMsg();                                                /* send message                 */

		public:
			MCP_CAN(INT8U _CS);
			INT8U begin(INT8U speedset);                              /* init can                     */
			INT8U init_Mask(INT8U num, INT8U ext, INT32U ulData);           /* init Masks                   */
			INT8U init_Filt(INT8U num, INT8U ext, INT32U ulData);           /* init filters                 */
			INT8U sendMsgBuf(INT32U id, INT8U ext, INT8U len, INT8U *buf);  /* send buf                     */
			INT8U readMsgBuf(INT8U *len, INT8U *buf);                       /* read buf                     */
			INT8U checkReceive(void);                                       /* if something received        */
			INT8U checkError(void);                                         /* if something error           */
			INT32U getCanId(void);                                          /* get can id when receive      */
	};

	#define UCAN_h

	#define UCAN_MCPRXPin					10 //NOT FINAL

	//   AVR-LOWMem specific stuff. Tweak this at your own risk of breaking UCAN.

	//4 messages in queue should be enough for everyone at 8 bytes a pop on a 2K system.
	#define UCAN_MSGStack_Size				4

	//Allow tracking of 16 different parameters (2 bytes each)
	#define UCAN_FeedStack_Size				16

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

	//define UCAN debug codes:
	#define UCAN_Debug_Unknown				0
	#define UCAN_Debug_Stub					1
	#define UCAN_Debug_RxMSG				2
	#define UCAN_Debug_TxMSG				3
	#define UCAN_Debug_BadID				10
	#define UCAN_Debug_BadCANInit			11

	#ifndef v_BUSDef
		//Declare our global CAN interface with CS pin
		
		
		#define v_BUSDef
	#endif

	/* WARNING FOR XXX-HIGHMEM library users:
	 *      UCAN watch stacks use dynamic memory allocation. 
	 *          Do not play games with the memory manager.
	 *             Do not over-use on low memory systems.
	 *               BAD THINGS MAY OTHERWISE HAPPEN.
	 *                    YOU HAVE BEEN WARNED.
	*/

	struct UCANMessage
	{
		int Address;
		uint8_t Channel;
		uint8_t Data[7];
	};

	class UCAN_UCANFeedStack //Initial implementation complete
	{
		int TrackingID[UCAN_FeedStack_Size];
		
		void TrackID(int id);
		void UnTrackID(int id);
		void SetIDTracking(int id, bool mode);
		bool IsIDTracked(int id);
		int FindIDPosition(int id);
		void Empty(void);
		void Initialize(void);
	};

	UCANMessage UCAN_EmptyMessage(void);
	void DebugMSG(int MSG);

	class UCAN_UCANMSGStack //Initial implementation complete
	{
		public:
			UCANMessage Data[UCAN_MSGStack_Size]; //Consider protecting this...
			bool InUse[UCAN_MSGStack_Size]; //Also consider protecting this data...
			
			int GetNextEmptySlot(void);
			int GetAvailableSlots(void);
			void Empty(void);
			void Initialize(void);
			void StoreMessage(int n, UCANMessage MSG);
			UCANMessage FetchMessage(int n);
	};

	class UCAN_UCANHandler //Stub Started
	{
		private:
			MCP_CAN *MCPCANBus;
			bool Initialized;
			int CAN_ID;
			
			uint8_t UCANStackMode;
			uint8_t UCANFeedMode;
			uint8_t UCANCallMode;
			
			UCAN_UCANMSGStack MSGStack;
			
			void RequestCANID(int ID);
			void SendMessage(UCANMessage msg);
			void IntProc_RXHandler(void);
			void FetchNewMessages(void);
			UCANMessage CAN_FetchMsgFromCAN(void);
			void CAN_SendMSG(UCANMessage MSG);
			
		public:
			void HandlerMode(int Mode);
			void SetID(int ID);
			void FeedMode(int Mode);
			void StackMode(int mode);
			void Initialize(void);
			void Empty(void);
			void WatchValue_f32(int ValueGUI, float *f32Pointer);
			void Main(void);
			bool IsMessagePending(void);
			UCANMessage GetNextMessage(void);
			void MarkMessageRead(void);
			
			uint8_t NewStreamDUID(void);
	};
	
	


