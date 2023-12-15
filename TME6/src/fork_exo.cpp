#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main () {
	const int N = 3;
	int pid_main = getpid();
	std::cout << "main pid=" << getpid() << std::endl;

	for (int i=1, j=N; i<=N && j==N && fork()==0 ; i++ ) {
		std::cout << " i:j " << i << ":" << j << std::endl;
		for (int k=1; k<=i && j==N ; k++) {
			if ( fork() == 0) {
				j=0;
				std::cout << "proc " << getpid() << " de père " << getppid() << std::endl;
				std::cout << " k:j " << k << ":" << j << std::endl;
			}

		}
	}

	for (int k = 0; k < N; k++)
	{
		wait(NULL);
	}

//		for (int k = 0; k < i; k++) {
//			if (wait(NULL) == -1)
//			{
//				std::cout << "trop de wait de " << getpid() << std::endl;
//			}
//		}
//		if (wait(NULL) != -1)
//			std::cout << "il en reste" << std::endl;




	return 0;
}
