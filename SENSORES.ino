// Con esta función Leo todos los sensores siempre
void LeerSensores()
{
  sistema_calentamiento.zonas_calentar[0].temperatura = 
  LeerPotenciometro (SensorTemperaturaZona1,sistema_calentamiento.zonas_calentar[0].sensor );
  sistema_calentamiento.zonas_calentar[1].temperatura =
  LeerPotenciometro (SensorTemperaturaZona2,sistema_calentamiento.zonas_calentar[1].sensor );
  sistema_calentamiento.zonas_calentar[2].temperatura =
  LeerPotenciometro (SensorTemperaturaZona3,sistema_calentamiento.zonas_calentar[2].sensor );

  sistema_calentamiento.colectores_solares[0].temperatura =
  LeerPotenciometro (TemperaturaColector1, sistema_calentamiento.colectores_solares[0].sensor);
  sistema_calentamiento.colectores_solares[1].temperatura =
  LeerPotenciometro (TemperaturaColector2, sistema_calentamiento.colectores_solares[1].sensor);
  sistema_calentamiento.temperatura_acumulador = 
  LeerPotenciometro (TemperaturaAcumulador, sistema_calentamiento.sensor_acumulador);
}
//Funcion para lectura del potenciometro; valor filtrado y calibrado
float LeerPotenciometro(int pin,struct t_parametros_sensores miPotenciometro)
{
  static float lecturaAnterior[numero_sensores];
  float lecturaFiltrada  = 
  FiltroExponencial(calibracion(analogRead(pin), miPotenciometro), &lecturaAnterior[pin], 0.25);
  lecturaAnterior[pin] = calibracion(analogRead(pin), miPotenciometro);
  return lecturaFiltrada;
}


float calibracion( float lecturaAnalogica,struct t_parametros_sensores miPotenciometro)
{
  float ResolucionPinesAnalog = 1024.0;
  float maxVoltagePinesAnalog = 5.0;
  float voltageLeido = lecturaAnalogica * (maxVoltagePinesAnalog / ResolucionPinesAnalog);
  float rango = miPotenciometro.limiteSuperior +(-1*miPotenciometro.limiteInferior);
  float gradiente = (rango /maxVoltagePinesAnalog)*miPotenciometro.ganancia;
  return gradiente*voltageLeido + miPotenciometro.offset;
  
}
//FILTRO EXPONENCIAL: Para regular el ruido del sensor. 
//Xk valor actual; Yk_1: Valor anterior; a factor de filtraje; Yk=Valor de salida 
float FiltroExponencial(float Xk, float *Yk_1, float a)
{
 float Yk = *Yk_1 + a * (Xk - *Yk_1);
  *Yk_1 = Yk;
  return Yk; 
}
