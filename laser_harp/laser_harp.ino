int laser1=25;  //green laser relays
int laser2=23; //red laser relays
int i=0;
int v= 127;                 //predefined note velocity
//change the size of the array according to the no. of lasers you are using:
int  a[13]={A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12}; //LDRs to detect lasers
int threshold[13];  
int note[13]={48,49,50,51,52,53,54,55,56,57,58,59,60};   //note mapping; MIDI note values.
char flag[13]={0,0,0,0,0,0,0,0,0,0,0,0,0};
int p[13];

void setup() {
Serial.begin(9600); //UART over USB (for debug data)
Serial1.begin(31250); //UART for MIDI
//Set Pin Mode
pinMode (laser1, OUTPUT); //Power to lasers
pinMode (laser2, OUTPUT); //Red lasers: black keys equivalent
//begin calibration of threshold level of voltage from Pot. Divider
digitalWrite(laser1,LOW);
digitalWrite(laser2,LOW);
delay(1000); 

for (i=0; i<13; i++)
{
Serial.print(i);Serial.println(" "); 
threshold[i]=analogRead(a[i]);;Serial.println("\n"); 
}

digitalWrite(laser1,HIGH);
digitalWrite(laser2,HIGH);
delay(1000); //delay to let lasers turn on to full intensity

for (i=0; i<13; i++)
{
Serial.print(i); Serial.println(" "); 
//calculates and automatically sets the threshold levels for analog values in order to detect a cut
threshold[i]+=analogRead(a[i]);   
threshold[i]=threshold[i]/2 + 50; 
Serial.print(threshold[i]);;Serial.println("\n"); 
}

}

void loop() {
  for (i=0; i<13; i++)
  {    
    p[i]=analogRead(a[i]);    
    if(p[i]<threshold[i]) p[i]=0;    
    else p[i]=1;           
                                   
      if (((p[i])^(flag[i])))    //check for note status change
     { reflect(p[i], i, v);                                
      //if note is on, set the flag
      if(p[i])  flag[i]++;  
      else flag[i]=0;
     }    
  }
}


void reflect(char status1,int note1, int vel1) //function to send MIDI data
{
  if (status1) {Serial1.write(145);  Serial.print(i); Serial.println("on");}  //145d= 1001 0001 b (note-on on channel1)
  else {Serial1.write(129);  Serial.print(i); Serial.println("off");}        //129d= 1000 0001 b (note-off on channel1)
  
  Serial1.write(note[note1]);
  
  Serial1.write(vel1);   
}
