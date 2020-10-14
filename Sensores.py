#Librerias
import serial, time
import io
import re
from os import system, name

arduino = serial.Serial('/dev/ttyACM0', 9600)       #Inicio Serial
time.sleep(1)

DistA = [0,0]
DistB = [0]
TempA = [0,0]
TempB = [0]
TempB = [0]
HumA = [0,0]
HumB = [0]
COA = [0]
COB = [0]
LPA = [0]
LPB = [0]
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

def buffer(lista,n):    #Mantiene el buffer al tamaǹo deseado n
    if len(lista)<n:
        return lista
    else:
        lista.pop(0)
        return lista

def gas(gas):           #Recibe un Bool y regrea el mensaje de el estado del Gas de colores
    if gas:
        return '\033[91m'+str(Gas)+'\033[0m'
    else:
        return '\033[34m'+str(Gas)+'\033[0m'

while True:
    rawString = arduino.readline()      #Leemos linea de Seral
    cadena = str(rawString)             #La Hacemos String
    if "Sensor D:" in cadena:           #Si Encuentra la cocurrencia en el mensaje
        DistA = re.findall("[0-9]+.[0-9][0-9]",cadena)      #Busca la cantidad en el String
        Dist = float(DistA[0])          #Hago flotante la cadena con la cantidad
        DistB.append(Dist)              #La agrego al buffer para mostrarla en el historial
    elif "Sensor H:" in cadena:
        HumA = re.findall("[0-9]+.[0-9][0-9]",cadena)
        Hum = float(HumA[0])
        HumB.append(Hum)
    elif "Sensor T:" in cadena:
        TempA = re.findall("[0-9]+.[0-9][0-9]",cadena)
        Temp = float(TempA[0])
        TempB.append(Temp)
        if Temp>31.00:                  #Si la temp pasa de 26, enciendo ventilador
            arduino.write(str('H').encode())   #Mando High al Arduino 
            Fan = True
        else:
            arduino.write(str('L').encode())
            Fan = False
    elif "CO:" in cadena:
        COA =re.findall("[0-9]+",cadena)
        CO = int(COA[0])
        if CO==0:
            Gas=False
        else:
            Gas=True
        COB.append(CO)
    elif "LP:" in cadena:
        LPA =re.findall("[0-9]+",cadena)
        LP = int(LPA[0])
        if LP==0:
            Gas=False
        else:
            Gas=True
        LPB.append(LP)

    print ("|-------------------------------------------------------")
    print ("|Temperatura Actual(C): "+'\033[33m'+ str(TempA[0])+'\033[0m'+" "+fanS(Fan))
    print ("|=> "+str(list(reversed(buffer(TempB,10)))))
    print ("|Humedad Actual: "+'\033[33m'+ str(HumA[0])+'\033[0m'+" ")
    print ("|=> "+str(list(reversed(buffer(HumB,10)))))
    print ("|Distancia Actual(CM): " +'\033[33m'+ str(DistA[0])+'\033[0m'+" ")
    print ("|=> "+str(list(reversed(buffer(DistB,10)))))
    print ("|:")
    print ("|Presencia de Gas(GLP/Propano)" + gas(Gas))
    print ("|Gas CO Actual: "+'\033[33m'+ str(COA[0])+'\033[0m'+" ppm")
    print ("|=> "+str(list(reversed(buffer(COB,10)))))
    print ("|Gas LP Actual: "+'\033[33m'+ str(LPA[0])+'\033[0m'+" ppm")
    print ("|=> "+str(list(reversed(buffer(LPB,10)))))
    print ("|-------------------------------------------------------")
    print ("|"+'\033[91m'+str(rawString)+'\033[0m')
    print ("|-------------------------------------------------------")
    time.sleep(1)
    clear()

arduino.close()
