#include <SPI.h>
#include <ucan.h>

float SomeImportantValuei32;
float SomeImportantValuel32;
float SomeImportantValuef32;
float SomeImportantValuei16;
float SomeImportantValuel16;

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
    UCAN.WatchValue_f32(321, &SomeImportantValuei32);
    UCAN.WatchValue_f32(322, &SomeImportantValuel32);
    UCAN.WatchValue_f32(323, &SomeImportantValuef32);
    UCAN.WatchValue_f32(161, &SomeImportantValuei16);
    UCAN.WatchValue_f32(162, &SomeImportantValuel16);
};

uint32_t LastCall = millis();

void loop()
{
	//An example main loop.
    int SomeThreshold = 100;
    
    while (true)
    {
		UCAN.Main();
		
		if (millis() - LastCall > 500)
		{
			LastCall = millis();
			DebugMSG(UCAN_Debug_UserDebug_1, SomeImportantValuei32);
			DebugMSG(UCAN_Debug_UserDebug_2, SomeImportantValuel32);
			DebugMSG(UCAN_Debug_UserDebug_3, SomeImportantValuef32);
			DebugMSG(UCAN_Debug_UserDebug_4, SomeImportantValuei16);
			DebugMSG(UCAN_Debug_UserDebug_5, SomeImportantValuel16);
	};
    };
};
