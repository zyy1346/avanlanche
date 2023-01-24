#include <SoftwareSerial.h>
#include <IRremote.h>

# define Start_Byte     0x7E
# define Version_Byte   0xFF
# define Command_Length 0x06
# define End_Byte       0xEF
# define Acknowledge    0x00
//--------------------------------------------------------//
// Replace these hex codes with your remote control button codes
# define pause_play       0xFFC23D
# define next_song        0xFF02FD
# define prev_song        0xFF22DD
# define vol_inc          0xFFA857
# define vol_dec          0xFFE01F  
# define sound_equalizer  0xFF906F
//-------------------------------------------------------//

SoftwareSerial mySerial(10,11);

const int receiver = A1;
IRrecv irrecv(receiver);    // create instance of 'irrecv'
decode_results dec;         // create instance of 'decode_results'

int volume = 20;
int eqset = 0;
boolean Playing = false;

void setup () 
{
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode");
  
  irrecv.enableIRIn();      // Start the receiver
  mySerial.begin(9600);
  delay(1000);
  playFirst();
  Playing = true;
}

void loop () 
{  
  if(irrecv.decode(&dec))
  {
    Serial.println(dec.value, HEX); // Here you can get your remote's buttons codes
    if (dec.value==pause_play)
    {
      if(Playing)
      {
        pause();
        Playing = false;
      }
      else
      {
        Playing = true;
        play();
      }
    }
    if (dec.value==next_song)
    {
      if(Playing)
      {
        next();
      }
    }
    if (dec.value==prev_song)
    {
      if(Playing)
      {
        previous();
      }
    }
    if(dec.value==vol_inc)
    {
     volumeINC(); 
    }
    if(dec.value==vol_dec)
    {
      volumeDEC();
    }
    if(dec.value==sound_equalizer)
    {
      equalizer();
    }
    irrecv.resume();
  }
}
  
void playFirst()
{
  exe_cmd(0x3F, 0, 0);
  delay(100);
  exe_cmd(0x06, 0, volume); 
  delay(100);
  exe_cmd(0x11,0,1); 
  delay(100);
}

void pause()
{
  exe_cmd(0x0E,0,0);
  delay(100);
}

void play()
{
  exe_cmd(0x0D,0,1); 
  delay(100);
}

void next()
{
  exe_cmd(0x01,0,1);
  delay(100);
}

void previous()
{
  exe_cmd(0x02,0,1);
  delay(100);
}

void volumeINC()
{
  volume = volume+1;
  if(volume == 31)
  {
    volume = 30;
  }
  exe_cmd(0x06, 0, volume); 
  delay(100);
}

void volumeDEC()
{
  volume = volume-1;
  if(volume == -1)
  {
    volume = 0;
  }
  exe_cmd(0x06, 0, volume); 
  delay(100);
}

void equalizer()
{
  eqset = eqset+1;
  if(eqset == 6)
  {
    eqset = 0;
  }
  exe_cmd(0x07, 0, eqset);
  delay(100);
}

void exe_cmd(byte CMD, byte Par1, byte Par2)
{
  word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
  byte Command_line[10] = {Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
                            Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
  for (byte x=0; x<10; x++)
  {
    mySerial.write(Command_line[x]);
  }
}
