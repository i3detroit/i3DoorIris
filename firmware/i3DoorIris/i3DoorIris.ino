#define max_open_current 27
#define max_close_current 32
#define motor_current 1
#define outside_pir 4
#define inside_pir 3
#define motor_pos 0
#define motor_neg 1
#define OPEN 1
#define CLOSED 0
#define UNDEF -1

unsigned char in = 0;
unsigned char last_in = 0;
unsigned char out = 0;
unsigned char last_out = 0;

unsigned int current = 0;
unsigned int cal_current = 0;

unsigned char iris_state = UNDEF;
unsigned char iris_req = UNDEF;

unsigned char max_lock = 10;
unsigned char current_lock = max_lock;

void setup() {
  pinMode(outside_pir,INPUT);
  pinMode(inside_pir,INPUT);
  pinMode(motor_pos,OUTPUT);
  pinMode(motor_neg,OUTPUT);

  cal_current = analogRead(motor_current);
}

void loop() {
  if(iris_state == UNDEF){
    iris_req = OPEN;
  }
  else
  {
    in = digitalRead(inside_pir);
    if(in != last_in){
      last_in = in;
      if(in == HIGH){
        iris_req = OPEN;
        current_lock = max_lock;
      }
      else if(in == LOW){
        iris_req = CLOSED;
        current_lock = max_lock;
      }
    }
    
    out = digitalRead(outside_pir);
    if(out != last_out){
      last_out = out;
      if(out == HIGH){
        iris_req = OPEN;
        current_lock = max_lock;
      }
      else if(out == LOW){
        iris_req = CLOSED;
        current_lock = max_lock;
      }
    }
  }
  
  if(iris_req != iris_state){
    switch(iris_req){
      case OPEN:
        digitalWrite(motor_pos,1);
        digitalWrite(motor_neg,0);
        break;
      case CLOSED:
        digitalWrite(motor_pos,0);
        digitalWrite(motor_neg,1);
        break;
      case UNDEF:
      default:
        digitalWrite(motor_pos,0);
        digitalWrite(motor_neg,0);
    }
    current = analogRead(motor_current);
    
    if(current > cal_current)
      current -= cal_current;
    else
      current = cal_current - current;
    
    if(iris_req == OPEN){
      if(current_lock == 0){
        if(current > max_open_current){
          digitalWrite(motor_pos,0);
          digitalWrite(motor_neg,0);
          iris_state = iris_req;
        }
      }
      else{
        --current_lock;
      }
    }
    else if(iris_req == CLOSED){
      if(current_lock == 0){
        if(current > max_close_current){
          digitalWrite(motor_pos,0);
          digitalWrite(motor_neg,0);
          iris_state = iris_req;
        }
      }
      else{
        --current_lock;
      }
    }
  }
}
