/*                    */

#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <Pixy2.h>

Pixy2 pixy; // This is the main Pixy object

#include "renk_algilama.h"

int mesafe = 400;
long time_mesafe = 0;
long time_hareket = 0;
long time_top_at = 0;
long time_arka_fon = 0;

bool oyun_basla = false;
bool rgb_blink_yap = true;
bool hareket_var = false;
bool turkce_eng = TURKCE;
bool etkinlik = true;
bool ses_deg = true;
bool arka_fon = true;

byte hareket_say = 0;
int oyun_say = -1;
int hata_say = 0;
byte random_renk_at = 0;
byte renk_at_dizi[4];
byte rgb_sig_sirali[7];
byte random_byte_dizi[10];
byte btn_gir_say = 0;
byte mod_say = 0;
byte renk_at_say = 0;
byte ses_rnd = 0;

uint16_t mp3_ses = 10;

byte calisma_modu = SERBEST_MOD;
//byte calisma_modu = OYUN_MOD;

SoftwareSerial mySerial(8, 7); // RX, TX   ARDU ya  göre olmalı...

void setup()
{
  Serial.begin(9600);
  //while(!Serial.available());   //leonarda seri port için
  mySerial.begin(9600);
  mp3_set_serial(mySerial); //set softwareSerial for DFPlayer-mini mp3 module
  delay(10);                //wait 1ms for mp3 module to set volume

  Serial.println("basla...");

  delay(1000); //bekleme olmazsa ilk baslangıc muzik çalınmıyor.
  mp3_ses = EEPROM.read(EE_ADR_MP3_SES);
  mp3_set_volume(mp3_ses);
  delay(100);
  mp3_stop();
  delay(100);
  mp3_play(MP3_BATTERY);
  //delay(1000);

  pinMode(ECHO1, INPUT);
  pinMode(TRIG1, OUTPUT);
  pinMode(BUTON_PIN, INPUT);
  pinMode(BUTON_SES1_PIN, INPUT);
  pinMode(BUTON_SES2_PIN, INPUT);

  rgb_blink();
  pixy.init();

  renk_at_dizi[3] = EEPROM.read(EE_ADR_RND);
  random_byte_dizi[9] = EEPROM.read(EE_ADR_RND2);

  if (EEPROM.read(EE_ADR_SES_DIL) == 0xFF)
    turkce_eng = TURKCE;
  else
    turkce_eng = EEPROM.read(EE_ADR_SES_DIL);

  if (EEPROM.read(EE_ADR_CALISMA_MOD) == 0xFF)
    calisma_modu = SERBEST_MOD;
  else
    calisma_modu = EEPROM.read(EE_ADR_CALISMA_MOD);

  if (calisma_modu < 200 && calisma_modu > 205)
    calisma_modu = SERBEST_MOD;

  mod_seslendir(calisma_modu);
  delay(3000); //kamera için bekleme süresi...
  ses_rnd = random_byte(0, 9);
  time_mesafe = millis();
  time_arka_fon = millis();
} //setup

//
void loop()
{

  // pixy.ccc.getBlocks();

  //if (!digitalRead(BUTON_PIN))
  if (digitalRead(BUTON_PIN))
  {
    delay(100);
    btn_gir_say++;
    Serial.println(btn_gir_say);
    if (btn_gir_say > 5)
    {
      calisma_modu = mod_secim();
      mod_seslendir(calisma_modu);
      btn_gir_say = 0;
      ses_rnd = random_byte(0, 9);
    }
  }

  mp3_ses_deg();

  if (calisma_modu == SERBEST_MOD)
  {
    serbest_mod();
  }
  if (calisma_modu == OYUN_MOD)
  {
    sensorlu_oyun_modu();
  }
  if (calisma_modu == ETKINLIK_MOD1)
  {
    etkinlik_oyun_1();
  }
  if (calisma_modu == ETKINLIK_MOD2)
  {
    etkinlik_oyun_2();
  }

} //loop
void mp3_ses_deg()
{
  if (digitalRead(BUTON_SES1_PIN))
  {
    renk_rgb(0, 0, 255);
    delay(250);
    mp3_ses++;
    if (mp3_ses > 29)
      mp3_ses = 29;
    Serial.print("ses:");
    Serial.println(mp3_ses);
  }
  if (digitalRead(BUTON_SES2_PIN))
  {
    renk_rgb(0, 0, 255);
    delay(250);
    mp3_ses--;
    if (mp3_ses < 1)
      mp3_ses = 1;
    Serial.print("ses:");
    Serial.println(mp3_ses);
  }
  EEPROM.write(EE_ADR_MP3_SES, mp3_ses);
  mp3_set_volume(mp3_ses);
  delay(100);
  renk_rgb(0, 0, 0);
}
void mod_seslendir(byte mod)
{
  if (mod == SERBEST_MOD)
  {
    if (turkce_eng == TURKCE)
      mp3_time_play(MP3_SERBEST_MOD, SES_SURE);
    else
      mp3_time_play(MP3_FREE_MOD, SES_SURE);
  }

  if (mod == OYUN_MOD)
  {
    if (turkce_eng == TURKCE)
      mp3_time_play(MP3_RENKLER_OYUNU, SES_SURE);
    else
      mp3_time_play(MP3_PLAY_COLORS, SES_SURE);
  }

  if (mod == ETKINLIK_MOD1)
  {

    if (turkce_eng == TURKCE)
      mp3_time_play(MP3_ETKINLIK1, SES_SURE);
    else
      mp3_time_play(MP3_ACTIVITY1, SES_SURE);
  }

  if (mod == ETKINLIK_MOD2)
  {
    if (turkce_eng == TURKCE)
      mp3_time_play(MP3_ETKINLIK2, SES_SURE);
    else
      mp3_time_play(MP3_ACTIVITY2, SES_SURE);
  }

} //

