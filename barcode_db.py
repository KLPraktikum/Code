import sqlite3
from Barcodes import Barcode

"""Datenbank fuer Barcodes"""

# To Do:
# Nur noch Datenbank befuellen mit gewuenschten Werten
# Beispiel:
# barcode_1 = Barcode(40269474,'Mozart Quelle Wasserflasche',0,0,0,0,0,0,1,3.0)
# Dann: insert_barcode(barcode_1)

conn = sqlite3.connect(':memory:')

c = conn.cursor()

c.execute("""CREATE TABLE barcodes(
            barcode integer,
            bezeichnung text,
            gelb integer,
            schwarz integer,
            gruen integer,
            bio integer,
            braunglas integer,
            gruenglas integer,
            weissglas integer,
            punkte real
            )""")

barcode_1 = Barcode(40269474, 'Mozart Quelle Wasserflasche', 0, 1, 0, 0, 0, 0, 1, 3.0)
barcode_2 = Barcode(10, 'Test1', 1, 0, 1, 1, 0, 0, 0, 50.0)
barcode_3 = Barcode(123456, 'User', 0, 0, 0, 0, 0, 0, 0, 0.0)
barcode_4 = Barcode(4001513009708, 'Gerolsteiner Wasser', 1, 0, 0, 0, 0, 0, 0, 20.0)



def insert_barcode(barcode):
    with conn:
        c.execute("INSERT INTO barcodes VALUES(:barcode, :bezeichnung,:gelb,:schwarz,:gruen,:bio,:braunglas, :gruenglas, :weissglas, :punkte)", {'barcode': barcode.barcode,'bezeichnung': barcode.bezeichnung,'gelb': barcode.gelb, 'schwarz':barcode.gruen, 'gruen': barcode.gruen, 'bio': barcode.bio, 'braunglas': barcode.braunglas, 'gruenglas': barcode.gruenglas, 'weissglas': barcode.weissglas, 'punkte': barcode.punkte})

def get_barcodes_by_name(barcodenummer):
    c.execute("SELECT * FROM barcodes WHERE barcode = :barcode", {'barcode': barcodenummer})
    return c.fetchall()

def remove_barcode(barcode):
    with conn:
        c.execute("DELETE from barcodes WHERE barcode = :barcode", {'barcode':barcode.barcode})

def update_punktezahl(barcode,punkte):
    with conn:
        c.execute("""UPDATE barcodes SET punkte = :punkte WHERE barcode = :barcode""", {'barcode': barcode, 'punkte': punkte})

def close_db():
    conn.close()

insert_barcode(barcode_1)
insert_barcode(barcode_2)
insert_barcode(barcode_3)
insert_barcode(barcode_4)
#conn.close()