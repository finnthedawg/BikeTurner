//Source: Arduino documentation.
//Source: https://www.youtube.com/watch?v=p7O1H-bzuhY
//Source: Documentation provided in class
//Source: http://forum.arduino.cc/index.php?topic=14479.0
//Source: http://etextilelounge.com/arduino-breadboard-blink-5-leds-in-sequence/

// There are 4 modes, click, double click, hold, long hold.

int VarButton = 2;        // Input pin of pulldown button
int LED1 = 9;          // digital output pin for LED 1
int LED2 = 10;          // digital output pin for LED 2
int LED3 = 11;          // digital output pin for LED 3
int LED4 = 12;          // digital output pin for LED 4
int LED5 = 13;          // digital output pin for LED 5

int DoubleClickGap = 300;   // Time the user has to double click
int HoldGap = 500;        //Time the user holds the button before it triggers hold
int debounce = 20;          // Debounce because button in the midst of a press may change states quickly

boolean CurrentButtonState = HIGH;   //The button's current state
boolean LastButtonState = HIGH;  // The button's last state
boolean WaitingForDoubleClick = false;  // whether we're waiting for a double click (down)
boolean DoubleClickWhenReleased = false;     // Should the next release mean a double click
boolean singleOK = true;    // whether it's OK to do a single click
long downTime = -1;         // time the button was pressed down
long upTime = -1;           // time the button was released
boolean ignoreUp = false;   // whether to ignore the button release because the click+hold was triggered
boolean waitForUp = false;        // when held, whether to wait for the up event
boolean holdEventPast = false;    // whether or not the hold event happened already

void setup() {

   pinMode(VarButton, INPUT); //Set button's pin to input
   pinMode(LED1, OUTPUT); // Set each of the LED pins to output.
   pinMode(LED2, OUTPUT);
   pinMode(LED3, OUTPUT);
   pinMode(LED4, OUTPUT);
   pinMode(LED5, OUTPUT);
   digitalWrite(LED1, HIGH); //Turn all the LEDs on
   digitalWrite(LED2, HIGH);
   digitalWrite(LED3, HIGH);
   digitalWrite(LED4, HIGH);
   digitalWrite(LED5, HIGH);
   Serial.begin(9600); //Open serial communications for debug
}

void loop() {
   //Call function checkButton() which returns an integer between 1-4
   //Which corresponds to the different button presses.
   int b = checkButton();
   //if b is one, it's a single click so flash right arrow LEDs
   if (b == 1){
    Serial.print("clicked");
        digitalWrite(LED1, HIGH); //Turn on LEDs to form arrow
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, HIGH);
        digitalWrite(LED5, LOW);
          for(int i =0; i<5; i++){ //Flash for 5 times
          delay(500);
          digitalWrite(LED1, LOW);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, LOW);
          delay(500);
          digitalWrite(LED1, HIGH);
          digitalWrite(LED3, HIGH);
          digitalWrite(LED4, HIGH);
          }
        digitalWrite(LED1, HIGH); //Turn all LEDs on so that the bike light returns to original state
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, HIGH);
        digitalWrite(LED5, HIGH);
    }
       //if b is two, it's a double click so flash left LEDs to form arrow.
   if (b == 2){
    Serial.print("double clicked");
        digitalWrite(LED1, HIGH);//Turn on LEDs to form arrow
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
          for(int i =0; i<5; i++){
          delay(500);
          digitalWrite(LED1, LOW);//Flash for 5 times
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, LOW);
          delay(500);
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, HIGH);
          }
        digitalWrite(LED1, HIGH);//Turn all LEDs on so that the bike light returns to original state
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, HIGH);
        digitalWrite(LED5, HIGH);
   }
   //if b is three, the user held so flash LEDs to form square.
   if (b == 3){
        Serial.print("hold");//Turn on LEDs to form arrow
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, HIGH);
        digitalWrite(LED5, LOW);
          for(int i =0; i<5; i++){//Flash for 5 times
          delay(700);
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, LOW);
          digitalWrite(LED5, HIGH);
          delay(700);
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, HIGH);
          digitalWrite(LED4, HIGH);
          digitalWrite(LED5, LOW);
          CurrentButtonState = digitalRead(VarButton); //Here check the current button state again
          if(CurrentButtonState == LOW){ //If the button is still held, it's a long hold
            Serial.print(CurrentButtonState);
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW); //Turn off the lights as the user turns off the bike light
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, LOW);
          digitalWrite(LED4, LOW);
          break;
          }
          }
          if(CurrentButtonState == HIGH){
          digitalWrite(LED1, HIGH); //Turn all LEDs on so that the bike light returns to original state
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, HIGH);
          digitalWrite(LED4, HIGH);
          digitalWrite(LED5, HIGH);
          }
   }
}


int checkButton() {     //Function check button to check button states
   int event = 0; //Define variable to record event
   CurrentButtonState = digitalRead(VarButton); //Read the current state of the button
   // Button pressed down
   if (CurrentButtonState == LOW && LastButtonState == HIGH && (millis() - upTime) > debounce)
   {
    //Record here the conditions when the button is pressed down
       downTime = millis(); //Time when this happened
       ignoreUp = false; //E.g Dont need to ignore the up press
       waitForUp = false; //E.d Isnt currently waiting for the up state
       singleOK = true;
       holdEventPast = false; //Make sure that the hold event has not already been activated
       if ((millis()-upTime) < DoubleClickGap && DoubleClickWhenReleased == false && WaitingForDoubleClick == true)  DoubleClickWhenReleased = true;
       else  DoubleClickWhenReleased = false; //Check if we are within the conditions for a double click and if we are, set double click when we release
       WaitingForDoubleClick = false; //If we are not, this single click begins the wait for the next click
   }
   // Button released
   else if (CurrentButtonState == HIGH && LastButtonState == LOW && (millis() - downTime) > debounce) //Condition for up state
   {
       if (not ignoreUp) //If we are not ignoring the up press
       {
           upTime = millis(); //Time when this happened
           if (DoubleClickWhenReleased == false) WaitingForDoubleClick = true; //If we are waiting for double click and it happened.
           else
           {
               event = 2; //Set event to 2, ready to return to the function
               DoubleClickWhenReleased = false; //Reset state counters back to 0
               WaitingForDoubleClick = false;
               singleOK = false;
           }
       }
   }
   // If the gap has already expired,It's a single click
   if ( CurrentButtonState == HIGH && (millis()-upTime) >= DoubleClickGap && WaitingForDoubleClick == true && DoubleClickWhenReleased == false && singleOK == true && event != 2)
   {
       event = 1; //Return single click
       WaitingForDoubleClick = false; //Not currently waiting for a double click
   }
   // If the time since down has been longer than the time we wait to consider the press a 'hold'
   if (CurrentButtonState == LOW && (millis() - downTime) >= HoldGap) {
       // Trigger the hold event
       if (not holdEventPast) //If hold event has not been activated
       {
           event = 3; //Set even to 3 to be ready to be returned
           waitForUp = true; //Reset all the variables for other clicks
           ignoreUp = true;
           DoubleClickWhenReleased = false;
           WaitingForDoubleClick = false;
           //downTime = millis();
           holdEventPast = true; //Hold event has just been activated
       }
   }
   LastButtonState = CurrentButtonState; //Set the value of the variable last button state to the state right now.
   return event; //Return the variable event to the function.
}
