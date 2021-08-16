//#include <Arduino.h>

//PIXY sıgnature renk tanimlari

#define KIRMIZI_TOP 1
#define YESIL_TOP   2
#define MAVI_TOP    3
#define SARI_TOP    4
#define TURUNCU_TOP 5
#define BEYAZ_TOP   6
//

#define MP3_BENIMLE_OYNA  1
#define MP3_TOP_AT_RENK   2
#define MP3_KIRMIZI_CAL   3
#define MP3_YESIL_CAL     4
#define MP3_MAVI_CAL      5
#define MP3_SARI_CAL      6
#define MP3_TURUNCU_CAL   7

#define MP3_KIRMIZI_TOP_AT  8
#define MP3_YESIL_TOP_AT    9
#define MP3_MAVI_TOP_AT     10
#define MP3_SARI_TOP_AT     11

#define MP3_ALKIS           12
#define MP3_HEY             13
#define MP3_TADA            14
#define MP3_MIYAV           15
#define MP3_ALERT           16
#define MP3_BATTERY         17

#define MP3_MOD_SECIM	    18
#define MP3_SERBEST_MOD	    19
#define MP3_ETKINLIK1	    20
#define MP3_ETKINLIK2	    21
#define MP3_TURKCE	        22
#define MP3_ENGLISH	        23
#define MP3_FREE_MOD	    24
#define MP3_ACTIVITY1	    25
#define MP3_ACTIVITY2	    26
#define MP3_CHANGE_OP   	27
#define MP3_KAYDET	        28
#define MP3_SAVE	        29
#define MP3_RENKLER_OYUNU   30
#define MP3_PLAY_COLORS	    31

#define MP3_RED		32
#define MP3_GREEN	33
#define MP3_BLUE	34
#define MP3_YELLOW	35
#define MP3_ORANGE	36
#define MP3_WHITE	37
#define MP3_BROWN	38
#define MP3_PURPLE	39

#define MP3_TURUNCU_TOP_AT	40
#define MP3_BEYAZ_TOP_AT	41

#define MP3_THROW_ME_RED_BALL	    42
#define MP3_THROW_ME_GREEN_BALL	    43
#define MP3_THROW_ME_BLUE_BALL	    44
#define MP3_THROW_ME_YELLOW_BALL	45

#define MP3_THROW_ME_ORANGE_BALL	46
#define MP3_THROW_ME_WHITE_BALL	47
#define MP3_BEYAZ_CAL   48
#define MP3_PLAY_WITH_ME  49
#define MP3_THROW_BALL	  50

#define MP3_BIR		51
#define MP3_IKI		52
#define MP3_UC		53
#define MP3_DORT	54
#define MP3_BES		55
#define MP3_ALTI	56
#define MP3_YEDİ	57
#define MP3_SEKIZ	58
#define MP3_DOKUZ	59

#define MP3_ON		60
#define MP3_YIRMI	61
#define MP3_OTUZ	62
#define MP3_KIRK	63
#define MP3_ELLI	64

#define MP3_SANIYE	70
#define MP3_SECOND	71

#define MP3_ONE		80
#define MP3_TWO		81
#define MP3_THREE	82
#define MP3_FOUR	83
#define MP3_FIVE	85
#define MP3_SIX		86
#define MP3_SEVEN	87
#define MP3_EIGHT	88
#define MP3_NINE	89
#define MP3_ENG_10	90
#define MP3_ENG_11	91
#define MP3_ENG_12	92
#define MP3_ENG_13	93
#define MP3_ENG_14	94
#define MP3_ENG_15	95
#define MP3_ENG_16	96
#define MP3_ENG_17	97
#define MP3_ENG_18	98
#define MP3_ENG_19	99
#define MP3_ENG_20	100
#define MP3_ENG_30	101
#define MP3_ENG_40	102
#define MP3_ENG_50	103

#define MP3_SENS_SES1	120
#define MP3_SENS_SES2	121
#define MP3_SENS_SES3	122
#define MP3_SENS_SES4	123
#define MP3_SENS_SES5	124
#define MP3_SENS_SES6	125
#define MP3_SENS_SES7	126
#define MP3_SENS_SES8	127
#define MP3_SENS_SES9	128
#define MP3_SENS_SES10	129

#define MP3_ARKA_FON	130


#define RED_LED_PIN     6
#define GREEN_LED_PIN   5
#define BLUE_LED_PIN    3

#define ECHO1           9
#define TRIG1           4
#define ECHO2           A0
#define TRIG2           A1

#define BUTON_PIN      2
#define BUTON_SES1_PIN A0       //DF player ses artırma azaltmak için.
#define BUTON_SES2_PIN A1

#define SES_SURE    1000
#define SSS         400   //saniye söyle süre

#define TURKCE      1
#define ENGLISH     0

#define SERBEST_MOD     200
#define OYUN_MOD        201
#define ETKINLIK_MOD1   202
#define ETKINLIK_MOD2   203
#define ENGLISH_MOD     204
#define TURKCE_MOD      205

#define EE_ADR_RND          0
#define EE_ADR_SES_DIL      1
#define EE_ADR_CALISMA_MOD  2
#define EE_ADR_RND2         3
#define EE_ADR_MP3_SES      4




//

// void mp3_time_play(uint16_t cal);
// byte mod_secim();
// void sensorlu_oyun_modu();
// void rgb_sig_sirala(byte *sirali);
// void serbest_mod();
// byte random_renk();
// int HC_SR04(int trigPin, int echoPin);
// void rgb_blink();
// void renk_rgb(byte r, byte g, byte b);
// void rgb_led_yak(int sig);
// void renk_seslendir(int sgn);
