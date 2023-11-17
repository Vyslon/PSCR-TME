//
// Created by Thomas SANTONI on 17/11/2023.
//
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <unistd.h>

//  TODO : Scanner argv à la recherche d'un pipe
int main (int argc, char *argv[]) {

    for (int i = 0; i < argc - 1; i++)
    {
        printf("%s\n",argv[i]);
    }

    int cpt = 0;
    int pid_fils;
    while (strcmp(argv[cpt], "|")) {
        cpt++;
        if (cpt == argc)
            break;
    }

    char *cmd[cpt - 1];
    char *cmd2[argc - cpt];
    for (int i = 1; i < cpt; i++) {
        // todo : remplir cmd
        cmd[i - 1] = argv[i];
    }
    cmd[cpt - 1] = nullptr;

    for (int i = cpt + 1; i < argc; i++) {
        // todo : remplir cmd2
        cmd2[i - (cpt + 1)] = argv[i];
    }
    cmd2[argc - cpt - 1] = nullptr;

    int psc[2];
    if (pipe(psc) == -1) {
        perror("pipe");
        exit(1);
    }
    pid_fils = fork();
    if (pid_fils == -1) {
        perror("fork");
        exit(2);
    }

    if (pid_fils == 0) { /* fils */
        dup2(psc[1], STDOUT_FILENO);
        close(psc[1]);
        close(psc[0]);
        if (execv(argv[1], cmd) == -1) {
            perror("execv");
            exit(3);
        }
    } else if (fork() == 0) {
        dup2(psc[0], STDIN_FILENO);
        close(psc[0]);
        close(psc[1]);
        if (execv(cmd2[0], cmd2) == -1) {
            perror("execv");
            exit(3);
        }
    }

    close(psc[0]);
    close(psc[1]);

    wait(NULL);
    wait(NULL);
}
