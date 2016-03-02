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
 * 	Standard			:			0A1
 * 	Development name	:			UCAN-AVR/Kryolith
 * 	Revision			:			1
 * 
 * This code is not currently placed under any license and therefore
 * 	not for public release. It is intended for internal use by UGRacing
 * 	only unless prioir written permission has been obtained for outside
 * 	use.
*/

	//#define RELEASE
	
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
			void Initialize(INT8U _CS);
			INT8U begin(INT8U speedset);                              /* init can                     */
			INT8U init_Mask(INT8U num, INT8U ext, INT32U ulData);           /* init Masks                   */
			INT8U init_Filt(INT8U num, INT8U ext, INT32U ulData);           /* init filters                 */
			INT8U sendMsgBuf(INT32U id, INT8U ext, INT8U len, INT8U *buf);  /* send buf                     */
			INT8U readMsgBuf(INT8U *len, INT8U *buf);                       /* read buf                     */
			INT8U checkReceive(void);                                       /* if something received        */
			INT8U checkError(void);                                         /* if something error           */
			INT32U getCanId(void);                                          /* get can id when receive      */
	};

	#include "ucandefine.h"
	#include "ucandebug.h"


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
		void unTrackID(int id);
		void SetIDTracking(int id, bool mode);
		bool IsIDTracked(int id);
		int FindIDPosition(int id);
		void Empty(void);
		void Initialize(void);
	};

	class UCAN_UCANWatchStack //Initial implementation complete
	{
		int TrackingID[UCAN_FeedStack_Size];
		float* Target[UCAN_FeedStack_Size];
		
		public:
			void TrackID(int id, float* TMem);
			bool IsDuplicate(int id, float* TMem);
			void unTrackID(int id, float* TMem);
			int FindEmptyPosition(void);
			//float GetIDTrackingTarget(int ID); //Currently not required
			int FindEntry(int id, float* TMem);
			int FindTargetPosition(float* LMem);
			void Empty(void);
			void Initialize(void);
			void UpdateIDValue(int ID, float value);
	};

	UCANMessage UCAN_EmptyMessage(void);
	
	void DebugMSG(int MSG);
	void DebugMSG(int MSG, int data);
	void DebugMSG(UCANMessage MSG);

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
			bool IsMessageEmpty(int n);
	};

	class UCAN_UCANHandler //Stub Started
	{
		private:
			MCP_CAN MCPCANBus;
			bool Initialized;
			int CAN_ID;
			int BootSecret;
			
			uint8_t UCANStackMode;
			uint8_t UCANFeedMode;
			uint8_t UCANCallMode;
			
			UCAN_UCANMSGStack MSGStack;
			UCAN_UCANWatchStack TrackingStack;
			
			void RequestCANID(int ID);
			
			void IntProc_RXHandler(void);
			void FetchNewMessages(void);
			UCANMessage CAN_FetchMsgFromCAN(void);
			void CAN_SendMSG(UCANMessage MSG);
			void ProcessMSGTriggers(void);
			void MSGDispatcher(UCANMessage MSG);
			void MSGProcessor_InfoServ(UCANMessage MSG);
			void MSGProcessor_DataServ(UCANMessage MSG);
			void SendMessage(UCANMessage msg);
			bool CAN_IsMessagePending(void);
			
		public:
			void Chan0_Announce(uint8_t flag);
			void Chan0_Request(uint8_t flag, int data);
			void HandlerMode(int Mode);
			void SetID(int ID);
			void FeedMode(int Mode);
			void StackMode(int mode);
			void Initialize(void);
			void Empty(void);
			void WatchValue_f32(int Value, float* f32Pointer);
			
			void SendValue_i16(int16_t Value, int16_t i16);
			void SendValue_l16(int16_t Value, uint16_t l16);
			
			void SendValue_i32(int16_t Value, int32_t i32);
			void SendValue_l32(int16_t Value, uint32_t l32);
			void SendValue_f32(int16_t Value, float f32);
			
			void SendBlock(int16_t Value, int16_t Size, uint8_t* Data); //Provisional
			
			void Main(void);
			bool IsMessagePending(void);
			UCANMessage GetNextMessage(void);
			void MarkMessageRead(void);
			
			uint8_t NewStreamDUID(void);
	};
	
	int16_t Bytes16Toi16(uint8_t b16_1, uint8_t b16_2);
	uint8_t BytesFromi16(int16_t i, uint8_t ByteNumber);	
	
	uint16_t Bytes16Tol16(uint8_t b16_1, uint8_t b16_2);
	uint8_t BytesFroml16(uint16_t i, uint8_t ByteNumber);	
	
	int32_t Bytes32Toi32(uint8_t b32_1, uint8_t b32_2, uint8_t b32_3, uint8_t b32_4);
	uint8_t BytesFromi32(int32_t i, uint8_t ByteNumber);

	uint32_t Bytes32Tol32(uint8_t b32_1, uint8_t b32_2, uint8_t b32_3, uint8_t b32_4);
	uint8_t BytesFroml32(uint32_t i, uint8_t ByteNumber);	

	float Bytes32Tof32(uint8_t b32_1, uint8_t b32_2, uint8_t b32_3, uint8_t b32_4);
	uint8_t BytesFromf32(float f, uint8_t ByteNumber);
	
	void UCANCallInterrupt_Default();
	
	#ifndef UCAN_h_Globals
		#define UCAN_h_Globals
		
		extern UCAN_UCANHandler UCAN;
	#endif



