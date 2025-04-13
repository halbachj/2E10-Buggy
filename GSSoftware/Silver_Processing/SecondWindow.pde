class SecondWindow extends PApplet {
  
  long counter = 0;
  
  public void exit() {
    remoteWindowOpen = false;
    dispose(); // Properly close the window
  }
  
  public void settings() {
    size(1000, 500);
  }

  void draw() {
    //text("Remote Control", width/2, 20);
    background(220);

    float centerX = width / 2;
    float centerY = height / 2;
  
    float dx = mouseX - centerX;
    float dy = mouseY - centerY;
    float distance = dist(mouseX, mouseY, centerX, centerY);

    if (distance > deadZoneRadius) {
      xOutput = constrain(dx / (width / 2), -1, 1);
      yOutput = constrain(dy / (height / 2), -1, 1);
    } else {
      xOutput = 0;
      yOutput = 0;
    }

    // Draw dead zone
    fill(150);
    ellipse(centerX, centerY, deadZoneRadius * 2, deadZoneRadius * 2);

    // Draw control surface position
    fill(255, 0, 0);
    ellipse(mouseX, mouseY, 10, 10);

    // Display x and y values
    fill(0);
    textAlign(LEFT);
    text("X: " + nf(xOutput, 1, 2), 10, height - 30);
    text("Y: " + nf(yOutput, 1, 2), 10, height - 10);
    if (counter % 5 ==0 ) {
      sendCommandPacket(xOutput, yOutput);
    }
    counter++;
  }

  private void sendCommandPacket(float x, float y) {
    ByteBuffer bb = ByteBuffer.allocate(72);
    bb.order(ByteOrder.LITTLE_ENDIAN);
    bb.putInt(4); // PacketType COMMAND
    bb.putFloat(x);
    bb.putFloat(y);
    client.write(bb.array());
  }
  
}
