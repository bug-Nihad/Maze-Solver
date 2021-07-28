void setup(){
    Serial.begin(9600);
    pinMode(2, OUTPUT);
    pinMode(3, INPUT);
    pinMode(4, INPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, INPUT);
    pinMode(13, OUTPUT); 
}
void Turn(int x, int y, int z, int time){
    digitalWrite(x, HIGH);
    digitalWrite(y, LOW);
    analogWrite(z, 100);
    delay(time);
    digitalWrite(x, LOW);
}
void leftTurn(){
    Turn(7, 8, 6, 5000);
}
// void rightTurn(){
//     Turn(9, 10, 11, 5000);
// }
void instantRightTurn(int time){
    //Serial.println("Right");
    digitalWrite(9, LOW);
    digitalWrite(10, HIGH);
    analogWrite(11, 70);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    analogWrite(6, 100);
    delay(time);
    digitalWrite(10, LOW);
    digitalWrite(7, LOW);
}

void u_turn(int time)
{
    digitalWrite(9, LOW);
    digitalWrite(10, HIGH);
    analogWrite(11, 100);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    analogWrite(6, 80);
    delay(time);
    digitalWrite(10, LOW);
    digitalWrite(7, LOW);
}

void instantLeftTurn(int time){
    digitalWrite(9, HIGH);  //Right motor
    digitalWrite(10, LOW);
    analogWrite(11, 70 + 10);
    digitalWrite(7, LOW);   //Left motor
    digitalWrite(8, HIGH);
    analogWrite(6, 40);
    delay(time);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
}

void goForward(int time){
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    analogWrite(11, 62);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    analogWrite(6, 70);
    delay(time);
    digitalWrite(9, LOW);
    digitalWrite(7, LOW);
}
float Sonar(int trigPin, int echoPin){
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  int duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  float distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  return distance;
}
float rightSonar(){
    float x = Sonar(5, 4);
    return x;
}
float leftSonar(){
    float x = Sonar(2, 3);
    return x;
}
float frontSonar(){
    float x = Sonar(13, 12);
    return x;
}
void parallel(float right, float left, int time){
    float x = right - left;
    if (abs(x) < 1*40)  //For sonar misreading... 
    {
        int rightPwm = 150 - int(5 * x);
        if (rightPwm > 255)
        {
            rightPwm =255 - 25;
        }
        else if (rightPwm < 0)
        {
            rightPwm = 0 + 30;
        }
                
        int leftPwm = 1.22 * (150 + int(5 * x));
        if (leftPwm > 255)
        {
            leftPwm = 255 - 25;
        }
        else if (leftPwm < 0)
        {
            leftPwm = 0 + 30;
        }
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        analogWrite(11, rightPwm);
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        analogWrite(6, leftPwm);
        
        delay(time);
        digitalWrite(9, LOW);
        digitalWrite(7, LOW);
    }
}
void fullbreak()
{
    digitalWrite(9, LOW);
    digitalWrite(10, HIGH);
    analogWrite(11, 180);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    analogWrite(6, 180);
    delay(3);
    digitalWrite(10, LOW);
    analogWrite(11, 0);
    digitalWrite(8, LOW);
    analogWrite(6, 0);
}

void loop()
{
    //----------1-------------
    int sideblock = 50 - 15;
    int frontsideblock = 15 + 0;  //Dont Change it
    
    // If left is Open
    if (leftSonar() > sideblock)
    {
        //--------------
        delay(400);  //Take another reading to be sure
        if (leftSonar() > sideblock) 
        {                 
            goForward(400);
            instantLeftTurn(310);
            // delay(20);
            goForward(150);
            delay(100);
        }
    }
    //If left is sideblocked, Check for front
    else if (frontSonar() > frontsideblock)
    {
        if ((leftSonar() < sideblock)  &&  (rightSonar() < sideblock -10))
        {
            parallel(rightSonar(), leftSonar(), 15);
        }
        else
        {
            goForward(200);
        }
    }
    //If left and front are sideblocked, check for right
    else if ( rightSonar() > sideblock - 10 )
    {
        //-----------------
        delay(300);  //Take another reading to be sure
        if (rightSonar() > sideblock-10)
        {
            goForward(100 + 200);
            instantRightTurn(400);
            // delay(20);
            goForward(300);
            delay(200);
        }
    }
    // Else rotate 360 degree
    else
    {
        // --------------
        delay(300);
        if ((leftSonar() < sideblock)  &&  (rightSonar() < sideblock -10) && (frontSonar() < frontsideblock))
        {
            u_turn(700 + 50 + 30);
            delay(400 - 100);
        }
    }
    delay(20 + 20 - 20 - 5);   
}
