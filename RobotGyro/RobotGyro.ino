
/*
* The code is released under the GNU General Public License.
* Developed by www.codekraft.it 
*/

#include "CurieIMU.h"

// IMU
float FS_SEL = 131;                                  // IMU gyro values to degrees/sec
unsigned long last_read_time;
float angle_z;                                       // These are the result angles
float last_z_angle;                                  // These are the filtered angles
float lGZA;                                          // Store the gyro angles to compare drift

// FUNCTIONS
void set_last_read_angle_data(unsigned long timee, float z, float z_gyro) 
{
  last_read_time = timee;
  last_z_angle = z;
  lGZA = z_gyro;
}

void setup() {
  
  Serial.begin(9600);
  while (!Serial);

  // init CurieImu
  CurieIMU.begin();
  // use the code below to calibrate accel/gyro offset values
  Serial.println("Internal sensor offsets BEFORE calibration...");

  Serial.print(CurieIMU.getZAccelOffset()); 
  Serial.print("\t");
  
  Serial.println(CurieIMU.getZGyroOffset());
  Serial.println("About to calibrate. Make sure your board is stable and upright");
  delay(1000);
  // The board must be resting in a horizontal position for 
  // the following calibration procedure to work correctly!
  Serial.print("Starting Gyroscope calibration...");
  CurieIMU.autoCalibrateGyroOffset();
  Serial.println(" Done");
  Serial.print("Starting Acceleration calibration...");
  CurieIMU.autoCalibrateAccelerometerOffset(Z_AXIS, 1);
  Serial.println(" Done");
  Serial.println("Enabling Gyroscope/Acceleration offset compensation");
  CurieIMU.setGyroOffsetEnabled(true);
  CurieIMU.setAccelOffsetEnabled(true);

  set_last_read_angle_data(millis(), 0, 0);
    CurieIMU.autoCalibrateGyroOffset();

}
 
void loop() {
  unsigned long t_now = millis();
  int ax;
  int ay;
  int az;

  CurieIMU.readAccelerometer(ax, ay, az);

  int gx;
  int gy;
  int gz;
  
  CurieIMU.readGyro(gx, gy, gz);

  float gyro_z = gz/FS_SEL;
  
  float dt =(t_now - last_read_time)/1000.0;

  float gyroZ = gyro_z*dt + last_z_angle;  

  float unfiltered_gyro_angle_z = gyro_z*dt + lGZA;

  float alpha = 0.96;
  angle_z = gyroZ;  

  set_last_read_angle_data(t_now, angle_z, unfiltered_gyro_angle_z);

  Serial.print("Z:" );
  Serial.println(angle_z);
 

}
