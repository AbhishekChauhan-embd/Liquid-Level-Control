/*===========================================================================*
*     ////    //////   //   //  //////   ///////  //   //  ///////  //   //   *
*   //    //  //   //  //   //    //    //        //   //  //       //  //    *
*   //    //  //   //  //   //    //    //        //   //  //       // //     *
*   ////////  //////   ///////    //     //////   ///////  /////    ////      *
*   ////////  //   //  //   //    //          //  //   //  //       // //     *
*   //    //  //   //  //   //    //          //  //   //  //       //  //    *
*   //    //  //////   //   //  //////  ///////   //   //  ///////  //   //   *
 *===========================================================================*/


 /*
  * Connection:
  *   Digital IO 12 --> High Sensor for top tank. 
  *   Digital IO 11 --> Low sendor for top tank.
  *   Digital IO 10 --> High sensor for bottom tank.
  *   Digital IO 09 --> Low sensor for bottom tank.
  *   Digital IO 08 --> Water motor pump signal.
  *   Digital IO 07 --> Water supply (Solenoid valve) signal.
  */


#define u8 char
/*Pin defination*/
#define topHighSensor     12
#define topLowSensor      11
#define bottomHighSensor  10
#define bottomLowSensor   9
#define motorPump         8
#define waterSupply       7

char bottomTankEmpty = 0;
char bothTankFullFlg = 0;
//char topTankEmpty = 0;
//char bottomTankFull = 0;
//char topTankFull = 0;

/*
 *Calibratoin section.
 */
int EMPTNKDLY = 3000;
int FULTNKDLY = 5000;
char TNK1PRSNT;
char TNK2PRSNT;
char NUMOFTNK = 1;


void setup() {
  /* initialize digital input/output pins.*/
  pinMode(topHighSensor, INPUT);
  pinMode(topLowSensor, INPUT);
  pinMode(bottomHighSensor, INPUT);
  pinMode(bottomLowSensor, INPUT);
  pinMode(motorPump, OUTPUT);
  pinMode(waterSupply, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  
  /*
   *For single tank
   */
  if(NUMOFTNK == 1)
  {
    int HighState = digitalRead(topHighSensor);
    int LowState = digitalRead(topLowSensor);
    
    if(LowState == 0)
    {
      digitalWrite(motorPump, HIGH);
      Serial.print("Motor ON ");
    }
    if(HighState == 1)
    {
    	digitalWrite(motorPump, LOW);
        Serial.print("Motor OFF"); 
    }
  }
  
  
  /*
   *For two tank
   */
  if(NUMOFTNK == 2)
  {

      int topHighState = digitalRead(topHighSensor);
      int topLowState = digitalRead(topLowSensor);
      int bottomHighState = digitalRead(bottomHighSensor);
      int bottomLowState = digitalRead(bottomLowSensor);

      /*
       * If top tank is empty check bottom tank,if not empty start motor.
       * If bottom tank is also empty, turn off pump and set the flag bottomTankEmpty.
       * Once water supply is there and tank start filling.
       * Once bottomLowState becomes HIGH check if tank was empty, if yes then wait for some time and start the pump. 
       * Wating is to avoid pump toggling.
      */
      if(topLowState == 0)
      {
         if(bottomLowState != 0)//is it empty?
         {
           if(bottomTankEmpty == 1) //was it empty?
           {
                delay(EMPTNKDLY);
                bottomTankEmpty = 0;
                digitalWrite(motorPump, HIGH);
                Serial.print("Motor ON ");   
           }
           else
           {
              digitalWrite(motorPump, HIGH);
              Serial.print("Motor ON ");
           }
        }
        else
        {
            digitalWrite(motorPump, LOW);
            Serial.print("Motor OFF");
            bottomTankEmpty = 1;
            Serial.print(" ");
        }
      }

      /*
       * if top tank is full, turn off the water pump.
       */
      if(topLowState == HIGH && topHighState == HIGH)
      {
          digitalWrite(motorPump, LOW);
          Serial.print("Motor OFF");
      }
      /*
       * if bottom tank is empty, turn off the water pump.
       */	
      if(bottomLowState == 0)
      {
        digitalWrite(motorPump, LOW);
        Serial.print("Motor OFF");
        bottomTankEmpty = 1;

      }
      /*
       * if top tank is not full and water fills in bottom tank then turn on the pump.
       * This featur is Optional.
       */
      else if(bottomLowState == 1 && bottomTankEmpty == 1 )
      {
        delay(EMPTNKDLY);
        bottomTankEmpty = 0;
        digitalWrite(motorPump, HIGH);
        Serial.print("Motor ON ");  
      }

      /*
       * If bottom tank if full, check top tank, if not full then turn on the water pump else cut off the water supply.
       * 
       */
      if(bottomHighState == 1) //is bottom tank full?
      {
        if(topHighState != 1) //is top tank not full?
        {
          if(bothTankFullFlg == 1)
          {
            delay(FULTNKDLY);
            digitalWrite(motorPump, HIGH);
            Serial.print("Motor ON ");
            bothTankFullFlg = 0;
          }
          else
          {
            digitalWrite(motorPump, HIGH);
            Serial.print("Motor ON ");  
          }
        }
        else
        {
          digitalWrite(waterSupply, HIGH);
          bothTankFullFlg = 1;
          Serial.print("Water Supply OFF"); 
        }
      }
      else
      {
        digitalWrite(waterSupply, LOW);
        Serial.print("Water Supply ON");
      }

      Serial.print(topHighState);
      Serial.print(topLowState);
      Serial.print(bottomHighState);
      Serial.print(bottomLowState);
      Serial.print(" ");
      Serial.print(bottomTankEmpty);

      Serial.print("\n");
  }
}
