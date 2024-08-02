#include <EEPROM.h>

const byte trigPin = 4;
const byte echoPin = 2;

const byte a1 = 3;
const byte a2 = 5;
const byte b1 = 6;
const byte b2 = 9;

byte min_aralik = 6; //x cm kala duvardan geri döner

byte oto_mod = 0;

bool test_mod = false;

const byte oto_mod_adr = 1; // .
const byte min_aralik_adr = 2; // .
const byte geri_kacma_ms_adr = 3; // .
const byte oto_geri_sag_hiz_adr = 4;// .
const byte hassasiyet_adr = 5; // .
const byte guncelleme_ms_adr = 6;// .

unsigned long gecen_zaman = 0;
uint16_t geri_kacma_ms = 300; //engel görünce geriye gitme süresi
uint16_t guncelleme_ms = 6;  
bool oto_sag = false;
byte oto_geri_sag_hiz = 250;//araç engelden kaçarken sağ arkaya doğru giderken kullanılacak hız (0-255 arası ayarlayın) 
byte oto_mod_sayac = 0;
byte komut_sayac = 0;

byte hiz_sol = 0;
byte hiz_sag = 0;
long hassasiyet = 0.75;// ileri sag-sol ve geri sag-sol dönüşlerinde keskinlik ayarı

void setup () 
{
Serial.begin(9600);
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(a1, OUTPUT);
pinMode(a2, OUTPUT);
pinMode(b1, OUTPUT);
pinMode(b2, OUTPUT);
Serial.println("Ayarlar Yukleniyor...");
//ayar_ver();
Serial.println("Arac Baslatildi");
}
void loop () 
{

  if(millis() - guncelleme_ms > gecen_zaman)
  {
   gecen_zaman = millis();
   oto_hareket_timer();
   komut_sayac = 0;
  }

if(oto_mod == 0)
{
  if(Serial.available() && komut_sayac < 1)
  {
    char veri = Serial.read();
     if(veri == 'S')
     {
       dur();
       komut_sayac++;
     }
    else
    {
       if(veri == 'F')
       {
        ileri();
        komut_sayac++;
       }
       else
       {
        if(veri == 'B')
        {
          geri();
          komut_sayac++;
        }
       }

          if(veri == 'R')
         {
          sag();
          komut_sayac++;
          }
          else
          {
             if(veri == 'L')
            {
              sol();
              komut_sayac++;
            }
          }
        

       /*
      else if(veri == 'K')
       {
        ileri_sag();
       }
      else if(veri == 'N')
       {
        ileri_sol();
       }
       else if(veri == 'P')
       {
         geri_sag();
       }
       else if(veri == 'O')
       {
        geri_sol();
       }
       */
       
        if(veri == 'M')
       {
        if(oto_mod == 0){oto_mod = 1;}
        else{oto_mod = 0;}
        komut_sayac++;
        dur();
       }
       else if(veri == 'H')
       {
        hiz_sag = Serial.parseInt();
        hiz_sol = hiz_sag;
        hiz_yaz(hiz_sol,hiz_sag);
        komut_sayac++;
       }
       else if(veri == 'E')
       {
        test_mod = !test_mod;
        komut_sayac++;
       }
        else if(oto_mod == 1)
        {
         bellek_degisim(veri);
        }
      
    } 
  }
}
  else
  {
    if(Serial.available())
    {
      char veri = Serial.read();
       if(veri == 'M')
       {
        if(oto_mod == 0){oto_mod = 1;}
        else{oto_mod = 0;}
        komut_sayac++;
        dur();
       }
    }
    else
    {
    oto_hareket();       
    }
  }
}
void dur()
{
  test("dur");
  digitalWrite(a1,LOW);
  digitalWrite(a2,LOW);
  digitalWrite(b1,LOW);
  digitalWrite(b2,LOW);
}
void ileri()
{
    test("ileri");

  analogWrite(a1,hiz_sag);
  analogWrite(a2,0);
  analogWrite(b1,hiz_sol);
  analogWrite(b2,0);
}
void geri()
{
    test("geri");

  analogWrite(a1,0);
  analogWrite(a2,hiz_sag);
  analogWrite(b1,0);
  analogWrite(b2,hiz_sol);
}
void ileri_sol()
{
  hiz_sol = hiz_sol*hassasiyet;
  analogWrite(a1,hiz_sag);
  analogWrite(a2,0);
  analogWrite(b1,hiz_sol);
  analogWrite(b2,0);
}
void ileri_sag()
{
  hiz_sag = hiz_sag*hassasiyet;
  analogWrite(a1,hiz_sag);
  analogWrite(a2,0);
  analogWrite(b1,hiz_sol);
  analogWrite(b2,0);
}
void sag()
{
    test("sag");

  analogWrite(a1,hiz_sag);
  analogWrite(a2,0);
  analogWrite(b1,0);
  analogWrite(b2,hiz_sol);
}
void sol()
{
    test("sol");

  analogWrite(a1,0);
  analogWrite(a2,hiz_sag);
  analogWrite(b1,hiz_sol);
  analogWrite(b2,0);
}
void geri_sol()
{
  
  hiz_sol = hiz_sol*hassasiyet;
  analogWrite(a1,0);
  analogWrite(a2,hiz_sag);
  analogWrite(b1,0);
  analogWrite(b2,hiz_sol);
}
void geri_sag()
{
    
  hiz_sag = hiz_sag*hassasiyet;
  analogWrite(a1,0);
  analogWrite(a2,hiz_sag);
  analogWrite(b1,0);
  analogWrite(b2,hiz_sol);
}
byte olcum()
{
digitalWrite(trigPin, LOW);
delayMicroseconds(3);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
long sure = pulseIn(echoPin, HIGH);
byte mesafe = ((sure-sure%2)/2) * 0.0343;
return mesafe; 
}
void oto_hareket()
{
  byte k = olcum();
  if(k <= min_aralik)
  {
    oto_sag = true;
  }
  else 
  {
    uint16_t a = (k-min_aralik)*50 + 50;
    if(a > 255){hiz_sag = map(a,0,a,0,255); hiz_sol = hiz_sag;}else{hiz_sol = a; hiz_sag = hiz_sol;}
    ileri();
  }
}
void oto_hareket_timer()
{
   if(oto_mod)
    {
      if(oto_sag)
      {
        hiz_yaz(hiz_sol,hiz_sag);
        byte k = olcum();
        hiz_sag = oto_geri_sag_hiz;
        hiz_sol = oto_geri_sag_hiz;
        geri_sag();       
          oto_mod_sayac++;
        if(oto_mod_sayac*guncelleme_ms > 3*geri_kacma_ms){dur(); oto_mod_sayac = 0; oto_sag = false; oto_mod = false; }
      }
    }
}

