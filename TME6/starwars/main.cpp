#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "rsleep.h"

int pdv = 3;

void perdrePdv(int) {
	pdv--;
	std::cout << " PDV " << getpid() << pdv << std::endl;
	if (pdv == 0) {
		std::cout << "proc " << getpid << " est mort" << std::endl;
		exit(1);
	}
}

void attaque (pid_t adversaire) {
	struct sigaction sa = {0};
	sa.sa_handler = &perdrePdv;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	if (kill(adversaire, SIGINT) == -1) {
		exit(0);
	}
	randsleep();
}

void defense() {
	struct sigaction sa = {0};
	sa.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL)
	randsleep();
}

void combat(pid_t adversaire) {
	while(1) {
		defense();
		attaque(adversaire);
	}
}

int main () {
	int vador = fork();
	int luke = fork();

	if (getpid() == luke) {
		combat(vador);
	}
	else
	if (getpid() == vador) {
		combat(luke);
	}
	// TODO : faire exit dans le handler
}
