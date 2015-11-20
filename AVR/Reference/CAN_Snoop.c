#include <SPI.h>
#include <ucan.h>

/*
 * Note: This code was written for the AVR/Kryolith UCAN branch.
 * 	As of September 2015, this is the only UCAN implementation tested
 * 	with this code. The UCAN specification is not designed to snoop all
 * 	bus traffic so this program provides some <hidden> and 'alternative'
 * 	functionality to the official specs and therefore represents a more 
 * 	advanced and non-standard use of UCAN.
*/

//As this output to the debug bridge - we do not set the RELEASE define
//#define RELEASE

void setup()
{
	Serial.begin(9600);
	
	UCAN.SetID(1792); //Join Ring-9
	
	//Define how we want UCAN to behave. In this case, we want most of the control.
    UCAN.HandlerMode(UCAN_CallMode_FullManual); //We will make all UCAN Handler calls manually
    UCAN.FeedMode(UCAN_FeedMode_Manual); //Allow UCAN to watch certain parameters and handle them automatically (recommended)
    UCAN.StackMode(UCAN_StackMode_Auto); //We will mark each message as read when we are done with it manually

    UCAN.Initialize();
};

void OutputToDebugBridge()
{
	uint8_t c = 0;
	UCANMessage CMsg;
	
	while (UCAN.IsMessagePending() == true)
	{
		CMsg = UCAN.GetNextMessage();
		
		DebugMSG(CMsg);
		
		//UCAN.MarkMessageRead();
	};
};

void loop()
{
    while (true)
    {
        UCAN.Main();
        OutputToDebugBridge();
    };
};
