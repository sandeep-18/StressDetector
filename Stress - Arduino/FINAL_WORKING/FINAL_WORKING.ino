#include <assert.h>
#include <MySignals.h>
#include "Wire.h"
#include "SPI.h"

uint8_t state = 0;

#define MAX_ELEMS 30

class Node { 
public: 
    int data; 
    Node* next; 
}; 


Node* temp_head = NULL; 
Node* temp_end_elem = NULL;
Node* temp_curr = NULL;
Node* temp_tmp = NULL;
unsigned int temp_sum_value;
int temp_num_elems;
unsigned int temp_average;

Node* bpm_head = NULL; 
Node* bpm_end_elem = NULL;
Node* bpm_curr = NULL;
Node* bpm_tmp = NULL;
unsigned int bpm_sum_value;
int bpm_num_elems;
unsigned int bpm_average;


Node* O2_head = NULL; 
Node* O2_end_elem = NULL;
Node* O2_curr = NULL;
Node* O2_tmp = NULL;
unsigned int O2_sum_value;
int O2_num_elems;
unsigned int O2_average;


void setup() {
  Serial.begin(115200);
  MySignals.begin();

  float getTemperature(void);
  float getCalibratedTemperature(uint8_t samples, uint16_t delaySample, float offset, uint8_t format);

  temp_sum_value = 0;
  temp_num_elems = 0;
  temp_average = 0;

  bpm_sum_value = 0;
  bpm_num_elems = 0;
  bpm_average = 0;

  O2_sum_value = 0;
  O2_num_elems = 0;
  O2_average = 0;

  MySignals.initSensorUART();
  MySignals.enableSensorUART(PULSIOXIMETER_MICRO);
  establishConnection();
  
// Test calls
//test_temp_average();
//test_bpm_average();
//test_O2_average();

}
  
void loop() {


  //MySignals.initSensorUART();
  //MySignals.enableSensorUART(PULSIOXIMETER_MICRO);

  //int temp = MySignals.getTemperature(); //- PUT BACK
  int temp = random(34,40);
  Serial.print("temp = ");
  Serial.println(temp);
  delay(2000);
  average_result_temp(temp);


  int bpm_val;
  int O2_val;

  
  //state = MySignals.getPulsioximeterMicro(); - PUT BACK  NEED IN MAIN
  state = 1;
  if (state == 1) {
    //int bpm_val = MySignals.pulsioximeterData.BPM;// - PUT BACK
    int bpm_val = random(60,115);
    delay(2000);
    //int O2_val = MySignals.pulsioximeterData.O2; - PUT BACK
    int O2_val = random(92,99);
    delay(2000);
    
    Serial.print("bpm_val = ");
    Serial.println(bpm_val);
    Serial.print("O2_val = ");
    Serial.println(O2_val);
    average_result_bpm(bpm_val);
    average_result_O2(O2_val);
  }
  else{
    Serial.println("Not Found - oximeter");
  }

  
  
  delay(2000);
  Serial.print("Average temp: ");
  Serial.println(temp_average);
  Serial.print("Average bpm: ");
  Serial.println(bpm_average);
  Serial.print("Average O2: ");
  Serial.println(O2_average);

  //establishConnection();

  //MySignals.enableSensorUART(WIFI_ESP8266);
  delay(1000);
  
  sendInformation(temp_average, 1);
  sendInformation(bpm_average, 2);
  sendInformation(O2_average, 3);


  int temp_rate = ((temp - temp_average)/(temp_average))*100;
  int bpm_rate = ((bpm_val - bpm_average)/(bpm_average))*100;
  //int O2_rate = ((O2_val - O2_average)/(O2_average))*100;



  if(temp_rate > 1.5 && bpm_rate > 1.5 && O2_average < 95){
    // High Stress
    sendStressInfo(1);
  }

  else{
    // No/Little Stress
    sendStressInfo(0);
  }
  

}



