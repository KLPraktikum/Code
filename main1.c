
#include "abdrive.h"
#include "simpletools.h"
#include "wifi.h"
#include "servo.h"
#include "ping.h"


int i = 0;
char string_hilfe1 [9];
char string_hilfe2[9];
char stringvorne[200];
char stringmittevorne[200];
char stringmittehinten[200];
char stringhinten[200];
int event, id, handle;
int getFromPageId1, getFromPageId2;
int postFromPageId;
int getFromPageId3;
volatile int *thread = 0;
volatile int run = 0;
volatile int labyL = 0;
volatile int labyTerminate = 0;

void labyrinth();
void laby_timer();
void musicplay();
void musicplay3();
void musicplaygot();

/* intlen ist eine  Hilfsfunktion, welche die Laenge des Strings ueberprueft */

int intlen(int i){
    if(i < 10)
        return 1;
    else if(i < 100)
        return 2;
    else if(i > 99)
        return 3;
}

/* setPingoServo setzt den Servo auf den gewuenschten Winkel */

void setPingServo(int degree, int pin) {
  servo_angle(pin, degree);
  int pause_time = 150;
  if(degree == 0) {
    pause_time = 150;
  }else{
    pause_time = 150;
  }
  pause(pause_time);
}

/* teilen ist eine Hilfsfunktion zum Teilen der Strings, um PingData an JavaScript uebermitteln zu koennen.*/

int teilen(int help, char *stringto){
  int hundert = help/100;
  int zehn = (help - hundert*100) / 10;
  int eins = help-hundert*100-zehn*10;
  
  int k = 0;
  if(hundert!=0){
    stringto[k++] = hundert + '0';
  }   
  if(zehn!=0){
    stringto[k++] = zehn + '0';
  }   
  stringto[k++] = eins + '0';
  stringto[k++] = '_';
  return k;  
  
}

/*ping_around misst mit 2 PINGS und 2 Servo Motoren alle 3 Grad um den Roboter.*/

void ping_around(){
  stringvorne[0] = '\0';
  stringmittevorne[0] = '\0'; 
  stringmittehinten[0] = '\0'; 
  stringhinten[0] = '\0';
   int l1 = 0;
   int l2 = 0;
   int l3 = 0;
   int l4 = 0;
   int help = 0;
  for(i = 0; i <= 180; i = i+6){
    
       setPingServo(i*10, 16);
      setPingServo(i*10, 17);
     if( i <= 90){
       help = ping_cm(11);
       l1 += teilen(help, stringvorne + (l1*sizeof(char)));
       //sprintf(stringvorne,"%i_",help);
       //l1 += intlen(help) + 1;
               

       help = ping_cm(10);
       //stringmittehinten[x] = help;
       //sprintf(stringmittehinten,"%i_",help);
       l2 += teilen(help, stringmittehinten + (l2*sizeof(char)));
       //stringmittehinten[x++] = '_'; 
     }else if(i > 90){
       help = ping_cm(11);
      // sprintf(stringmittevorne,"%i_",help);
       l3 += teilen(help, stringmittevorne + (l3*sizeof(char)));
               

       help = ping_cm(10);
       l4 += teilen(help, stringhinten + (l4*sizeof(char)));
       
     }              
     
  }
 stringvorne[l1 -1] = '\0';
 stringmittevorne[l3 - 1] = '\0';
 stringmittehinten[l2 - 1] = '\0';
 stringhinten[l4 - 1] = '\0';
 servo_angle(16, 0);
 servo_angle(17, 0);
}

