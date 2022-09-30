#include <EEPROM.h>
#define eeAdress_ParametrosPredefinidos 0
#define eeAdress_ParametrosConfigurables sizeof(t_parametros_predefinidos)+1


/*
   EEPROMSeed_Initialize_PredefineParameters()
   Read PredefineParameters exist. I
   If PredefineParameters doesn't exist
   put the structure in EEPROM
*/

/*Format CRUD Structs:
   Create_NameStruct()
   Read_NameStruct()
   Delete_NameStruct()
*/

bool Crear_Datos_Fabrica(t_parametros_predefinidos parametros_predefinidos)
{
  //Guardamos
  EEPROM.put(eeAdress_ParametrosPredefinidos, parametros_predefinidos);
  //Comprobamos
  for (int address = eeAdress_ParametrosPredefinidos; address < eeAdress_ParametrosConfigurables; address++)
  {
    if (EEPROM.read(address) != 0) return true;
  }
  return false;
}

void Leer_Datos_Fabrica()
{
  //Obtenemos datos EEPROM
  t_parametros_predefinidos ParametrosPredefinidos;
  EEPROM.get(eeAdress_ParametrosPredefinidos, ParametrosPredefinidos);
  //Cargamos en sistema
  sistema_calentamiento.sensor_acumulador = ParametrosPredefinidos.parametros_predefinidos_sensores;
  sistema_calentamiento.min_temperatura_acumulador = ParametrosPredefinidos.min_temperatura_acumulador;
  for (int colector = 0; colector < numero_colectores; colector++)
  {
    sistema_calentamiento.colectores_solares[colector].sensor = ParametrosPredefinidos.parametros_predefinidos_sensores;
    sistema_calentamiento.colectores_solares[colector].parametros = ParametrosPredefinidos.parametros_colector_solar;
  }
  for (int zona = 0; zona < numero_zonas; zona++)
  {
    sistema_calentamiento.zonas_calentar[zona].sensor = ParametrosPredefinidos.parametros_predefinidos_sensores;
    sistema_calentamiento.zonas_calentar[zona].Configuracion = ParametrosPredefinidos.parametros_zonas_calentar;
  }
}

bool Borrar_Datos_Fabrica()
{
  //Borramos
  for (int address = eeAdress_ParametrosPredefinidos; address < sizeof(t_parametros_predefinidos); address++)
  {
    EEPROM.write(address, 0);
  }
  //Comprobamos
  //En caso de que se devuelva true, se habrán borrado los parametros por defecto
  for (int address = eeAdress_ParametrosPredefinidos; address < eeAdress_ParametrosConfigurables; address++)
  {
    if (EEPROM.read(address) != 0) return false;
  }
  return true;
}
/*FUNCIONES PARA:
   CREAR

   BORRAR
   LEER
   DE LOS DATOS CONFIGURABLES
*/
bool Crear_Datos_Configurables(t_parametros_configurables Parametros_Configurables)
{
  //Guardar
  EEPROM.put(eeAdress_ParametrosConfigurables, Parametros_Configurables);
  //Comprobar que los datos se han guardado
  for (int adress = eeAdress_ParametrosConfigurables; adress <= (eeAdress_ParametrosConfigurables + sizeof(t_parametros_configurables)); adress++)
  {
    if (EEPROM.read(adress) != 0) return true;
  }
  return false;
}

bool Borrar_Datos_Configurables()
{
  //Borro
  for (int adress = eeAdress_ParametrosConfigurables; adress <= (eeAdress_ParametrosConfigurables + sizeof(t_parametros_configurables)); adress++)
  {
    EEPROM.write(adress, 0);
  }
  //Compruebo

  for (int adress = eeAdress_ParametrosConfigurables; adress <= (eeAdress_ParametrosConfigurables + sizeof(t_parametros_configurables)); adress++)
  {
    if ( EEPROM.read(adress) != 0) return false; //Si hay una casilla distinta de 0, me devuelve false, no se ha borrado
  } return true; //Si todas las casillas son 0, significa que se ha borrado.
}

/*LOS PARAMETROS CONFIGURABLES SE GUARDAN EN LA EEPROM:
 * Los configuro por ventana de comandos, la primera vez y los guardo en la EEPROM
 * En caso de irse la alimentacion esos comandos no tengo que volverlos a meter. 
 * Con el RST los parametros configurables se BORRAN Y SOLO QUEDAN LOS DE FÁBRICA.
 */
void Leer_Datos_Configurables()
{
  //añadir el comando de horario activado como parametroconfigurable pa k se me quede en la eeprom
  //Obtener datos EEPROM
  t_parametros_configurables parametros_configurables;
  EEPROM.get(eeAdress_ParametrosConfigurables, parametros_configurables);
  for (int colector = 0; colector < numero_colectores; colector++)
  {
    sistema_calentamiento
    .colectores_solares[colector].parametros =
      parametros_configurables.colector_solar[colector];
  }
  sistema_calentamiento
  .zonas_calentar[0].sensor = parametros_configurables.sensores[0];
  sistema_calentamiento
  .zonas_calentar[1].sensor = parametros_configurables.sensores[1];
  sistema_calentamiento
  .zonas_calentar[2].sensor = parametros_configurables.sensores[2];
  sistema_calentamiento
  .colectores_solares[0].sensor = parametros_configurables.sensores[3];
  sistema_calentamiento
  .colectores_solares[1].sensor = parametros_configurables.sensores[4];
  sistema_calentamiento
  .sensor_acumulador = parametros_configurables.sensores[5];
  for (int zona = 0; zona < numero_zonas; zona++)
  {
    sistema_calentamiento
    .zonas_calentar[zona].Configuracion = parametros_configurables.Configuracion_Zonas[zona];
  }
  sistema_calentamiento.min_temperatura_acumulador =
    parametros_configurables.min_temperatura_acumulador;

}


