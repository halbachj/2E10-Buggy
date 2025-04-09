/**
 * @file GroundStation.pde
 * @author Conor Quinn & Cristina Franco Ortuno
 * @brief This is the Processing software running
 * on the ground station computer to control the buggy.
 *
 */


import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import processing.net.*;
import java.util.*;
import controlP5.*;
import java.math.BigDecimal;
import java.math.RoundingMode;

ControlP5 cp5; // UI Control
PGraphics alertScreen;
float obstacleDist = -1;
String message = "No connection";
PFont font;
float leftWheel = 0;
float rightWheel = 0;
double robstacleDist;
Client client = null; // Start as null
boolean clientConnected = false;
int connectionAttempts = 0;
int maxAttempts = 5;  // Limit retries
int lastConnectionAttempt = 0;
int buggyState;

// Matrix Variables
int cols = 12;
int rows = 8;
int cellSize = 12;
int[][] matrix = new int[rows][cols];

int[][] arrowPattern = {
    {0,0,0,0,0,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,0,0,0},
    {0,1,1,1,1,1,1,1,1,1,0,0},
    {0,0,0,0,0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,1,1,0,0,0,0},
    {0,0,0,0,0,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0}
};

int[][] stopPattern = {
    {0,0,0,0,1,1,1,1,0,0,0,0},
    {0,0,0,1,0,0,0,0,1,0,0,0},
    {0,0,1,0,1,0,0,0,0,1,0,0},
    {0,0,1,0,0,1,0,0,0,1,0,0},
    {0,0,1,0,0,0,1,0,0,1,0,0},
    {0,0,1,0,0,0,0,1,0,1,0,0},
    {0,0,0,1,0,0,0,0,1,0,0,0},
    {0,0,0,0,1,1,1,1,0,0,0,0}
};

int[][] warningPattern = {
    {0,1,0,0,0,0,0,0,0,0,1,0},
    {0,0,1,1,0,0,0,0,1,1,0,0},
    {0,0,0,0,1,0,0,1,0,0,0,0},
    {0,0,1,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,1,1,1,1,0,0,0,0},
    {0,0,0,1,0,0,0,0,1,0,0,0}
};


/**
 * @brief This is the setup method. Similar to the Arduino setup mehod.
 * 
 * In here the environment for processing is set up, like the window and the buttons.
 */
void setup() {
  size(600, 500);
  
  alertScreen = createGraphics(400, 200);
  cp5 = new ControlP5(this);
  font = createFont("Arial", 16);
  
  // Start & Stop Buttons
  cp5.addButton("start")
     .setPosition(50, 425)
     .setSize(100, 45)
     .setColorBackground(color(0, 100, 0))
     .setColorForeground(color(0, 150, 0))
     .setColorActive(color(0, 200, 0))
     .setColorLabel(color(255))
     .getCaptionLabel().setFont(font).toUpperCase(true);

  cp5.addButton("stop")
     .setPosition(450, 425)
     .setSize(100, 45)
     .setColorBackground(color(150, 0, 0))
     .setColorForeground(color(200, 50, 50))
     .setColorActive(color(240, 0, 0))
     .setColorLabel(color(255))
     .getCaptionLabel().setFont(font).toUpperCase(true);
  
  cp5.addButton("reset distance")
   .setPosition(200, 425)
   .setSize(200, 45)
   .setColorBackground(color(50, 50, 150))
   .setColorForeground(color(80, 80, 200))
   .setColorActive(color(100, 100, 255))
   .setColorLabel(color(255))
   .getCaptionLabel().setFont(font).toUpperCase(true);

  initializeMatrix(); // Default matrix values
}

/**
 * @brief The draw method is called every frame. This corresponds to the loop method in Arduino.
 */
void draw() {
  background(200);
  
  // Render UI first before handling connection
  drawWheelDistances();
  updateAlertScreen();
  drawMatrix(width /3 + 25 , 65);

  // Attempt connection only if not already connected & within retry limit
  if (!clientConnected && connectionAttempts < maxAttempts) {
    int currentMillis = millis();
    if (currentMillis - lastConnectionAttempt > 3000) { // Retry every 3 seconds
      connectToBuggy();
      lastConnectionAttempt = currentMillis;
    }
  }

  // Read data if client is connected
  if (clientConnected && client.available() >= 72) {
    byte[] buffer = new byte[72];
    client.readBytes(buffer);
    parsePacket(buffer);
  }
}

