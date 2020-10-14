#Librerias
import serial, time
import io
import re
from os import system, name

arduino = serial.Serial('/dev/ttyACM0', 9600)       #Inicio Serial
time.sleep(1)

DistB = []
TempB = []
HumB = []
COB = []
LPB = []
Fan = False
Gas = False

def clear(): #Método que limpia la pantalla de la consola
    if name == 'nt':
        _ = system('cls')
    else:
        _ = system('clear')

def fanS(flag): #Recibe un Bool, y regresa el mensaje del estado del ventilador de colores
    if flag:
        return '\033[91m'+"FAN ON"+'\033[0m'
    else:
        return '\033[34m'+"FAN OFF"+'\033[0m'

def buffer(lista,n):    #Mantiene el buffer al tamaño deseado n
    if len(lista)<n:
        return lista
    else:
        lista.pop(0)
        return lista

def gas(gas):           #Recibe un Bool y regrea el mensaje de el estado del Gas de colores
    if gas:
        return '\033[91m'+"GAS TOXICO DETECTADO"+'\033[0m'
    else:
        return '\033[34m'+"Sin precencia de gas"+'\033[0m'

def read():
    arduino.flush()
    rawString = arduino.readline()      #Leemos linea de Seral
    cadena = str(rawString)             #La Hacemos String
    cantidades = re.findall(r"[0-9]+\.[0-9]{2}",cadena)     #Buscamos las mediciones en la cadena
    gases = re.findall(' [0-9]+ ',cadena)                   #Buscamos las mediciones de gas
    if len(cantidades)<3 or len(gases)<2 :                   #Si la primera lectura no tiene todos los datos, completamos con 0s
        while len(cantidades)<3:
            cantidades.append(0)
        while len(gases)<2:
            gases.append(0**2)
    return cantidades, gases, cadena

def draw(cantidades, gases, rawString):
    #Humedad
    HumA = float(cantidades[0])
    HumB.append(HumA)
    #Temperatura
    TempA = float(cantidades[1])
    TempB.append(TempA)
    if TempA>31.00:                  #Si la temp pasa de 26, enciendo ventilador
        arduino.write(str('H').encode())   #Mando High al Arduino
        Fan = True
    else:
        arduino.write(str('L').encode())
        Fan = False
    #Distancia
    DistA = float(cantidades[2])
    DistB.append(DistA)
    #Gas LP
    LPA = int(gases[0])
    LPB.append(LPA)
    if LPA==0:
        Gas=False
    else:
        Gas=True
    #Gas CO
    COA = int(gases[1])
    COB.append(COA)
    if COA==0:
        Gas=False
    else:
        Gas=True

    print ("|----------------------------------------------------------------")
    print ("|Temperatura Actual(C): "+'\033[33m'+ str(TempA)+'\033[0m'+" "+fanS(Fan))
    print ("|=> "+str(list(reversed(buffer(TempB,10)))))
    print ("|Humedad Actual: "+'\033[33m'+ str(HumA)+'\033[0m'+" ")
    print ("|=> "+str(list(reversed(buffer(HumB,10)))))
    print ("|Distancia Actual(Cm): " +'\033[33m'+ str(DistA)+'\033[0m'+" ")
    print ("|=> "+str(list(reversed(buffer(DistB,10)))))
    print ("|")
    print ("|Presencia de Gas(GLP/Propano) " + gas(Gas))
    print ("|Gas CO Actual: "+'\033[33m'+ str(COA)+'\033[0m'+" ppm")
    print ("|=> "+str(list(reversed(buffer(COB,10)))))
    print ("|Gas LP Actual: "+'\033[33m'+ str(LPA)+'\033[0m'+" ppm")
    print ("|=> "+str(list(reversed(buffer(LPB,10)))))
    print ("|----------------------------------------------------------------")
    print ("|"+'\033[91m'+str(rawString)+'\033[0m')
    print ("|----------------------------------------------------------------")
    time.sleep(1)

while True:
    cantidades, gases, cadena = read()
    time.sleep(.15)
    clear()
    draw(cantidades, gases, cadena)
arduino.close()
