// functions declarations
void trigger_light_on(void);
void light_off(void);
int set_light_on(String args);
int set_light_off(String args);

// variable declarations
int ledPin = D6;
volatile int state = LOW;
unsigned long current_time;
volatile unsigned long set_time;
volatile bool hold_light_off = false;
volatile bool can_turn_light_on = false;

// CONSTANTS
#define FIFTEEN_MIN_MILLIS (15 * 60 * 1000)
#define THREE_HOURS_MILLIS (3 * 60 * 60 * 1000)

void setup(){
  pinMode(D0, INPUT_PULLDOWN );
  pinMode(ledPin, OUTPUT);
  attachInterrupt(D0, trigger_light_on, RISING);
  set_time = millis();
  Spark.function("LightsOn", set_light_on);
  Spark.function("LightsOff", set_light_off);
}

void loop(){
  if(can_turn_light_on){
    set_time = millis();
    state = HIGH;
    digitalWrite(ledPin, state);
    can_turn_light_on = false;
  }
  current_time = millis();
  if( set_time+FIFTEEN_MIN_MILLIS < current_time ){
    light_off();
    hold_light_off = false;
  }
}

void trigger_light_on(){
  if (!hold_light_off){
    set_light_on("triggered");
  }
}

void light_off(){
  state = LOW;
  digitalWrite(ledPin, state);
}



int set_light_on(String args){
    if(args != "triggered"){
        hold_light_off = false;
    }
  can_turn_light_on = true;
  return 1;
}

int set_light_off(String args){
  hold_light_off = true;
  set_time = millis() + THREE_HOURS_MILLIS;
  light_off();
  return 1;
}
