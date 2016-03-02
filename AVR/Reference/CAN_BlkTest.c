/*
 * UCAN Channel 1 Block Transmission Support POC/Tech Demo (Provisional)
 * 
 * Implements provisional support for Channel 1 block transmission.
 * Proof of concept/technology demonstrator code.
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
uint8_t BlkData[20];

void setup()
{
	Serial.begin(9600);
	
	DebugMSG(UCAN_Debug_Boot); //Let any debug code know we just booted up
	
	UCAN.SetID(602); //We want to be known as device 601
	
    //These are basically the defaults, though it is always a good idea
    // to set the manually so it is clear what behaviour we expect
    UCAN.HandlerMode(UCAN_CallMode_FullAuto); //Let the UCAN handler call itself
    UCAN.FeedMode(UCAN_FeedMode_Auto); //Allow UCAN to watch certain parameters and handle them automatically (recommended)
    UCAN.StackMode(UCAN_StackMode_Auto); //Whenever we read a message from the receive queue directly, assume we are done with it
    
    //Lets get everythin working (makes UCAN live)
    UCAN.Initialize();
    
    uint8_t c = 0;
    while (c < 20)
    {
		BlkData[c] = c + 1;
		c ++;
	};
};

void loop()
{
	//An example main loop.
    int SomeInt = 0;
    
    UCAN.SendBlock(44, 20, BlkData);
    
    delay(250);
};
