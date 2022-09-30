void Modo_Encendido()
{

  LeerSensores();
  #if DEBUG_ZONAS
  Serial.print("Temperatura zona1: ");
  Serial.println(sistema_calentamiento.zonas_calentar[0].temperatura);
  
  Serial.print("Temperatura zona2: ");
  Serial.println(sistema_calentamiento.zonas_calentar[1].temperatura);

  Serial.print("Temperatura zona3: ");
  Serial.println(sistema_calentamiento.zonas_calentar[2].temperatura);
#endif
  for (int zona = 0; zona < numero_zonas; zona++)
  { //Si el tiempo de la zona programado es mayor que el de la hora actual. Se activa la temperatura, si es menor se desactiva.
    if ( (CompararTiempo (
            &sistema_calentamiento.zonas_calentar[zona].Configuracion.On,
            &sistema_calentamiento.Sistema_Tiempo.Reloj
          ) == mayor
          &&
          CompararTiempo (
            &sistema_calentamiento.zonas_calentar[zona].Configuracion.Off,
            &sistema_calentamiento.Sistema_Tiempo.Reloj
          ) == menor
         )
         &&
         sistema_calentamiento.zonas_calentar[zona].ActivarControlHorario == HIGH
       )
    {
      float TactZona = sistema_calentamiento.zonas_calentar[zona].temperatura;
      float TobjZona = sistema_calentamiento.zonas_calentar[zona].Configuracion.temperatura_objetivo;
      float h = sistema_calentamiento.zonas_calentar[zona].sensor.histeresis;
      Bascula(h, TactZona, TobjZona, &sistema_calentamiento.zonas_calentar[zona].ManiobraInterruptor);
      if (sistema_calentamiento.zonas_calentar[zona].ManiobraInterruptor == HIGH)
      {
        //encendemos bomba y valvula de la zona
        if (zona == 0) digitalWrite(ValvulaBombaZona1, HIGH);
        if (zona == 1) digitalWrite(ValvulaBombaZona2, HIGH);
        if (zona == 2) digitalWrite(ValvulaBombaZona3, HIGH);
      } else {
        //apagamos bomba y valvula de la zona
        if (zona == 0) digitalWrite(ValvulaBombaZona1, LOW);
        if (zona == 1) digitalWrite(ValvulaBombaZona2, LOW);
        if (zona == 2) digitalWrite(ValvulaBombaZona3, LOW);

      }
    } else {
      //Si la maniobra principal está apagada también lo están las otras zonas.
      //apagamos bomba y valvula de la zona
      if (zona == 0) digitalWrite(ValvulaBombaZona1, LOW);
      if (zona == 1) digitalWrite(ValvulaBombaZona2, LOW);
      if (zona == 2) digitalWrite(ValvulaBombaZona3, LOW);
    }
  }
//Si existe una zona encendida se tiene que activar tambien la maniobra principqal que es otro led
  if (AlgunaZonaEncendida() == true ) digitalWrite(ManiobraPrincipal, HIGH);
  else digitalWrite(ManiobraPrincipal, LOW);

 //PARA APAGADO/ENCENDIDO DEL ACUMULADOR

 //SI LA TEMPERATURA DEL ACUMULADOR ES INFERIOR A 40ºC (CONFIGURABLES) SE DEBE ACTIVAR LA CALDERA DE APOYO
static float histeresis_acumulador = 2.0;
#if DEBUG_ACUMULADOR
  Serial.print("Temperatura del acumulador: ");
  Serial.println(sistema_calentamiento.temperatura_acumulador);
#endif

//Se usa la Bascula de Smitch para saber si se encuentra dentro del rango:
  Bascula(histeresis_acumulador,
          sistema_calentamiento.temperatura_acumulador,
          sistema_calentamiento.min_temperatura_acumulador,
          &sistema_calentamiento.AcumuladorInterruptor);

  if (sistema_calentamiento.AcumuladorInterruptor == HIGH) digitalWrite(Caldera, HIGH);
  else digitalWrite(Caldera, LOW);

#if DEBUG_COLECTORES
  Serial.print("Temperatura COLECTOR_1: ");
  Serial.println(sistema_calentamiento.colectores_solares[0].temperatura);
  Serial.print("Temperatura COLECTOR_2: ");
  Serial.println(sistema_calentamiento.colectores_solares[1].temperatura);
#endif

// SI EL COLECTOR ALCANZA 60ºC SE ACTIVA EL LED durante un tiempo 
//Para eso se utiliza la funcion InterrutorTemporizador
  for (int colector = 0; colector < numero_colectores; colector++)
  {
    InterruptorTemporizador(
      sistema_calentamiento.colectores_solares[colector].temperatura,
      sistema_calentamiento.colectores_solares[colector].parametros.temperaturaMaxima,
      ObtenerPeriodo(sistema_calentamiento.colectores_solares[colector].parametros.tiempoActivacion),
      &sistema_calentamiento.colectores_solares[colector]
    );
    if (sistema_calentamiento.colectores_solares[colector].estado == HIGH)
    {
      if (colector == 0) digitalWrite(ColectorSolar1, HIGH);
      if (colector == 1) digitalWrite(ColectorSolar2, HIGH);
    }
    else
    {
      if (colector == 0) digitalWrite(ColectorSolar1, LOW);
      if (colector == 1) digitalWrite(ColectorSolar2, LOW);
    }
  }
}

//COLECTOR:
/*Interruptor Temperorizador: 
 * ACTIVA EL TEMPORIZADOR CON UN FLANCO POSITIVO, CUANDO SOBREPASA EL NIVEL 
 * SE QUEDA ACTIVADO EL LED HASTA QUE PASA UN DETERMINADO TIEMPO 
*/
void InterruptorTemporizador(float TempActual, float TempMax, unsigned long periodo, struct t_colector_solar *colector)
{
  static unsigned long tiempo;
  static unsigned long ultimoTiempo;
  if (TempActual < TempMax + 3.0)
  {
    if (TempActual >= TempMax) {
      colector->ultimoTiempo = millis();
      colector->estado = HIGH;
    }
  }
  colector->tiempo = millis() - colector->ultimoTiempo;
  if (colector->tiempo > periodo)
  {
    colector->estado = LOW;
    colector->tiempo = 0;
  }
}
//Funcion para convertir la hora del colector en milisegundos para usar la estrutura como periodo
unsigned long ObtenerPeriodo(struct t_Reloj Tiempo)
{
  unsigned long horas = long(Tiempo.hora) * 3600000;
  unsigned long minutos = long(Tiempo.minutos) * 60000;
  unsigned long segundos = long(Tiempo.segundos) * 1000;
  return (horas + minutos + segundos);
}

bool AlgunaZonaEncendida()
{
  if (digitalRead(ValvulaBombaZona1) == HIGH || digitalRead(ValvulaBombaZona2) == HIGH || digitalRead(ValvulaBombaZona3) == HIGH) {
    return true;
  } else {
    return false;
  }
}
