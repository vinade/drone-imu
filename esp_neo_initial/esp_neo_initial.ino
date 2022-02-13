
#include <NeoSWSerial.h>

NeoSWSerial esp_serial(8, 9);

void setup()
{

    Serial.begin(9600); // communication with the host computer
    esp_serial.begin(9600);

    Serial.println("");
    Serial.println("Remember to to set Both NL & CR in the serial monitor.");
    Serial.println("Ready");
    Serial.println("");

    // Comandos para serem executados no monitor
    // AT
    // AT+UART_DEF=9600,8,1,0,0
    // AT+CWMODE=1
    // AT+CWLAP
    // AT+CWJAP="NOME_DA_REDE","SENHA"
    // AT+CIFSR
}

void loop()
{

    if (esp_serial.available())
    {
        Serial.write(esp_serial.read());
    }

    if (Serial.available())
    {
        esp_serial.write(Serial.read());
    }
}
