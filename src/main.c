#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>

#define MAX_CHILDREEN 4

int isPrimo(long int num) {
	if ((num != 2 && !(num%2)) || num<2) {
		return 0;
	} else {
		long int i;
		for (i = 2; i <= num/2; i++) {
			if (!(num%i)) {
				return 0;
			}
		}
		return 1;
	}
}

int main() {
	// Estas variaveis sao utilizadas para a leitura da entrada
	char line[1024];
	char const *p;
	long int x;
	long int y;
	int n;

	int n_childreen = 0;
	int *n_primos = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);

	fgets(line, 1024, stdin);

	pid_t pid=1;
	for(p = line; sscanf(p, "%ld%n", &x, &n) == 1; p += n) {
		pid = fork();
		if (pid == 0) {
			*n_primos += isPrimo(x);
			exit(0);
		} else {
			n_childreen++;
			if (n_childreen == MAX_CHILDREEN) {
				wait(NULL);
				n_childreen--;
			}
		}
	}

	while(wait(NULL) > 0) ; // Espera todos os filhos retornarem.

	printf("%d\n", *n_primos);
	munmap(n_primos, sizeof(int));

	return 0;
}
