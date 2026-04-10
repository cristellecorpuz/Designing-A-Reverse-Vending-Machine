
void open_door()
{
  for (int x = 0 ; x < 50; x++) {
    srvDoor.write(10); srvDoor2.write(170); delay(20);
  }
}
void close_door()
{
  for (int x = 0 ; x < 20; x++) {
    srvDoor.write(90); srvDoor2.write(90); delay(20);
  }
}
void step_home()
{
  if (digitalRead(lmtHome) == 1) {
    digitalWrite(l_ena, 0); digitalWrite(l_dir, 1);
    while (1) {
      digitalWrite(l_step, 1); delayMicroseconds(lDelay);
      digitalWrite(l_step, 0); delayMicroseconds(lDelay);

      if (digitalRead(lmtHome) == 0) {
        delay(500);
        if (digitalRead(lmtHome) == 0) {
          delay(500);
          if (digitalRead(lmtHome) == 0) {
            delay(2000);  
            break;
          }
        }
      }
    }
    digitalWrite(l_ena, 1);
  }
}

void goto_bin(long pulse)
{
  //  pulse = pulse * 10;
  digitalWrite(l_ena, 0); digitalWrite(l_dir, 0);
  for (long x = 0; x < pulse; x++) {
    digitalWrite(l_step, 1); delayMicroseconds(lDelay);
    digitalWrite(l_step, 0); delayMicroseconds(lDelay);
  }
  delay(3000);
  digitalWrite(l_ena, 1);
}
