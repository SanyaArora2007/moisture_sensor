#include <Serial_LCD_SparkFun.h>

Serial_LCD_SparkFun lcd = Serial_LCD_SparkFun();

int water = 0;
String level = "";
int motion = 1;
int sleep_counter = 0;

void setup()
{
    lcd.clear();

    // set up pins
    pinMode( D6, OUTPUT );
    pinMode( A5, INPUT );
    pinMode( D2, INPUT );
    digitalWrite( D7, LOW );

    // turn off RGB
    RGB.control( true );
    RGB.color( 0, 0, 0 );

    // register a particle variable
    Particle.variable( "water", water );
    Particle.variable( "level", level );
    Particle.variable( "motion", motion );
}

int read_moisture_level()
{
    // read the moisture level
    digitalWrite( D6, HIGH );
    delay( 50 );
    int val = analogRead( A5 );
    digitalWrite( D6, LOW );
    return val;
}


String check_water_level( int x )
{
    String level;
    if ( x > 3310 )
    {
        level = "I am Wet :)";
    }
    else if ( x > 3290 )
    {
        level = "I am OK";
    }
    else if ( x > 3200 )
    {
        level = "I need water :(";
    }
    else
    {
        level = "I am dying :(:(";
    }
    return level;
}


void loop()
{
    motion = digitalRead( D2 );

    if ( motion == 1 )
    {
        if ( Particle.connected() )
        {
            Particle.publish( "Motion" );
        }

        for ( int counter = 15; counter>0; counter-- )
        {
            water = read_moisture_level();
            level = check_water_level( water );

            // display the value
            lcd.clear();
            lcd.setBrightness( 30 );
            Serial1.print( water );
            lcd.selectLine( 2 );
            Serial1.print( level );
            delay( 1000 );
        }
    }
    else
    {
        if ( sleep_counter < 600 )
        {
            lcd.setBrightness( 1 );
            sleep_counter++;
        }
        else
        {
            water = read_moisture_level();
            level = check_water_level( water );
            sleep_counter = 0;
        }
    }

    delay( 100 );
}