int main(){
	
  drive_setAcceleration(0,200);
  drive_setAcceleration(1,400);
  drive_speed(0,0);
  /*run = 1;
  labyrinth();
  return 0;
  */
  //cog_run(&musicplay,50);
  //cog_run(&musicplay3,50);
  int count = 0;
  wifi_start(7, 8, 115200, WX_ALL_COM);
  
  
  getFromPageId1 = wifi_listen(HTTP, "/ping");
  getFromPageId2 = wifi_listen(HTTP, "/pingdata");
  getFromPageId3 = wifi_listen(HTTP, "/status"); 
  postFromPageId = wifi_listen(HTTP, "/post");
  print("%d %d %d %d\n",getFromPageId1,getFromPageId2,getFromPageId3,postFromPageId);

  while(1)
  {    
    wifi_poll(&event, &id, &handle);       
      if(event == 'G')
      {
        //print("id: %d\n", id);
        if(id == getFromPageId1)
        {
          wifi_print(GET, handle, "ok");
          ping_around(); 
          count = 0;
          
        }
        if(id == getFromPageId2){
            //print("%d", count);                      
            if(count == 0){     
               wifi_print(GET, handle, "%s\n", stringvorne);
              //print("count0\n"); 
              count = 1;                       
            }else if(count == 1){
              //print("count1\n");
              wifi_print(GET, handle, "%s\n", stringmittevorne); 
              //print(stringmittevorne);  
              count = 2;        
            }else if(count == 2){
              //print("count2\n");
              wifi_print(GET, handle, "%s\n", stringmittehinten);
              //print(stringmittehinten);  
              count = 3;         
            }else if(count == 3){
              //print("count3\n");
              wifi_print(GET, handle, "%s\n", stringhinten);          
              count = -2;
            }            
             
        }
        if(id == getFromPageId3){
          if(!run){
            wifi_print(GET, handle, "%s\n", "ok");
          }else{
            wifi_print(GET, handle, "%s\n", "laby");
          }                    
           
        }                                         
     }else if(event == 'P'){       
        if(id == postFromPageId)
        {
          //printf("Incoming POST request\r");
          char s[50];
          wifi_scan(POST, handle, "cmd%s",   &s);
          //printf("test = %s\n", s);
          if(s[0] == 'X'){
            if(run == 0){
              run = 1;
              thread = cog_run(&labyrinth,150); 
            }else{                          
              run = 0;        
            }
          }else{
            int i = 0;
            while(s[i] != ','){
                i++;
               }
            s[i] = '\0';
             int drehen1 = atoi(s);
            int j = i + 1;
            while(s[j] != ',') {
                j++;
              }
             s[j] = '\0';
            int drehen2 = atoi(&s[i+1]);
            
            int z = j+1;
            while(s[z] != ',') {
              z++;
              }
            s[z] = '\0';
            int fahren = atoi (&s[j+1]);
            drive_goto(drehen1,drehen2);
           drive_goto(fahren,fahren);  
         }                                             
        }    
   }        
                                      
     pause(100);
  }  
}

/*labyrinth startet den Laby Modus, welches den Roboter automatisches durch 
ein Labyrinth fahren laesst.
Bricht entweder durch manuelle Eingabe oder durch hohe Abstandsmessungen ab.*/

void laby_timer() {
    int pause_time = 100;
    labyTerminate = 1000 * 60; // 10s
    while(run && labyTerminate > 0){
      labyTerminate -= pause_time; 
      pause(pause_time);
   }     
}  

/*labyrinth startet den Laby Modus, welches den Roboter automatisches durch 
ein Labyrinth fahren laesst.
Bricht entweder durch manuelle Eingabe oder durch hohe Abstandsmessungen ab.*/

