#include "CurieIMU.h"
#include "MadgwickAHRS.h"

Madgwick filter; // initialise Madgwick object
int ax, ay, az;
int gx, gy, gz;
float yaw;
float pitch;
float roll;
int factor = 800; // variable by which to divide gyroscope values, used to control sensitivity
// note that an increased baud rate requires an increase in value of factor

int calibrateOffsets = 1; // int to determine whether calibration takes place or not


void setup() {
  // initialize Serial communication
  Serial.begin(9600);

  // initialize device
  CurieIMU.initialize();

  // verify connection
  if (!CurieIMU.testConnection()) {
    Serial.println("CurieImu connection failed");
  }


  if (calibrateOffsets == 1) {
    // use the code below to calibrate accel/gyro offset values
    Serial.println("Internal sensor offsets BEFORE calibration...");
    Serial.print(CurieIMU.getXAccelOffset()); Serial.print("\t");
    Serial.print(CurieIMU.getYAccelOffset()); Serial.print("\t");
    Serial.print(CurieIMU.getZAccelOffset()); Serial.print("\t");
    //Serial.print(CurieImu.getXGyroOffset()); Serial.print("\t");
    //Serial.print(CurieImu.getYGyroOffset()); Serial.print("\t");
    //Serial.print(CurieImu.getZGyroOffset()); Serial.print("\t");
    Serial.println("");

    // To manually configure offset compensation values, use the following methods instead of the autoCalibrate...() methods below
    //    CurieImu.setXGyroOffset(220);
    //    CurieImu.setYGyroOffset(76);
    //    CurieImu.setZGyroOffset(-85);
    //    CurieImu.setXAccelOffset(-76);
    //    CurieImu.setYAccelOffset(--235);
    //    CurieImu.setZAccelOffset(168);

    //IMU device must be resting in a horizontal position for the following calibration procedure to work correctly!

    Serial.print("Starting Gyroscope calibration...");
    CurieIMU.autoCalibrateGyroOffset();
    Serial.println(" Done");
    Serial.print("Starting Acceleration calibration...");
    CurieIMU.autoCalibrateXAccelOffset(0);
    CurieIMU.autoCalibrateYAccelOffset(0);
    CurieIMU.autoCalibrateZAccelOffset(1);
    Serial.println(" Done");

    Serial.println("Internal sensor offsets AFTER calibration...");
    Serial.print(CurieIMU.getXAccelOffset()); Serial.print("\t");
    Serial.print(CurieIMU.getYAccelOffset()); Serial.print("\t");
    Serial.print(CurieIMU.getZAccelOffset()); Serial.print("\t");
    //Serial.print(CurieImu.getXGyroOffset()); Serial.print("\t");
    //Serial.print(CurieImu.getYGyroOffset()); Serial.print("\t");
    //Serial.print(CurieImu.getZGyroOffset()); Serial.print("\t");
    Serial.println("");
    Serial.println("Enabling Gyroscope/Acceleration offset compensation");
    CurieIMU.setGyroOffsetEnabled(true);
    CurieIMU.setAccelOffsetEnabled(true);
  }

  digitalWrite(13, HIGH);
}

void loop() {
  // read raw accel/gyro measurements from device
  ax = CurieIMU.getAccelerationX();
  ay = CurieIMU.getAccelerationY();
  az = CurieIMU.getAccelerationZ();
  gx = CurieIMU.getRotationX();
  gy = CurieIMU.getRotationY();
  gz = CurieIMU.getRotationZ();

  // use function from MagdwickAHRS.h to return quaternions
  filter.updateIMU(gx / factor, gy / factor, gz / factor, ax, ay, az);

  // functions to find yaw roll and pitch from quaternions
  yaw = filter.getYaw();
  roll = filter.getRoll();
  pitch = filter.getPitch();
  
  // print gyro and accel values for debugging only, comment out when running Processing
  
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.print(gz); Serial.print("\t");
  Serial.println("");
  

  if (Serial.available() > 0) {
    int val = Serial.read();
    if (val == 's') { // if incoming serial is "s"
      Serial.print(ax);
      Serial.print(","); // print comma so values can be parsed
      Serial.print(ay);
      Serial.print(","); // print comma so values can be parsed
      Serial.println(az);
    }
  }
}
