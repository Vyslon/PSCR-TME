#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <vector>


using namespace std;
using namespace pr;

void producteur (Stack<char> * stack) {
	char c ;
	while (cin.get(c)) {
		stack->push(c);
	}
}

void consomateur (Stack<char> * stack) {
	while (true) {
		char c = stack->pop();
		cout << c << flush ;
	}
}

int doit = false;

int main () {
    size_t len = sizeof(Stack<char>);
    //int fd = shm_open("/myshm", O_CREAT|O_EXCL|O_RDWR, 0666);
    //ftruncate(fd, len);
    void * sp = mmap(nullptr, len, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	Stack<char> * s = new (sp) Stack<char>();

    // TODO sig_handler avant fork
	pid_t pp = fork();
	if (pp==0) {
		producteur(s);
		return 0;
	}

	pid_t pc = fork();
	if (pc==0) {
		consomateur(s);
		return 0;
	}

	wait(0);
	wait(0);

    munmap(sp, len);
    shm_unlink("/myshm");

	return 0;
}

