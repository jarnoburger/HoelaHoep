# 1 "k:\\AERenderFolder\\2020-07-02 Water hoela hoop\\ArduinoMQTT\\WiFiSimpleReceive\\WiFiSimpleReceive.ino"
# 2 "k:\\AERenderFolder\\2020-07-02 Water hoela hoop\\ArduinoMQTT\\WiFiSimpleReceive\\WiFiSimpleReceive.ino" 2
# 3 "k:\\AERenderFolder\\2020-07-02 Water hoela hoop\\ArduinoMQTT\\WiFiSimpleReceive\\WiFiSimpleReceive.ino" 2
# 4 "k:\\AERenderFolder\\2020-07-02 Water hoela hoop\\ArduinoMQTT\\WiFiSimpleReceive\\WiFiSimpleReceive.ino" 2

/*

  ArduinoMqttClient - WiFi Simple Receive



  This example connects to a MQTT broker and subscribes to a single topic.

  When a message is received it prints the message to the serial monitor.



  The circuit:

  - Arduino MKR 1000, MKR 1010 or Uno WiFi Rev.2 board



  This example code is in the public domain.

*/
# 16 "k:\\AERenderFolder\\2020-07-02 Water hoela hoop\\ArduinoMQTT\\WiFiSimpleReceive\\WiFiSimpleReceive.ino"
Led _led;
Taken _taken;
Network _network;

unsigned long currentMillis;
unsigned long previousMillis;
unsigned long interval = 1000;
int count;

void setup() {
  count = 0;

    //Initialize serial and wait for port to open:
  SerialUSB.begin(115200);
  //  while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}
  SerialUSB.println("*** Goededag meneertje !");
  SerialUSB.print("Led built in pin : ");
  SerialUSB.println((13u));

  _led.Start();

  SerialUSB.println("*** Starting network");
  _taken.Start(_led);
  _network.Start(_taken);
}

void loop() {
  _led.Loop();
  _network.Loop();
}
