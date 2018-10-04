/*  TESTBENCH FOR MINI CANSLATOR HARDWARE
 *  Use this firmware to test the requisite hardware functions on the miniCANSlator 
 * 
 *  This firmware aims to use the same standard modules that are to be used in practice 
 * 
*/ 

#include <Arduino.h>
#include <SPI.h>
#include <park_lock.h>
#include <brake_booster.h>
#include <FlexCAN.h>
#include <mcp2515.h>
#include <main.h>

// INPUT PIN DEFINITIONS 
#define SHIFT_OK 36
#define RUN_CRANK 23
#define ASSY_WAKE 21
#define BOOST_SENSE 17

// OUTPUT PIN DEFINITIONS - DRIVE HIGH  
#define SHIFT_DRIVE 22
#define PARK_DRIVE  14//15
#define BOOST_DRIVE 16

// SPI PIN DEFINITIONS -- Defaults may work 
#define MCP2515_CS 10 
#define MCP2515_SI 11 
#define MCP2515_SO 12
#define MCP2515_SCK 13

// FLEX CAN MESSAGE OBJECTS 
CAN_message_t ghs1out; 
CAN_message_t ghs2out; 

// MCP2515 Setup
struct can_frame canMsg;
MCP2515 mcp2515(10);
struct can_frame canMsg1;   // for testing only 
struct can_frame canMsg2;   // for testing only 

// ASSUMED FROM COLLIN KIDDER FLEX CAN EXAMPLE 
static uint8_t hex[17] = "0123456789abcdef";
static void hexDump(uint8_t dumpLen, uint8_t *bytePtr)
{
  uint8_t working;
  while( dumpLen-- ) {
    working = *bytePtr++;
    Serial.print( hex[ working>>4 ] );
    Serial.println( hex[ working&15 ] );
  }
  Serial.print('\r');
  Serial.print('\n');
}


void setup() {
    Serial.begin(9600);

    // Setup the output pins 
    pinMode(SHIFT_DRIVE, OUTPUT);
    pinMode(PARK_DRIVE, OUTPUT);
    pinMode(BOOST_DRIVE, OUTPUT);

    // Setup the input pins 
    pinMode(SHIFT_OK, INPUT);
    pinMode(RUN_CRANK, INPUT);
    pinMode(ASSY_WAKE, INPUT);

    // Start CAN 
    Can0.begin(250000);       //GMLANHS1
    Can1.begin(250000);       //GMLANHS2

    // Define the GMLANHS (CAN) messages
    ghs1out.xtn = 0;
    ghs1out.id = 0x3;
    ghs1out.len = 8;

    ghs2out.xtn = 0; 
    ghs2out.id = 0x5; 
    ghs2out.len = 8; 
    
    int c = 10;
    for(int i = 0; i < 8; i++){
        ghs1out.buf[i] = c; 
        c++;
        ghs2out.buf[i] = c;
        c++;
    }

    // For MCP2515 Testing 
    canMsg1.can_id  = 0x0F6;
    canMsg1.can_dlc = 8;
    canMsg1.data[0] = 0x8E;
    canMsg1.data[1] = 0x87;
    canMsg1.data[2] = 0x32;
    canMsg1.data[3] = 0xFA;
    canMsg1.data[4] = 0x26;
    canMsg1.data[5] = 0x8E;
    canMsg1.data[6] = 0xBE;
    canMsg1.data[7] = 0x86;

    canMsg2.can_id  = 0x036;
    canMsg2.can_dlc = 8;
    canMsg2.data[0] = 0x0E;
    canMsg2.data[1] = 0x00;
    canMsg2.data[2] = 0x00;
    canMsg2.data[3] = 0x08;
    canMsg2.data[4] = 0x01;
    canMsg2.data[5] = 0x00;
    canMsg2.data[6] = 0x00;
    canMsg2.data[7] = 0xA0;

    // Defien the GMLANLS messages 
    SPI.begin();
    mcp2515.reset();
    mcp2515.setBitrate(CAN_250KBPS);
    mcp2515.setNormalMode();
    //delay(100);
    //test_gmlanhs();
}

