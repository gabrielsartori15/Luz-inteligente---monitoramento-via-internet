# Welcome to Smart light - monitoring via internet
Smart light - monitoring via internet

## Project description: Luminosity monitoring system with ESP 8266.
in this project we use the esp8266 that will connect the wifi network and the mqttbox server. We will use the free program MQTTBox to create MQTT clients, it makes possible the connection with several Mqtt brokers simultaneously, it allows the publication and subscription to topics. The esp8266 will have the ldr sensor that will measure the ambient light level, which can vary between 0 and 1024. It will have the onboarding led as an actuator and a relay that will activate a lamp. In mqttbox we will work with 3 topics: 2 publishers in mqttbox and one subscribed to it. For the configuration of the esp8266, the Arduino ide was used to do its programming because it already has libraries that are tools that help in communication. It connects with wifi network and mqttbox. On the Arduino platform, you can check the status of the wifi network, ip and configuration that was programmed in the ide.

## Technologies Used:

<table>
  <tr>
    <td>Mqttbox</td>
    <td>Json </td>
  </tr>
</table>

## Description of the components used in the project: 
LDR: (Light dependent dependent resistor) or photoconductive cell
The light dependent resistor (LDR) is made from a piece of exposed semiconductor material such as cadmium sulfide which changes its electrical resistance from several thousand Ohms in the dark to just a few hundred Ohms when exposed to light, creating pairs of electrons -gaps in the material when exposed.

Resistors: are electronic components whose main function is to limit the flow of electrical charges by converting electrical energy into thermal energy. Resistors are generally made from dielectric materials with high electrical resistance. The large electrical resistance of resistors makes these components capable of reducing the flow of electric current.

Protoboard: Also known as a contact matrix or prototyping board, a protoboard is a board that has holes and internal connections for circuit assembly, used for testing electronic components.

NodeMCU ESP8266: The microcontroller used in this project will be the ESP8266 that mounted on a development board along with other components such as resistors, capacitors, crystal oscillator among other components, form the so-called NodeMCU ESP8266.
ESP8266 is a low cost 32-bit RISC microcontroller with Wi-Fi connectivity, capable of operating at 80Mhz or 160Mhz. It has 64KiB of RAM for instructions and 96KiB of RAM for data.
For firmware and other data stores, an external QSPI flash memory is connected to it and the size can vary between 512KiB and 4MiB. The chip itself has 16 GPIO pins (general purpose input/output pins), but some of them are used for connecting to flash QSPI memory. The other pins can function as a Serial Peripheral Interface (SPI), I2C, I2S, asynchronous Universal Receiver/Transmitter (UART) and a 10-bit analog-to-digital converter (ADC).
Wi-Fi connectivity complies with the IEEE 802.11 b/g/n standard and offers WPA/WPA2 and WEP authentication, but can also connect to open hosts

---------------------------------------------------------------------------------------------------


## Learn More

To learn more about Mqttbox, take a look at the following resources:

- [Mqttbox](https://mqttbox.en.softonic.com) - learn about Mqttbox features.


## Authors
+ Gabriel Do Nascimento Sartori
+ Sorayha Alves Guedes


Enjoy!
