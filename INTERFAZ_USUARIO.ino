void Interfaz_Usuario()
{
  VentanaComandos();
  Botones();
}
void ConfigurarInterfazUsuario()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(botonOn, INPUT_PULLUP);
  pinMode(Reset, INPUT_PULLUP);
  pinMode(ModoViaje, INPUT_PULLUP);
}
/*La ventana de comandos se utiliza para configurar ciertos parámetros de nuestro programa,
  con ella podemos indicar:
  SENSORES:
  Ganancia sensores
  Offset Sensores
  Histéresis
  ZONAS
  Temperatura de la zona
  HORARIOS
  Hora Inicial
  Hora Final
  Control de Temperatura y Tiempo
  ACUMULADOR:
  Temperatura
  COLECTOR:
  Temperatura
  Tiempo Activacion de la Bomba/Electroválvula*/

void VentanaComandos()
{
  if ( Serial.available() > 0)
  {
    String cmd = Serial.readString();
    Serial.println(cmd);
    cmd.toLowerCase();//Creo un comando y lo leo

    /*__________________ACUMULADOR____________________*/
    //CONFIGURAR LA MINIMA TEMPERATURA DEL ACUMULADOR
    //AcumuladorTempMin(-10.0). LA TEMPERATURA QUE QUIERO QUE TENGA ES DE 8.00 ºC
    if (cmd.substring(0, 17) == "acumuladortempmin")
    {
      sistema_calentamiento.min_temperatura_acumulador = cmd.substring(18, 23).toFloat();
      Serial.println(sistema_calentamiento.min_temperatura_acumulador);
    }
    //CONFIGURAR LA HISTERESIS DEL SENSOR DEL ACUMULADOR
    //acumuladorhisteresis(1.5)
    if (cmd.substring(0, 20) == "acumuladorhisteresis")
    {
      sistema_calentamiento.sensor_acumulador.histeresis = cmd.substring(21, 24).toFloat();
      Serial.println(sistema_calentamiento.sensor_acumulador.histeresis);
    }
    //CONFIGURAR LA GANANCIA DEL SENSOR DEL ACUMULADOR
    //acumuladorganancia(+00.0)
    if (cmd.substring(0, 18) == "acumuladorganancia")
    {
      sistema_calentamiento.sensor_acumulador.ganancia = cmd.substring(19, 24).toFloat();
      Serial.println(sistema_calentamiento.sensor_acumulador.ganancia);
    }
    //CONFIGURAR EL OFFSET DEL SENSOR DEL ACUMULADOR
    //acumuladoroffset(+100.5)
    if (cmd.substring(0, 16) == "acumuladoroffset")
    {
      sistema_calentamiento.sensor_acumulador.offset = cmd.substring(17, 23).toFloat();
      Serial.println(sistema_calentamiento.sensor_acumulador.offset);
    }
    //CONFIGURAR EL RANGO DEL SENSOR DE TEMPERATURA
    //acumuladorrangoinferior(-100.2)
    if (cmd.substring(0, 23) == "acumuladorrangoinferior")
    {
      sistema_calentamiento.sensor_acumulador.limiteInferior = cmd.substring(24, 30).toFloat();
      Serial.println(sistema_calentamiento.sensor_acumulador.limiteInferior);
    }
    //CONFIGURAR EL RANGO DEL SENSOR DE TEMPERATURA
    //acumuladorrangosuperior(+100.5)
    if (cmd.substring(0, 23) == "acumuladorrangosuperior")
    {

      sistema_calentamiento.sensor_acumulador.limiteSuperior = cmd.substring(24, 30).toFloat();
      Serial.println(sistema_calentamiento.sensor_acumulador.limiteSuperior);
    }

    /*__________________COLECTORES____________________*/
    //Se enumeran desde 0

    //CONFIGURAR TEMPERATURA MAXIMA/OBJETIVO DEL COLECTOR
    //colectortempmax(1,+100.0);
    if (cmd.substring(0, 15) == "colectortempmax")
    {
      int n_colectores = cmd.substring(16, 17).toInt();

      if (n_colectores < 2)
      {
        sistema_calentamiento
        .colectores_solares[n_colectores]
        .parametros.temperaturaMaxima = cmd.substring(18, 24).toFloat();

        Serial.print("MaxTemp Col: ");
        Serial.print(n_colectores);
        Serial.print(" es ");
        Serial.println(sistema_calentamiento
                       .colectores_solares[n_colectores]
                       .parametros.temperaturaMaxima);
      } else {
        Serial.println("N_col: 0 y 1");
      }
    }

    //CONFIGURAR PERIODO DE ACTIVACION DEL COLECTOR
    //colectorperiodo(1,hh:mm:ss)
    if (cmd.substring(0, 15) == "colectorperiodo") //Realmente el Tiempo de activacion es el Periodo que va a estar activo el Colector
    { //Primero pasamos: horas,mins y seg a enteros
      //TiempoActivacionColector
      int n_colector = cmd.substring(16, 17 ).toInt();
      if (n_colector < 2) {
        sistema_calentamiento.colectores_solares[n_colector].parametros.tiempoActivacion.hora = cmd.substring(18, 20).toInt();
        sistema_calentamiento.colectores_solares[n_colector].parametros.tiempoActivacion.minutos = cmd.substring(21, 23).toInt();
        sistema_calentamiento.colectores_solares[n_colector].parametros.tiempoActivacion.segundos = cmd.substring(24, 26). toInt();
        //Con esto despues lo pasamos a milisegundos, sumamos horas+mins+seg= Periodo de activacion del colector
        Serial.print("Tact col ");
        Serial.print(n_colector);
        Serial.print(" es ");
        Serial.print(sistema_calentamiento.
                     colectores_solares[n_colector].
                     parametros.tiempoActivacion.hora);
        Serial.print(":");
        Serial.print(sistema_calentamiento.
                     colectores_solares[n_colector].
                     parametros.tiempoActivacion.minutos);
        Serial.print(":");
        Serial.print(sistema_calentamiento.
                     colectores_solares[n_colector].
                     parametros.tiempoActivacion.segundos);
      } else {
        Serial.println("Ncol: 0 y 1");
      }
    }


    //CONFIGURAR PARAMETROS DEL COLECTOR: HISTERESIS, GANANCIA,OFSETT, RANGO
    //colectorhisteresis(1,2.0)
    if (cmd.substring(0, 18) == "colectorhisteresis")
    {
      int n_colector = cmd.substring(19, 20).toInt();
      if (n_colector < 2) {
        sistema_calentamiento.colectores_solares[n_colector].sensor.histeresis = cmd.substring(21, 23).toFloat();
        Serial.print("Hcol");
        Serial.print(n_colector);
        Serial.print(" es ");
        Serial.println(sistema_calentamiento.colectores_solares[n_colector].sensor.histeresis);
      } else {
        Serial.println("Ncol: 0 y 1");
      }
    }

    //colectorganancia(1,+00.0)
    if (cmd.substring(0, 16) == "colectorganancia")
    {
      int n_colector = cmd.substring(17, 18).toInt();
      if (n_colector < 2) {
        sistema_calentamiento.colectores_solares[n_colector].sensor.ganancia = cmd.substring(19, 24).toFloat();
        Serial.print("col: "); Serial.print(n_colector); Serial.print(" G es: ");
        Serial.println(sistema_calentamiento.colectores_solares[n_colector].sensor.ganancia);
      } else {
        Serial.println("Ncol: 0 y 1");
      }

    }
    //colectoroffset(1,+100.0)
    if (cmd.substring(0, 14) == "colectoroffset")
    {
      int n_colector = cmd.substring(15, 16).toInt();
      if (n_colector < 2) {
        sistema_calentamiento.colectores_solares[n_colector].sensor.offset = cmd.substring(17, 22).toFloat();
        Serial.print("col: "); Serial.print(n_colector); Serial.print(" G es: ");
        Serial.println(sistema_calentamiento.colectores_solares[n_colector].sensor.offset);
      } else {
        Serial.println("Ncol: 0 y 1");
      }


    }
    //colectorrangoinferior(1,-100.0)
    if (cmd.substring(0, 21) == "colectorrangoinferior")
    {
      int n_colector = cmd.substring(22, 23).toInt();
      if (n_colector < 2) {
        sistema_calentamiento.colectores_solares[n_colector].sensor.limiteInferior = cmd.substring(24, 30).toFloat();
        Serial.print("col: "); Serial.print(n_colector); Serial.print(" L_inf: ");
        Serial.println(sistema_calentamiento.colectores_solares[n_colector].sensor.limiteInferior);
      } else {
        Serial.println("Ncol: 0 y 1");
      }
    }
    //colectorrangosuperior(1,-100.0)
    if (cmd.substring(0, 21) == "colectorrangosuperior")
    {
      int n_colector = cmd.substring(22, 23).toInt();
      if (n_colector < 2) {
        sistema_calentamiento.colectores_solares[n_colector].sensor.limiteSuperior = cmd.substring(24, 30).toFloat();
        Serial.print("col: "); Serial.print(n_colector); Serial.print(" L_sup: ");
        Serial.println(sistema_calentamiento.colectores_solares[n_colector].sensor.limiteSuperior);
      } else {
        Serial.println("Ncol: 0 y 1");
      }
    }

    /*__________________COMANDOS ZONAS________________*/
    //ACTIVAR CONTROL HORARIO DE CADA UNA DE LAS ZONAS
    ///<Summary>
    //controlhorarioon(numero_zona,Activar_Desactivar)
    //controlhorarioon(0,1)
    //controlhorarioon(1,1)
    //controlhorarioon(2,1)
    //numero_zona:0,1,2
    //Activar_Desactivar: 0 = false, 1 = true
    ///<Summary)
    if (cmd.substring(0, 16) == "controlhorarioon")
    {
      //ACTIVAR CONTROL HORARIO
      int numero_zona = cmd.substring(17, 18).toInt();
      int Activar_Desactivar = cmd.substring(19, 20).toInt();

      if (Activar_Desactivar == 0)
      {
        sistema_calentamiento.zonas_calentar[numero_zona].ActivarControlHorario = LOW;
      }
      if (Activar_Desactivar == 1)
      {
        sistema_calentamiento.zonas_calentar[numero_zona].ActivarControlHorario = HIGH;
      }
      Serial.print("Z: "); Serial.print(numero_zona); Serial.print(" CH: ");
      Serial.println(sistema_calentamiento.zonas_calentar[numero_zona].ActivarControlHorario);
    }
    //tEMPERATURA DE LAS ZONAS
    //temperaturazona(nºzona,temperatura)
    //temperaturazona(1,+25.0)
    if (cmd.substring(0, 15) == "temperaturazona")
    {
      int n_zonas = cmd.substring(16, 17).toInt();
      float temp = cmd.substring(18, 23).toFloat();
      sistema_calentamiento.zonas_calentar[n_zonas].Configuracion.temperatura_objetivo = temp;
      Serial.println(sistema_calentamiento.zonas_calentar[n_zonas].Configuracion.temperatura_objetivo);
    }

    ///<Summary>
    //horariozona(numero_zona,hora_on,hora_off)
    //Ej: horariozona(1,12:50:20,23:12:40)
    //numero_zona= 0,1,2
    //hora_on = hh:mm:ss
    //hora_off = hh:mm:ss
    ///<Summary>
    if (cmd.substring(0, 11) == "horariozona")
    {
      int numero_zona = cmd.substring(12, 13).toInt();

      sistema_calentamiento.zonas_calentar[numero_zona].Configuracion.On.hora = cmd.substring(14, 16).toInt();
      sistema_calentamiento.zonas_calentar[numero_zona].Configuracion.On.minutos = cmd.substring(17, 19).toInt();
      sistema_calentamiento.zonas_calentar[numero_zona].Configuracion.On.segundos = cmd.substring(20, 22).toInt();
      sistema_calentamiento.zonas_calentar[numero_zona].Configuracion.Off.hora = cmd.substring(23, 25).toInt();
      sistema_calentamiento.zonas_calentar[numero_zona].Configuracion.Off.minutos = cmd.substring(26, 28).toInt();
      sistema_calentamiento.zonas_calentar[numero_zona].Configuracion.Off.segundos = cmd.substring(29, 31).toInt();

      Serial.print("Z: "); Serial.println(numero_zona);

      Serial.print("On: ");
      Serial.print(sistema_calentamiento.zonas_calentar[numero_zona].Configuracion.On.hora); Serial.print(":");
      Serial.print(sistema_calentamiento.zonas_calentar[numero_zona].Configuracion.On.minutos); Serial.print(":");
      Serial.println(sistema_calentamiento.zonas_calentar[numero_zona].Configuracion.On.segundos);

      Serial.print("Off: ");
      Serial.print(sistema_calentamiento.zonas_calentar[numero_zona].Configuracion.Off.hora); Serial.print(":");
      Serial.print(sistema_calentamiento.zonas_calentar[numero_zona].Configuracion.Off.minutos); Serial.print(":");
      Serial.println(sistema_calentamiento.zonas_calentar[numero_zona].Configuracion.Off.segundos);
    }
    ///<Summary>
    //histeresissensor(numero_zona, histeris)
    //Ej: HisteresisSensor(1,2.0)
    //numero_zona: 0,1,2
    //histerisis: [0,2]
    ///<Summary>
    if (cmd.substring(0, 16) == "histeresissensor")
    {
      int n_zonas = cmd.substring(17, 18).toInt();
      sistema_calentamiento.zonas_calentar[n_zonas].sensor.histeresis = cmd.substring(19, 22).toFloat();
      Serial.print("Z: "); Serial.print(n_zonas); Serial.print(" H_SENS: ");
      Serial.println(sistema_calentamiento.zonas_calentar[n_zonas].sensor.histeresis);
    }
    //CONFIGURAR GANANCIA DE SENSOR ZONAS
    ///<Summary>
    //GananciaSensor(numero_zona,ganancia)
    //Ej:GananciaSensor(1,+00.0)
    //numero_zona = 0,1,2
    //ganancia = N
    ///<Summary>
    if (cmd.substring(0, 14) == "gananciasensor")
    {
      int n_zonas = cmd.substring(15, 16).toInt();
      sistema_calentamiento.zonas_calentar[n_zonas].sensor.ganancia = cmd.substring(17, 23).toFloat();
      Serial.print("Z: "); Serial.print(n_zonas); Serial.print(" Gsensor: ");
      Serial.println(sistema_calentamiento.zonas_calentar[n_zonas].sensor.ganancia);
    }
    //CONFIGURAR OFFSET DE SENSOR ZONAS
    if (cmd.substring(0, 12) == "offsetsensor")
    {
      //offsetsensor(2,+100.0)

      int n_zonas = cmd.substring(13, 14).toInt();
      sistema_calentamiento.zonas_calentar[n_zonas].sensor.offset = cmd.substring(15, 21).toFloat();
      Serial.print("Z: "); Serial.print(n_zonas); Serial.print("OffsetS: ");
      Serial.println(sistema_calentamiento.zonas_calentar[n_zonas].sensor.offset);
    }
    //limiteinferiorsensor(2,-100.0)
    if (cmd.substring(0, 20) == "limiteinferiorsensor")
    {
      int n_zonas = cmd.substring(21, 22).toInt();
      sistema_calentamiento.zonas_calentar[n_zonas].sensor.limiteInferior = cmd.substring(23, 29).toFloat();
      Serial.print("Z:"); Serial.println( n_zonas);
      Serial.print("LinfS:"); Serial.println(sistema_calentamiento.zonas_calentar[n_zonas].sensor.limiteInferior);
    }
    //limitesuperiorsensor(2,+100.0)
    if (cmd.substring(0, 20) == "limitesuperiorsensor")
    {
      int n_zonas = cmd.substring(21, 22).toInt();
      sistema_calentamiento.zonas_calentar[n_zonas].sensor.limiteSuperior = cmd.substring(23, 29).toFloat();
      Serial.print("Z:"); Serial.println( n_zonas);
      Serial.print("LsupS:"); Serial.println(sistema_calentamiento.zonas_calentar[n_zonas].sensor.limiteSuperior);
    }
    //UNA VEZ INTRODUCIDOS TODOS LOS PARAMETROS CONFIGURABLESS, INTROZCO EL SIGUIENTE COMANDO:
    //        guardarconfiguracion
    if (cmd.substring(0, 20) == "guardarconfiguracion")
    {
      Guardar_Parametros_Configurables_EEPROM();
    }

  }
}