void rgby_say_seslendir(int r_say, byte renk)
{
  int birler = 0;
  birler = r_say % 10;

  renk_seslendir(renk);

  if (turkce_eng == TURKCE)
  {

    if (r_say < 10)
    {
      mp3_time_play(50 + r_say, SSS);
    }
    if (r_say == 10)
      mp3_time_play(MP3_ON, SSS);
    if (r_say > 10 && r_say < 20)
    {
      mp3_time_play(MP3_ON, SSS);
      mp3_time_play(50 + birler, SSS);
    }
    if (r_say == 20)
      mp3_time_play(MP3_YIRMI, SSS);
    if (r_say > 20 && r_say < 30)
    {
      mp3_time_play(MP3_YIRMI, SSS);
      mp3_time_play(50 + birler, SSS);
    }
    if (r_say == 30)
      mp3_time_play(MP3_OTUZ, SSS);
    if (r_say > 30 && r_say < 40)
    {
      mp3_time_play(MP3_OTUZ, SSS);
      mp3_time_play(50 + birler, SSS);
    }
    if (r_say == 40)
      mp3_time_play(MP3_KIRK, SSS);
    if (r_say > 40 && r_say < 50)
    {
      mp3_time_play(MP3_KIRK, SSS);
      mp3_time_play(50 + birler, SSS);
    }
    if (r_say == 50)
      mp3_time_play(MP3_ELLI, SSS);
    if (r_say > 40 && r_say < 50)
    {
      mp3_time_play(MP3_ELLI, SSS);
      mp3_time_play(50 + birler, SSS);
    }
  }
  else
  {
    if (r_say < 20)
    {
      mp3_time_play(80 + r_say, SES_SURE);
    }
    if (r_say == 20)
      mp3_time_play(MP3_ENG_20, SSS);
    if (r_say > 20 && r_say < 30)
    {
      mp3_time_play(MP3_ENG_20, SSS);
      mp3_time_play(80 + birler, SSS);
    }
    if (r_say == 30)
      mp3_time_play(MP3_ENG_30, SSS);
    if (r_say > 30 && r_say < 40)
    {
      mp3_time_play(MP3_ENG_30, SSS);
      mp3_time_play(80 + birler, SSS);
    }
    if (r_say == 40)
      mp3_time_play(MP3_ENG_40, SSS);
    if (r_say > 40 && r_say < 50)
    {
      mp3_time_play(MP3_ENG_40, SSS);
      mp3_time_play(80 + birler, SSS);
    }
    if (r_say == 50)
      mp3_time_play(MP3_ENG_50, SSS);
    if (r_say > 50 && r_say < 60)
    {
      mp3_time_play(MP3_ENG_50, SSS);
      mp3_time_play(80 + birler, SSS);
    }
  }
}

//////
void (*resetFunc)(void) = 0; //Arduino software reset için gerekli fonksiyon.
//////

