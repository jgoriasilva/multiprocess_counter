/* Nome: João Antônio Gória Silva
 * RA: 199567
 */


#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define N_FILHOS 4
#define SIZE 256

int main(){
	int flag = 0, count_filho=0;
	unsigned long long n;
	char line[SIZE], *val;
	char delims[] = " \n";
	pid_t filhos[N_FILHOS]; 
	int pipefd[2];
	
	/* Definir flags de protecao e visibilidade de memoria */
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  /* Criar area de memoria compartilhada */
  int *count;
  count = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);

	/* Definir pipe de comunicação entre processos */
	pipe(pipefd);

	/* Ler entrada do programa */
	fgets(line, SIZE, stdin);
	val = strtok(line, delims);
	//printf("val: %s\n", val);


	/* Colocar cada número no pipe, individualmente */
	for(;;){
    if(sscanf(val, "%llu", &n)){
	    write(pipefd[1], &n, sizeof(n));
			val = strtok(NULL, delims);
			//printf("val: %s\n", val);
			if(!(val == NULL))
				sscanf(val, "%llu", &n);
			else
				break;
		}
  }
	/* Fecha terminação de escrita do pipe (nada mais é escrito no pipe) */
	close(pipefd[1]);

	//printf("Loop terminado\n");

	/* Gera N_FILHOS filhos para processamento dos números */
	for(char j=0; j<N_FILHOS; j++){
		//printf("Gerando filho %d...\n", j);
		filhos[j] = fork();
		//printf("PID do filho %d = %d\n", j, filhos[j]);
		if(filhos[j] == 0){
			//printf("Filho %d gerado\n", j);
			unsigned long long n;
			char flag=1;
			close(pipefd[1]);
			while(read(pipefd[0], &n, sizeof(n))>0){
				//printf("Proc. filho %d escreve: %llu\n", j, n);
				/* Verifica se o número é primo */
				if(n==1)
					flag=0;
				else{
					for(unsigned long long i = 2; i < n/2; i++){
						if(n%i==0){
							flag = 0;
							//printf("Filho %d diz que %llu nao eh primo!\n", j, n);
							break;
						}
					}
				}
				if(flag){
					//printf("Filho %d diz que %llu eh primo!\n", j, n);
					(*count)++;
				}
			}
			/* Ao sair do processo filho, fecha saída de leitura do pipe */
			close(pipefd[0]);
			//printf("Saindo do processo %d\n", j);
			exit(0);
		}
			//printf("Pai de numero %d\n", j);
	}

	/* Espera finalização de todos os processos filhos */
	//printf("Todos os filhos foram gerados. Esperando...\n");
  for (char j=0; j<N_FILHOS; j++) {
    waitpid(filhos[j], NULL, 0);
  }

  //printf("Todos os filhos terminaram!\n");
	//printf("Numero de primos = %d\n", *count);
	printf("%d\n", *count);

	return(0);
}
