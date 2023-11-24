#pragma once

#include <cstring> // size_t,memset
#include <semaphore.h>

namespace pr {

#define STACKSIZE 100

template<typename T>
class Stack {
	T tab [STACKSIZE];
    sem_t sempop; // Nb cases pleines
    sem_t sempush; // Nb cases vides
    sem_t semmut; // mutex
	size_t sz;
public :
	Stack () {
        sz = 0;
        sem_init(&sempop, 0, 0);
        sem_init(&sempush, 0, STACKSIZE);
        sem_init(&semmut, 0, 1);
        memset(tab,0,sizeof tab);
    }

    ~Stack () {
        // TODO : symétrique de memset
        sem_destroy(&sempop);
        sem_destroy(&sempush);
        sem_destroy(&semmut);
    }

	T pop () {
        sem_wait(&sempop);
        sem_wait(&semmut);
		// bloquer si vide
		T toret = tab[--sz];
        sem_post(&semmut);
        sem_post(&sempush);
		return toret;
	}

	void push(T elt) {
        sem_wait(&sempush);
        sem_wait(&semmut);
		tab[sz++] = elt;
        sem_post(&semmut);
        sem_post(&sempop);
	}
};

}
