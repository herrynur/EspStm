#define led PC13
#define RXD2 PA3
#define TXD2 PA2

String dt[10];
int i;
String payload ="--";

//Timer
int time1start,time2start,time3start;
int time1end,time2end,time3end;
int freq1,freq2,freq3;

unsigned long previousMillis = 0;
const long interval = 4000;

void setup() {
  // put your setup code here, to run once:
pinMode(led, OUTPUT);
Serial.begin(9600);
Serial2.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial2.available())
    {
      payload = Serial2.readString();
      Serial.println("Data berhasil masuk");
      Serial.println(payload);
      parsing();
      tampil();
    }
    
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    tampil();
  }
}

void parsing()
{
  int j=0;
  dt[j]="";
      for(i=1;i<payload.length();i++)
      {
        if ((payload[i] == '#') || (payload[i] == ','))
         {
             j++;
             dt[j]="";
          }
         else
           {
              dt[j] = dt[j] + payload[i];
           }
      }
      //Mengubah string to INT
      time1start = dt[0].toInt();
      time1end = dt[1].toInt();
      time2start = dt[2].toInt();
      time2end = dt[3].toInt();
      time3start = dt[4].toInt();
      time3end = dt[5].toInt();
      //Frekuensi
      freq1 = dt[6].toInt();
      freq2 = dt[7].toInt();
      freq3 = dt[8].toInt();
}

void tampil()
{
  Serial.print("Time 1 Start : ");
  Serial.println(time1start);
  Serial.print("Time 1 End : ");
  Serial.println(time1end);
  Serial.print("Time 2 Start : ");
  Serial.println(time2start);
  Serial.print("Time 2 End : ");
  Serial.println(time2end);
  Serial.print("Time 3 Start : ");
  Serial.println(time3start);
  Serial.print("Time 3 End : ");
  Serial.println(time3end);
  Serial.print("Frequency 1 : ");
  Serial.println(freq1);
  Serial.print("Frequency 2 : ");
  Serial.println(freq2);
  Serial.print("Frequency 3 : ");
  Serial.println(freq3);
  Serial.println();
}
