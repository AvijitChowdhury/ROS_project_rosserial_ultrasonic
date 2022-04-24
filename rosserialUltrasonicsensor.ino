
#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>

ros::NodeHandle  nh;

sensor_msgs::Range range_msg;
ros::Publisher pub_range( "/ultrasound_range", &range_msg);
char frameid[] = "/ultrasound";

//this constant won't change.It's the pin number of the sensor's ouput.
//const int adc_pin = 0;
const int pingPin = 7;
const boolean CENTIMETERS = true;
const boolean INCHES = false;


//float getRange_Ultrasound(int pin_num){
//  int val = 0;
//  for(int i=0; i<4; i++) val += analogRead(pin_num);
//  float range =  val;
//  return range /322.519685;   // (0.0124023437 /4) ; //cvt to meters
//}

void setup()
{
  nh.initNode();
  nh.advertise(pub_range);
  
  
  range_msg.radiation_type = sensor_msgs::Range::ULTRASOUND;
  range_msg.header.frame_id =  frameid;
  range_msg.field_of_view = 0.1;  // fake
  range_msg.min_range = 0.002;//2 cm
  range_msg.max_range = 6.47;//150 cm
  
//  pinMode(8,OUTPUT);
//  digitalWrite(8, LOW);
}
long getRange(int pinNumber,boolean in_centimeters){
  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  }
long range_time;

void loop()
{
  
  //publish the adc value every 50 milliseconds
  //since it takes that long for the sensor to stablize
//  if ( millis() >= range_time ){
//    int r =0;

    range_msg.range = getRange(pingPin,CENTIMETERS);
    range_msg.header.stamp = nh.now();
    pub_range.publish(&range_msg);
//    range_time =  millis() + 50;
 // }
  
  nh.spinOnce();delay(500);
}
long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: https://www.parallax.com/package/ping-ultrasonic-distance-sensor-downloads/
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}
