#include <Wire.h>
#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT11   // DHT 11 
char temp[6]; //2 int, 2 dec, 1 point, and \0
char hum[6];  
char msg[13];
DHT dht(DHTPIN, DHTTYPE);

void setup() {
 Wire.begin(8);                /* join i2c bus with address 8 */
  dht.begin();
 Wire.onReceive(receiveEvent); /* register receive event */
 Wire.onRequest(requestEvent); /* register request event */
 Serial.begin(9600);           /* start serial for debug */

}

void loop() {
  //get the temp/humid into chars to format
  ftoa(temp,readTemp());
  ftoa(hum,readHumidity());
  
  //Compile a comma delimited string to send to the log
  sprintf(msg,"%s,%s",temp, hum);
  Serial.println(msg);
  delay(3000);
}

// function that executes whenever data is received from master
void receiveEvent(int howMany) {
 while (0 <Wire.available()) {
    char c = Wire.read();      /* receive byte as a character */
    Serial.print(c);           /* print the character */
  }
 Serial.println();             /* to newline */
}

// function that executes whenever data is requested from master
void requestEvent() {
  Wire.write(msg);
/*send string on request */
}

float readTemp(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float t = dht.readTemperature();
  return   t;
}

float readHumidity(){
  float h = dht.readHumidity();  
  return h;
}



int ftoa(char *a, float f)  //translates floating point readings into strings
{
  int left=int(f);
  float decimal = f-left;
  int right = decimal *100; //2 decimal points
  if (right > 10) {  //if the decimal has two places already. Otherwise
    sprintf(a, "%d.%d",left,right);
  } else { 
    sprintf(a, "%d.0%d",left,right); //pad with a leading 0
  }
  
}
