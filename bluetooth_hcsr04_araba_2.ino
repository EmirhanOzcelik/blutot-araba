const byte a1 = 3;
const byte a2 = 5;
const byte b1 = 6;
const byte b2 = 9;

unsigned long gecen_zaman = 0;
uint16_t guncelleme_ms = 20;  
byte komut_sayac = 0;

byte hiz_sol = 0;
byte hiz_sag = 0;
long hassasiyet = 0.75;// ileri sag-sol ve geri sag-sol dönüşlerinde keskinlik ayarı

void setup () 
{
Serial.begin(9600);
pinMode(a1, OUTPUT);
pinMode(a2, OUTPUT);
pinMode(b1, OUTPUT);
pinMode(b2, OUTPUT);
Serial.println("Ayarlar Yukleniyor...");
Serial.println("Arac Baslatildi");
}
void loop () 
{

  if(millis() - guncelleme_ms > gecen_zaman)
  {
   gecen_zaman = millis();
   komut_sayac = 0;
  }

  else if(Serial.available() > 0 && komut_sayac < 2)
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
       }
      else if(veri == 'B')
       {
        geri();
       }
       else if(veri == 'R')
       {
         sag();
       }
       else if(veri == 'L')
       {
        sol();
       }
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
       else if(veri == 'H')
       {
        hiz_sag = Serial.parseInt();
        hiz_sol = hiz_sag;
        hiz_yaz(hiz_sol,hiz_sag);
        komut_sayac++;
       }
    }
  } 
}


void dur()
{
  digitalWrite(a1,LOW);
  digitalWrite(a2,LOW);
  digitalWrite(b1,LOW);
  digitalWrite(b2,LOW);
}
void ileri()
{

  analogWrite(a1,hiz_sag);
  analogWrite(a2,0);
  analogWrite(b1,hiz_sol);
  analogWrite(b2,0);
}
void geri()
{

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

  analogWrite(a1,hiz_sag);
  analogWrite(a2,0);
  analogWrite(b1,0);
  analogWrite(b2,hiz_sol);
}
void sol()
{

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

void hiz_yaz(byte l,byte r)
{
  Serial.print("*Y" + String(l) + "*");
  Serial.print("*Z" + String(r) + "*");
  Serial.println("");
}