byte etkinlik_oyun_2()
{
  int i = 0;
  int oyun_sure = 0;
  btn_gir_say = 0;
  int mavi_say = 0, kirmizi_say = 0, yesil_say = 0, sari_say = 0;
  long time_rnd = 0;
  bool oyun_basla = false;
  arka_fon = true;
  time_arka_fon = millis();

  while (etkinlik)
  {
    mp3_ses_deg();
    //if (!digitalRead(BUTON_PIN))
    if (digitalRead(BUTON_PIN))
    {
      delay(300);
      btn_gir_say++;
      //Serial.println("Etkinlik");
      if (btn_gir_say < 2)
      {
        random_renk_at = random_renk();
        // Serial.println(random_renk_at);

        if ((mp3_ses + 5) > 30)
          mp3_set_volume(25);
        else
          mp3_set_volume(mp3_ses + 5);
        delay(100);

        if (turkce_eng == TURKCE)
          mp3_time_play(random_renk_at, SES_SURE);
        else
          mp3_time_play(34 + random_renk_at, SES_SURE);
        oyun_basla = true;
        time_top_at = millis();
        time_rnd = millis();
      }
      else if (btn_gir_say > 5)
      {
        mp3_time_play(MP3_BATTERY, SES_SURE);
        btn_gir_say = 0;
        etkinlik = false;
        delay(3000);
        calisma_modu = SERBEST_MOD;
        if (turkce_eng == TURKCE)
          mp3_time_play(MP3_SERBEST_MOD, SES_SURE);
        else
          mp3_time_play(MP3_FREE_MOD, SES_SURE);
        break;
      }
    }

    if ((millis() - time_rnd > (random_renk_at * 1000)) && oyun_basla) //yeni renk
    {
      random_renk_at = random_renk();
      if (turkce_eng == TURKCE)
        mp3_time_play(random_renk_at, SES_SURE);
      else
        mp3_time_play(34 + random_renk_at, SES_SURE);

      time_rnd = millis();
    }

    pixy.ccc.getBlocks();

    if (pixy.ccc.numBlocks)
    {
      for (i = 0; i < pixy.ccc.numBlocks; i++)
      {
        Serial.print("  block ");
        Serial.print(i);
        Serial.print(": ");
        pixy.ccc.blocks[i].print();

        rgb_led_yak(pixy.ccc.blocks[i].m_signature);

        if (pixy.ccc.blocks[i].m_signature == (random_renk_at - 7))
        {
          mp3_time_play(MP3_ALKIS, SSS);

          switch (pixy.ccc.blocks[i].m_signature)
          {
          case KIRMIZI_TOP:
            kirmizi_say++;
            break;
          case YESIL_TOP:
            yesil_say++;
            break;
          case MAVI_TOP:
            mavi_say++;
            break;
          case SARI_TOP:
            sari_say++;
            break;
          }
        }
        else
        {
          mp3_time_play(MP3_ALERT, SES_SURE);
        }
        renk_rgb(0, 0, 0); //tum ledleri söndür....
      }                    //for
    }

    if ((millis() - time_top_at > 60000) && oyun_basla) //oyun max 1 dak. sürer.
    {

      //sonuc soylenmeli
      Serial.print("R:");
      Serial.println(kirmizi_say);
      Serial.print("G:");
      Serial.println(yesil_say);
      Serial.print("B:");
      Serial.println(mavi_say);
      Serial.print("Y:");
      Serial.println(sari_say);

      mp3_time_play(MP3_BATTERY, SES_SURE);

      if (kirmizi_say > 0)
        rgby_say_seslendir(kirmizi_say, KIRMIZI_TOP);
      delay(500);
      if (yesil_say > 0)
        rgby_say_seslendir(yesil_say, YESIL_TOP);
      delay(500);
      if (mavi_say > 0)
        rgby_say_seslendir(mavi_say, MAVI_TOP);
      delay(500);
      if (sari_say > 0)
        rgby_say_seslendir(sari_say, SARI_TOP);
      delay(500);

      //mp3_time_play(MP3_BATTERY, SES_SURE);
      btn_gir_say = 0;

      resetFunc(); //ardu sofware reset
      //return;
    }

    if ((millis() - time_arka_fon > 60000) && !oyun_basla)
    {
      if (arka_fon)
      {
        arka_fon = false;
        mp3_set_volume(mp3_ses);
        delay(100);
        mp3_time_play(MP3_ARKA_FON, SES_SURE);
      }
    }
    if ((millis() - time_arka_fon > 220000) && !oyun_basla)
    {

      if (!arka_fon)
        mp3_time_play(MP3_ARKA_FON, SES_SURE);

      time_arka_fon = millis();
    }
  } //while

} //

