#define laser_pin 7
#define sensor_TRIG 8
#define sensor_ECHO 9

float measure_distance();

void setup()
{
  Serial.begin(9600);
  pinMode(laser_pin, OUTPUT);
  pinMode(sensor_TRIG, OUTPUT);
  pinMode(sensor_ECHO, INPUT);
  digitalWrite(laser_pin, LOW); //turn on the laser
}

void loop()
{
  char serial_data;
  float dis;
  //dis=measure_distance(sensor_TRIG, sensor_ECHO);
  //Serial.println(dis);
  //delay(1000);
  if(Serial.available()>0)//serial test
  {
    serial_data=Serial.read();
    if(serial_data=='0')
    {
      pinMode(laser_pin, LOW);
    }
    
    }
    else if(serial_data=='1')//turn on laser
    {
      pinMode(laser_pin, HIGH);
    }
    
    else if(serial_data=='2')//measure distance 3times
    {
      for(int i=0;i<=3;i++)
      {
        dis=measure_distance(sensor_TRIG, sensor_ECHO);
        Serial.println(dis);
        delayMicroseconds(100);
      }
    }
}

inline float measure_distance(int TRIG, int ECHO)
{
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH) * 0.017;
}