void labyrinth() 
{
  //drive_goto(30,30);
  //servo_angle(16,0);
  int dist = 720;
  int start_dist = 1500;
  int spee = 20;
  int maxspeed = 30;
  double factor = .007;
  int rechts, vorne;
  int rspeed = 20;
  int lspeed = 20;
  
  int rerror, lerror;
  
  servo_angle(16,900);

  servo_angle(17,1800);
  pause(300);
  
  // ERSTE WAND FINDEN 
  drive_speed(lspeed*4,4*rspeed);
  //print("driving to wall\n");
  int p = ping(11);
  while(p > start_dist){
    // rechts   
    servo_angle(16,0);
    pause(400);
    p = ping(11);
    if(p < start_dist)
      break;
    
    // vorne   
    servo_angle(16,900);
    pause(400);
    p = ping(11);
    
  }
  
  drive_speed(0,0);
  
  servo_angle(16,0);
  pause(400);
  p = ping(10);
  if(p > start_dist)
    drive_goto(-5,5);
    
  //print("wall found\n");
  
  servo_angle(17,1200);


  cog_run(&laby_timer,150);
  pause(100);
  
  while(run)
  {
    servo_angle(16,0);
    pause(300);
    rechts = ping(11);
    
    // RECHTSKURVE
    rerror = dist - rechts;
    rspeed = spee + factor * rerror;
    lspeed = spee - factor * rerror;
    
    servo_angle(16,800);
    pause(300);
    vorne = ping(11);
    
	//print("v:%d r:%d\n",vorne,rechts);
    // ABBRUCHBEDINGUNG
    if(labyTerminate == 0 && (vorne > 4500 || rechts > 5000)){
      ////print("abbruch\n");
      run = 0;
      drive_speed(0,0);
      break;
    }     
    // KOPF AN DER WAND
    if(vorne <  dist * 1.3/* && rechts < dist * 2*/){
      if(vorne > dist * .7)
        lspeed = 1;
      else
        lspeed = -8;
      ////print("links\n");
      rspeed = spee * .75;
      /*
       if(rechts < dist * 1.2) {// links
        if(vorne > dist * .7)
          lspeed = 1;
        else
          lspeed = -8;
        ////print("links\n");
        rspeed = spee * .75;
       } else {                 // rechts
         if(vorne > dist * .7)
          rspeed = 1;
         else
          rspeed = -8;
         ////print("rechts\n");
         lspeed = spee*.75;
       }                */
    } else {
      ////print("rechts / geradeaus\n");
    }      
    /*
    if(vorne < 70 && rechts < 70){
      rspeed = -7;
      lspeed = -7;
    }     */               
    if(rspeed > maxspeed) rspeed = maxspeed;
    if(rspeed < 0) rspeed = 0;
    if(lspeed > maxspeed)lspeed = maxspeed;
    if(lspeed < -7) lspeed = -7;    
    if(abs(lspeed-rspeed) > 1.5*spee){
      lspeed *= .5;
      rspeed *= .5;
    }
    
    /*if(vorne < 950){
      if(rechts < 1500){
        lspeed = -7;
        rspeed = spee/2;
      }else{
        lspeed = spee/4;
        rspeed = -2;
      }                    
    }   */      
    //drive_rampStep(lspeed,rspeed);  
    drive_speed(lspeed,rspeed);
    ////print("v%d,r%d\n",vorne,rechts);  
  }  
  drive_speed(0,0);
  
  drive_goto(30,30);
  if(rechts > 5000)
    drive_goto(23,-24);
  servo_angle(16, 0);
  servo_angle(17, 0);
  pause(300);
  ////print("terminating\n");
  cog_end(thread);
}