void connectToBuggy() {
  connectionAttempts++;
  println("Attempt " + connectionAttempts + ": Connecting to Buggy...");
  
  try {
    client = new Client(this, "192.168.4.1", 1234);
    delay(500); // Small delay to prevent UI freezing

    if (client.active()) {
      println("Connected to Buggy!");
      clientConnected = true;
      message = "Connected";
    } else {
      println("Connection failed.");
      client.stop();
      client = null;
    }
  } catch (Exception e) {
    println("Connection error: " + e.getMessage());
  }
}

void start() {
  if (clientConnected) {
    sendCommandPacket(1, 0);
  } else {
    println("Cannot send command, not connected.");
  }
  
  buggyState = 0;
}

void stop() {
  if (clientConnected) {
    sendCommandPacket(0, 0);
  } else {
    println("Cannot send command, not connected.");
  }
  
  buggyState = 2;
}

void initializeMatrix() {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      matrix[i][j] = 0; // Default: all cells OFF
    }
  }
}

void drawMatrix(int xOffset, int yOffset) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (matrix[i][j] == 1) {
        fill(255, 0, 0); // Red for active cells
      } else {
        fill(255, 255, 255); // White for inactive cells
      }
      stroke(0);
      rect(j * cellSize + xOffset, i * cellSize + yOffset, cellSize, cellSize);
    }
  }
}

void parsePacket(byte[] buffer) {
  ByteBuffer bb = ByteBuffer.wrap(buffer);
  bb.order(ByteOrder.LITTLE_ENDIAN);
  int type = bb.getInt();
  println(type);
  //println(boolean(bb.getInt()));
  boolean objectDetected = bb.getInt() != 0; // Convert int to boolean
  message = objectDetected ? "Object Detected" : "No Object";
  obstacleDist = bb.getFloat();
  robstacleDist = new BigDecimal(obstacleDist).setScale(2, RoundingMode.HALF_UP).doubleValue();
  println(bb.getInt());
  println(bb.getInt());
  leftWheel = bb.getFloat();
  rightWheel = bb.getFloat();
  println(bb.getInt());
  println(bb.getInt());
  //println(Arrays.toString(buffer));
  
}


void sendCommandPacket(int commandType, int data) {
  if (!clientConnected) {
    println("Not connected, cannot send command.");
    return;
  }
  ByteBuffer bb = ByteBuffer.allocate(72);
  bb.order(ByteOrder.LITTLE_ENDIAN);
  bb.putInt(3);
  bb.putInt(commandType);
  bb.putShort((short) data);
  client.write(bb.array());
}

void updateAlertScreen() {
  alertScreen.beginDraw();
  alertScreen.background(0);
  alertScreen.fill(255);
  alertScreen.textSize(20);
  alertScreen.textAlign(CENTER);
  
  if(message == "Object Detected" )
  {
    alertScreen.background(240, 0, 0);
    alertScreen.textSize(30);
  }
  
  if(message == "No Object" && buggyState != 2)
    buggyState = 0;
  
  if(message == "Object Detected" && buggyState != 2)
    buggyState = 1;
    
  if(message != null)
    alertScreen.text(message, 200, 100);
    

  if (robstacleDist > -1 && obstacleDist < 30) {
    alertScreen.textSize(20);
    alertScreen.text("Obstacle Distance: " + robstacleDist + " cm", 200, 140);
  }

  alertScreen.endDraw();
  image(alertScreen, 100, 200);
  
  if(buggyState == 0)
    copyPattern(arrowPattern);
  else if(buggyState == 1)
    copyPattern(warningPattern);
  else
    copyPattern(stopPattern);
}

void drawWheelDistances() {
  float Distance = (leftWheel + rightWheel) / 2;
  double rDistance = new BigDecimal(Distance).setScale(2, RoundingMode.HALF_UP).doubleValue();
  fill(0);
  textSize(20);
  textAlign(CENTER);
  text("Total Distance Travelled " + rDistance +  " m" , width / 2, 40);
  text("Buggy Alerts", width / 2, 185);
}

void copyPattern(int[][] pattern) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      matrix[i][j] = pattern[i][j];
    }
  }
}

void controlEvent(ControlEvent theEvent) {
  if (theEvent.isController()) {
    String buttonName = theEvent.getController().getName();
    if (buttonName.equals("reset distance")) {  
      resetDistance();  // Call function when button is clicked
    }
  }
}

void resetDistance() {
  if (clientConnected) {
    sendCommandPacket(2, 0); // Assuming 2 is the reset command type
    println("Reset distance command sent.");
  } else {
    println("Cannot reset, not connected.");
  }
}
