
#include <EEPROM.h>
#include "avr/wdt.h"
uint16_t guncelleme_ms = 25; //makine hızı için fabrika ayarı
double hassasiyet = 0.75; //ileri sag-sol ve geri sag-sol dönüşlerinde keskinlik fabrika ayarı
uint16_t test_izin = 1; //test modu için fabrika ayarı
uint16_t oto_mod = 0; //sensör modu
const uint16_t guncelleme_ms_adr = 1;
const uint16_t hassasiyet_adr = 2;
const uint16_t test_izin_adr = 3;
const uint16_t oto_mod_adr = 4;
const uint16_t delay_sure1 = 25;
const uint16_t trigPin = 4;
const uint16_t echoPin = 2;
const uint16_t a1 = 3;
const uint16_t a2 = 5;
const uint16_t b1 = 6;
const uint16_t b2 = 9;
bool baglanti = true;
uint16_t afk_sayac = 0;
uint16_t seri_kapa_sayac = 0;
unsigned long gecen_zaman = 0;
uint16_t hiz = 0;
uint16_t hiz_sol = 0;
uint16_t hiz_sag = 0;
uint16_t ks [] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const uint16_t ks_say = 24;
const double hssytS[] = {0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,0.95};
uint16_t sayac1 = 0;
bool afk_modu = true;
 uint8_t min_aralik = 5;
 uint16_t kacma_suresi = 80;
bool mod_sag = false;
uint16_t guncelleme_ms_f = 25; //makine hızı için fabrika ayarı
int test_izin_f = 1; //test modu için fabrika ayarı
int oto_mod_f = 0; //sensör modu
 uint8_t min_aralik_f = 5;
 uint16_t kacma_suresi_f = 80;
 bool afk_modu_f = true;

void setup () 
{
Serial.begin(9600);
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(a1, OUTPUT);
pinMode(a2, OUTPUT);
pinMode(b1, OUTPUT);
pinMode(b2, OUTPUT);
Serial.println("\n");
yavas_yaz("____________________");
yavas_yaz("Ayarlar Yukleniyor...");
ayar_yukle();
yavas_yaz("Arac Baslatildi");
yavas_yaz("____________________");
Serial.println("\n");
}
void loop () 
{
  if(Serial.available())
  {
    bt_sor(Serial.read());      
  } 

}

void bt_sor(char veri)
{
if(veri == 'A')
      {
        bt_ayar(Serial.parseInt());
      }
  else
  {
   if(veri == 'S' && ks[0] == 0)
     {
       dur(); kres(0);
     }
    else
    {
      if(veri == 'F' && ks[1] == 0)
       {
        ileri(); kres(1);
       }
      else if(veri == 'B' && ks[2] == 0)
       {
        geri(); kres(2);
       }
       else if(veri == 'R' && ks[3] == 0)
       {
         sag(); kres(3);
       }
       else if(veri == 'L' && ks[4] == 0)
       {
        sol(); kres(4);
       }
      else if(veri == 'K' && ks[5] == 0)
       {
        ileri_sag(); kres(5);
       }
      else if(veri == 'N' && ks[6] == 0)
       {
        ileri_sol(); kres(6);
       }
       else if(veri == 'P' && ks[7] == 0)
       {
         geri_sag(); kres(7);
       }
       else if(veri == 'O' && ks[8] == 0)
       {
        geri_sol(); kres(8);
       }
    }
  }
}
void bt_ayar(uint16_t w)
{
          if(w < 256){hiz = w; hiz_sag = hiz; hiz_sol = hiz_sag; geri_bildiri(hiz_sol,hiz_sag);}
          else if(w == 260){ayar_al(w,10);}//1 1
          else if(w == 261){ayar_al(w,25);}//1 2
          else if(w == 262){ayar_al(w,50);}//1 3

          else if(w == 263){ayar_al(w,1);}//2 1
          else if(w == 264){ayar_al(w,2);}//2 2
          else if(w == 265){ayar_al(w,3);}//2 3
          else if(w == 266){ayar_al(w,4);}//2 4
          else if(w == 267){ayar_al(w,5);}//2 5
          else if(w == 268){ayar_al(w,6);}//2 6
          else if(w == 269){ayar_al(w,7);}//2 7
          else if(w == 270){ayar_al(w,8);}//2 8
          else if(w == 271){ayar_al(w,9);}//2 9
          else if(w == 272){ayar_al(w,10);}//2 10

          else if(w == 274){ayar_al(w,0);}//3 1
          else if(w == 275){ayar_al(w,1);}//3 2

          else if(w == 276){ayar_al(w,0);}//4 1
          else if(w == 277){ayar_al(w,1);}//4 2

          else if(w == 299){ayar_sifirla();} // 5 1
          else if(w == 300){kart_resetle();} // 6 1
}
void geri_bildiri(uint16_t l,uint16_t r)
{
  Serial.print("*Y" + String(l) + "*");
  Serial.print("*Z" + String(r) + "*");
}
void geri_bildiri2(uint16_t p)
{
  Serial.print("*C" + String(p) + "*");
  test("\n");
}
void kres(uint16_t a)
{
  for(int y=0; y<ks_say; y++)
  {
    if(y == a){ks[a] = 1;}else{ks[y] = 0;}
  }
}
void test(String a)
{
  if(test_izin)
  {
    Serial.println(a);
  }
}
void test2(double a)
{
  if(test_izin)
  {
    Serial.println(a);
  }
}
void test3(uint16_t a)
{
  if(test_izin)
  {
    Serial.println(a);
  }
}
void yavas_yaz(String s)
{
  uint16_t uzunluk = s.length();
  Serial.println();
  for(int y=0; y<uzunluk; y++)
  {
    char harf = s.charAt(y);
    Serial.print(harf);
    delay(delay_sure1);
  }
}

