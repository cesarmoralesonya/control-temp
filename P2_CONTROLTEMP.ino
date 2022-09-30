#define DEBUG true
#define DEBUG_ACUMULADOR false
#define DEBUG_ZONAS true
#define DEBUG_COLECTORES true
//PRACTICA 2:
//PINES DEL ARDUINO QUE UTILIZAMOS
#define numero_colectores 2
#define numero_zonas 3
#define numero_sensores 6
//BOTONES
#define botonOn 12
#define Reset 11
#define ModoViaje 10
//VALVULAS Y BOMBAS: LEDS
#define ManiobraPrincipal 7 //Es una valvula y una bomba
#define ValvulaBombaZona1 4
#define ValvulaBombaZona2 5
#define ValvulaBombaZona3 6
//LEDS
#define Caldera 8
#define ColectorSolar1 2
#define ColectorSolar2 3
//POTENCIOMETROS PARA TEMPERATURA: 2 COLECTORES, 3 ZONAS, 1 ACUMULADOR
#define TemperaturaAcumulador 5
#define TemperaturaColector1 3
#define TemperaturaColector2 4
#define SensorTemperaturaZona1 0
#define SensorTemperaturaZona2 1
#define SensorTemperaturaZona3 2
//Temperaturas para el standby: MODO VIAJE, MODO AHORRO/ECONOMICO
#define Tobj1 10.0
#define Tobj2 5.0
#define hysteris 1.0

//ESTADOS DEL SISTEMA:
enum estados { encendido, apagado, standby};

//SECCION ESTRUCTURAS
//Estructura para hacer un reloj
typedef struct t_Reloj {
  short int hora;
  short int minutos;
  short int segundos;
};
typedef struct t_Calendario {
  short int year;
  short int mes;
  short int dia;
};

enum compararTiempo { menor, igual, mayor};
//Creamos una estructura que engloba el reloj y el calendario.
typedef struct t_Sistema_Tiempo {

  t_Reloj Reloj;
  t_Calendario Calendario;
};

//Estructura donde aparecen los parametros de los sensores
typedef struct t_parametros_sensores {
  float histeresis;
  float ganancia;
  float offset;
  float limiteInferior; //RANGOS
  float limiteSuperior;
};
typedef struct t_parametros_colector_solar {
  float temperaturaMaxima;
  t_Reloj tiempoActivacion;//PERIODO. NO SE COMPARA CON EL SENSOR
};
typedef struct t_colector_solar {
  t_parametros_colector_solar parametros;
  t_parametros_sensores sensor;
  unsigned long tiempo;
  unsigned long ultimoTiempo;
  bool estado;
  float temperatura;
};
typedef struct t_parametros_zona_calentar {
  t_Reloj On;
  t_Reloj Off;
  float temperatura_objetivo;
};
typedef struct t_zona_calentar {
  t_parametros_sensores sensor;//1 Sensor de temperatura por zona a calentar
  t_parametros_zona_calentar Configuracion;
  bool ActivarControlHorario; //Para activar/desactivar control horario de la zona
  bool ManiobraInterruptor;
  float temperatura;
};

//Estructura principal del sistema:
typedef struct t_sistema_calentamiento {
  t_Sistema_Tiempo Sistema_Tiempo;
  t_zona_calentar zonas_calentar[numero_zonas]; //array de estructuras. Array de 3 zonas = #define numero_zonas 3
  t_colector_solar colectores_solares[numero_colectores];
  t_parametros_sensores sensor_acumulador;
  float temperatura_acumulador;
  float min_temperatura_acumulador;//CONFIGURABLE
  bool AcumuladorInterruptor;
  short int estado;
  /*Creamos un bool para maniobra_principal y otro para cada zonas, para controlar cuando se activa/descctiva la maniobra principal
    En funcion de cada una de las zonas*/
  bool ManiobraPrincipalInterruptor;
  bool configurado;

};

t_sistema_calentamiento sistema_calentamiento;

//Esta estructura la utilizo para meter los datos en la eeprom:
typedef struct t_parametros_predefinidos {
  t_parametros_sensores parametros_predefinidos_sensores;
  bool Control_temp_time;
  float min_temperatura_acumulador;
  t_parametros_colector_solar parametros_colector_solar;
  t_parametros_zona_calentar parametros_zonas_calentar;
};
//Esta estructura la utilizo para meter los datos en la eeprom:
typedef struct t_parametros_configurables {
  t_parametros_colector_solar colector_solar[numero_colectores];
  t_parametros_sensores sensores[numero_sensores];
  t_parametros_zona_calentar Configuracion_Zonas[numero_zonas];
  float min_temperatura_acumulador;
};

void setup() {
  /*La primera vez que se ejecuta el programa, el sistema se encuentra apagado
     No tendría sentido que estuviese el sistema encendido al compralo
  */
  sistema_calentamiento.estado = apagado;
  IniciarReloj();
  ConfigurarInterfazUsuario();
  ConfigurarActuadores();
  Inicializar_Parametros_Predefinidos_EEPROM();
  #if DEBUG
  Serial.println(Comprobacion_Sistema_Configurado());
  #endif
  if (Comprobacion_Sistema_Configurado() == true) Leer_Datos_Configurables();
  else  Leer_Datos_Fabrica();
}

void loop()
{
  Interfaz_Usuario();
  if (sistema_calentamiento.estado == apagado)
  {
    MODOAPAGADO();
  }
  else if (sistema_calentamiento.estado == encendido)
  {
    Modo_Encendido();
  }
  else if (sistema_calentamiento.estado == standby)
  {
    estadoStandby();
  }
}