byte etkinlik_oyun_1()
{
  int i = 0;
  int oyun_sure = 0;
  byte birler = 0;
  btn_gir_say = 0;
  arka_fon = true;
  time_top_at = time_arka_fon = millis();

  while (etkinlik)
  {
    pixy.ccc.getBlocks();
    mp3_ses_deg();
    //if (!digitalRead(BUTON_PIN))
    if (digitalRead(BUTON_PIN))
    {
      delay(300);
      btn_gir_say++;
      //Serial.println("Etkinlik");
      if (btn_gir_say < 2)
      {
        random_renk_at = random_renk();
        // Serial.println(random_renk_at);
        if ((mp3_ses + 5) > 30)
          mp3_set_volume(25);
        else
          mp3_set_volume(mp3_ses + 5);
        delay(100);

        if (turkce_eng == TURKCE)
          mp3_time_play(random_renk_at, SES_SURE);
        else
          mp3_time_play(34 + random_renk_at, SES_SURE);

        time_top_at = millis();
      }
      else if (btn_gir_say > 5)
      {
        mp3_time_play(MP3_BATTERY, SES_SURE);
        btn_gir_say = 0;
        etkinlik = false;
        delay(3000);
        calisma_modu = SERBEST_MOD;
        if (turkce_eng == TURKCE)
          mp3_time_play(MP3_SERBEST_MOD, SES_SURE);
        else
          mp3_time_play(MP3_FREE_MOD, SES_SURE);
        break;
      }
    }

    if (millis() - time_top_at > 60000 && btn_gir_say == 1) //oyun max 1 dak. sürer.
    {
      mp3_time_play(MP3_BATTERY, SES_SURE);
      resetFunc();
    }

    if ((millis() - time_arka_fon > 60000))
    {
      if (arka_fon)
      {
        arka_fon = false;
        mp3_set_volume(mp3_ses);
        delay(100);
        mp3_time_play(MP3_ARKA_FON, SES_SURE);
      }
    }
    if ((millis() - time_arka_fon > 220000))
    {

      if (!arka_fon)
        mp3_time_play(MP3_ARKA_FON, SES_SURE);

      time_arka_fon = millis();
    }

    if (pixy.ccc.numBlocks)
    {
      for (i = 0; i < pixy.ccc.numBlocks; i++)
      {
        Serial.print("  block ");
        Serial.print(i);
        Serial.print(": ");
        pixy.ccc.blocks[i].print();

        rgb_led_yak(pixy.ccc.blocks[i].m_signature);

        //pixy.ccc.blocks[i].print();
        if (pixy.ccc.blocks[i].m_signature == random_renk_at - 7)
        {
          mp3_time_play(MP3_ALKIS, SES_SURE * 2);

          oyun_sure = int((millis() - time_top_at) / 1000);
          Serial.print("Osure:");
          Serial.println(oyun_sure);
          birler = oyun_sure % 10;

          if (turkce_eng == TURKCE)
          {
            if (oyun_sure < 10)
            {
              mp3_time_play(50 + oyun_sure, SSS);
            }
            if (oyun_sure == 10)
              mp3_time_play(MP3_ON, SSS);
            if (oyun_sure > 10 && oyun_sure < 20)
            {
              mp3_time_play(MP3_ON, SSS);
              mp3_time_play(50 + birler, SSS);
            }

            if (oyun_sure == 20)
              mp3_time_play(MP3_YIRMI, SSS);
            if (oyun_sure > 20 && oyun_sure < 30)
            {
              mp3_time_play(MP3_YIRMI, SSS);
              mp3_time_play(50 + birler, SSS);
            }

            if (oyun_sure == 30)
              mp3_time_play(MP3_OTUZ, SES_SURE / 3);
            if (oyun_sure > 30 && oyun_sure < 40)
            {
              mp3_time_play(MP3_OTUZ, SSS);
              mp3_time_play(50 + birler, SSS);
            }

            if (oyun_sure == 40)
              mp3_time_play(MP3_KIRK, SSS);
            if (oyun_sure > 40 && oyun_sure < 50)
            {
              mp3_time_play(MP3_KIRK, SSS);
              mp3_time_play(50 + birler, SSS);
            }

            if (oyun_sure == 50)
              mp3_time_play(MP3_ELLI, SSS);
            if (oyun_sure > 50 && oyun_sure < 60)
            {
              mp3_time_play(MP3_ELLI, SSS);
              mp3_time_play(50 + birler, SSS);
            }
            mp3_time_play(MP3_SANIYE, SES_SURE);
          }
          else //ingilizce mod...
          {
            if (oyun_sure < 20)
            {
              mp3_time_play(80 + oyun_sure, SES_SURE);
            }
            if (oyun_sure == 20)
              mp3_time_play(MP3_ENG_20, SSS);
            if (oyun_sure > 20 && oyun_sure < 30)
            {
              mp3_time_play(MP3_ENG_20, SSS);
              mp3_time_play(80 + birler, SSS);
            }
            if (oyun_sure == 30)
              mp3_time_play(MP3_ENG_30, SSS);
            if (oyun_sure > 30 && oyun_sure < 40)
            {
              mp3_time_play(MP3_ENG_30, SSS);
              mp3_time_play(80 + birler, SSS);
            }
            if (oyun_sure == 40)
              mp3_time_play(MP3_ENG_40, SSS);
            if (oyun_sure > 40 && oyun_sure < 50)
            {
              mp3_time_play(MP3_ENG_40, SSS);
              mp3_time_play(80 + birler, SSS);
            }
            if (oyun_sure == 50)
              mp3_time_play(MP3_ENG_50, SSS);
            if (oyun_sure > 50 && oyun_sure < 60)
            {
              mp3_time_play(MP3_ENG_50, SSS);
              mp3_time_play(80 + birler, SSS);
            }
            mp3_time_play(MP3_SECOND, SES_SURE);
          }                  //
          renk_rgb(0, 0, 0); //tum ledleri söndür....
          //resetFunc();
          return 0;
        }
        else
        {
          if(btn_gir_say==1) mp3_time_play(MP3_ALERT, SES_SURE);
        }
        renk_rgb(0, 0, 0); //tum ledleri söndür....
      }                    //for

    } //if (pixy.ccc.numBlocks)
  }
}

