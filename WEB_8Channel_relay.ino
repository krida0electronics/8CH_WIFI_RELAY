
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>

#define ADDR 0x27          // i2c expander slave address. not changeable!
#define NETWORK_LED 2      // led "NETWORK" on pcb


MDNSResponder mdns;

// Wi-Fi
const char* ssid = "*****";              // replace here to your wifi ssid
const char* password = "*****";          // replace here to your wifi password

byte arduino_mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192,168,0,35);                                    // setup here ip address for wifi relay module
IPAddress gateway(192,168,0,1);                                // check your WIFI router settings. it can be 192.168.0.1 or 192.168.1.1
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

unsigned char relay_1_state = 0;
unsigned char relay_2_state = 0;
unsigned char relay_3_state = 0;
unsigned char relay_4_state = 0;
unsigned char relay_5_state = 0;
unsigned char relay_6_state = 0;
unsigned char relay_7_state = 0;
unsigned char relay_8_state = 0;

unsigned char NewEvent;
unsigned char i2c_register;


void setup(void){

  pinMode(NETWORK_LED, OUTPUT);  
  digitalWrite(NETWORK_LED, LOW);
  
  Wire.begin(); 
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  
 
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
   Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/",      []() { server.send(200, "text/html", webPage()); });
  
  server.on("/1_on",  []() { relay_1_state = 1;
                             server.send(200, "text/html", webPage()); });
  
  server.on("/1_off", []() { relay_1_state = 0;
                             server.send(200, "text/html", webPage()); });

  server.on("/2_on",  []() { relay_2_state = 1;
                             server.send(200, "text/html", webPage()); });
  
  server.on("/2_off", []() { relay_2_state = 0;
                             server.send(200, "text/html", webPage()); });

  server.on("/3_on",  []() { relay_3_state = 1;
                             server.send(200, "text/html", webPage()); });
  
  server.on("/3_off", []() { relay_3_state = 0;
                             server.send(200, "text/html", webPage()); });

  server.on("/4_on",  []() { relay_4_state = 1;
                             server.send(200, "text/html", webPage()); });
  
  server.on("/4_off", []() { relay_4_state = 0;
                             server.send(200, "text/html", webPage()); });

  server.on("/5_on",  []() { relay_5_state = 1;
                             server.send(200, "text/html", webPage()); });
  
  server.on("/5_off", []() { relay_5_state = 0;
                             server.send(200, "text/html", webPage()); });

  server.on("/6_on",  []() { relay_6_state = 1;
                             server.send(200, "text/html", webPage()); });
  
  server.on("/6_off", []() { relay_6_state = 0;
                             server.send(200, "text/html", webPage()); });

  server.on("/7_on",  []() { relay_7_state = 1;
                             server.send(200, "text/html", webPage()); });
  
  server.on("/7_off", []() { relay_7_state = 0;
                             server.send(200, "text/html", webPage()); });

  server.on("/8_on",  []() { relay_8_state = 1;
                             server.send(200, "text/html", webPage()); });
  
  server.on("/8_off", []() { relay_8_state = 0;
                             server.send(200, "text/html", webPage()); });

  server.on("/all_on", []() { relay_1_state = 1;
                              relay_2_state = 1;
                              relay_3_state = 1;
                              relay_4_state = 1;
                              relay_5_state = 1;
                              relay_6_state = 1;
                              relay_7_state = 1;
                              relay_8_state = 1;
                              server.send(200, "text/html", webPage()); });        

  server.on("/all_off", []() { relay_1_state = 0;
                               relay_2_state = 0;
                               relay_3_state = 0;
                               relay_4_state = 0;
                               relay_5_state = 0;
                               relay_6_state = 0;
                               relay_7_state = 0;
                               relay_8_state = 0;
                               server.send(200, "text/html", webPage()); });                                                        

                             


  
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();

  if (WL_CONNECTED) { digitalWrite(NETWORK_LED, HIGH); }          // turn ON "NETWORK" led on pcb, when connection is ok
   else digitalWrite(NETWORK_LED, LOW);

  if(NewEvent){                                                   // if received new data, format it and send to i2c register

  i2c_register &= 0b01111111; i2c_register |= relay_1_state<<7;
  i2c_register &= 0b10111111; i2c_register |= relay_2_state<<6;
  i2c_register &= 0b11011111; i2c_register |= relay_3_state<<5;
  i2c_register &= 0b11101111; i2c_register |= relay_4_state<<4;
  i2c_register &= 0b11110111; i2c_register |= relay_5_state<<3;
  i2c_register &= 0b11111011; i2c_register |= relay_6_state<<2;
  i2c_register &= 0b11111101; i2c_register |= relay_7_state<<1;
  i2c_register &= 0b11111110; i2c_register |= relay_8_state;
    
  Wire.beginTransmission(ADDR);
  Wire.write(~(i2c_register));
  Wire.endTransmission(); 

  NewEvent = 0;
     
  }
  
} 

