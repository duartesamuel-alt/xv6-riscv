#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Programa de prueba para observar el planificador de XV6.
// Crea 3 procesos hijos que solo usan CPU (no esperan nada).
// Cada hijo hace 5 vueltas de trabajo e imprime cuando termina cada una.

#define NHIJOS 3
#define VUELTAS 5

void
trabajar(int n)
{
  volatile int x = 0;
  for(int i = 0; i < n; i++)
    x = x + 1;
}

int
main(int argc, char *argv[])
{
  int n = 20000000;
  if(argc > 1)
    n = atoi(argv[1]);

  printf("rrtest: padre pid %d crea %d hijos (trabajo por vuelta = %d)\n",
         getpid(), NHIJOS, n);

  for(int h = 1; h <= NHIJOS; h++){
    int pid = fork();
    if(pid < 0){
      printf("rrtest: fork fallo\n");
      exit(1);
    }
    if(pid == 0){
      // Codigo del hijo
      for(int v = 1; v <= VUELTAS; v++){
        trabajar(n);
        printf("[tick %d] hijo %d (pid %d) termino vuelta %d\n",
               uptime(), h, getpid(), v);
      }
      exit(0);
    }
  }

  // El padre espera a que terminen los 3 hijos
  for(int h = 0; h < NHIJOS; h++)
    wait(0);
  printf("rrtest: todos los hijos terminaron\n");
  exit(0);
}