void Inicializar_Parametros_Predefinidos_EEPROM()
{
  /*1. SE COMPRUEBA SI EXISTEN PARAMETROS PREDEFINIDOS O NO EN LA EEPROM
     2. SI NO LOS HAY YA, SE INTRODUCEN
     3. SE COMPRUEBA SI SE HAN INTRODUCIDO CORRECTAMENTE
  */
  //Comprobación de que existen los parametros predefinidos en la EEPROM
  bool Parametros_predefinidos_en_EEPROM = false;
  for (int Address = eeAdress_ParametrosPredefinidos; Address < eeAdress_ParametrosConfigurables; Address ++)
  {
    if (EEPROM.read(Address) != 0) Parametros_predefinidos_en_EEPROM = true;
  }
  //Entonces si los parametros predefinidos es false: NO HAY, se introducen:
  if (Parametros_predefinidos_en_EEPROM == false)
  {
    t_parametros_sensores parametros_predefinidos_sensores{
      histeresis: 1.5,
      ganancia: 1.5,
      offset: 0.5,
limiteInferior: -10.0,
      limiteSuperior: 60.0
    };
    t_Reloj tiempoActivacionColectorSolar{
      hora: 0,
      minutos: 0, //20
      segundos: 50
    };
    t_parametros_colector_solar parametros_colector_solar
    {
      temperaturaMaxima: 60.0,
tiempoActivacion: tiempoActivacionColectorSolar,
    };
    t_Reloj On{
      hora: 20,
      minutos: 30,
      segundos: 00
    };
    t_Reloj Off{
      hora: 23,
      minutos: 30,
      segundos: 00
    };
    t_parametros_zona_calentar parametros_zona_calentar{
On: On,
Off: Off,
      temperatura_objetivo: 25
    };

    t_parametros_predefinidos ParametrosPredefinidos
    {
parametros_predefinidos_sensores: parametros_predefinidos_sensores,
Control_temp_time: false,
      min_temperatura_acumulador: 40.0,
parametros_colector_solar: parametros_colector_solar,
parametros_zonas_calentar: parametros_zona_calentar
    };

    //COMPROBAR SI SE HAN GUARDADO O NO
    if (Crear_Datos_Fabrica(ParametrosPredefinidos))
    {
      Serial.println("PREDEF_ok");
    }
  }
}

void Guardar_Parametros_Configurables_EEPROM()
{
  //Obtenener datos del sistema
  t_parametros_configurables parametros_configurables;
  for (int colector = 0; colector < numero_colectores; colector++)
  {
    parametros_configurables.colector_solar[colector] = sistema_calentamiento
        .colectores_solares[colector].parametros;
  }
  parametros_configurables.sensores[0] = sistema_calentamiento
                                         .zonas_calentar[0].sensor;
  parametros_configurables.sensores[1] = sistema_calentamiento
                                         .zonas_calentar[1].sensor;
  parametros_configurables.sensores[2] = sistema_calentamiento
                                         .zonas_calentar[2].sensor;
  parametros_configurables.sensores[3] = sistema_calentamiento
                                         .colectores_solares[0].sensor;
  parametros_configurables.sensores[4] = sistema_calentamiento
                                         .colectores_solares[1].sensor;
  parametros_configurables.sensores[5] = sistema_calentamiento
                                         .sensor_acumulador;
  for (int zona = 0; zona < numero_zonas; zona++)
  {
    parametros_configurables.Configuracion_Zonas[zona] = sistema_calentamiento
        .zonas_calentar[zona].Configuracion;
  }
  parametros_configurables.min_temperatura_acumulador =
    sistema_calentamiento.min_temperatura_acumulador;

  //Comprobar resultado de crear datos en la EEPROM
  if (Crear_Datos_Configurables(parametros_configurables))
  {
    Serial.println("CONFIG_OK");
  }
}

/*Con esta funcion se recorre todas las celdas de la eprom una a una,
  el tamaño del for coincide con la longitud que ocupa en memoria los parametros configurables
  En caso de que encuentre un byte que valga 0, devuelve FALSE: NO ESTÁ CONFIGURADO EL SISTEMA
  En caso de que la respuesta sea TRUE, estarán las celdas llenas

*/
//SE UTILIZA EN EL SETUP ANTES DE EMPEZAR
bool Comprobacion_Sistema_Configurado()
{
  for (int adress = eeAdress_ParametrosConfigurables; adress <= (eeAdress_ParametrosConfigurables + sizeof(t_parametros_configurables)); adress++)
  {
    if (EEPROM.read(adress) !=0) return true;
  }
  return false;
}
