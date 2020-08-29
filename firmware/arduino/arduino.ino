static const int VERSION = 3; // 版本号
static const int BUFFERLEN = 600; // 缓存大小

static const int PIN_BUTTON = 4; //可选，接引脚4，连接一端接地的实体捕获按钮
static const int PIN_IN = 2; //中断，接引脚2，接收器，接433、315mhz、IR等接收模块
static const int PIN_OUT = 3; //接引脚3，发射器，接发射模块

int top = 0;
int buttonLastPressed = 0;
bool last = false;
bool buttonPressed = false;
bool serialCap = false;
unsigned int data[BUFFERLEN]; //第一个是低电平的延时

bool killermode = false;

void onChange() {
  static unsigned long lastTime = 0;
  const long time = micros();
  const bool now = digitalRead(PIN_IN);

  if(now == last || (time - lastTime) < 100) return;

  //Serial.print(last?"t":"f");
  //Serial.println(time - lastTime);
  save(time - lastTime); //必定以true开始！

  last = now;
  lastTime = time;
}

void save(unsigned int time) {
  if(top >= BUFFERLEN) {
    Serial.println("overflow!!");
    report();
    top = 0;
  }
    
  data[top] = time;
  top ++;
}

void repeat(int repeat, int delay) {
  Serial.print("S|");
  Serial.print(repeat);
  Serial.print("|");
  Serial.println(delay);

  for(int j=0;j<repeat;j++){
    bool flag = true;
    
    for(int i=1;i<top;i++) {
      digitalWrite(PIN_OUT, flag);
      delayMicroseconds(data[i]);
      flag = !flag;
    }
    digitalWrite(PIN_OUT, LOW);
    delayMicroseconds(delay);
  }
  Serial.println("Done!");
}

void report() {

  Serial.print("R");
  data[1] = 0;

  for(int i=1;i<top;i++) {
      Serial.print(data[i]);
      Serial.print(",");
  }
  
  Serial.println();
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(50);

  Serial.println();
  Serial.println();
  Serial.println();
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(PIN_OUT, OUTPUT);
  digitalWrite(PIN_OUT, LOW);
  
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_IN, INPUT_PULLUP);
}

void loop() {
  if(!digitalRead(PIN_BUTTON) || serialCap) {
    if(!buttonLastPressed)
      buttonLastPressed = millis();
    else if(millis() - buttonLastPressed >= 1000) {
      if(!buttonPressed) {
        Serial.println("Button Long Pressing...");
        attachInterrupt(digitalPinToInterrupt(PIN_IN), onChange, CHANGE);
      }
      buttonPressed = true;
    }
  } else {
    if(buttonLastPressed) {
      if(buttonPressed) {
        detachInterrupt(PIN_IN);
        detachInterrupt(0); //Nano测试时，与attachInterrupt填一样无效
        report();
        last = false;
        top = 0;
      }

      buttonLastPressed = 0;
      buttonPressed = false;
    }
  }

 
  int status = 0; 
  int repeat_time = 0;
  int delay = 0;
  
  while(Serial.available()) {
    if(!status) {
      byte t = Serial.read();
      if(t == 'R') {
        status = 2;
        top = 0;
        repeat_time = Serial.parseInt();
        delay = Serial.parseInt();
        save(0);
      } else if(t == 'K'){
        if(killermode) {
          killermode = false;
          digitalWrite(PIN_OUT, LOW);
          Serial.println("KF");
        } else {
          killermode = true;
          Serial.println("KO");
        }
        break;
      } else if(t == 'A'){
        serialCap = true;
      } else if(t == 'B'){
        serialCap = false;
      } else if(t == 'H'){
        Serial.print("HandShake|v");
        Serial.println(VERSION);
      } else {
        status = 1;
        break;
      }
    }
    
    int i = Serial.parseInt();
    if(!i)
      continue;
 
    save(i);
  }

  if(status)
    while(Serial.read()!=-1) {} //清空串口缓冲
    
  if(status==2)
    repeat(repeat_time, delay);

  static unsigned long lasttime_killer = 0;
  static bool last_killer = false;
  if(killermode && micros() - lasttime_killer>=800) {
    digitalWrite(PIN_OUT, last_killer);
    last_killer = !last_killer;
    lasttime_killer = micros();
  }
}
