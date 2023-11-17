#pragma once

#include <cstring> // size_t,memset
#include <semaphore.h>

namespace pr {

#define STACKSIZE 100

template<typename T>
class Stack {
	T tab [STACKSIZE];
    sem_t *semp; // Nb cases pleines
    sem_t *semv; // Nb cases vides
    // TODO : mutex ?
	size_t sz;
public :
	Stack () {
        sz = 0;
        memset(tab,0,sizeof tab);
        sem_init(semp, 0, 0);
        sem_init(semv, 0, STACKSIZE);
    }

	T pop () {
		// bloquer si vide
		T toret = tab[--sz];
		return toret;
	}

	void push(T elt) {
		//bloquer si plein
		tab[sz++] = elt;
	}
};

}