void musicplaygot(){
  int port2 = 2;
  int pauset = 40;
  int c0 = 130;
  int d0 = 146;
  int e0 = 164;
  int f0 = 174;
  int g0 = 195;
  int a0 = 220;
  int h0 = 246;
  int c = 261;
  int d = 293;
  int e = 329;
  int f = 349;
  int g = 391;
  int a = 440;
  int h = 493;
  int c2 = 523;
  int cisdes = 554;
  int d2 = 587;
  int e2 = 659;
  int f2 = 698;
  int g2 = 783;
  int a2 = 880;
  int h2 = 987;
  int c3 = 1046;
  int d3 = 1174;
  int eis = 311;
  int voll = 1000;
  int ganz = 2000;
  int halbe = 500;
  int viertel = 250;
  int i = 1;
  int noten[] = {g,c,e,f,g,c,e,f,g,c,e,f,g,c,eis,f,g,c,e,f,g,c,e,f,g,c,e,f};
  int lange[] = {halbe,halbe,viertel,viertel,halbe,halbe,viertel,viertel,halbe,halbe,viertel,viertel,
  halbe,halbe,viertel,viertel,halbe,halbe,viertel,viertel,halbe,halbe,viertel,viertel,
  halbe,halbe,viertel,viertel,halbe,halbe,viertel,viertel};
  while(i > 0){ 
      for(int u = 0; u < 31; u++){
        if(noten[u] != 0){
          freqout(port2, lange[u], noten[u]); 
        }else{
          pause(lange[u]);
        }                    
      }        
  }
}
void musicplay(){
  int port2 = 2;
  int pauset = 40;
  int c0 = 130;
  int d0 = 146;
  int e0 = 164;
  int f0 = 174;
  int g0 = 195;
  int a0 = 220;
  int h0 = 246;
  int c = 261;
  int d = 293;
  int e = 329;
  int f = 349;
  int g = 391;
  int a = 440;
  int h = 493;
  int c2 = 523;
  int cisdes = 554;
  int d2 = 587;
  int e2 = 659;
  int f2 = 698;
  int g2 = 783;
  int a2 = 880;
  int h2 = 987;
  int c3 = 1046;
  int d3 = 1174;
  int i = 1;
  
  while(i > 0){  
  freqout(port2, 200, f); 
  freqout(port2, 200, g); 
  
  freqout(port2, 1000, a);
  freqout(port2, 1000, c2);
  freqout(port2, 1000, a);
  freqout(port2, 1000, g);
  
  freqout(port2, 2000, f);
  pause(1000);
  freqout(port2, 500, a);
  freqout(port2, 500, c2);
  
  freqout(port2, 1000, d2);
  freqout(port2, 1000, f2);
  freqout(port2, 1000, e2);
  freqout(port2, 1000, c2);
  
  freqout(port2, 2000, a);
  freqout(port2, 1000, g);
  freqout(port2, 500, f);
  freqout(port2, 500, g);
  
  freqout(port2, 1000, a);
  freqout(port2, 1000, c2);
  freqout(port2, 1000, a);
  freqout(port2, 1000, g);
  
  freqout(port2, 2000, f);
  pause(1000);
  freqout(port2, 500, a);
  freqout(port2, 500, c2);
  
  freqout(port2, 2000, d2);
  pause(pauset);
  freqout(port2, 500, d2);
  pause(pauset);
  freqout(port2, 500, d2); 
  freqout(port2, 500, c2);
  freqout(port2, 500, a);
  
  freqout(port2, 3000, g);
  pause(1000);
  
  freqout(port2, 500, f);
  freqout(port2, 500, g);
  freqout(port2, 3000, a);
  
  pause(3000);
  freqout(port2, 250, a);
  freqout(port2, 500, d2);
  freqout(port2, 500, e2);
  
  freqout(port2, 1500, f2);
  freqout(port2, 250, g2);
  freqout(port2, 250, f2);
  freqout(port2, 500, e2);
  freqout(port2, 500, c2);
  
  freqout(port2, 2000, a);
  freqout(port2, 1000, g);
  freqout(port2, 500, f);
  freqout(port2, 500, g);
  
  freqout(port2, 4000, a);
  
  pause(3000);
  freqout(port2,250, a);
  freqout(port2, 500, d2);
  freqout(port2, 500, e2);
  
  freqout(port2, 2000, f);
  freqout(port2, 1500, g);
  freqout(port2, 250, f);
  freqout(port2, 250, g);
  
  freqout(port2, 2000, a2);
  pause(pauset);
  freqout(port2, 1000, a2);
  freqout(port2, 500, f2);
  freqout(port2, 500, d2);
  freqout(port2, 500, f2);
  
  pause(pauset);
  freqout(port2, 1000, f2);
  freqout(port2, 3000, c2);
  //neue Seite
  pause(pauset);
  freqout(port2, 1000, c2);
  pause(3000);
  freqout(port2, 250, h);
  freqout(port2, 250, c2);
  
  freqout(port2, 1500, 554);//622;
  freqout(port2, 250, c2);
  freqout(port2, 250, h);
  freqout(port2, 1500, 466);//415
  freqout(port2, 250, h);
  freqout(port2, 250, c2);
  
  freqout(port2, 2000, h);
  freqout(port2, 1000, 466);//415
  freqout(port2, 1000, g);
  
  freqout(port2, 2000, f);
  freqout(port2, 1500, 311);//eis
  pause(40);
  freqout(port2, 250, e);
  pause(pauset);
  freqout(port2, 250, e);
  
  freqout(port2, 3000, f);
  pause(500);
  freqout(port2, 250, h);
  freqout(port2, 250, c);
  
  freqout(port2, 1500, 554);//622
  freqout(port2, 250, c2);
  freqout(port2, 250, d2);
  freqout(port2, 1000, 622);//
  freqout(port2, 500, g);
  freqout(port2, 500, a);
  int x = 1;
  while(x > 0){
  freqout(port2, 1000, h);
  freqout(port2, 1000, d2);
  freqout(port2, 1000, h);
  freqout(port2, 1000, a);
  // zeile zwei takt drei anfang
  
  freqout(port2, 2000, g);
  pause(1000);
  freqout(port2, 500, h);
  freqout(port2, 500, d2);
  
  freqout(port2, 2000, e2);
  pause(pauset);
  freqout(port2, 500, e2);
  freqout(port2, 500, g2);
  freqout(port2, 500, f2);
  freqout(port2, 500, d2);
  
  freqout(port2, 2000, h);
  freqout(port2, 1000, a);
  freqout(port2, 500, g);
  freqout(port2, 500, a);
  
  freqout(port2, 500, h);
  freqout(port2, 500, d2);
  freqout(port2, 500, h);
  freqout(port2, 500, a);
  
  freqout(port2, 2000, g);
  pause(500);
  freqout(port2, 500, g);
  freqout(port2, 500, h);
  freqout(port2, 500, d2);
  
  freqout(port2, 2000, e2);
  pause(pauset);
  freqout(port2, 500, e2);
  pause(pauset);
  freqout(port2, 500, e2);
  freqout(port2, 500, d2);
  freqout(port2, 500, h);
 
  freqout(port2, 2000, a);
  pause(pauset);
  freqout(port2, 500, a);
  pause(pauset);
  freqout(port2, 500, a);
  freqout(port2, 500, g);
  freqout(port2, 500, f);
  
  freqout(port2, 4000, g);
  
  x-- ;
  }  
  freqout(port2, 500, 5000);
  
  //i--;
  }  
  }
