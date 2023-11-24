#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <vector>
#include <csignal>


using namespace std;
using namespace pr;

void handler(int sig) {
    cout << "main_cons terminé proprement" << endl;
}

void consomateur (Stack<char> * stack) {
    while (true) {
        char c = stack->pop();
        cout << c << flush ;
    }
}

int main () {
    int fd = shm_open("/myshm2", O_RDWR, 0666);
    size_t len = sizeof(Stack<char>);
    Stack<char> *s = (Stack<char>*)mmap(nullptr, len, PROT_WRITE, MAP_SHARED, fd, 0);
    if (s == MAP_FAILED) {
        ::perror(0);
    }
    consomateur(s);
    struct sigaction sa;
    sigfillset(&sa.sa_mask);
    sa.sa_handler = &handler;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    shm_unlink("/myshm2");

    return 0;
}

