#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

unsigned char buffer[64]; // buffer array for data recieve over serial port
int count=0;     // counter for buffer array 

#define SoftPowerPin   2

int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

volatile int Col=0,Row=0;
volatile char LCDBuffer[16][50][1]; 
volatile int dispRow;



void setup()  
{
  for (int a=0; a<16; a++)
    for (int b=0; b<50; b++)
      LCDBuffer[a][b][1]=32;
  lcd.begin(16, 2);              // start the library
  lcd.clear();
  lcd.setCursor(0,0);
  //         0123456789012345
  lcd.print("    GSM/GPRS    ");
  lcd.setCursor(0,1);
  lcd.print("Antonis Maglaras");
  delay(5000);
  pinMode(SoftPowerPin, OUTPUT);
  // Open serial communications and wait for port to open:
  Serial.begin(19200);
  Serial.println("Power On");
  lcd.clear();
  lcd.print("Power On");
  delay(1000);
  Serial1.begin(19200);
  PowerUp();
  dispRow=0;
}

void loop() // run over and over
{
 if (Serial1.available())              // if date is comming from softwareserial port ==> data is comming from Serial1 shield
 {
   while(Serial1.available())          // reading data into char array 
   {
     buffer[count++]=Serial1.read();     // writing data into array
     if (count == 64)
       break;
   }
   Serial.write(buffer,count);            // if no data transmission ends, write buffer to hardware serial port
   clearBufferArray();              // call clearBufferArray function to clear the storaged data from the array
   count = 0;                       // set counter of while loop to zero
 }
 if (Serial.available())            // if data is available on hardwareserial port ==> data is comming from PC or notebook
   Serial1.write(Serial.read());       // write it to the Serial1 shield
   
   
 if (read_LCD_buttons()==btnUP)
 {
   dispRow-=1;
   if (dispRow<0)
     dispRow=0;
   DisplayTextOnLCD();
   delay(150);
 }
 
 if (read_LCD_buttons()==btnDOWN)
 {
   dispRow+=1;
   if (dispRow>Row)
     dispRow=Row-1;
   DisplayTextOnLCD();
   delay(150);
 }   
 
 if (read_LCD_buttons()==btnLEFT)
 {
   delay(100);
   AnswerCall();
 }
 
 if (read_LCD_buttons()==btnSELECT)
 {
   if (read_LCD_buttons()==btnSELECT)
     while (read_LCD_buttons()==btnSELECT) {};
   delay(100);
   DialOut();
 }
 
 if (read_LCD_buttons()==btnRIGHT)
 {
   delay(100);
   HangUpCall();
 }
 
}



void clearBufferArray()              // function to clear buffer array
{
  for (int i=0; i<count; i++)
  { 
    if ((buffer[i]>=32) && (buffer[i]<127))
    {
      if (Col>15)
      {
        Row++;
        if (Row>48)
        {
          EmptyOneLineFromBuffer();
          Row-=1;
          Col=0;
        }
        Col=0;
      }
      LCDBuffer[Col][Row][1]=buffer[i];
      Col++;
    }
    else
      if (buffer[i]==13)
      {
        boolean twocrs=false;
        for (int ena=0; ena<16; ena++)
          if (LCDBuffer[ena][Row][1]!=32)
             twocrs=true;
        if (twocrs)
        {
          Row++;
          if (Row>48)
          {
            EmptyOneLineFromBuffer();
            Row-=1;
            Col=0;
          }
          Col=0;
        }
      }
    buffer[i]=NULL;
  }
  dispRow=Row-1;
  DisplayTextOnLCD();
}

void PowerUp()
{
 digitalWrite(SoftPowerPin,LOW);
 delay(100);
 digitalWrite(SoftPowerPin,HIGH);
 delay(2000);
 digitalWrite(SoftPowerPin,LOW);
}

void PowerDown()
{
 digitalWrite(SoftPowerPin,LOW);
 delay(100);
 digitalWrite(SoftPowerPin,HIGH);
 delay(2000);
 digitalWrite(SoftPowerPin,LOW);
}


// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor
 // my buttons when read are centered at these valies: 0, 131, 306, 478, 720
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 if (adc_key_in < 50)   return btnRIGHT; 
 if (adc_key_in < 185)  return btnUP;
 if (adc_key_in < 360)  return btnDOWN;
 if (adc_key_in < 530)  return btnLEFT;
 if (adc_key_in < 770)  return btnSELECT;  
 return btnNONE;  // when all others fail, return this...
}
 

