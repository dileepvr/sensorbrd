#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <Adafruit_BME280.h>

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);


#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

uint16_t UV = 0;
uint16_t broadband = 0;
uint16_t visible = 0;
uint16_t infrared = 0;
uint16_t solarOCV = 0;

int UVin = A4;
int VISin = A3;
int PVin = A5;

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
*/
/**************************************************************************/
void configureSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  /* Update these values depending on what you've set above! */  
/*  Serial.println("------------------------------------");
  Serial.print  ("Gain:         "); Serial.println("Auto");
  Serial.print  ("Timing:       "); Serial.println("13 ms");
  Serial.println("------------------------------------");
  */
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void) 
{
  Serial.begin(115200);
  Serial.println("Light Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!tsl.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
  /* Display some basic information on this sensor */
//  displaySensorDetails();
  
  /* Setup the sensor gain and integration time */
  configureSensor();
  
  /* We're ready to go! */
 // Serial.println("");
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void) 
{  
  /* Get a new sensor event */ 
  sensors_event_t event;
 

  UV = analogRead(UVin);
  visible = analogRead(VISin);
  solarOCV = analogRead(PVin);
      
  tsl.getEvent(&event);

 
  /* Display the results (light is measured in lux) */
  if (event.light)
  {
    tsl.getLuminosity(&broadband, &infrared);
    /*
    Serial.print(event.light); Serial.print(" lux");
    Serial.print(", Broadband: "); Serial.print(broadband); 
    Serial.print(", Infrared: "); Serial.print(infrared);
    */
    Serial.print("L:"); Serial.print(event.light); // luminosity in lux
    Serial.print(",B:"); Serial.print(broadband);
    Serial.print(",I:"); Serial.print(infrared);
  }
  else
  {
    /* If event.light = 0 lux the sensor is probably saturated
       and no reliable data could be generated! */
    Serial.println("Sensor overload");
  }
  /*
  Serial.print(", UV: "); Serial.print(UV);
  Serial.print(", Visible: "); Serial.print(visible); 
  Serial.print(", PV OCV: "); Serial.print(solarOCV);
  */
  Serial.print(",U:"); Serial.print(UV);
  Serial.print(",V:"); Serial.print(visible);
  Serial.print(",O:"); Serial.print(solarOCV);
  /*
  Serial.print(", T = "); Serial.print(bme.readTemperature()); Serial.print(" *C");
  Serial.print(", P = "); Serial.print(bme.readPressure() / 100.0F); Serial.print(" hPa");
  Serial.print(", H = "); Serial.print(bme.readHumidity()); Serial.println(" %");
  */
  Serial.print(",T:"); Serial.print(bme.readTemperature()); // degree Celcius
  Serial.print(",P:"); Serial.print(bme.readPressure()); // Pascals
  Serial.print(",H:"); Serial.println(bme.readHumidity()); // percentage
  
  delay(250); // 250 ms
}


