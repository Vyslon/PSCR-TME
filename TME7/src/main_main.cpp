//
// Created by Thomas SANTONI on 24/11/2023.
//
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
    cout << "main_main terminé proprement" << endl;
}

int main () {
    shm_unlink("/myshm2");
    size_t len = sizeof(Stack<char>);
    int fd = shm_open("/myshm2", O_CREAT | O_EXCL | O_RDWR, 0666);
    ftruncate(fd, len);
    void *sp = mmap(0, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (sp == MAP_FAILED) {
        ::perror(0);
    }
    Stack<char> *s = new(sp) Stack<char>();


    struct sigaction sa;
    sigfillset(&sa.sa_mask);
    sa.sa_handler = &handler;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    sigsuspend(&sa.sa_mask);


    shm_unlink("/myshm2");
    munmap(sp, len);
    return 0;
}