void ayar_al(uint16_t ayar_no,uint16_t deger)
{
if(ayar_no == 260){EEPROM.write(guncelleme_ms_adr, deger); guncelleme_ms = deger; test3(deger);} // 1 1
else if(ayar_no == 261){EEPROM.write(guncelleme_ms_adr, deger); guncelleme_ms = deger; test3(deger);} // 1 2
else if(ayar_no == 262){EEPROM.write(guncelleme_ms_adr, deger); guncelleme_ms = deger; test3(deger);} // 1 3

else if(ayar_no == 263){EEPROM.write(hassasiyet_adr, deger); hassasiyet = hssytS[1]; test2(hassasiyet);} // 2 1
else if(ayar_no == 264){EEPROM.write(hassasiyet_adr, deger); hassasiyet = hssytS[2]; test2(hassasiyet);} // 2 2
else if(ayar_no == 265){EEPROM.write(hassasiyet_adr, deger); hassasiyet = hssytS[3]; test2(hassasiyet);} // 2 3
else if(ayar_no == 266){EEPROM.write(hassasiyet_adr, deger); hassasiyet = hssytS[4]; test2(hassasiyet);} // 2 4
else if(ayar_no == 267){EEPROM.write(hassasiyet_adr, deger); hassasiyet = hssytS[5]; test2(hassasiyet);} // 2 5
else if(ayar_no == 268){EEPROM.write(hassasiyet_adr, deger); hassasiyet = hssytS[6]; test2(hassasiyet);} // 2 6
else if(ayar_no == 269){EEPROM.write(hassasiyet_adr, deger); hassasiyet = hssytS[7]; test2(hassasiyet);} // 2 7
else if(ayar_no == 270){EEPROM.write(hassasiyet_adr, deger); hassasiyet = hssytS[8]; test2(hassasiyet);} // 2 8
else if(ayar_no == 271){EEPROM.write(hassasiyet_adr, deger); hassasiyet = hssytS[9]; test2(hassasiyet);} // 2 9
else if(ayar_no == 272){EEPROM.write(hassasiyet_adr, deger); hassasiyet = hssytS[10]; test2(hassasiyet);} // 2 10

else if(ayar_no == 274){EEPROM.write(test_izin_adr, deger); test_izin = false; test("test kapali");} // 3 1
else if(ayar_no == 275){EEPROM.write(test_izin_adr, deger); test_izin = true; test("test acik");} // 3 2

else if(ayar_no == 276){EEPROM.write(oto_mod_adr, deger); oto_mod = false; test("oto mod kapali");} // 4 1
else if(ayar_no == 277){EEPROM.write(oto_mod_adr, deger); oto_mod = true; test("oto mod acik");} // 4 2
}
void ayar_yukle()
{
  guncelleme_ms = EEPROM.read(guncelleme_ms_adr);
  yavas_yaz("makine yenileme(ms) = "); Serial.print(guncelleme_ms);
  uint16_t e2 = EEPROM.read(hassasiyet_adr);
  for(int y=1;y<10;y++)
  {
    if(y == e2){hassasiyet = hssytS[y]; yavas_yaz("donus hassasiyeti = "); Serial.print(hassasiyet);}
  }  
  test_izin = EEPROM.read(test_izin_adr);
  yavas_yaz("test izni = "); Serial.print(test_izin);
  oto_mod = EEPROM.read(oto_mod_adr);
  yavas_yaz("sensor modu = "); Serial.print(oto_mod); 
}
void ayar_sifirla()
{
EEPROM.write(guncelleme_ms_adr,guncelleme_ms_f);
EEPROM.write(hassasiyet_adr,6);
EEPROM.write(test_izin_adr,test_izin_f);
EEPROM.write(oto_mod_adr,oto_mod_f);
Serial.println("sifirlandi");
}
void kart_resetle(){wdt_enable(WDTO_15MS);}

