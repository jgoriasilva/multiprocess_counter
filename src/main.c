#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define N_FILHOS 4


int main(){
	int n, flag = 0, count_filho=0;
	char temp;
	pid_t filhos[N_FILHOS]; 
	int pipefd[2];
	
	pipe(pipefd);
	
	for(;;){
		printf("Escaneando...\n");
    scanf("%d%c", &n, &temp);
		printf("a");
    close(pipefd[0]);
		printf("b");
    write(pipefd[1], &n, sizeof(n));

    if(temp=='\n')
      break;
  }

	
	for(char j=0; j<N_FILHOS; j++){
		filhos[count_filho] = fork();
		if(filhos[count_filho] = 0){
			printf("Filho %d gerado\n", count_filho);
			int n;
			close(pipefd[1]);
			while(read(pipefd[0], &n, sizeof(n))){
				printf("Proc. filho %d escreve: %d\n", count_filho, n);
				// processa primo
			}
		exit(1);
		}
	}

	
	printf("Filhos gerados...\n");	

	printf("Todos os filhos foram gerados. Esperando...\n");
  for (char j=0; j<N_FILHOS; j++) {
    waitpid(filhos[j], NULL, 0);
  }

  printf("Todos os filhos terminaram!");


	return(0);
}
