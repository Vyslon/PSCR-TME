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
    cout << "main_prod terminé proprement" << endl;
}

void producteur (Stack<char> * stack) {
    char c ;
        while (cin.get(c)) {
            stack->push(c);
        }
}

int main () {
    int fd = shm_open("/myshm2", O_RDWR, 0666);
    size_t len = sizeof(Stack<char>);
    Stack<char> *s = (Stack<char>*)mmap(nullptr, len, PROT_WRITE, MAP_SHARED, fd, 0);
    if (s == MAP_FAILED) {
        ::perror(0);
    }
    producteur(s);
    /*
    struct sigaction sa;
    sigfillset(&sa.sa_mask);
    sa.sa_handler = &handler;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
     */
    munmap(s, len);

    return 0;
}

