void estadoStandby()
{
  LeerSensores();
  float TempZona[numero_zonas];
  static bool RiesgoCongelacion;
  static bool AltoRiesgoCongelacion;
  for (int zona = 0; zona < numero_zonas; zona++)
  {
    TempZona[zona] = sistema_calentamiento.zonas_calentar[zona].temperatura;
  }
  float tempMedia = TempMediaCasa(TempZona);
  #if DEBUG_ZONAS
  Serial.println("LA TEMPERATURA MEDIA DE LA CASA ES:");
  Serial.println(tempMedia);
  #endif
  
  Bascula(hysteris, tempMedia, Tobj1, &RiesgoCongelacion);
  Bascula(hysteris, tempMedia, Tobj2, &AltoRiesgoCongelacion);
  if (RiesgoCongelacion == true)
  {
    digitalWrite(ManiobraPrincipal, HIGH);
    digitalWrite(ValvulaBombaZona1, HIGH);
    digitalWrite(ValvulaBombaZona2 , HIGH);
    digitalWrite(ValvulaBombaZona3 , HIGH);
    if (AltoRiesgoCongelacion == true)
    {
      digitalWrite(Caldera, HIGH);
    } else {
      digitalWrite(Caldera, LOW);
    }
  } else
  {
    digitalWrite(ManiobraPrincipal, LOW);
    digitalWrite(ValvulaBombaZona1, LOW);
    digitalWrite(ValvulaBombaZona2, LOW);
    digitalWrite(ValvulaBombaZona3, LOW);
  }
}
float TempMediaCasa(float TempZona[numero_zonas])
{
  float sumaTemp;
  for (int zona = 0; zona < numero_zonas; zona++)
  {
    sumaTemp = sumaTemp + TempZona[zona];

  }
  return sumaTemp / numero_zonas;
}

void Bascula(float h, float Tact, float Tobj, bool *interruptorBascula)
{
  if (Tact < (Tobj - h))
  {
    *interruptorBascula = HIGH;
  }
  if (Tact > (Tobj + h))
  {
    *interruptorBascula = LOW;
  }
}
