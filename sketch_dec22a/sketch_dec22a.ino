// Define variables
unsigned long previousMillis = 0;  // will store last time Hello was printed
const long interval = 5000;       // interval at which to print Hello (milliseconds)
int inputValue;
void setup() {
  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  do{
    if (!Serial.available()) {//waits for user input
      // Read a line of input until a newline character is received
      String input = Serial.readStringUntil('\n');
      // Convert the string to an integer
      inputValue = input.toInt();//if not an integer then it is 0
      // Print the received integer back to the Serial Monitor
      Serial.print("You entered: ");
      Serial.println(inputValue);
      
    }
  }while(inputValue <= 0);
  
  // Get the current time
  unsigned long currentMillis = millis();
  
  // Check if the specified interval has passed
  if (currentMillis - previousMillis >= interval) {
    Serial.println("current Millis:" + String(currentMillis));
    // Save the current time
    previousMillis = currentMillis;

    // Print "Hello" to the serial monitor
    Serial.println("Hello");
  }

  // Other loop code goes here
}