void mp3_time_play(uint16_t cal, int sure)
{
  mp3_stop();
  delay(100);
  mp3_play(cal);
  delay(sure);
}

byte mod_secim()
{
  byte son_mod = calisma_modu;
  btn_gir_say = 0;
  mod_say = SERBEST_MOD - 1;
  bool ses = false;
  long time_kaydet = 0xFFFFFFFF;

  if ((mp3_ses + 5) > 30)
    mp3_set_volume(25);
  else
    mp3_set_volume(mp3_ses + 5);
  delay(100);

  if (turkce_eng == TURKCE)
    mp3_time_play(MP3_MOD_SECIM, SES_SURE);
  else
    mp3_time_play(MP3_CHANGE_OP, SES_SURE);

  rgb_blink();
  delay(1000);

  while (true)
  {
    delay(100);
    //if (!digitalRead(BUTON_PIN))
    if (digitalRead(BUTON_PIN))
    {
      delay(500);
      mod_say++;
      ses = true;
      time_kaydet = millis();
    }
    else
      ses = false;

    if (mod_say > TURKCE_MOD)
      mod_say = SERBEST_MOD;

    if (ses)
    {
      switch (mod_say)
      {
      case SERBEST_MOD:
        if (turkce_eng == TURKCE)
          mp3_time_play(MP3_SERBEST_MOD, SES_SURE);
        else
          mp3_time_play(MP3_FREE_MOD, SES_SURE);
        break;

      case OYUN_MOD:
        if (turkce_eng == TURKCE)
          mp3_time_play(MP3_RENKLER_OYUNU, SES_SURE);
        else
          mp3_time_play(MP3_PLAY_COLORS, SES_SURE);
        break;

      case ETKINLIK_MOD1:
        if (turkce_eng == TURKCE)
          mp3_time_play(MP3_ETKINLIK1, SES_SURE);
        else
          mp3_time_play(MP3_ACTIVITY1, SES_SURE);
        break;

      case ETKINLIK_MOD2:
        if (turkce_eng == TURKCE)
          mp3_time_play(MP3_ETKINLIK2, SES_SURE);
        else
          mp3_time_play(MP3_ACTIVITY2, SES_SURE);
        break;

      case TURKCE_MOD:
        mp3_time_play(MP3_TURKCE, SES_SURE);
        break;

      case ENGLISH_MOD:
        mp3_time_play(MP3_ENGLISH, SES_SURE);
        break;
      } //switch
      time_kaydet = millis();
    }

    if (millis() - time_kaydet > 5000)
    {

      if (mod_say == TURKCE_MOD)
      {
        turkce_eng = TURKCE;
        EEPROM.write(EE_ADR_SES_DIL, TURKCE);
      }
      if (mod_say == ENGLISH_MOD)
      {
        turkce_eng = ENGLISH;
        EEPROM.write(EE_ADR_SES_DIL, ENGLISH);
      }
      if (mod_say == ETKINLIK_MOD1 || mod_say == ETKINLIK_MOD2)
        etkinlik = true;

      if (turkce_eng == TURKCE)
        mp3_time_play(MP3_KAYDET, SES_SURE);
      else
        mp3_time_play(MP3_SAVE, SES_SURE);

      rgb_blink();
      delay(1000);

      if (mod_say == TURKCE_MOD || mod_say == ENGLISH_MOD)
        return son_mod;
      else
      {
        EEPROM.write(EE_ADR_CALISMA_MOD, mod_say);
        return mod_say;
      }
    }
  }
}

