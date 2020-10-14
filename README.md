-----------

Examen 1 Realizado por: Jorge David Elihu Mejia Castro

-----------

# Archivo Sensores-test1.py 

## Prerequisitos
El Script hace uso de la libreria PySerial para la comunicación serial, la cual puede ser instalada con 
el siguiente comando.

    python -m pip install PySerial

## Uso
Ejecutar el Script Sensores-test1.py (El Script antiguo es el de Sensores.py) después de conectar el arduino a la PC, El Script esta probado en Linux, pero en windows deberia funcionar con los ajustes que se detallan:

En la linea 6 se encuentra la configuración del puerto serial

    arduino = serial.Serial('/dev/ttyACM0', 9600)

Para su funcionamiento en windows se necesita cambiar la cadena que se le pasa al método Serial
Ejemplo:

    arduino = serial.Serial('COM3', 9600)

El Script recibe Los mensajes del Puerto Serial en STDIO, y los interpreta para presentarlos
en la pantalla.

    |-------------------------------------------------------
    |Temperatura Actual: (Temperatura Actual)
    |=> (Lista de las últimas 9 temperaturas)
    |Humedad Actual: (Humedad Actual)
    |=> (Lista de las últimas 9 Mediciones de humedad)
    |Distancia Actual: (Distancia Actual)
    |=> (Lista de las últimas 9 Mediciones de Distancia)
    |:
    |Presencia de Gas(GLP/Propano): (True si detecta gas, False si no)
    |Gas CO Actual: (CO)
    |=> (Lista de las últimas 9 Mediciones de CO)
    |Gas LP Actual: (LP)
    |=> (Lista de las últimas 9 Mediciones de LP)
    |-------------------------------------------------------
    | Mensaje actual recibido de el Puerto serial
    |-------------------------------------------------------

### Cambios en la nueva versión
Se cambio el código de Arduino, siendo el nuevo código el que está en la carpeta Sensores-test, las ventajas de estos cambios estan en que reducen el uso del CPU y una mejora en la respuesta de los sensores ya que lee todos los datos de una y no uno de cada segundo.

# Carpeta Sensores02 

Proyecto de netbeans, Solo tiene funcional la conversión de Texto a binario y la representación gráfica 
ya que no pude hacer uso de la libreria.

# Link del video

https://www.youtube.com/watch?v=DIH8ORRZIDs

> La unica diferencia entre entre el código antiguo y este nuevo es que en el nuevo los cambios se ven reflejados de manera instánea en el script por lo que no vi necesario grabar y subir otro video ya que toma bastante tiempo con mi internet.