void average_result_temp(int valRead){
  if(temp_num_elems == 0){
    temp_head = new Node();
    temp_head->data = valRead;
    temp_head->next = NULL;
    temp_sum_value += valRead;
    temp_curr = temp_head;
    temp_end_elem = temp_head;
    temp_average = temp_sum_value;
    temp_num_elems += 1;
    
  }
 else if(temp_num_elems < MAX_ELEMS){
    temp_tmp = new Node();
    temp_tmp->data = valRead;
    temp_tmp->next = NULL;
    temp_sum_value += valRead;
    temp_curr->next = temp_tmp;  
    temp_end_elem = temp_tmp;
    temp_num_elems += 1;
    temp_average = temp_sum_value / temp_num_elems;

 }
 else{
  temp_sum_value -= temp_head->data;
  temp_head = temp_head->next;
  temp_tmp = new Node();
  temp_tmp->data = valRead;
  temp_tmp->next = NULL;
  temp_sum_value += valRead;
  temp_end_elem = temp_tmp;
  temp_average = temp_sum_value/MAX_ELEMS;
  //num_elems += 1;
 }

}



void average_result_bpm(int valRead){
  if(bpm_num_elems == 0){
    bpm_head = new Node();
    bpm_head->data = valRead;
    bpm_head->next = NULL;
    bpm_sum_value += valRead;
    bpm_curr = bpm_head;
    bpm_end_elem = bpm_head;
    bpm_average = bpm_sum_value;
    bpm_num_elems += 1;
    
  }
 else if(bpm_num_elems < MAX_ELEMS){
    bpm_tmp = new Node();
    bpm_tmp->data = valRead;
    bpm_tmp->next = NULL;
    bpm_sum_value += valRead;
    bpm_curr->next = bpm_tmp;  
    bpm_end_elem = bpm_tmp;
    bpm_num_elems += 1;
    bpm_average = bpm_sum_value / bpm_num_elems;

 }
 else{
  bpm_sum_value -= bpm_head->data;
  bpm_head = bpm_head->next;
  bpm_tmp = new Node();
  bpm_tmp->data = valRead;
  bpm_tmp->next = NULL;
  bpm_sum_value += valRead;
  bpm_end_elem = bpm_tmp;
  bpm_average = bpm_sum_value/MAX_ELEMS;
  //num_elems += 1;
 }

}


void average_result_O2(int valRead){
  if(O2_num_elems == 0){
    O2_head = new Node();
    O2_head->data = valRead;
    O2_head->next = NULL;
    O2_sum_value += valRead;
    O2_curr = O2_head;
    O2_end_elem = O2_head;
    O2_average = O2_sum_value;
    O2_num_elems += 1;
    
  }
 else if(O2_num_elems < MAX_ELEMS){
    O2_tmp = new Node();
    O2_tmp->data = valRead;
    O2_tmp->next = NULL;
    O2_sum_value += valRead;
    O2_curr->next = O2_tmp;  
    O2_end_elem = O2_tmp;
    O2_num_elems += 1;
    O2_average = O2_sum_value / O2_num_elems;

 }
 else{
  O2_sum_value -= O2_head->data;
  O2_head = O2_head->next;
  O2_tmp = new Node();
  O2_tmp->data = valRead;
  O2_tmp->next = NULL;
  O2_sum_value += valRead;
  O2_end_elem = O2_tmp;
  O2_average = O2_sum_value/MAX_ELEMS;
  //num_elems += 1;
 }

}



void establishConnection(){

  //Enable WiFi ESP8266 Power -> bit1:1
  bitSet(MySignals.expanderState, EXP_ESP8266_POWER);
  MySignals.expanderWrite(MySignals.expanderState);

  //MySignals.initSensorUART();

  MySignals.enableSensorUART(WIFI_ESP8266);
  delay(1000);

  // Checks if the WiFi module is started
  int8_t answer = sendATcommand("AT", "OK", 6000);
  if (answer == 0)
  {
    MySignals.println("Error");
    // waits for an answer from the module
    while (answer == 0)
    {
      // Send AT every two seconds and wait for the answer
      answer = sendATcommand("AT", "OK", 6000);
    }
  }
  else if (answer == 1)
  {

    MySignals.println("WiFi succesfully working!");


    if (sendATcommand("AT+CWMODE=1", "OK", 6000))
    {
      MySignals.println("CWMODE OK");
    }
    else
    {
      MySignals.println("CWMODE Error");

    }


    if (sendATcommand("AT+CWJAP=\"iPhone\",\"hello123\"", "OK", 20000))
    {
      MySignals.println("Connected!");
    }
    else
    {
      MySignals.println("Error");
      establishConnection();
    }
   
  }
  
}

