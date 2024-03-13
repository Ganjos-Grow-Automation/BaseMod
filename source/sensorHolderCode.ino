#include <OneWire.h>
#include <Wire.h>
#include <math.h>
#include <GravityRtc.h>

// Declatration of variables

GravityRtc rtc;

// Waterproof temp sensor
int DS18S20_Pin = 2; // DS18S20 Signal pin
OneWire ds(DS18S20_Pin);

// Light intensity sensor
int BH1750address = 0x23;
byte buff[2];

// declaration of functions
void getTime()
{

    // Get timestamp from sensor
    rtc.read();

    // write sensor data - can be optimized, probably need a return here for the JSON object
    Serial.print(rtc.year);
    Serial.print('/');
    Serial.print(rtc.month);
    Serial.print('/');
    Serial.print(rtc.day);
    Serial.print('-');
    Serial.print(rtc.hour);
    Serial.print(':');
    Serial.print(rtc.minute);
    Serial.print(':');
    Serial.println(rtc.second);

    // create variable to return all the data in a string
}

double getWaterproofTemp()
{
    byte data[12];
    byte addr[8];

    if (!ds.search(addr))
    {
        ds.reset_search();
        return -1000;
    }

    if (OneWire::crc8(addr, 7) != addr[7])
    {
        Serial.println("Crc us bit valid!");
        return -1000;
    }

    if (addr[0] != 0x10 && addr[0] != 0x28)
    {
        Serial.println("Device is not recognized");
        return -1000;
    }

    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1);

    byte present = ds.reset();
    ds.select(addr);
    ds.write(0xBE);

    for (int i = 0; i < 9; i++)
    {
        data[i] = ds.read();
    }

    ds.reset_search();

    byte MSB = data[1];
    byte LSB = data[0];

    float tempRead = ((MSB << 8) | LSB);
    float TemperatureSum = tempRead / 16;

    return TemperatureSum;
}

int getLightIntensity()
{
    int i;
    uint16_t val = 0;
    BH1750_Init(BH1750address);
    delay(200);

    if (2 == BH1750_Read(BH1750address))
    {
        val = ((buff[0] << 8) | buff[1]) / 1.2;
        Serial.print(val, DEC);
        Serial.println("[lx]");
    }
    delay(150);
}

int BH1750_Read(int address)
{
    int i = 0;
    Wire.beginTransmission(address);
    Wire.requestFrom(address, 2);
    while (Wire.available())
    {
        buff[1] = Wire.read();
        i++;
    }
    Wire.endTransmission();
    return i;
}

void BH1750_Init(int address)
{
    Wire.beginTransmission(address);
    Wire.write(0x10);
    Wire.endTransmission();
}

// Lets go little robot

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);

    // RTC setup
    rtc.setup();
    rtc.adjustRtc(F(__DATE__), F(__TIME__));

    // Lightsensor
    Wire.begin();
}

void loop()
{
    // put your main code here, to run repeatedly:
    getLightIntensity();
    /*getTime();
    float temperature = getWaterproofTemp();
    Serial.println(temperature);
    */
    delay(1000);
}