import RPi.GPIO as GPIO
from mfrc522 import SimpleMFRC522

reader = SimpleMFRC522()

try:
    while True:
        print("Kartı okutmak için bekleniyor...")
        id, text = reader.read()
        print("Kart ID:", id)
        print("Kart içeriği:", text)

finally:
    GPIO.cleanup()