void sensorlu_oyun_modu() // sistemin 2-3 metre önüne gelen bir çocukla renkli toplar oyunu oynar
{
  int i;

  pixy.ccc.getBlocks();

  if (millis() - time_mesafe > 100)
  {
    mesafe = HC_SR04(TRIG1, ECHO1);
    Serial.print("cm:");
    Serial.println(mesafe);
    time_mesafe = millis();
  }

  if ((millis() - time_arka_fon > 60000))
  {
    if (arka_fon)
    {
      arka_fon = false;
      mp3_set_volume(mp3_ses);
      delay(100);
      mp3_time_play(MP3_ARKA_FON, SES_SURE);
    }
  }
  if ((millis() - time_arka_fon > 220000))
  {

    if (!arka_fon)
      mp3_time_play(MP3_ARKA_FON, SES_SURE);

    time_arka_fon = millis();
  }

  if (mesafe > 10 && mesafe < 150)
  {
    hareket_say++;
    time_hareket = millis();
    time_arka_fon = millis();
    arka_fon = true;

    if (rgb_blink_yap)
    {
      if ((mp3_ses + 5) > 30)
        mp3_set_volume(25);
      else
        mp3_set_volume(mp3_ses + 5);
      delay(100);

      mp3_time_play(120 + ses_rnd, SSS);
      rgb_blink();

      if (hareket_say >= 3)
      {
        hareket_say = 0;
        // calisma_modu = OYUN_MOD;

        if (turkce_eng == TURKCE)
          mp3_time_play(MP3_BENIMLE_OYNA, SES_SURE);
        else
          mp3_time_play(MP3_PLAY_WITH_ME, SES_SURE);

        delay(500);

        if (turkce_eng == TURKCE)
          mp3_time_play(MP3_TOP_AT_RENK, SES_SURE);
        else
          mp3_time_play(MP3_THROW_BALL, SES_SURE);

        oyun_say = 0;
        rgb_blink_yap = false;
        hareket_var = true;
      }
    }
  }

  if (millis() - time_hareket > 5000 && !hareket_var)
  {
    hareket_say = 0;
    rgb_blink_yap=true;
    time_hareket = millis();
  }

  // If there are detect blocks, print them!
  if (pixy.ccc.numBlocks && oyun_say >= 0)
  {
    //Serial.print("Detected ");
    //Serial.println(pixy.ccc.numBlocks);
    rgb_sig_sirali[0] = 255;
    for (i = 0; i < pixy.ccc.numBlocks; i++)
    {
      Serial.print("  block ");
      Serial.print(i);
      Serial.print(": ");
      pixy.ccc.blocks[i].print();

      renk_seslendir(pixy.ccc.blocks[i].m_signature);
    }
    oyun_say++;
  }

  if (oyun_say >= 3)
  {
    random_renk_at = random_renk();

    if (turkce_eng == TURKCE)
      mp3_time_play(random_renk_at, SES_SURE);
    else
      mp3_time_play(34 + random_renk_at, SES_SURE);

    time_top_at = millis();

    while (true)
    {
      mp3_ses_deg();
      pixy.ccc.getBlocks();

      if (millis() - time_top_at > 10000)
      {
        mp3_time_play(MP3_ALERT, SES_SURE);
        time_top_at = millis();
        hata_say++;
        //break;
      }

      if (pixy.ccc.numBlocks)
      {
        Serial.print("  block ");
        Serial.print(i);
        Serial.print(": ");
        pixy.ccc.blocks[i].print();

        for (i = 0; i < pixy.ccc.numBlocks; i++)
        {
          rgb_led_yak(pixy.ccc.blocks[i].m_signature);

          //pixy.ccc.blocks[i].print();
          if (pixy.ccc.blocks[i].m_signature == random_renk_at - 7)
          {
            mp3_time_play(MP3_ALKIS, SES_SURE);
            time_top_at = millis();
            oyun_say++;
            //Serial.println(oyun_say);
            break;
          }
          else
          {
            mp3_time_play(MP3_ALERT, SES_SURE);
            time_top_at = millis();
            hata_say++;
          }
        }
        renk_rgb(0, 0, 0); //tum ledleri söndür....
      }

      if (oyun_say >= 6 || hata_say >= 5)
      {
        oyun_say = -1;
        rgb_blink_yap = true;
        hareket_var = false;
        //resetFunc();
        mp3_time_play(MP3_BATTERY, SES_SURE);
        mod_seslendir(calisma_modu);
        ses_rnd = random_byte(0, 9); //sesnsorun önünden geçildiğinde rastgele uyarı sesi belirler.
        break;
      }
    }
  }
} //