int8_t sendATcommand(char* ATcommand, char* expected_answer1, unsigned int timeout)
{

  uint8_t x = 0,  answer = 0;
  char response[500];
  unsigned long previous;

  memset(response, '\0', sizeof(response));    // Initialize the string

  delay(100);

  while ( Serial.available() > 0) Serial.read();   // Clean the input buffer

  delay(1000);
  Serial.println(ATcommand);    // Send the AT command

  x = 0;
  previous = millis();

  // this loop waits for the answer
  do
  {

    if (Serial.available() != 0)
    {
      response[x] = Serial.read();
      x++;
      
      // check if the desired answer is in the response of the module
      if (strstr(response, expected_answer1) != NULL)
      {
        answer = 1;
        //MySignals.println(response);
      }
    }
    // Waits for the asnwer with time out
  }while ((answer == 0) && ((millis() - previous) < timeout));
  MySignals.print("Response: ");
  MySignals.println(response);
  return answer;
}



void sendInformation(int results, int field){
  //establishConnection();

  //Serial.println(results);

  String tval = String(results);
  char *result = tval.c_str();

  
  if (sendATcommand("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80", "OK", 100000)) {
    
        sendATcommand("AT+CIPSEND=50", "OK", 100000);

        char *getData;
        
        if(field == 1){
          getData = "GET /update?api_key=WLMR1EFG742S9H0P&field1=00";
          getData[44] = result[0];
          getData[45] = result[1];
        }
        
        else if(field == 2){
          getData = "GET /update?api_key=WLMR1EFG742S9H0P&field2=00";
          getData[44] = result[0];
          getData[45] = result[1];
        }
        
        else if(field == 3){
            getData = "GET /update?api_key=WLMR1EFG742S9H0P&field3=00";
            getData[44] = result[0];
            getData[45] = result[1];          
        }
        
        sendATcommand(getData, "OK", 20000);
        sendATcommand("AT+CIPCLOSE", "OK", 20000);
      }
      else {
        MySignals.println("Unable to send");
        sendInformation(results, field);
      }
  
  delay(2000);

}


void sendStressInfo(int field){

  if (sendATcommand("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80", "OK", 100000)) {
    
        sendATcommand("AT+CIPSEND=51", "OK", 100000);

        char *getData;
        
        if(field == 1){
          getData = "GET /update?api_key=WLMR1EFG742S9H0P&field4=1.0";
        }
        
        else if(field == 0){
          getData = "GET /update?api_key=WLMR1EFG742S9H0P&field4=0.0";
        }
        else{
          return;
      }
        

        
        sendATcommand(getData, "OK", 20000);
        sendATcommand("AT+CIPCLOSE", "OK", 20000);
      }
      else {
        MySignals.println("Unable to send");
        sendStressInfo(field);
      }

      delay(2000);
}





// UNIT TESTING

void test_temp_average(){

  int val1 = 10;
  int val2 = 20;

  average_result_temp(val1);
  assert(temp_average == 10);

  average_result_temp(val2);
  assert(temp_average == 20);
  
  
}



void test_bpm_average(){

  int val1 = 10;
  int val2 = 20;

  average_result_bpm(val1);
  assert(bpm_average == 10);

  average_result_bpm(val2);
  assert(bpm_average == 20);
  
  
}

void test_O2_average(){

  int val1 = 10;
  int val2 = 20;

  average_result_O2(val1);
  assert(O2_average == 10);

  average_result_O2(val2);
  assert(O2_average == 20);
  
  
}
