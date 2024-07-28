  // Based on the Paho C code example from www.eclipse.org/paho/
  #include <iostream>
  #include <cstring>
  #include <chrono>
  #include <ctime>
  #include <sstream>
  #include <fstream>
  #include <string.h>
  #include <unistd.h>
  #include "MQTTClient.h"
  #include "adxl345.h"

  #define CPU_TEMP "/sys/class/thermal/thermal_zone0/temp"
  using namespace std;
  //Please replace the following address with the address of your server
  #define ADDRESS "tcp://192.168.0.21:1883"
  #define CLIENTID "rpi1"
  #define AUTHMETHOD "user1"
  #define AUTHTOKEN "user1"
  #define TOPIC "Sensor/Temp"
  #define LWT_TOPIC "Sensor/LastWill"
  #define LWT_MESSAGE "{\"d\":{\"status\": \"Sensor Disconnected\"}}
  #define QOS 1
  #define TIMEOUT 10000L
  #define SLEEP_DURATION 1
  
float getCPUTemperature() { // get the CPU temperature
  int cpuTemp; // store as an int
  fstream fs;
  fs.open(CPU_TEMP, fstream::in); // read from the file
  fs >> cpuTemp;
  fs.close();
  return (((float)cpuTemp)/1000);
}

int main(int argc, char* argv[]) {
  ADXL345 sensor;
  int x,y,z;
  char str_payload[256]; // Set your max message size here
  
  MQTTClient client;
  MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
  MQTTClient_message pubmsg = MQTTClient_message_initializer;
  MQTTClient_deliveryToken token;

  // Set the LWT message
  MQTTClient_willOptions will_opts = MQTTClient_willOptions_initializer;
  will_opts.topicName = LWT_TOPIC;
  will_opts.message = LWT_MESSAGE;
  will_opts.qos = QOS;
  will_opts.retained = 0;
  
  opts.keepAliveInterval = 20;
  opts.cleansession = 1;
  opts.username = AUTHMETHOD;
  opts.password = AUTHTOKEN;
  
  MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
  
  int rc;
  if ((rc = MQTTClient_connect(client, &opts)) != MQTTCLIENT_SUCCESS) {
  cout << "Failed to connect, return code " << rc << endl;
  return -1;
  }
  while(true){
    sensor.readXYZ(x,y,z);
    float cpuTemp = getCPUTemperature();
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);  
  
    sprintf(str_payload, "{\"d\":{\"X\":%d, \"Y\":%d,\'Z\":%d, \"CPUTemp\": %.2f, \"Timestamp\": \"%s\" }}", x,y,z,getCPUTemperature(), ctime(&now_c));
    pubmsg.payload = str_payload;
    pubmsg.payloadlen = strlen(str_payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
  
  MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
  cout << "Waiting for up to " << (int)(TIMEOUT/1000) <<
  " seconds for publication of " << str_payload <<
  " \non topic " << TOPIC << " for ClientID: " << CLIENTID << endl;
  rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
  cout << "Message with token " << (int)token << " delivered." << endl;
  sleep(SLEEP_DURATION);
  }
  
  MQTTClient_disconnect(client, 10000);
  MQTTClient_destroy(&client);
  return rc;
  }