void DisplayTextOnLCD()
{
  lcd.setCursor(0,0);
  for (int tmp=0; (tmp<2); tmp++)
    for (int tCol=0; tCol<16; tCol++)
    {
      lcd.setCursor(tCol,tmp);
      lcd.print(LCDBuffer[tCol][tmp+dispRow][1]);
    }
}


void AnswerCall()
{
  Serial1.println("ATA");
  DisplayTextOnLCD();
}

void HangUpCall()
{
  Serial1.println("+++");
  delay(50);
  Serial1.println("ATH");
  DisplayTextOnLCD();
}

void DialOut()
{
  lcd.clear();
  lcd.print("Dial:");
  boolean StayInside=true;
  char* number = "";
  int num=0;
  int digit=1;
  String dialno="";
  boolean dialout=false;
  while (StayInside)
  {
    lcd.setCursor(5+digit,0);
    lcd.print(num);
    switch (read_LCD_buttons())
    {
      case btnUP:
        num+=1;
        if (num>9)
          num=0;
        lcd.setCursor(5+digit,0);
        lcd.print(num);
        delay(200);
        break;
      case btnDOWN:
        num-=1;
        if (num<0)
          num=9;
        lcd.setCursor(5+digit,0);
        lcd.print(num);         
        delay(200);
        break;
      case btnLEFT:
        lcd.setCursor(5+digit,0);
        lcd.print(" ");
        digit-=1;
        if (digit<1)
          digit=1;
        num=0;
        lcd.setCursor(5+digit,0);
        lcd.print(num);         
        dialno.substring(0, digit);
        delay(200);
        break;
      case btnRIGHT:
        digit+=1;
        if (digit>10)
        {
          digit=10;
          dialno.substring(0,9);
          dialno+=String(num);
        }
        else
        {
          dialno+=String(num);
          num=0;
        }
        lcd.setCursor(5+digit,0);
        lcd.print(num);
        delay(200);
        break; 
      case btnSELECT:
        if (dialno.length()<10)
          dialno+=String(num);
        StayInside=false;
        dialout=true;
        delay(200);
        break;
    }
  }
  if (dialout)
  {
    lcd.clear();
    lcd.print("Dial ");
    lcd.print(dialno);
    if (Selection())
    {
      Serial1.println("ATD ++30"+dialno);
    }
  }
  DisplayTextOnLCD();
}


boolean Selection()
{
  lcd.setCursor(0,1);
  //         0123456789012345
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("    YES   NO    ");
  boolean StayInside=true;
  boolean yes=true, whattoreturn=false;
  while (StayInside)
  {
    if (yes)
    {
      lcd.setCursor(3,1);
      lcd.print("[");
      lcd.setCursor(7,1);
      lcd.print("]");
      lcd.setCursor(9,1);
      lcd.print(" ");
      lcd.setCursor(12,1);
      lcd.print(" ");
    }
    else
    {
      lcd.setCursor(3,1);
      lcd.print(" ");
      lcd.setCursor(7,1);
      lcd.print(" ");
      lcd.setCursor(9,1);
      lcd.print("[");
      lcd.setCursor(12,1);
      lcd.print("]");
    }
    if (read_LCD_buttons()==btnSELECT)
    {
      while (read_LCD_buttons()==btnSELECT);
      if (yes) 
      {
        StayInside=false;
        whattoreturn=true;
      }
      else
      {
        StayInside=false;
        whattoreturn=false;
      }
    }
    if (read_LCD_buttons()==btnLEFT)
    {
      yes=true;
      delay(100);
    }
    if (read_LCD_buttons()==btnRIGHT)
    {
      yes=false;
      delay(100);
    }
  }
  if (yes)
    return true;
  else
    return false;
}

void EmptyOneLineFromBuffer()
{
  for (int tRow=0; tRow<48; tRow++)
  {
    for (int tCol=0; tCol<16; tCol++)
    {
      LCDBuffer[tCol][tRow][1] = LCDBuffer[tCol][tRow+1][1];
    }
  }
  for (int tCol=0; tCol<16; tCol++)
  {
    LCDBuffer[tCol][48][1]=32;
    LCDBuffer[tCol][49][1]=32;
  }
  Row=48;
  dispRow=47;
  DisplayTextOnLCD();
}
