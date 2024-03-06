import serial
import mysql.connector

# Serielle Schnittstelle konfigurieren
ser = serial.Serial('/dev/ttyUSB0', 9600) # Anpassen des seriellen Anschlusses

# MySQL-Datenbankverbindung herstellen
mydb = mysql.connector.connect(
  host="localhost",
  user="username",
  password="password",
  database="mydatabase"
)
cursor = mydb.cursor()

# Endlosschleife zum Empfangen und Speichern der Daten
while True:
    data = ser.readline().decode().strip().split(',')
    temperature, humidity, co2 = map(float, data)

    # Daten in die Datenbank einfügen
    sql = "INSERT INTO environment_data (temperature, humidity, co2) VALUES (%s, %s, %s)"
    val = (temperature, humidity, co2)
    cursor.execute(sql, val)
    mydb.commit()
