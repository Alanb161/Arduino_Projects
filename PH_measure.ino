
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 6);

void setup() {
  //CONFIGURNG DIGITAL PIN FOR THE RELAY
  Serial.begin(9600);
  pinMode(2, OUTPUT);

  //CONFIGURING LCD 16X2 DISPLAY
  lcd.begin(16, 2);
}

//STARTING THE VARIABLES
float voltage_raw = 0.0;
float voltage_calib = 0.0;                                                                                                                                                                                                                          
float ph_value = 0.0;                                                                                                                                                                                                                        
float calibracao = 0.0;                                                                                             
float valores_sensor[10];
int i = 0;
float SomaPH = 0.0;
float MediaPH = 0.0;

void loop() {
  //COLLECTING 10 SENSOR DATA VALUES
  while (i<10){
    valores_sensor[i] = analogRead(A1);
    delay(100);
    SomaPH = SomaPH + valores_sensor[i];
    i = i + 1;
  }
  //SUMJMING THE VALUES AND CALCULATING THEIR MEAN 
  MediaPH = SomaPH/10.0;

  //CONVERTING THE MEAN SENSOR VALUE INTO A VOLTAGE VALUE (0 - 1023) -> (0 - 5V)
  voltage_raw = MediaPH*(4.85/1023.0);

  //ADDING CALIBRATION TO VOLTAGE VALUE
  voltage_calib = voltage_raw + calibracao;

  //CONVERTING VOLTAGE VALUE INTO A PH SCALE VALUE 
  //ph_value = (-14.0/4.8)*voltage_calib + 14.0;    //->EQUATION CREATED BASED ON THE FACTS THAT: 0V == (PH) 14 AND 5V == (PH) 0
  ph_value = -5.6*voltage_calib + 19.95; 
  Serial.println(ph_value);

  //=======================================================================================================
  //ACTIVATING THE RELAY IF PH VALUE IS OUT OF THE CORRECT RANGE
  if ( (ph_value > 6.00) && (ph_value<7.00) ){
    digitalWrite(2, HIGH);
    delay(2000);                                                        //HIGH == RELAY OFF
  }
  else if ( (ph_value < 6.00) || (ph_value > 7.00) ){
    digitalWrite(2, LOW);
    delay(2000);                                                       //LOW == RELAY ON
  }
  //======================================================================================================

  //PRINTING PH VALUE IN THE LCD DISPLAY
  lcd.print("CONTINENTAL - CONTITECH");
  lcd.setCursor(0, 1);
  lcd.print("PH:");
  lcd.setCursor(11,1);
  lcd.print(ph_value);

  //RESTARTING VARIABLES FOR NOT MESSING UP THE MEAN CALCULATION
  i = 0;
  MediaPH = 0.0;
  SomaPH = 0.0;
  voltage_raw = 0.0;
  voltage_calib = 0.0;
  ph_value = 0.0;
}
