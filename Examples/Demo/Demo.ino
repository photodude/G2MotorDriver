// Include the G2MotorDriver library
#include "G2MotorDriver.h"

// Initialize library
G2MotorDriver md;

void stopIfFault()
{
  if (md.getFault())
  {
    Serial.println("Motor fault");
    while(1);
  }
}

void setup()
{
  // start serial port
  Serial.begin(115200);
  Serial.println("G2 Motor Driver");

  // Start up the library
  md.init();
}

void loop()
{
  for (int i = 0; i <= 400; i++)
  {
    md.setSpeed(i);
    stopIfFault();
    if (i%200 == 100)
    {
      Serial.print("Motor current: ");
      Serial.println(md.getCurrentMilliamps());
    }
    delay(2);
  }
  
  for (int i = 400; i >= -400; i--)
  {
    md.setSpeed(i);
    stopIfFault();
    if (i%200 == 100)
    {
      Serial.print("Motor  current: ");
      Serial.println(md.getCurrentMilliamps());
    }
    delay(2);
  }
  
  for (int i = -400; i <= 0; i++)
  {
    md.setSpeed(i);
    stopIfFault();
    if (i%200 == 100)
    {
      Serial.print("Motor  current: ");
      Serial.println(md.getCurrentMilliamps());
    }
    delay(2);
    }
}