void musicplay3(){
  int port2 = 3;
  int pauset = 40;
  int c0 = 130;
  int d0 = 146;
  int e0 = 164;
  int f0 = 174;
  int g0 = 195;
  int a0 = 220;
  int h0 = 246;
  int c = 261;
  int d = 293;
  int e = 329;
  int f = 349;
  int g = 391;
  int a = 440;
  int h = 493;
  int c2 = 523;
  int cisdes = 554;
  int d2 = 587;
  int e2 = 659;
  int f2 = 698;
  int g2 = 783;
  int a2 = 880;
  int h2 = 987;
  int c3 = 1046;
  int d3 = 1174;
  int i = 1;
  
  while(i > 0){  
  freqout(port2, 200, a); 
  freqout(port2, 200, g); 
  
  freqout(port2, 1000, c2);
  freqout(port2, 1000, e2);
  freqout(port2, 1000, c2);
  freqout(port2, 1000, h);
  
  freqout(port2, 2000, a);
  pause(1000);
  freqout(port2, 500, c2);
  freqout(port2, 500, e2);
  
  freqout(port2, 1000, f2);
  freqout(port2, 1000, a2);
  freqout(port2, 1000, g2);
  freqout(port2, 1000, e2);
  
  freqout(port2, 2000, c2);
  freqout(port2, 1000, h);
  freqout(port2, 500, a);
  freqout(port2, 500, h);
  
  freqout(port2, 1000, c2);
  freqout(port2, 1000, e2);
  freqout(port2, 1000, c2);
  freqout(port2, 1000, h);
  
  freqout(port2, 2000, a);
  pause(1000);
  freqout(port2, 500, c2);
  freqout(port2, 500, e2);
  
  freqout(port2, 2000, f2);
  pause(pauset);
  freqout(port2, 500, f2);
  pause(pauset);
  freqout(port2, 500, f2); 
  freqout(port2, 500, e2);
  freqout(port2, 500, c2);
  
  freqout(port2, 3000, h);
  pause(1000);
  
  freqout(port2, 500, a);
  freqout(port2, 500, h);
  freqout(port2, 3000, c2);
  
  pause(3000);
  freqout(port2, 250, c2);
  freqout(port2, 500, f2);
  freqout(port2, 500, g2);
  
  freqout(port2, 1500, a2);
  freqout(port2, 250, h2);
  freqout(port2, 250, a2);
  freqout(port2, 500, g2);
  freqout(port2, 500, e2);
  
  freqout(port2, 2000, c2);
  freqout(port2, 1000, h);
  freqout(port2, 500, a);
  freqout(port2, 500, h);
  
  freqout(port2, 4000, c2);
  
  pause(3000);
  freqout(port2,250, c2);
  freqout(port2, 500, f2);
  freqout(port2, 500, g2);
  
  freqout(port2, 2000, a);
  freqout(port2, 1500, h);
  //hier ich bin
  freqout(port2, 250, f);
  freqout(port2, 250, g);
  
  freqout(port2, 2000, a2);
  pause(pauset);
  freqout(port2, 1000, a2);
  freqout(port2, 500, f2);
  freqout(port2, 500, d2);
  freqout(port2, 500, f2);
  
  pause(pauset);
  freqout(port2, 1000, f2);
  freqout(port2, 3000, c2);
  //neue Seite
  pause(pauset);
  freqout(port2, 1000, c2);
  pause(3000);
  freqout(port2, 250, h);
  freqout(port2, 250, c2);
  
  freqout(port2, 1500, 554);//622;
  freqout(port2, 250, c2);
  freqout(port2, 250, h);
  freqout(port2, 1500, 466);//415
  freqout(port2, 250, h);
  freqout(port2, 250, c2);
  
  freqout(port2, 2000, h);
  freqout(port2, 1000, 466);//415
  freqout(port2, 1000, g);
  
  freqout(port2, 2000, f);
  freqout(port2, 1500, 311);//eis
  pause(40);
  freqout(port2, 250, e);
  pause(pauset);
  freqout(port2, 250, e);
  
  freqout(port2, 3000, f);
  pause(500);
  freqout(port2, 250, h);
  freqout(port2, 250, c);
  
  freqout(port2, 1500, 554);//622
  freqout(port2, 250, c2);
  freqout(port2, 250, d2);
  freqout(port2, 1000, 622);//
  freqout(port2, 500, g);
  freqout(port2, 500, a);
  int x = 1;
  while(x > 0){
  freqout(port2, 1000, h);
  freqout(port2, 1000, d2);
  freqout(port2, 1000, h);
  freqout(port2, 1000, a);
  // zeile zwei takt drei anfang
  
  freqout(port2, 2000, g);
  pause(1000);
  freqout(port2, 500, h);
  freqout(port2, 500, d2);
  
  freqout(port2, 2000, e2);
  pause(pauset);
  freqout(port2, 500, e2);
  freqout(port2, 500, g2);
  freqout(port2, 500, f2);
  freqout(port2, 500, d2);
  
  freqout(port2, 2000, h);
  freqout(port2, 1000, a);
  freqout(port2, 500, g);
  freqout(port2, 500, a);
  
  freqout(port2, 500, h);
  freqout(port2, 500, d2);
  freqout(port2, 500, h);
  freqout(port2, 500, a);
  
  freqout(port2, 2000, g);
  pause(500);
  freqout(port2, 500, g);
  freqout(port2, 500, h);
  freqout(port2, 500, d2);
  
  freqout(port2, 2000, e2);
  pause(pauset);
  freqout(port2, 500, e2);
  pause(pauset);
  freqout(port2, 500, e2);
  freqout(port2, 500, d2);
  freqout(port2, 500, h);
 
  freqout(port2, 2000, a);
  pause(pauset);
  freqout(port2, 500, a);
  pause(pauset);
  freqout(port2, 500, a);
  freqout(port2, 500, g);
  freqout(port2, 500, f);
  
  freqout(port2, 4000, g);
  
  x-- ;
  }  
  freqout(port2, 500, 5000);
  
  //i--;
  }  
}