void rgb_sig_sirala(byte *sirali)
{
  int temp, i;
  if (pixy.ccc.numBlocks)
  {
    for (i = 0; i < pixy.ccc.numBlocks; i++)
      sirali[i] = pixy.ccc.blocks[i].m_signature;

    for (i = 0; i < pixy.ccc.numBlocks - 1; i++)
      for (int j = 0; j < pixy.ccc.numBlocks - 1; j++)
      {
        if (sirali[j] > sirali[j + 1])
        {
          temp = sirali[j];
          sirali[j] = sirali[j + 1];
          sirali[j + 1] = temp;
        }
      }
    Serial.println();
    for (int i = 0; i < pixy.ccc.numBlocks; i++)
    {
      Serial.print(sirali[i]);
    }
    Serial.println();
  }
}
void serbest_mod()
{
  byte i;
  pixy.ccc.getBlocks();

  rgb_sig_sirala(rgb_sig_sirali);

  if ((millis() - time_arka_fon > 60000))
  {
    if (arka_fon)
    {
      arka_fon = false;
      mp3_set_volume(mp3_ses);
      delay(100);
      mp3_time_play(MP3_ARKA_FON, SES_SURE);
    }
  }
  if ((millis() - time_arka_fon > 220000))
  {

    if (!arka_fon)
      mp3_time_play(MP3_ARKA_FON, SES_SURE);

    time_arka_fon = millis();
  }

  if (pixy.ccc.numBlocks)
  {
    time_arka_fon = millis();
    arka_fon = true;

    Serial.print("Detected ");
    Serial.println(pixy.ccc.numBlocks);

    for (i = 0; i < pixy.ccc.numBlocks; i++)
    {
      Serial.print("  block ");
      Serial.print(i);
      Serial.print(": ");
      pixy.ccc.blocks[i].print();
    }

    switch (pixy.ccc.numBlocks)
    {
      if ((mp3_ses + 5) > 30)
        mp3_set_volume(25);
      else
        mp3_set_volume(mp3_ses + 5);
      delay(100);
    case 1:

      renk_seslendir(pixy.ccc.blocks[0].m_signature);

      break;
    case 2:

      if (rgb_sig_sirali[0] == KIRMIZI_TOP && rgb_sig_sirali[1] == YESIL_TOP)
      {
        renk_rgb(255, 255, 0); //sarı
                               // renk_seslendir(SARI_TOP);
      }

      if (rgb_sig_sirali[0] == KIRMIZI_TOP && rgb_sig_sirali[1] == MAVI_TOP)
      {
        renk_rgb(255, 0, 255); //MOR
      }
      if (rgb_sig_sirali[0] == KIRMIZI_TOP && rgb_sig_sirali[1] == SARI_TOP)
      {
        renk_rgb(255, 127, 0); //TURUNCU
      }
      if (rgb_sig_sirali[0] == MAVI_TOP && rgb_sig_sirali[1] == SARI_TOP)
      {
        renk_rgb(0, 255, 0); // yeşil
      }
      if (rgb_sig_sirali[0] == YESIL_TOP && rgb_sig_sirali[1] == MAVI_TOP)
      {
        renk_rgb(0, 255, 255); // turkuaz
      }

      if (rgb_sig_sirali[0] == KIRMIZI_TOP && rgb_sig_sirali[1] == YESIL_TOP)
      {
        renk_rgb(255, 50, 0); // kahverengi
      }

      break;
    case 3:
      if (rgb_sig_sirali[0] == KIRMIZI_TOP && rgb_sig_sirali[1] == YESIL_TOP && rgb_sig_sirali[2] == MAVI_TOP)
      {
        renk_rgb(255, 255, 255); //sarı
      }
      if (rgb_sig_sirali[0] == KIRMIZI_TOP && rgb_sig_sirali[1] == YESIL_TOP && rgb_sig_sirali[2] == SARI_TOP)
      {
        renk_rgb(200, 171, 15); //sarı
      }
      break;
    }
  }
}

byte random_renk()
{
  byte i = 0;
  byte rnd = 0;
  bool yeni_renk = true;
  long time_rnd = millis();

  while (true)
  {
    yeni_renk = true;

    while (1)
    {
      rnd = random(1, 100);
      if (rnd > 0)
      {
        if (rnd >= 8 && rnd <= 11)
          break;
      }
    }
    //Serial.print("rnd:");
    //Serial.println(rnd);

    for (i = 0; i < 4; i++)
    {
      //Serial.println(renk_at_dizi[i]);

      if (renk_at_dizi[i] == rnd)
        yeni_renk = false;
    }
    if (millis() - time_rnd > 1500)
    {
      renk_at_say = 0;
      memset(renk_at_dizi, 0, sizeof(renk_at_dizi));
      time_rnd = millis();
    }

    if (yeni_renk)
    {
      EEPROM.write(EE_ADR_RND, rnd);
      renk_at_dizi[renk_at_say++] = rnd;
      if (renk_at_say >= 3)
      {
        renk_at_say = 0;
        memset(renk_at_dizi, 0, sizeof(renk_at_dizi));
      }
      break;
    }
  }

  return rnd;
}