void oto_surus()
{
  volatile uint16_t K = olcum();
  if(millis() - 2 > gecen_zaman)
  {
    gecen_zaman = millis();
    if(mod_sag)
    {
      if(sayac1*guncelleme_ms >= kacma_suresi){mod_sag = false; sayac1 = 0;}else{sayac1++; geri_sag();}
    }
  }
  if(K > min_aralik && !mod_sag)
  {
    if(mod_sag){mod_sag = false;}ileri();
  }
  else
  {
    if(!mod_sag){mod_sag = true; dur();}
  }
}

uint16_t olcum()
{
digitalWrite(trigPin, LOW);
delayMicroseconds(3);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
uint16_t sure = pulseIn(echoPin, HIGH);
uint16_t mesafe = ((sure-sure%2)/2) * 0.0343;
return mesafe; 
}
uint16_t olcum2()
{
  uint16_t m;
if(Serial.available())
{
  if(Serial.read() == 'k')
  {
    m = Serial.parseInt();
  }
}
return m; 
}

void dur()
{
  hiz_sol = 0; 
  hiz_sag = 0;
  digitalWrite(a1,LOW);
  digitalWrite(a2,LOW);
  digitalWrite(b1,LOW);
  digitalWrite(b2,LOW);
  geri_bildiri(hiz_sol,hiz_sag);
} 
void ileri()
{
  hiz_sol = hiz; 
  hiz_sag = hiz_sol;
  analogWrite(a1,hiz_sag);
  analogWrite(a2,0);
  analogWrite(b1,hiz_sol);
  analogWrite(b2,0);
  geri_bildiri(hiz_sol,hiz_sag);
}
void geri()
{
  hiz_sol = hiz; 
  hiz_sag = hiz_sol;
  analogWrite(a1,0);
  analogWrite(a2,hiz_sag);
  analogWrite(b1,0);
  analogWrite(b2,hiz_sol);
  geri_bildiri(hiz_sol,hiz_sag);
}
void ileri_sol()
{
  hiz_sag = hiz; 
  hiz_sol = hiz_sag*hassasiyet; 
  analogWrite(a1,hiz_sag);
  analogWrite(a2,0);
  analogWrite(b1,hiz_sol);
  analogWrite(b2,0);
  geri_bildiri(hiz_sol,hiz_sag);
}
void ileri_sag()
{
  hiz_sol = hiz; 
  hiz_sag = hiz_sol*hassasiyet;
  analogWrite(a1,hiz_sag);
  analogWrite(a2,0);
  analogWrite(b1,hiz_sol);
  analogWrite(b2,0);
  geri_bildiri(hiz_sol,hiz_sag);
}
void sag()
{
  hiz_sol = hiz; 
  hiz_sag = hiz_sol;
  analogWrite(a1,hiz_sag);
  analogWrite(a2,0);
  analogWrite(b1,0);
  analogWrite(b2,hiz_sol);
  geri_bildiri(hiz_sol,hiz_sag);
}
void sol()
{
  hiz_sol = hiz; 
  hiz_sag = hiz_sol;
  analogWrite(a1,0);
  analogWrite(a2,hiz_sag);
  analogWrite(b1,hiz_sol);
  analogWrite(b2,0);
  geri_bildiri(hiz_sol,hiz_sag);
}
void geri_sol()
{
  hiz_sag = hiz; 
  hiz_sol = hiz_sag*hassasiyet;
  analogWrite(a1,0);
  analogWrite(a2,hiz_sag);
  analogWrite(b1,0);
  analogWrite(b2,hiz_sol);
  geri_bildiri(hiz_sol,hiz_sag);
}
void geri_sag()
{
  hiz_sol = hiz; 
  hiz_sag = hiz_sol*hassasiyet;
  analogWrite(a1,0);
  analogWrite(a2,hiz_sag);
  analogWrite(b1,0);
  analogWrite(b2,hiz_sol);
  geri_bildiri(hiz_sol,hiz_sag);
}
void afk_mod_kontrol()
{
  if(afk_modu)
  {
  if(millis() - 1000 > gecen_zaman)
    {
      gecen_zaman = millis();
      if(baglanti){afk_sayac++; test3(5-afk_sayac);}
      if(afk_sayac >= 5)
      {
      if(baglanti){baglanti = false; afk_sayac = 0; Serial.println("afk modu acildi,cikmak icin komut gonderin"); Serial.println("\n"); dur(); kres(24);}       
      }
    }
  }
}
