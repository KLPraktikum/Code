from barcode_db import get_barcodes_by_name

#Programm fuer das Barcode einlesen


def main():
    print("Barcode eingeben")

    while(1):
        barcodestring = input()
        if get_barcodes_by_name(barcodestring):
            test = get_barcodes_by_name(barcodestring)
            print(test)


# Jetzt noch die ganzen ifs, was zu tun ist, wenn Wert A den Wert X hat


main()