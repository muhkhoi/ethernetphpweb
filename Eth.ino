#include <SPI.h>
#include <Ethernet.h>
byte mac[] = { 0X52, 0X64, 0X75, 0X69, 0X6E, 0X6F };
IPAddress ip( 192,168,2,199 );
EthernetServer server(80); // Port 80 is HTTP port
char new_state[1024];
int state = 0;
int buttonState = 0;

void setup() {
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode (4, INPUT);
 
  Ethernet.begin(mac, ip);
digitalWrite(3, HIGH);
digitalWrite(2, HIGH);
  
 server.begin();
}
void loop() {
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      int i = 0;
      int head = 1;
      int body = 0;

      while(client.available()) {
        char c = client.read();
        if (c == '\n') {

          if ( i <= 2 ) {
            // an http request ends with a blank line

           sendPage(client,state);
            if ( head == 1 ) {
              body = 1;
              head = 0;
            }
          }
          i = -1;
        }
        if ( body == 1 ) {
          new_state[i] = c;
        }
        i++;
        new_state[i] = '\0';
      }
      i = 0;
    }
  
   if(strncmp(new_state+6, "hidup", 5) == 0){             //new_state itu data dari POST, contoh --> pinD2=Off
    state = 0;                                          // state 2 itu blink, 1 itu on, 0 itu off
     } 
     
   if(strncmp(new_state+6, "mati", 4) == 0){
    state = 1;  
     } 
     
  if(strncmp(new_state+6, "stop", 4) == 0){
    state = 2;  
     }   
      
      Serial.println(new_state); 

    if(state == 0){
      digitalWrite(3, LOW);
      digitalWrite(2, HIGH);  
      } 
      else if(state == 1){
          digitalWrite(3, HIGH);
         digitalWrite(2, LOW);
      } 
      else if(state == 2){
          digitalWrite(3, HIGH);
         digitalWrite(2, HIGH);
      }  }

 

void sendPage(EthernetClient client, int statevar)
{
  // send a standard http response header
  client.println("HTTP/1.0 200 OK\Content-Type: text/html\n\n<html>\n<head>");
  client.println("<link rel='icon' href='data:;base64,iVBORw0KGgo='>");
  client.println("<title>POST Pin controller</title>\n</head>\n<body>\n");     
  client.println("<form method='post' action='/' name='pins'>\n");

  char line[1024];
  int pin;
  sprintf(line, "<input name='pinD2' type='submit' value='OffOn' />\n", pin);
  client.print(line);
  sprintf(line, "<input name='pinD2' type='submit' value='Onoff' />\n", pin, pin);
  client.print(line);
 
  client.println("</form>\n</body>\n</html>");
  client.stop();
}