byte random_byte(byte min, byte max)
{
  byte i = 0;
  byte rnd = 0;
  bool yeni_renk = true;
  long time_rnd = millis();

  while (true)
  {
    yeni_renk = true;

    while (1)
    {
      rnd = random(1, 100);
      if (rnd > 0)
      {
        if (rnd >= min && rnd <= max)
          break;
      }
    }

    for (i = 0; i < 10; i++)
    {
      //Serial.println(renk_at_dizi[i]);

      if (random_byte_dizi[i] == rnd)
        yeni_renk = false;
    }
    if (millis() - time_rnd > 1500)
    {
      renk_at_say = 0;
      memset(random_byte_dizi, 0, sizeof(random_byte_dizi));
      time_rnd = millis();
    }

    if (yeni_renk)
    {
      EEPROM.write(EE_ADR_RND2, rnd);
      random_byte_dizi[renk_at_say++] = rnd;
      if (renk_at_say >= 9)
      {
        renk_at_say = 0;
        memset(random_byte_dizi, 0, sizeof(random_byte_dizi));
      }
      break;
    }
  }

  return rnd;
}
int HC_SR04(int trigPin, int echoPin)
{
  //const int trigPin = 8;
  //const int echoPin = 12 ;
  long duration = 0;

  //pinMode(trigPin, OUTPUT);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  return duration / 29 / 2; //cm
}

void rgb_blink()
{
  int x = 0;
  for (x = 0; x < 20; x++)
  {
    renk_rgb(255, 255, 255);
    delay(25);
    renk_rgb(0, 0, 0);
    delay(25);
  }
}

void renk_rgb(byte r, byte g, byte b)
{
  analogWrite(RED_LED_PIN, r);
  analogWrite(BLUE_LED_PIN, b);
  analogWrite(GREEN_LED_PIN, g);
}

void rgb_led_yak(int sig)
{
  switch (sig)
  {
  case KIRMIZI_TOP:
    renk_rgb(255, 0, 0);
    break;
  case YESIL_TOP:
    renk_rgb(0, 255, 0);
    break;
  case MAVI_TOP:
    renk_rgb(0, 0, 255);
    break;
  case SARI_TOP:
    renk_rgb(255, 255, 0);
    break;
  case TURUNCU_TOP:
    renk_rgb(255, 102, 0);
    break;
  case BEYAZ_TOP:
    renk_rgb(255, 255, 255);
  }
  //delay(SES_SURE);
}

void renk_seslendir(int sgn)
{
  switch (sgn)
  {
  case KIRMIZI_TOP:
    renk_rgb(255, 0, 0);
    if (turkce_eng == TURKCE)
      mp3_time_play(MP3_KIRMIZI_CAL, SES_SURE);
    else
      mp3_time_play(MP3_RED, SES_SURE);
    break;

  case YESIL_TOP:
    renk_rgb(0, 255, 0);
    if (turkce_eng == TURKCE)
      mp3_time_play(MP3_YESIL_CAL, SES_SURE);
    else
      mp3_time_play(MP3_GREEN, SES_SURE);

    break;
  case MAVI_TOP:
    renk_rgb(0, 0, 255);
    if (turkce_eng == TURKCE)
      mp3_time_play(MP3_MAVI_CAL, SES_SURE);
    else
      mp3_time_play(MP3_BLUE, SES_SURE);
    break;
  case SARI_TOP:
    renk_rgb(255, 255, 0);
    if (turkce_eng == TURKCE)
      mp3_time_play(MP3_SARI_CAL, SES_SURE);
    else
      mp3_time_play(MP3_YELLOW, SES_SURE);
    break;
  case TURUNCU_TOP:
    renk_rgb(255, 102, 0);
    if (turkce_eng == TURKCE)
      mp3_time_play(MP3_TURUNCU_CAL, SES_SURE);
    else
      mp3_time_play(MP3_ORANGE, SES_SURE);
    break;
  case BEYAZ_TOP:
    renk_rgb(255, 255, 255);
    if (turkce_eng == TURKCE)
      mp3_time_play(MP3_BEYAZ_CAL, SES_SURE);
    else
      mp3_time_play(MP3_WHITE, SES_SURE);
    break;
  }
  renk_rgb(0, 0, 0); //ledleri sondur.
}
