#include <SoftwareSerial.h>

SoftwareSerial s(2, 5); // TX Arduino line is pin 2, RX Arduino line is pin 5.

// Initialize 3 boolean flags for storing the state of the system.
bool led1 = false;
bool led2 = false;
bool led3 = false;


/*
  Function used to send data to ESP8266.
  Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
  Returns: The response from the esp8266 (if there is a reponse)
*/


String sendData(String command, const int timeout, bool debug = false)
{
  String response = "";

  s.print(command);   // send the read character to the esp8266

  long int time = millis();

  while((time + timeout) > millis())
  {
    while (s.available())
    {
      // The esp has data so display its output to the serial window
      char c = s.read();   // read the next character.
      response += c;
    }
  }

  if(debug) Serial.print(response);    // print response in serial monitor in case debug==true.

  return response;
}



void setup()
{

  Serial.begin(115200);
  s.begin(115200);    // adjusting to the esp's baud rate

  // Initiate the LED pins.
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);

  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  // Send AT commands to the esp to configure it for the tasks.
  sendData("AT+RST\r\n", 2000); // reset module
  sendData("AT+CWMODE=2\r\n", 1000); // configure as access point
  sendData("AT+CIFSR\r\n", 1000, true); // get ip address
  sendData("AT+CIPMUX=1\r\n", 1000); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n", 1000); // turn on server on port 80

  // Inform about the new client
  Serial.println("\nWe have a new client!!");

  // Display initial state of the system.
  Serial.println("\nCurrent State of the System is :");
  Serial.println("LED1 : OFF");
  Serial.println("LED2 : OFF");
  Serial.println("LED3 : OFF");
}


void loop()
{
  String inString = "";
  if (s.available())              // check if the esp is sending a message
  {
    if (s.find("+IPD,"))
    {
      delay(10);              // wait for the serial buffer to fill up (read all the serial data)

      int pinNumber = 0;
      String tempS = "";

      s.find("pin=");                 // advance cursor to "pin="
      char temp = s.read();               // Read the output given by esp
      tempS = temp;

      pinNumber = tempS.toInt();         // convert String to int.
      
      digitalWrite(pinNumber, !digitalRead(pinNumber));   // toggle pin

      Serial.println("");
      if(pinNumber==11)
      {
        led1 = !led1;
        if(led1) Serial.println("LED1 was switched ON.");
        else Serial.println("LED1 was switched OFF.");
      }
      
      if(pinNumber==12)
      {
        led2 = !led2;
        if(led2) Serial.println("LED2 was switched ON.");
        else Serial.println("LED2 was switched OFF.");
      }

      if(pinNumber==13)
      {
        led3 = !led3;
        if(led3) Serial.println("LED3 was switched ON.");
        else Serial.println("LED3 was switched OFF.");
      }

      // display current state of the system
      
      Serial.println("\nCurrent State of the System is :");
      Serial.print("LED1 : "); Serial.println((led1==false)?"OFF":"ON");
      Serial.print("LED2 : "); Serial.println((led2==false)?"OFF":"ON");
      Serial.print("LED3 : "); Serial.println((led3==false)?"OFF":"ON");

    }
  }

}
