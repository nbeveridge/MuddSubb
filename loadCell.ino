

#include "HX711.h"

#define DOUT  3
#define CLK  2

float calibration1 = .7;
float calibration2 = 1.7;
float calibration3 = 2.7;

int calibrationState = 0;

float calibrationFactor1 = 0;
float calibrationFactor2 = 0;
float calibrationFactor3 = 0;



HX711 scale;

float calibration_factor = -7050; //-7050 worked for my 440lb max scale setup

void setup() {
  Serial.begin(9600);
  Serial.println("Digital Scale");
  Serial.println("Remove all weight from scale");

  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);

  pinMode(7, INPUT);
  pinMode(9, INPUT);  
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  Serial.print("Reading: ");
  Serial.print(scale.get_units(), 1);
  Serial.print(" lbs"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();


  if((digitalRead(7) == HIGH))  { 
    delay(100);

    calibrate3();   //weight of airpods =.1lbs

    if(calibrationState >=3) {
      calibrationState = 0;
      }
    else {
      calibrationState +=1;
    }
  }



  Serial.print("Reading: ");
  Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" lbs"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();

  
}


void calibrate3() {
  if(calibrationState==1) {
    calibrationFactor1 = calibrate(calibration1);
    Serial.print("calibration 1 complete");
    Serial.println(calibration_factor);
  }
  else if(calibrationState==2) {
    calibrationFactor2 = calibrate(calibration2);
    Serial.print("calibration 2 complete");
    Serial.println(calibration_factor);
  }
  else if(calibrationState==3) {
    calibrationFactor3 = calibrate(calibration3);
    Serial.print("calibration 3 complete");
    Serial.println(calibration_factor);
    Serial.println("new calibration factor to left");

    scale.set_scale((calibrationFactor1+calibrationFactor2+calibrationFactor3)/3);
  }
 }


int calibrationIncrease = 400;

int calibrate(float weight)  {
  Serial.println("calibrating");
  Serial.println(scale.get_units());
  
  while(  (scale.get_units() > (weight+.1) )  || (scale.get_units() <= (weight-.1))  ){            //change this so it just has to be withing some range close enough


    Serial.println("not equals");
    Serial.println(calibration_factor);

    Serial.println(scale.get_units());


    
    if(  (scale.get_units())< weight)  {       // dive into header file to figure out how calibration factor works
      calibration_factor += calibrationIncrease;
      Serial.println("adding");
      }
    else  {
      calibration_factor -= calibrationIncrease; ///change to 10
      Serial.println("subtracting");

      }
    scale.set_scale(calibration_factor);
    }
  Serial.println("done calibrating");
  delay(5000);

  return calibration_factor;
  }


//Add in calibrations for multiple weights then average out the calibration factor to find the best calibration factor

// shits fucked up... fix messed up calibration factor averageing
