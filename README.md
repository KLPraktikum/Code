# Code
Sämtlicher Code (Datenbank, Klasse, Main) für das Projekt

Kommunikationsprotokoll:
	1= gelbe Tonne
	2= schwarze Tonne
	3= grüne Tonne
	4= braune Tonne
	
	s= successful
	t= timeout

Ablauf:
	Raspberry (1-4) -> Arduino
	Arduino steuert LEDs an
	Arduino überprüft zu benutzenden Sensor
	Arduino (s/t) -> Raspberry
	Raspberry verarbeitet Daten, je nachdem ob s oder t (Punktesystem)

