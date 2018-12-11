from barcode_db import get_barcodes_by_name
from barcode_db import close_db
from barcode_db import update_punktezahl

#Programm fuer das Barcode einlesen

# To Do:
# LEDS ansteuern
# Sensoren fuer Muellstand hinzufuegen und Szenario, wenn Muell voll
# Kleiner Bildschirm fuer Ausgabe / evtl. Touchscreen zum Beenden?

def main():

    bonuspunkte = 0
    print("Barcode einlesen.")

    while(1):
        barcodestring = input()

        if get_barcodes_by_name(barcodestring):
            query_answer = get_barcodes_by_name(barcodestring)

            bonuspunkte = bonuspunkte + int(query_answer[0][9])

            if (query_answer[0][2] + query_answer[0][3] + query_answer[0][4] + query_answer[0][5] +
                    query_answer[0][6] + query_answer[0][7] + query_answer[0][8]) > 1:
                print(str(query_answer[0][1])+" muss in mehreren Mülltonnen enstorgt werden.")

            if query_answer[0][2] == 1:
                print(str(query_answer[0][1])+" gehört in die gelbe Tonne")
                #passende LED ansteuern

            if query_answer[0][3] == 1:
               print(str(query_answer[0][1])+" gehört in die schwarze Tonne")
               # passende LED ansteuern

            if query_answer[0][4] == 1:
                print(str(query_answer[0][1])+" gehört in die grüne Tonne")
               # passende LED ansteuern

            if query_answer[0][5] == 1:
                print(str(query_answer[0][1])+" gehört in die Bio Tonne")

            if query_answer[0][6] == 1:
                print(str(query_answer[0][1])+" gehört in die Braunglas Tonne")
                # passende LED ansteuern

            if query_answer[0][7] == 1:
                print(str(query_answer[0][1])+" gehört in die Grünglas Tonne")
                # passende LED ansteuern

            if query_answer[0][8] == 1:
                print(str(query_answer[0][1])+" gehört in die Weissglass Tonne")
                # passende LED ansteuern

            if query_answer[0][1] == 'User':
                update_punktezahl(query_answer[0][0], bonuspunkte)
                user_punktzahl_update = get_barcodes_by_name(barcodestring)
                print(user_punktzahl_update)
                break

        elif (barcodestring =='Ende'):
            print("Ende")
            break

        else:
            print("Barcode nicht in Datenbank vorhanden")

    print("Es wurden " + str(bonuspunkte) + " Bonuspunkte gesammelt")

    close_db()

main()