void loop() {
    //test_brake_booster();
    //test_bcm_park_lock();
    //test_accessory_wakeup();
    //test_run_crank();
    //test_gmlanls();
    test_mcp2515();
}

void test_brake_booster() {
	Serial.println();
	Serial.println("--TESTING BRAKE BOOSTER CONTROL--");
	uint8_t boost_pressure = analogRead(BOOST_SENSE); 
    Serial.println();
    Serial.print("Boost pressure is read directly as; ");
    Serial.println(boost_pressure);
    Serial.println();
	Serial.print("External Stimulation; Brake boost pump should be ");
	if(brake_booster(boost_pressure)) {
		Serial.println("ON"); 
		digitalWrite(BOOST_DRIVE, HIGH);
	} else {
		Serial.println("OFF");
		digitalWrite(BOOST_DRIVE, LOW);
	}
	delay(3000);

	// Test the output regardless of the input... 
	/*
    digitalWrite(BOOST_DRIVE, HIGH);
	Serial.println("Brake boost pump should be on for 3 seconds...");
	delay(3000);
	digitalWrite(BOOST_DRIVE, LOW);
	Serial.println("Brake boost pump should be off for >3 seconds...");
	delay(3000);
	Serial.println();
    */
}

void test_bcm_park_lock() {
	Serial.println();
	Serial.println("--TESTING VOLT BCM DRIVEN PARK LOCK SENSE--");
	Serial.println();
	Serial.print("External Stimulation; Park Lock Sense is sensed as ");
	if(digitalRead(SHIFT_OK)) {
		Serial.println("HIGH"); 
	} else {
		Serial.println("LOW");
	}
	Serial.println();
    delay(3000);
    // Testing that we are able to drive pin 14, the shift interlock driver 
    Serial.println("Testing the inversion method for shift solenoid..");
    Serial.print("Volt shift signal sensed as; ");
    Serial.println(digitalRead(SHIFT_OK));
    Serial.println();
    Serial.print("Driving the park output as ");
    Serial.println(park_lock(digitalRead(SHIFT_OK)));
    digitalWrite(PARK_DRIVE, park_lock(digitalRead(SHIFT_OK)));
    delay(3000);
    Serial.println("Resetting...");
    digitalWrite(PARK_DRIVE, LOW);
    delay(3000);
    Serial.println();
}

void test_accessory_wakeup() {
	Serial.println();
	Serial.println("--TESTING ACCESSORY WAKEUP--");
	Serial.println();
	Serial.print("External Stimulation; Accessory Wakeup Sensed is ");
	if(digitalRead(ASSY_WAKE)) {
		Serial.println("HIGH");
	} else {
		Serial.println("LOW");
	}
	delay(1000);
	Serial.println();
}

void test_run_crank() {
   	Serial.println();
	Serial.println("--TESTING RUN/CRANK--");
	Serial.println();
	Serial.print("External Stimulation; Run/Crank Sensed is ");
	if(digitalRead(RUN_CRANK)) {
		Serial.println("HIGH");
	} else {
		Serial.println("LOW");
	}
	delay(1000);
    Serial.println();     
}

void test_park_lock() {
    Serial.println();
    Serial.println("--TESTING PARK LOCK--");
    Serial.println();
    Serial.print("External Stimulation; Shift OK Senses As ");
    if(SHIFT_OK) {
        Serial.println("HIGH");
        Serial.println();
        Serial.println("Driving the park lock solenoid to release shifter.");
        digitalWrite(SHIFT_DRIVE, HIGH);
    } else {
        Serial.println("LOW");
        Serial.println();
        Serial.println("Sensed as low, not freeing the shifter.");
    }
    Serial.println();
    delay(1000);
    Serial.println();
}

