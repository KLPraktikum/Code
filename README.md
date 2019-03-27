# Code
Sämtlicher Code (Datenbank, Klasse, Main) für das Projekt

##Kommunikationsprotokoll:
	1= gelbe Tonne
	2= schwarze Tonne
	3= grüne Tonne
	4= braune Tonne
	
	s= successful
	t= timeout

##Ablauf:
	Raspberry (1-4) -> Arduino
	Arduino steuert LEDs an
	Arduino überprüft zu benutzenden Sensor
	Arduino (s/t) -> Raspberry
	Raspberry verarbeitet Daten, je nachdem ob s oder t (Punktesystem)

##Stromverbrauch@5V
- Arduino Nano 16Mhz @ 5V - 10mA (Datenblatt)
- Propeller Board @ 5V - 300mA (Datenblatt worst case)
- VL53L0X TOF Sensor @ 3.3V - 20mA (Active)/10uA ~= 0 (Datenblatt)
- APA102 LEDs @ 5V - ~1A pro Meter bei voller Helligkeit

| Komponente 	| Leistungsaufnahme (W)  	| Duty-Cycle  	| Anzahl	|
|:-:	|:-:	|:-:	|:-:	|
| Arduino Nano  	| 0,05W  	| 100%  	|1	|
| Propeller Board  	| 1,5W  	| 100%  	|1	|
| VL53L0X  	| ((3,3×0,02×20+3,3×0,00001)÷20)×4=0,069697742W  	| 5%  	|4	|
| APA102  	| ((5W÷3)×0,05)=0,083333333W  	| 5%  	|1	|