/*El sistema Tiempo está compuesto por dos subsistemas:
   Reloj
   Calendario.
*/

#include <TimerOne.h>

//FUNCIONES

//Funcion para Iniciar el reloj.

void IniciarReloj()
{
  PonerHoraReloj();
  Timer1.initialize(1000000); //¿Cada cuanto tiempo quiero que se ejecuite la funcion? Cada segundo
  Timer1.attachInterrupt(RelojSistema); //Aqui le digo que funcion quiero que se ejecute. Cada segundo Ejecuta la funcion.¿¡¡¡¡ Tiene que ser tipo void.!!!!! Void nombre(void)
}
//Funcion para poner en hora el reloj

void PonerHoraReloj()
{
  char myreloj[9]; //00:00:00
  strcpy(myreloj, __TIME__); //Copia la hora de __TIME__ a la que se compilo el programa, y la COPIA en el array HoraConfiguracion

  sistema_calentamiento.Sistema_Tiempo.Reloj.hora = (myreloj[0] - 48) * 10 + myreloj[1] - 48;
  sistema_calentamiento.Sistema_Tiempo.Reloj.minutos = (myreloj[3] - 48) * 10 + myreloj[4] - 48;
  sistema_calentamiento.Sistema_Tiempo.Reloj.segundos = (myreloj[6] - 48) * 10 + myreloj[7] - 48;

}
//Funcion para leer el valor del reloj.
//Consiste en hacer un contador con el reloj.

void RelojSistema()
{
  sistema_calentamiento.Sistema_Tiempo.Reloj.segundos ++;

  if (sistema_calentamiento.Sistema_Tiempo.Reloj.segundos == 60) //Cuando los segundos llegan a 60, se ponen a 0 y se suma un minuto a minutos.
  {
    sistema_calentamiento.Sistema_Tiempo.Reloj.segundos = 0;
    sistema_calentamiento.Sistema_Tiempo.Reloj.minutos ++;
  }
  if (sistema_calentamiento.Sistema_Tiempo.Reloj.minutos == 60)
  {
    sistema_calentamiento.Sistema_Tiempo.Reloj.minutos = 0;
    sistema_calentamiento.Sistema_Tiempo.Reloj.hora++;
  }
  if (sistema_calentamiento.Sistema_Tiempo.Reloj.hora == 24) //Cuando las horas llegan a 24 se ponen a 0
  {
    sistema_calentamiento.Sistema_Tiempo.Reloj.hora = 0;
  }
}

//FUNCION PARA COMPARAR TIEMPO: COMPARAMOS LA HORA PROGRAMADA CON EL RELOJ TIEMPO REAL
//TIEMPO1 HORA PROGRAMADA; TIEMPO2 HORA RTC
int CompararTiempo (t_Reloj *tiempo1, t_Reloj *tiempo2)
{
  int comp;

  if (tiempo2->hora > tiempo1->hora) comp = mayor;
  if (tiempo2->hora < tiempo1->hora) comp = menor;
  if (tiempo2->hora == tiempo1->hora)
  {
    if (tiempo2->minutos > tiempo1->minutos) comp = mayor;
    if (tiempo2->minutos < tiempo1->minutos) comp = menor;
    if (tiempo2->minutos == tiempo1->minutos)
    {
      if (tiempo2->segundos > tiempo1->segundos) comp = mayor;
      if (tiempo2->segundos < tiempo1->segundos) comp = menor;
      if (tiempo2->segundos == tiempo1->segundos) comp = igual;
    }
  }
    return comp;
  }
