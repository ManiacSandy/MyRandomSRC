/*
Botsumo Event Source Code for Arduino
Coding by Sandeep Kumar Panda
Feel free to modify !!
If further Doucmentation or Clarification is necessary, 
Mail me @ b213033@iiit-bh.ac.in
Source Code Licensed under GNU GPL
Materials used -> UltraSonic Sensor ( 3 Pin ), Motor ( 2 Pins per motor ), Infrared Sensor ( 1 Pin per sensor )
*/

const int max_sensors = 8; // Number of Sensors present in the bot
const int US = 4; // Number of Ultra Sonic Sensors
const int motor_output = 4; // Number of Motor pins ( Output )
const int max_lag = 1000; // Maximum Lag set by the programmer
const int Direction[] = {6,7,8,9,10,11,12,13}; // Pins set for light sensor pins
const int Motor[] = {2,3,4,5}; // Pins set for motor pins
const int lag[] = {max_lag, max_lag / 2, max_lag / 4, max_lag / 8, max_lag / 16}; // Lag values in decreasing order in an array
const int sensors_ps = 2; // Number of Sensors per side in the bot ( "ps" refers to "per side" )

// Change these values according to the orientation of your sensors
// Change from here
const int sensor_offset[][motor_output / 2] = {{0,1},{1,0},{1,0},{0,1}}; // Reference values ( Change it when necessary )
const int us_offset[][motor_output] = {{0,0,0,1},{1,0,0,1},{1,0,0,0},{0,1,1,0}}; // Reference values ( Change it when necessary )
const int def_offset[][motor_output] = {{1,0,1,0},{0,1,0,1}}; // Reference values ( Change it when necessary )
//Stop Now

int itr = 0; // Iterative Variable
int i = 0;// Itrative Variable
int Motor_Instance [motor_output - 1]; // Instance to be passed as the final output to the motor
int s_flag = 0; // Sensor Flag variable to check if the infrared or utlra sonic senors respond or not 
int d_flag = 1; // Flag to check whether to keep the default motor instance set or not ( Influenced under the 's_flag' falg variable ) 
int random_instance = 0; // a random number between '0' and '1'
boolean Instance_bool[max_sensors  + US - 1]; // Boolean Instances captured from the Light and Ultra Sonic Sensors 


void setup()
{
  //Setting Pin Mode for Motor and Sensor Pins
  
  for ( itr = 0; itr < max_sensors + motor_output ; ++itr )
  {
    if( itr < motor_output )
      pinMode(Motor[itr], OUTPUT);
    else
      pinMode(Direction[itr - motor_output], INPUT);
  }
  
  //Setting Pin Mode for Ultra Sonic Sensor
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
}

void loop()
{
  for ( itr = 0; itr < max_sensors ; ++itr )
    Instance_bool[itr] = digitalRead(Direction[0]);
    
  // Reading Instances from the Ultra Sonic Sensor
  
  Instance_bool[itr] = digitalRead(A0);++itr;
  Instance_bool[itr] = digitalRead(A1);++itr;
  Instance_bool[itr] = digitalRead(A2);++itr;
  Instance_bool[itr] = digitalRead(A3);
  
  // Finished Reading Instances
  
  // Main Secret Algorithm
  
  for ( itr = 0; itr < motor_output; ++itr )
    Motor_Instance[itr] = 1;
    
  for ( itr = 0 ; itr < max_sensors  ; itr += sensors_ps )
  {
    for ( i = 0 ; i < sensors_ps ; ++i )
    {
      if ( Instance_bool[itr + i] == HIGH )
      {
        s_flag = 1;
        Motor_Instance[itr % (sensors_ps * sensors_ps)] *= sensor_offset[itr / sensors_ps][0];
        Motor_Instance[(itr % (sensors_ps * sensors_ps)) + 1] *= sensor_offset[itr / sensors_ps][1];    
      }
    }
  }
  
  if (!s_flag)
  {
    for ( itr ; itr < max_sensors + US ; ++itr )
    {
      if ( Instance_bool[itr] == HIGH )
      {
        s_flag = 1;
        for ( i = 0 ; i < motor_output ; ++i )
          Motor_Instance[i] = us_offset[itr][i];
      }
    }
  }
  
  if(s_flag)
    d_flag = 1;
  
  if(!s_flag)
  {
    if(d_flag)
    {
      d_flag = 0;
      random_instance = random(0,(motor_output / 2));
    }
    for ( itr = 0 ; itr < motor_output ; ++itr )
      Motor_Instance[itr] = def_offset[random_instance][itr];
  }
  
  // End of Parsing the Secret Algorithm
  
  // Passing on the final instance (Motor_Instance) to the motor
  
  for ( itr = 0 ; itr < motor_output ; ++itr )
  {
    if ( Motor_Instance[itr] == 1 )
      digitalWrite(Motor[itr], HIGH);
    else
      digitalWrite(Motor[itr], LOW);
  }
  delay(lag[2]);
  for ( itr = 0 ; itr < motor_output ; ++itr )
    digitalWrite(Motor[itr], LOW);
  delay(lag[4]);
 }

