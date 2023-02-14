String name = "Arduino";
String msg;

void setup() {
 	Serial.begin(9600);
}

void readSerialPort() {
 	msg = "";
 	if (Serial.available()) {
 			delay(10);
 			while (Serial.available() > 0) {
 					msg += (char)Serial.read();
 			}
 			Serial.flush();
 	}
}
void sendData() {
 	//write data
 	Serial.print(name);
  if (msg == "go") {
     	Serial.print(" received : ");
  }
}

void loop() {
 	readSerialPort();
 	if (msg != "") {
 			sendData();
 	}
 	delay(500);
}
