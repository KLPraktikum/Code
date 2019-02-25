import serial

from barcode_db import get_barcodes_by_name
from barcode_db import close_db
from barcode_db import update_punktezahl


#Programm fuer das Barcode einlesen


def main():
    line = []
    ser = serial.Serial('/dev/ttyUSB0')  # open serial port
    print(ser.name)  # check which port was really used

    bonuspunkte = 0
    print("Barcode einlesen.")

    while(1):
        barcodestring = input()
        bonuspunkte = 0

        if get_barcodes_by_name(barcodestring):
            query_answer = get_barcodes_by_name(barcodestring)

            if (query_answer[0][2] + query_answer[0][3] + query_answer[0][4] + query_answer[0][5] +
                    query_answer[0][6] + query_answer[0][7] + query_answer[0][8]) > 1:
                print(str(query_answer[0][1])+" muss in mehreren Mülltonnen enstorgt werden.")

            if query_answer[0][2] == 1:
                print(str(query_answer[0][1])+" gehört in die gelbe Tonne")
                ser.write("1".encode())
                line = ser.readline()
                if (line[0] == 's'):
                    #ser.write(str.encode("5"))
                    line[0] == ''
                    bonuspunkte = bonuspunkte + int(query_answer[0][9])

                if (line[0] == 't'):
                    #ser.write(str.encode("6"))
                    line[0] == ''

            if query_answer[0][3] == 1:
               print(str(query_answer[0][1])+" gehört in die schwarze Tonne")
               ser.write("2".encode())
               line = ser.readline()
               if (line[0] == 's'):
                   bonuspunkte = bonuspunkte + int(query_answer[0][9])
                   #ser.write(str.encode("5"))
                   line[0] = ''
               if (line[0] == 't'):
                   #ser.write(str.encode("6"))
                   line[0] = ''

            if query_answer[0][4] == 1:
                print(str(query_answer[0][1])+" gehört in die grüne Tonne")
                ser.write(str.encode("3"))
                line = ser.readline()
                if (line[0] == 's'):
                    #ser.write(str.encode("5"))
                    bonuspunkte = bonuspunkte + int(query_answer[0][9])
                    line[0] = ''
                if (line[0] == 't'):
                    #ser.write(str.encode("6"))
                    line[0] = ''

            if query_answer[0][5] == 1:
                print(str(query_answer[0][1])+" gehört in die Bio Tonne")
                ser.write(str.encode("4"))
                line = ser.readline()
                if (line[0] == 's'):
                    #ser.write(str.encode("5"))
                    bonuspunkte = bonuspunkte + int(query_answer[0][9])
                    line[0] = ''
                if (line[0] == 't'):
                    #ser.write(str.encode("6"))
                    line[0] = ''

            if query_answer[0][6] == 1:
                print(str(query_answer[0][1])+" gehört in die Braunglas Tonne")
                # passende LED ansteuern
                #für Prototyp irrelevant

            if query_answer[0][7] == 1:
                print(str(query_answer[0][1])+" gehört in die Grünglas Tonne")
                # passende LED ansteuern
                # für Prototyp irrelevant

            if query_answer[0][8] == 1:
                print(str(query_answer[0][1])+" gehört in die Weissglass Tonne")
                # passende LED ansteuern
                # für Prototyp irrelevant

            if query_answer[0][1] == 'User':
                update_punktezahl(query_answer[0][0], bonuspunkte)
                user_punktzahl_update = get_barcodes_by_name(barcodestring)
                print(user_punktzahl_update)
                ser.write(str.encode("7"))
                print("Es wurden " + str(bonuspunkte) + " Bonuspunkte gesammelt")
                bonuspunkte = 0
                break

        elif (barcodestring =='Ende'):
            print("Ende")
            break

        else:
            print("Barcode nicht in Datenbank vorhanden")


    close_db()

main()