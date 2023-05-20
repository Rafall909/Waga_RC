#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>             // Arduino SPI library
#include "HX711.h"
#include "autoBW.h"

 // definiowanie wag

HX711 scaleFL;
HX711 scaleFR;
HX711 scaleRL;
HX711 scaleRR;

int FL,FR,RL,RR;  // przechowywanie aktualnych wyników
int FL1,FR1,RL1,RR1; // kopie wyników do porównania z nowszymi
long FLzero,FRzero,RLzero,RRzero; // wartości z tarowania wag
int FLstala,FRstala,RLstala,RRstala; // stałe wag
int ile_sr = 4;  // ile pomiarów z wagi należy uśredniać do ostatecznego wyniku

  // piny do NANO

uint8_t clockPinFL = 2;
uint8_t clockPinFR = 4;
uint8_t clockPinRL = A0;
uint8_t clockPinRR = A2;
uint8_t dataPinFL = 3;
uint8_t dataPinFR = 5;
uint8_t dataPinRL = A1;
uint8_t dataPinRR = A3;

#define TFT_CS     10
#define TFT_RST    9
#define TFT_DC     8

  //piny do UNO

/*
uint8_t clockPinFL = A0;
uint8_t clockPinFR = A2;
uint8_t clockPinRL = 6;
uint8_t clockPinRR = 4;
uint8_t dataPinFL = A1;
uint8_t dataPinFR = A3;
uint8_t dataPinRL = 5;
uint8_t dataPinRR = 3;

#define TFT_CS     10
#define TFT_RST    8  // define reset pin, or set to -1 and connect to Arduino RESET pin
#define TFT_DC     9  // define data/command pin
*/

  // definiowanie ekranu

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup(void) {

  Serial.begin(9600);

  // inicjacja wag

  scaleFL.begin(dataPinFL, clockPinFL);
  scaleFR.begin(dataPinFR, clockPinFR);
  scaleRL.begin(dataPinRL, clockPinRL);
  scaleRR.begin(dataPinRR, clockPinRR);

  // inicjacja ekranu

  tft.init(240, 320, SPI_MODE2);
  tft.setRotation(1);

  // ekran startowy

  tft.fillScreen(ST77XX_WHITE);
  tft.setTextSize(3,3);
  tft.setTextColor(ST77XX_MAGENTA, ST77XX_WHITE);
  tft.setCursor(45,30);
  tft.println("Weight Scale");
  tft.setCursor(50,60);
  tft.println("for RC cars");
  tft.setTextSize(2,3);
  tft.setTextColor(ST77XX_BLACK, ST77XX_WHITE);
  tft.setCursor(130,140);
  tft.println("By:");
  tft.setCursor(30,180);
  tft.println("Rafal");
  tft.setCursor(210,180);
  tft.println("Grzegorz");
  tft.setCursor(10,210);
  tft.println("Muszynski");
  tft.setCursor(200,210);
  tft.println("Nasilowski");

 // tarowanie wag

  FLzero=scaleFL.read_average();
  FRzero=scaleFR.read_average();
  RLzero=scaleRL.read_average();
  RRzero=scaleRR.read_average();

 // tymczasowe stałe wag (póki krzywe ważenia nie zostaną wytworzone i dodane do programu)

  FLstala=1970;
  FRstala=1970;
  RLstala=1970;
  RRstala=1970;

  delay(800);

 // czyszczenie ekranu startowego

  tft.setTextSize(3, 3);
  tft.setTextColor(ST77XX_WHITE, ST77XX_WHITE);
  tft.setCursor(45, 30);
  tft.println("Weight Scale");
  tft.setCursor(50, 60);
  tft.println("for RC cars");
  tft.setTextSize(2, 3);

  tft.setCursor(130, 140);
  tft.println("By:");

  tft.setCursor(30, 180);
  tft.println("Rafal");
  tft.setCursor(210, 180);
  tft.println("Grzegorz");
  tft.setCursor(10, 210);
  tft.println("Muszynski");
  tft.setCursor(200, 210);
  tft.println("Nasilowski");

 // rysowanie ekranu ważenia

  tft.drawBitmap(115,30, autoBW,90,180,ST77XX_WHITE,ST77XX_BLACK);
  tft.setTextSize(1,2);
  tft.setTextColor(ST77XX_BLACK, ST77XX_WHITE);

  tft.setCursor(10,10);
  tft.println("Front Left [g]");

  tft.setCursor(220,10);
  tft.println("Front Right [g]");

  tft.setCursor(10,160);
  tft.println("Rear Left [g]");

  tft.setCursor(220,160);
  tft.println("Rear Right [g]");


  tft.setTextSize(4);  // Set text size. Goes from 0 (the smallest) to 20 (very big)

}
void loop() {

  // ważenie

  for(int i=0;i<ile_sr;i++){
    FL=FL+((scaleFL.read()-FLzero)/FLstala);
    FR=FR+((scaleFR.read()-FRzero)/FRstala);
    RL=RL+((scaleRL.read()-RLzero)/RLstala);
    RR=RR+((scaleRR.read()-RRzero)/RRstala);}
    
  FL=FL/ile_sr;
  FR=FR/ile_sr;
  RL=RL/ile_sr;
  RR=RR/ile_sr;

  if(FL<2){FL=0;}
  if(FR<2){FR=0;}
  if(RL<2){RL=0;}
  if(RR<2){RR=0;}

  // wyświetlenie wyników

  if (FL!=FL1){
    tft.setTextColor(ST77XX_WHITE, ST77XX_WHITE);
    tft.setCursor(25, 30);
    tft.println(FL1);
    tft.setTextColor(ST77XX_BLUE, ST77XX_WHITE);
    tft.setCursor(25, 30); 
    tft.println(FL);
    FL1=FL; }

  if(FR!=FR1){
    tft.setTextColor(ST77XX_WHITE, ST77XX_WHITE);
    tft.setCursor(240, 30);
    tft.println(FR1);
    tft.setTextColor(ST77XX_BLUE, ST77XX_WHITE);
    tft.setCursor(240, 30);
    tft.println(FR);
    FR1=FR; }

  if(RL!=RL1){
    tft.setTextColor(ST77XX_WHITE, ST77XX_WHITE);
    tft.setCursor(25, 180);
    tft.println(RL1);
    tft.setTextColor(ST77XX_BLUE, ST77XX_WHITE);
    tft.setCursor(25, 180);
    tft.println(RL);
    RL1=RL; }

  if(RR!=RR1){
    tft.setTextColor(ST77XX_WHITE, ST77XX_WHITE);
    tft.setCursor(240, 180);
    tft.println(RR1);
    tft.setTextColor(ST77XX_BLUE, ST77XX_WHITE); 
    tft.setCursor(240, 180);
    tft.println(RR);
    RR1=RR; }

}