void Botones()
{
  if (InterruptorEncendido(botonOn)) sistema_calentamiento.estado = encendido;
  else sistema_calentamiento.estado = apagado;
  if (InterruptorStandby(ModoViaje)) sistema_calentamiento.estado = standby;
  if (BotonReset(Reset))ProcesoReset();
}

bool InterruptorEncendido(int pin)
{
  bool estadoActual = digitalRead(pin);
  static bool estadoAnterior;
  static int estado;
  if (estadoAnterior == HIGH && estadoActual == LOW )
  {
    estado = 1 - estado;
  }
  estadoAnterior = estadoActual;
  if (estado == 1) return HIGH;
  else return LOW;
}

bool InterruptorStandby(int pin)
{
  bool estadoActual = digitalRead(pin);
  static bool estadoAnterior;
  static int estado;
  if (estadoAnterior == HIGH && estadoActual == LOW )
  {
    estado = 1 - estado;
  }
  estadoAnterior = estadoActual;
  if (estado == 1) return HIGH;
  else return LOW;
}

//TODO: REVISAR
bool BotonReset(int pin)
{
  if (digitalRead(pin) == LOW) return true;
  else return false;
}

//Cuando se pulsa el boton de RESET, se configuran los parámetros de FÁBRICA
void ProcesoReset()
{
  //Primero: borramos los parámetros configurables
  if (Borrar_Datos_Configurables() == true)
  {
    //Segundo: Si se han borrado->Cargamos los parámetros de fábrica, por defecto:
    Leer_Datos_Fabrica();
    Serial.println("RESETEO Ok");
    //Cuando se resetea el sistema se vuelve al estado de Encendido con los parametros predefinidos de fabrica
    sistema_calentamiento.estado = encendido;
  } else {

    Serial.println("FALLO AL RESETEAR");
  }

}
