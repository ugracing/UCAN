#include <SPI.h>
#include <ucan.h>

float SomeImportantValue;

void setup()
{
	Serial.begin(9600);
	
	DebugMSG(UCAN_Debug_Boot); //Let any debug code know we just booted up
	
	UCAN.SetID(601); //We want to be known as device 601
	
    //These are basically the defaults, though it is always a good idea
    // to set the manually so it is clear what behaviour we expect
    UCAN.HandlerMode(UCAN_CallMode_FullAuto); //Let the UCAN handler call itself
    UCAN.FeedMode(UCAN_FeedMode_Auto); //Allow UCAN to watch certain parameters and handle them automatically (recommended)
    UCAN.StackMode(UCAN_StackMode_Auto); //Whenever we read a message from the receive queue directly, assume we are done with it
    
    //Lets get everythin working (makes UCAN live)
    UCAN.Initialize();
    
    //We want to track parameter 123. Have UCAN dump the value in our variable automatically
    UCAN.WatchValue_f32(123, &SomeImportantValue);
};

void loop()
{
	//An example main loop.
    int SomeThreshold = 100;
    
    while (true)
    {
		DebugMSG(UCAN_Debug_UserDebug_1, SomeImportantValue);
		//UCAN.Main();
       
        delay(100);
    };
};