// tests the high-speed GMLAN modules by communicating between them using Collin Kidder's example  
void test_gmlanhs() {
    Serial.println();
    Serial.println("--TESTING GMLANHS CHANNELS--");
    Serial.println();
    Serial.println("Transmitting CAN Messages...");
    Serial.println();
    Can1.write(ghs2out);
    Can0.write(ghs1out);
    delay(10);
    Serial.println("Done transmistting...");
    Serial.println();
    delay(100);
    CAN_message_t ghs1in;
    CAN_message_t ghs2in;
    while(Can0.available()) {
        Serial.println("GMLANHS1 Received; ");
        Can0.read(ghs1in); 
        Serial.print("CAN ID = ");
        Serial.println(ghs1in.id);
        for(int i = 0; i < 8; i++) {
            Serial.print("Data "); Serial.print(i); Serial.print(" = ");
            Serial.println(ghs1in.buf[i]);
        }
        //Serial.print(" data = ");
        //hexDump(8, ghs1in.buf);
    } 
    delay(10);
    while(Can1.available()) {
        Serial.println("GMLANHS2 Received; ");
        Can1.read(ghs2in);
        Serial.print("CAN ID = ");
        Serial.println(ghs2in.id);
        for(int i = 0; i < 8; i++) {
            Serial.print("Data "); Serial.print(i); Serial.print(" = ");
            Serial.println(ghs2in.buf[i]);
        }
        //hexDump(8, ghs2in.buf);
    }
    delay(20);
}

// tests the low-speed GMLAN using the MCP2515 interface 
void test_gmlanls() {
    Serial.println("--TESTING GMLANLS CHANNEL VIA MCP2515--");
    Serial.println();
    Serial.println("Begining read loop..");
    Serial.println();
    if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
        Serial.print("ID: ");
        Serial.print(canMsg.can_id, HEX); // print ID
        Serial.print(" DLC:"); 
        Serial.print(canMsg.can_dlc, HEX); // print DLC
        Serial.print(" Data: ");
        
        for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
            Serial.print(canMsg.data[i],HEX);
            Serial.print(" ");
        }
        Serial.println();  
    } else {
        Serial.println("Did not read any messages..");
        Serial.print("Read error as "); Serial.println(mcp2515.readMessage(&canMsg));
        Serial.println();
        Serial.print("Status of received is; "); Serial.println(mcp2515.checkReceive());
        Serial.println(); 
        Serial.print("Error is; "); Serial.println(mcp2515.getErrorFlags());
        Serial.println(); 
        Serial.println("Most recent message received: "); 
        Serial.print("ID: ");
        Serial.print(canMsg.can_id, DEC); // print ID
        Serial.print(" DLC:"); 
        Serial.print(canMsg.can_dlc, DEC); // print DLC
        Serial.print(" Data: ");
        for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
            Serial.print(canMsg.data[i],DEC);
            Serial.print(" ");
        }

        Serial.println(); 


    }

    delay(20);
    Serial.println();
    /*
    Serial.print("Attempting to send a CAN frame... ERROR is;");
    struct can_frame frame;
    frame.can_id = 0x000;
    frame.can_dlc = 4;
    frame.data[0] = 0xFF;
    frame.data[1] = 0xFF;
    frame.data[2] = 0xFF;
    frame.data[3] = 0xFF;
    Serial.println(mcp2515.sendMessage(&frame));
    Serial.println();
    */
}

// Sends and receives messages, prints the results 
void test_mcp2515(){
    Serial.println("--TESTING MCP2515--");
    Serial.println();
    Serial.println("Sending two messages.");
    Serial.println();
    Serial.println(mcp2515.sendMessage(&canMsg1));
    //mcp2515.sendMessage(&canMsg2);
    Serial.println("Messages sent.");
    Serial.println();
    Serial.println("Reading messages from the bus.");
    Serial.println();
    if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
            Serial.print("ID: ");
            Serial.print(canMsg.can_id, HEX); // print ID
            Serial.print(" DLC:"); 
            Serial.print(canMsg.can_dlc, HEX); // print DLC
            Serial.print(" Data: ");
            
            for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
                Serial.print(canMsg.data[i],HEX);
                Serial.print(" ");
            }
            Serial.println();  
    } else {
        Serial.println("Did not read any data.");
    }
    delay(100);
}