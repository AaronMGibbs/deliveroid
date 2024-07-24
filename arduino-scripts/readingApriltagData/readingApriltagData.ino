String last_value;
char my_string[30];
String string_data;


void setup(){
  Serial.begin(115200);
  Serial.setTimeout(1);
  
}

void loop(){
  while(!Serial.available()){
    string_data = Serial.readString();

    if (!string_data.isEmpty()){
      last_value = string_data;
      Serial.println(last_value);
    }
  }
}