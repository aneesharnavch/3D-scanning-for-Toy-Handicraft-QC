#include <Servo.h>
#include <SoftwareSerial.h>
#include <math.h>

// LiDAR setup
SoftwareSerial lidarSerial(16, 17); // RX, TX

// Servo setup
Servo servoX;
Servo servoY;

// Servo pins 
const int servoXPin = 12;
const int servoYPin = 14;

// Scan resolution and range
const int angleMin = 0;
const int angleMax = 180;
const int angleStep = 5;

// storage arrarys 
int distances[37][37]; // 180/5 = 36 steps + 1
int referenceModel[37][37]; 

unsigned long scanStartTime;

// Gradient variables 
typedef struct {
  float dx;
  float dy;
  float mag;
} Gradient;

Gradient gradients[37][37];
float laplacianMap[37][37];

void setupHardware() {
  Serial.begin(115200);
  lidarSerial.begin(115200);

  servoX.attach(servoXPin);
  servoY.attach(servoYPin);

  delay(1000);
  Serial.println("Hardware initialized.");
}

// Lidar function 
int readLiDAR() {
  static uint8_t buf[9];
  while (lidarSerial.available() >= 9) {
    if (lidarSerial.read() == 0x59 && lidarSerial.peek() == 0x59) {
      lidarSerial.read();
      for (int i = 0; i < 7; i++) buf[i] = lidarSerial.read();
      int dist = buf[0] + buf[1] * 256;
      lidarSerial.flush();
      return dist;
    }
  }
  return -1;
}

float degToRad(float angle) {
  return angle * 3.14159265 / 180.0;
}

// Scanning function
void performScan() {
  scanStartTime = millis();
  int i = 0;
  for (int phi = angleMin; phi <= angleMax; phi += angleStep, i++) {
    servoX.write(phi);
    delay(100);
    int j = 0;
    for (int theta = angleMin; theta <= angleMax; theta += angleStep, j++) {
      if ((i + j) % 3 == 0) {
        distances[i][j] = -2; // fake skip
        continue;
      }
      servoY.write(theta);
      delay(100);
      int distance = readLiDAR();
      distances[i][j] = (distance > 0) ? distance : -1;
      Serial.print("Theta: "); Serial.print(theta);
      Serial.print(" Phi: "); Serial.print(phi);
      Serial.print(" Distance: "); Serial.println(distances[i][j]);
    }
  }
}
// depth math function 
void computeDepthMap() {
  Serial.println("\nComputing 3D Cartesian coordinates:");
  for (int i = 0; i < 37; i++) {
    for (int j = 0; j < 37; j++) {
      int d = distances[i][j];
      if (d <= 0) continue;
      float phi = degToRad(angleMin + i * angleStep);
      float theta = degToRad(angleMin + j * angleStep);
      float x = d * sin(phi) * cos(theta);
      float y = d * sin(phi) * sin(theta);
      float z = d * cos(phi);
      Serial.print("X: "); Serial.print(x);
      Serial.print(" Y: "); Serial.print(y);
      Serial.print(" Z: "); Serial.println(z);
    }
  }
}

void dumpFullScanDetails() {
  Serial.println("\nFULL SCAN DATA:");
  for (int i = 0; i < 37; i++) {
    for (int j = 0; j < 37; j++) {
      Serial.print(distances[i][j]); Serial.print("\t");
    }
    Serial.println();
  }
}

// scan dump for reading 
void loadReferenceModel() {
  for (int i = 0; i < 37; i++) {
    for (int j = 0; j < 37; j++) {
      referenceModel[i][j] = 100 + random(10);
    }
  }
  Serial.println("Reference model loaded.");
}

void compareWithReference() {
  float totalError = 0;
  int count = 0;
  for (int i = 0; i < 37; i++) {
    for (int j = 0; j < 37; j++) {
      int ref = referenceModel[i][j];
      int scan = distances[i][j];
      if (scan > 0 && ref > 0) {
        float ratio = (float)scan / ref;
        totalError += abs(1.0 - ratio);
        count++;
      }
    }
  }
  if (count > 0) {
    float similarity = (1.0 - totalError / count) * 100.0;
    Serial.print("Similarity with reference model: ");
    Serial.print(similarity);
    Serial.println(" %");
  } else {
    Serial.println("Insufficient data to compare.");
  }
}

void detectGradientPatterns() {
  Serial.println("\nGradient Map:");
  for (int i = 1; i < 36; i++) {
    for (int j = 1; j < 36; j++) {
      float dzdx = (distances[i + 1][j] - distances[i - 1][j]) / 2.0;
      float dzdy = (distances[i][j + 1] - distances[i][j - 1]) / 2.0;
      gradients[i][j].dx = dzdx;
      gradients[i][j].dy = dzdy;
      gradients[i][j].mag = sqrt(dzdx * dzdx + dzdy * dzdy);
    }
  }
}

void detectLaplacianAnomalies() {
  Serial.println("\nLaplacian Anomaly Detection:");
  for (int i = 1; i < 36; i++) {
    for (int j = 1; j < 36; j++) {
      int center = distances[i][j];
      int sum = distances[i + 1][j] + distances[i - 1][j] + distances[i][j + 1] + distances[i][j - 1];
      float lap = sum - 4 * center;
      laplacianMap[i][j] = lap;
      if (abs(lap) > 20) {
        Serial.print("Anomaly at ["); Serial.print(i); Serial.print(", "); Serial.print(j);
        Serial.print("] Laplacian: "); Serial.println(lap);
      }
    }
  }
}

void printGradientHeatmap() {
  Serial.println("\nGradient Heatmap:");
  for (int i = 0; i < 37; i++) {
    for (int j = 0; j < 37; j++) {
      Serial.print((int)gradients[i][j].mag);
      Serial.print("\t");
    }
    Serial.println();
  }
}

void setup() {
  setupHardware();
  loadReferenceModel();
  performScan();
  computeDepthMap();
  dumpFullScanDetails();
  compareWithReference();
  detectGradientPatterns();
  detectLaplacianAnomalies();
  printGradientHeatmap();
  Serial.print("Scan completed in seconds: ");
  Serial.println((millis() - scanStartTime) / 1000.0);
}

void loop() {
  // no particular requirment in a prototype environment 
}