String webPage()
{
  String web; 
  
  web += "<!DOCTYPE html><html>";
  web += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title>WIFI RELAY</title>";
  web += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  
  web += ".button  {background-color: #00c5fc; border: none; color: black; text-align: center; text-decoration: none; display: inline-block; font-size: 20px; margin: 1px 1px; cursor: pointer; height: 40px; width: 250px; border-radius: 5px;}";
  web += ".button2 {background-color: #91e7ff; border: none; color: grey; text-align: center; text-decoration: none; display: inline-block; font-size: 20px; margin: 1px 1px; cursor: pointer; height: 40px; width: 250px; border-radius: 5px;}";
  web += ".button3 {background-color: none; border: 1px solid; color: black; text-align: center; text-decoration: none; display: inline-block; font-size: 12px; margin: 1px 1px; cursor: pointer; height: 40px; width: 82px; border-radius: 5px; background-image: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFIAAAAoCAIAAADi9zprAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAJ3SURBVGhD7ZjLaiJBFIaTjmYzGxXH7NX4LragqBibbLwQ8hS+gohRdCmCCToNKr5NDGlQwet21jo/XQcZSCajnToidH8rz6kqqM+qPl3VFztb4mjbCUfbTjjadsLR5mQymVQqFVVVg8HgD5Pb29toNFqr1abTKXU6IezaEM7lcm63++IfoOnh4eHE8rzauq57PB7hdn9///Ly8v7+/tvk7e0NoaZp4h9Bt9FoRMP4YdSez+eKokAplUphzSn7AcMw0uk0ul1eXjabTcoyw6i93W5LpVK73ab4S/DkQ/vq6mowGFCKkxOVtEMol8tYc5/Pt16vKcXGGWmDeDwO80KhQDEb8rV/mVBwJCgBqHDX19fcCy5ZG9P9aWJ53qK84VGnmAfJ2plMBpMG+EGpI8FbDcNR/CnmQaZ2r9cTzgJrW308HmNsOBymmAdp2pvNJhAICGGB3+9fLpfUfAD7nfI3d3d31CwVadqfTjqRSFDzAeCPu7m5oZEm+ONWqxU1S0Xysw1CoRBmjL1K8TEMh0MhLLD8Rvgv8rWTySRm3O12KT6S7xfFQ5CvLQ5blif9/VfgIcjXxtMojhyz2YxSR2Ked7i2t0C+Nsjn81hw7HaKzw8WbSy41+uFOfdhyzIs2qDf7+MWibtktVql1Je0Wi3cUnFXpZgZLm3QaDSgjTXHMdswDMp+AE14vaOboiiLxYKyzDBqg9FoJHY7ipymac/Pz6+vr/uPSp1OJ5vNulwudMA1W9d1GsYPrzZAPX98fBQfzD4FNb9YLFou+9Zg1xbAql6vq6oaiUT2H4xjsdjT09OJhQUn0j43HG074WjbCUfbTthSe7f7A3Fxlk+YQ0PMAAAAAElFTkSuQmCC') }";
  web += ".button4 {background-color: none; border: 1px solid; color: black; text-align: center; text-decoration: none; display: inline-block; font-size: 12px; margin: 1px 1px; cursor: pointer; height: 40px; width: 82px; border-radius: 5px; background-image: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFIAAAAoCAYAAABtla08AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsQAAA7EAZUrDhsAAAAGYktHRAD/AP8A/6C9p5MAAARgSURBVGhD7ZpJLC1BFIbPfYRrjCnmhSEirKzNUyISCxGxNGyIrRBrG0IIlvYkLO3NRIK9eVqYV2Imol799aq9bu+6HvpV3bz0l5xU96nuqvhV1ak6fV2MQw7f5ocsHb6JI6RNOELahCOkTThC2oTPRe37+3uampqihYUFOjk5oaOjI+FPTk6mxMREKioqovLycnK73cLvM0BIX2B/f5/V19ezkJAQ/GO9Gp5paGgQ7/gK2oV8eHhgbW1tLCAgwKNo3iwwMJC1t7ezx8dH2Zo+tAp5dnbGcnNzPYpkWFhYGAsNDfVYZ1h+fj47Pz+XrepBm5AQMTU19Q9RMjIyWH9/P6uoqBD3w8PDwnBdWVnJBgYGxDPmd2BpaWlaxdQStflUpJqaGjo4OJAeIr7u0dDQEK2vr1NOTo4INABBxQgsx8fHog7PDA4OUnBwsPADvl5SbW0tPT09SY9ipKBKwZqIrg2Li4tja2trbGNjg+Xl5VnqRkZGhJl9mMp4dmVlhcXGxlrqOjo6ZC9qUS7k3t6eJbDwUSVEnJmZYeHh4RZRYJ6EhEVERLC5uTm2urrKgoKCXv0IQHyky97UoXxqd3V1WaZfT0+PmNbV1dV0dXUlvR9zeXkp3uHiU3d3t/T+WjbQh3KkoEq4u7uz7BMzMzMZF9Vr5H5vRBpWUFAg2jAHIER5vrGXvapB6YjEieX29lbeEbW2ttLs7CwtLy9Lz+dZXFwUpyC0ZXBzc0PT09PyTg1KhcQfbKaqqorGxsbk3dcZHR0VbZmZn5+XV2pQKiS2LwZRUVGUnp7+rdFogDb41KbIyEjpsfalAqVCGntDEB8fL0o7/mAjscG3UaIE5r5UoFRIl8slr4ien59F6e/vL8r3mJiYoPHxcXnnGT8/P1G+vLyIUgdKhUxISJBXRKenp6JEaswbfH8pApI3kGID5lH4Ubt2o1TIpKQkeUV0fX1NOzs7VFhYKD1fB21sbW2JaG1giKsKpUIiKWtmcnKS6urq5N3XQRtoy8zbvv45cj+pBOQekRZDt7CUlBThKy0tffV91oqLi0U+Etkfw6djQ678rI3MtlkIpMW2t7cZ3w5Z/H9j0dHRbHd3l/X19Vn8TU1Nsjd1KBcSnweQWDD+aFwvLS0xvln/lJgQkZ9qxHvmJIjb7WaHh4eyN3UoFxLg84BZlJiYGCEmDz6spKTEUufJsBRgJEJIvGuu6+zslL2oRYuQWNOQUzQLgJGJzDjq+JmcNTc3s6ysLLHewbKzs1lLSwvjZ2ixrmI6v/3Ow6O3SGDoQIuQAJ8FzAHCMHx+6O3tZXw7I5/8zebmpqhDkHr7Hj9usouLC/mkerQJCSDm25FpNiR9kWqD4drTMzCMRJ0iAq1CAkxlfB4wB6C/NQQWrIloQzfahTRApG1sbPzw0ysMe1FscXRE5/fwuZ+s8EAikrLIJ3r6yQrfgFNZWRnxESz8voLzi12bUHrW/p9xhLQJR0ibcIS0CUdIWyD6CeKM6d23c955AAAAAElFTkSuQmCC') }";
  web += ".button5 {background-color: none; border: 1px solid; color: black; text-align: center; text-decoration: none; display: inline-block; font-size: 12px; margin: 1px 1px; cursor: pointer; height: 40px; width: 82px; border-radius: 5px; background-image: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFIAAAAoCAYAAABtla08AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsQAAA7EAZUrDhsAAAAGYktHRAD/AP8A/6C9p5MAAATNSURBVGhD7ZpHS2xLEMfriQlU0KUJ45g+gDnniKDuDLhyKeatfgvxKwi6UHFjVnStGzFgzooBxTAzSt3+F3Pum+dTL/fO0Ecu84NmuvrM9MG/Vd3Vdc4/rCAPLuPl+PTgIh4h3YRHSDfhEdJNeIR0E99u135+fqbp6WlaWFigs7MzOj4+lvGIiAgKCwujvLw8Ki4uJn9/fxn/LnwbIXd3d2lgYIBGR0fJz8+PioqKKDIyUgQEEPTw8JBmZmbIZrNRQ0MD9ff3U0xMjFw3HQhpJi8vL9zZ2cm+vr5cU1PDs7OzbLfbHVf/D64pMbmqqkp+093dLXOYjalCnp+fc2ZmJicmJvLi4qJj9L/c399L+4j5+Xm2WCyclZXFFxcXjlFzME1IiKjCksvKyvj29tYxyry1tSVeNjk5KXZ7e7s0MDY2Jte2t7fFBjc3N1xSUsKxsbGmimnKrm21Wqmuro4SEhJoYmKCgoOD6enpiVSIU0pKCq2trVF4eLh8V4WtNIA1c3V1Vb6jBJWNKSQkhJToFBcXJ3Ni/TQFh6BawZqIcDY8EZ6UmprKycnJvLy8LGMGbW1t0pxZWlripKQkTktL48vLSxmDZ6p/DPf09IitG+1C7uzsyCah0huxlSeKiPn5+Xx3dydjznwkJMA/QaVCnJ6eLnOAubk5Vjs+qwxAbJ1oF7K5uZmrq6sdFnNHR4d44kcigs+EBBATno1106CyspJbWloclj60CgnPCQgIkBQHbG5uso+PD6+srIj9EV8JCRDmmMPYgFQyz4GBgazWT7F1oXWzwYkFyXZOTo7Yg4ODVFBQQBkZGWL/CdnZ2ZSbmytzAfSVsJK460SrkCpXpMLCQvL29hZ7fHycGhsbpe8KTU1NMheAiPjn4IipE61CnpycUFRUlPSvr69JbTykEnKxXQFzqNAmtWaKHR0dLffSiXYhUXgAKuWRTyNfdAXjPG7MiXucnp5KXxdahfTy+vd2Rni/vr7K52fU19dL+4q3tzf5dJ5fN1rvHBoa+tNT0Ae/8pzS0lJpX2GEseHtmNPo60KrkAjjg4MD6QcFBZHFYiGVvojtCtjEVD5JKu0Re39//2e460KrkCjKIi2x2+1i19bW0vDwsPRdAXNgLoC5VZ4q99KKI5/UApJkJMuoJ4L3x8U/Ack9joV7e3tiT01NmZKQaz8i4viGoqxBV1eXFBtQdPhdVArF8fHx3Nvb6xhhrqio4NbWVoelD+1CoqAAD0JRFqC6jcKsOpH8lpgQUZ2QpFmtVhmDpzt7p060CwlQZHD2wqurKxET1W5UcH4FwhmeqI6H8ltgeGdfX5/YujFFSMMLUdk2ns9gDAJjzSwqKuKhoSFeX1/nh4cHaehjDNfgdQhnwxNtNpuMO3unbkwREqCYi8cDENM5pBH68Cp4LPZC54aSGa45hy48ESKa/ajBNCEB/nB4JkT7KKQfHx95Y2NDGvrvwZqIcIYnmikiMFVIgJDG4wGEK4qyqCciVD8D15DilJeXy2/goWaFszPf5gUBFa7ygsDIyAipdVJKYagU4YEXODo6klMRkm2cz/GCAL5vVJPM5tu9soInhjj9oJ6IM/P7V1by8/Olpul5ZeUvxby601+GR0g34RHSTXiEdBMeId0C0Q+F6vHUm7tbIwAAAABJRU5ErkJggg==') }";

  web += "</style></head>";

  web += "<body><h2>WIFI 8 Channel Relay</h2>";
  
  if (relay_1_state == 0)
   { web += "<p><a href=\"/1_on\"><button class=\"button button2\"><b>OFF</button></a></p>"; }
  else 
   { web += "<p><a href=\"/1_off\"><button class=\"button\"><b>ON</button></a></p>"; }

  
  if (relay_2_state == 0)
   { web += "<p><a href=\"/2_on\"><button class=\"button button2\"><b>OFF</button></a></p>"; }
  else 
   { web += "<p><a href=\"/2_off\"><button class=\"button\"><b>ON</button></a></p>"; }

  
  if (relay_3_state == 0)
   { web += "<p><a href=\"/3_on\"><button class=\"button button2\"><b>OFF</button></a></p>"; }
  else 
   { web += "<p><a href=\"/3_off\"><button class=\"button\"><b>ON</button></a></p>"; }

  
  if (relay_4_state == 0)
   { web += "<p><a href=\"/4_on\"><button class=\"button button2\"><b>OFF</button></a></p>"; }
  else 
   { web += "<p><a href=\"/4_off\"><button class=\"button\"><b>ON</button></a></p>"; }

  
  if (relay_5_state == 0)
   { web += "<p><a href=\"/5_on\"><button class=\"button button2\"><b>OFF</button></a></p>"; }
  else 
   { web += "<p><a href=\"/5_off\"><button class=\"button\"><b>ON</button></a></p>"; }

  
  if (relay_6_state == 0)
   { web += "<p><a href=\"/6_on\"><button class=\"button button2\"><b>OFF</button></a></p>"; }
  else 
   { web += "<p><a href=\"/6_off\"><button class=\"button\"><b>ON</button></a></p>"; }

  
  if (relay_7_state == 0)
   { web += "<p><a href=\"/7_on\"><button class=\"button button2\"><b>OFF</button></a></p>"; }
  else 
   { web += "<p><a href=\"/7_off\"><button class=\"button\"><b>ON</button></a></p>"; }

  
  if (relay_8_state == 0)
   { web += "<p><a href=\"/8_on\"><button class=\"button button2\"><b>OFF</button></a></p>"; }
  else 
   { web += "<p><a href=\"/8_off\"><button class=\"button\"><b>ON</button></a></p>"; }


  web += "<p></a><a\ href=\"/all_on\"><button class=\"button button4\"></button></a><a\ href=\"/\"><button class=\"button button3\"></button><a\ href=\"/all_off\"><button class=\"button button5\"></button></a></p>"; 
   
  web += "</body></html>";

  NewEvent = 1; // flag for I2C when new data is received
  
  return(web);
}