void ayar_ver()
{
  oto_mod = EEPROM.read(oto_mod_adr);
  min_aralik = EEPROM.read(min_aralik_adr);
  oto_geri_sag_hiz = EEPROM.read(oto_geri_sag_hiz_adr);
  guncelleme_ms = EEPROM.read(guncelleme_ms_adr);
  byte d = EEPROM.read(hassasiyet_adr);  
  byte j = EEPROM.read(geri_kacma_ms_adr);
  geri_kacma_ms = j*25;
  if(d == 1)
  {
    hassasiyet = 0,80;
  }
  else if(d == 2)
  {
    hassasiyet = 0,60;
  }
  else if(d == 3)
  {
    hassasiyet = 0,50;
  }
  else if(d == 4)
  {
    hassasiyet = 0,40;
  }
  else if(d == 5)
  {
    hassasiyet = 0,25;
  }
}
void ayar1(uint16_t a)//hassasiyet , aralik ve geri_kacma_ms ayar
{
  if(a <= 10)
  {
    if(a != 0){min_aralik = a; EEPROM.write(min_aralik_adr,min_aralik);}else{Serial.println("0 dan farkli bir deger gir");}
  }
  else if(a == 11)
  {
    hassasiyet = 0,80;
    EEPROM.write(hassasiyet_adr,1);
  }
  else if(a == 12)
  {
    hassasiyet = 0,60;
    EEPROM.write(hassasiyet_adr,2);
  }
  else if(a == 13)
  {
    hassasiyet = 0,50;
    EEPROM.write(hassasiyet_adr,3);
  }
  else if(a == 14)
  {
    hassasiyet = 0,40;
    EEPROM.write(hassasiyet_adr,4);
  }
  else if(a == 15)
  {
    hassasiyet = 0,25;
    EEPROM.write(hassasiyet_adr,5);
  }
  else if(a >= 500)
  {
    geri_kacma_ms = a;
    EEPROM.write(geri_kacma_ms_adr,a/25);
  }
}
void ayar2(uint16_t c) //oto geri sag hiz 
{
  oto_geri_sag_hiz = c;
  EEPROM.write(oto_geri_sag_hiz_adr,oto_geri_sag_hiz);
}
void ayar3(uint16_t x) //guncelleme ms
{
  guncelleme_ms = x;
  EEPROM.write(guncelleme_ms_adr,guncelleme_ms);
}
void test(String k)
{
  if(test_mod)
  {
    Serial.println(k);
  }
}
void test2(uint16_t y)
{
  if(test_mod)
  {
    Serial.println(y);
  }
}
void ayar_sifirla()
{
  EEPROM.write(oto_mod_adr, 0);
  EEPROM.write(min_aralik_adr, 6);
  EEPROM.write(geri_kacma_ms_adr, 60);
  EEPROM.write(oto_geri_sag_hiz_adr, 180);
  EEPROM.write(hassasiyet_adr, 2);
  EEPROM.write(guncelleme_ms_adr, 50);
}
void hiz_yaz(byte l,byte r)
{
  Serial.print("*Y" + String(l) + "*");
  Serial.print("*Z" + String(r) + "*");
  Serial.println("");
}
void bellek_degisim(char veri)
{
   if(veri == 'A')
    {
      uint16_t aa = Serial.parseInt();
      ayar1(aa); //0-10=aralik , 11-15=hassasiyet , 500-? = geri kaçma ms
      komut_sayac++;
    }
    else if(veri == 'W')
    {
      uint16_t aa = Serial.parseInt();
      ayar2(aa); //0-255 = oto geri sag hiz
      komut_sayac++;
    }
    else if(veri == 'Q')
    {
      uint16_t aa = Serial.parseInt();
      ayar3(aa); //10-100 = guncelleme_ms (10 ve katları)
      komut_sayac++;
    }
    else if(veri == 'G')
    {
      ayar_sifirla();
      komut_sayac++;
    }
}
