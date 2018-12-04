import sqlite3
from Barcodes import Barcode

"""Datenbank fuer Barcodes"""

conn = sqlite3.connect(':memory:')

c = conn.cursor()

c.execute("""CREATE TABLE barcodes(
            barcode integer,
            gelb integer,
            schwarz integer,
            gruen integer,
            bio integer,
            braunglas integer,
            gruenglas integer,
            weissglas integer,
            groesse real
            )""")

barcode_1 = Barcode('40269474','0','1','1','0','0','1','1','0.9')

def insert_barcode(barcode):
    with conn:
        c.execute("INSERT INTO barcodes VALUES(:barcode,:gelb,:schwarz,:gruen,:bio,:braunglas, :gruenglas, :weissglas, :groesse)", {'barcode': barcode.barcode,'gelb': barcode.gelb, 'schwarz':barcode.gruen, 'gruen': barcode.gruen, 'bio': barcode.bio, 'braunglas': barcode.braunglas, 'gruenglas': barcode.gruenglas, 'weissglas': barcode.weissglas, 'groesse': barcode.groesse})

def get_barcodes_by_name(barcodenummer):
    c.execute("SELECT * FROM barcodes WHERE barcode = :barcode", {'barcode': barcodenummer})
    return c.fetchall()

def remove_barcode(barcode):
    with conn:
        c.execute("DELETE from barcodes WHERE barcode = :barcode", {'barcode':barcode.barcode})


insert_barcode(barcode_1)

#conn.close()