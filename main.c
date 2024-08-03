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
  *   Digital IO 12 --> High Sensor for the top tank. 
  *   Digital IO 11 --> Low sensor for the top tank.
  *   Digital IO 10 --> High sensor for the bottom tank.
  *   Digital IO 09 --> Low sensor for the bottom tank.
  *   Digital IO 08 --> Water motor pump signal.
  *   Digital IO 07 --> Water supply (Solenoid valve) signal.
  */


#define u8 char
/*Pin definition*/
#define topHighSensor     12
#define topLowSensor      11
#define bottomHighSensor  10
#define bottomLowSensor   9
#define motorPump         8
#define waterSupply       7

char bottomTankEmpty = 0;
//char topTankEmpty = 0;
//char bottomTankFull = 0;
//char topTankFull = 0;


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

//The loop function runs over and over again forever
void loop() {

  int topHighState = digitalRead(topHighSensor);
  int topLowState = digitalRead(topLowSensor);
  int bottomHighState = digitalRead(bottomHighSensor);
  int bottomLowState = digitalRead(bottomLowSensor);

  /*
   * If the top tank is empty check the bottom tank, if not empty start the motor.
   * If the bottom tank is also empty, turn off the pump and set the flag bottomTankEmpty.
   * Once the water supply is there, the tank starts filling.
   * Once bottomLowState becomes HIGH check if the tank was empty, if yes then wait for some time and start the pump. 
   * Wating is to avoid pump toggling.
  */
  if(topLowState == 0)
  {
     if(bottomLowState != 0)//is it empty?
     {
       if(bottomTankEmpty == 1) //was it empty?
       {
         	delay(3000);
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
   * if the top tank is full, turn off the water pump.
   */
  if(topLowState == HIGH && topHighState == HIGH)
  {
      digitalWrite(motorPump, LOW);
      Serial.print("Motor OFF");
  }

  /*
   * If the bottom tank is full, check the top tank, if not full then turn on the water pump, or else cut off the water supply.
   * 
   */
  if(bottomHighState == 1) //is bottom tank full?
  {
    if(topHighState != 1) //is top tank not full?
    {
      digitalWrite(motorPump, HIGH);
      Serial.print("Motor ON ");  
    }
    else
    {
      digitalWrite(waterSupply, HIGH);
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
