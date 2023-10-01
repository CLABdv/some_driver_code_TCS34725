#include <Wire.h>
#define DEVICE_ADDRESS 0x29
#define COMMAND_BIT 0x80
#define BLOCK_PROTOCOL 0b00100000
// NOTES:
// First writes are command register.  
// Depending on what the high 3 bits are, the low 5 bits do different stuff, just use the specified OR-masks
// and write the next register, and you won't have to worry about special function stuff or whatev.
// see product specification for more information.

void setup() {
  // led stuff
  const unsigned char ledPin = 13;
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,1);

  Wire.begin();
  Serial.begin(9600);

//set some misc options in the enable register
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(0b10100000);
  Wire.write(0b00001011); //Enable register. Init sensor, block data
  Wire.endTransmission();

// set integration time
  Wire.beginTransmission(0x29);
  Wire.write(0x80 | BLOCK_PROTOCOL | 0x01); //integration time, whatever that is. i think it is how long a read is
  Wire.write(0x00); // Maximum
  Wire.endTransmission();

// set gain (control register)
  Wire.beginTransmission(0x29);
  Wire.write(0x80 | BLOCK_PROTOCOL | 0x0F);
  Wire.write(0b10); // only the lowest two bits matter
  Wire.endTransmission();
  Serial.println("hellooo");

}

void loop() {
  //now requesting information
// read all the colour data.
  Wire.beginTransmission(0x29);
  Wire.write(0x14 | COMMAND_BIT | BLOCK_PROTOCOL); // writes to command register to read from CDATAL, which is the lowest of the data registers
  Wire.endTransmission();
  Wire.requestFrom(DEVICE_ADDRESS,8);
  unsigned char data[8]={0};
  char sbuf[50]={0};
  int i=0;
  while(Wire.available())
  {
    data[i]=0;
    data[i]=Wire.read();
    /*
    Serial.print("Loop at: ");
    Serial.println(i);
    Serial.print("Value: ");
    Serial.println(data[i],HEX);
    */
    ++i;
  }
  // just for integration, this is super high to overcompensate for me being lazy and not looking up how long is needed
  // TODO: read product specification about integration time
  delay(1000); 
  

  unsigned short CDAT = data[0] + (data[1] << 8);
  unsigned short RDAT = data[2] + (data[3] << 8);
  unsigned short GDAT = data[4] + (data[5] << 8);
  unsigned short BDAT = data[6] + (data[7] << 8);
  sprintf(sbuf,"RAW: %04X %04X %04X %04X",CDAT,RDAT,GDAT,BDAT);
  Serial.println(sbuf);
  float r,g,b;
  if (CDAT == 0)
  {
    r = g = b = 0;
  }
  unsigned int sum = CDAT;
  r = (float)RDAT / sum;
  g = (float)GDAT / sum;
  b = (float)BDAT / sum;
  sprintf(sbuf,"Human:\nRed: %d    Green: %d    Blue: %d\n",(int)(r*255),(int)(g*255),(int)(b*255));
  Serial.println(sbuf);
}
