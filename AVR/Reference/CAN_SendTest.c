/*
 * UCAN Channel 1 Data Tansmission
 * 
 * Implements a basic node on the UCAN network to send paramter values
 * over the UCAN bus.
 * 
 * This code is provided as is for use as a reference use case for the
 * afformentioned feature. In the case of provisional features, proofs 
 * of concept and technology demonstrators, changes may be made to the
 * function and/or inclusion of the features. Reference code is not a 
 * complete documentation for UCAN - please consult the UCAN Technical
 * Reference Manual available on the forum, or in the GUES room.
 * 
 *  Author: Benjamin Gregg-Smith (bgreggsmith@live.co.uk)
*/

#include <SPI.h>
#include <ucan.h>

float SomeImportantValue;

void setup()
{
	Serial.begin(9600);
	
	DebugMSG(UCAN_Debug_Boot); //Let any debug code know we just booted up
	
	UCAN.SetID(602); //We want to be known as device 602
	
    //These are basically the defaults, though it is always a good idea
    // to set the manually so it is clear what behaviour we expect
    UCAN.HandlerMode(UCAN_CallMode_FullAuto); //Let the UCAN handler call itself
    UCAN.FeedMode(UCAN_FeedMode_Auto); //Allow UCAN to watch certain parameters and handle them automatically (recommended)
    UCAN.StackMode(UCAN_StackMode_Auto); //Whenever we read a message from the receive queue directly, assume we are done with it
    
    //Lets get everythin working (makes UCAN live)
    UCAN.Initialize();
    
    //We want to track parameter 123. Have UCAN dump the value in our variable automatically
    //UCAN.WatchValue_f32(123, &SomeImportantValue);
};

void loop()
{
	//An example main loop.
    int SomeInt = 0;
    
    while (true)
    {
		UCAN.Main();
		SomeInt ++;
		
		UCAN.SendValue_i32(321, SomeInt);
		UCAN.SendValue_l32(322, SomeInt);
		UCAN.SendValue_f32(323, SomeInt);
		UCAN.SendValue_i16(161, SomeInt);
		UCAN.SendValue_l16(162, SomeInt);
		delay(250);
    };
};
