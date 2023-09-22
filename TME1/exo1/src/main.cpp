#include "List.h"
#include <string>
#include <iostream>
#include <cstring>

int main () {

	std::string abc = "abc";
	char * str = new char [4];
	str[0] = 'a';
	str[1] = 'b';
	str[2] = 'c';
	// FAUTE : str n'avait que 3 caractères mais il lui en fallait 4 pour être comparé à abc (il manquait le \0 terminal)
	str[3] = '\0';
	size_t i = 0;

	if (! strcmp (str, abc.c_str())) {
		std::cout << "Equal !";
	}

	pr::List list;
	list.push_front(abc);
	list.push_front(abc);

	std::cout << "Liste : " << list << std::endl;
	std::cout << "Taille : " << list.size() << std::endl;

	// Affiche à l'envers
	// FAUTE : i étant un size_t, lorsqu'on est à 0 et qu'on décrémente, i prend la valeur SIZE_T_MAX
	for (i= list.size() - 1 ; i >= 0 && i < list.size() ; i--) {
		std::cout << "elt " << i << ": " << list[i] << std::endl;
	}

	// liberer les char de la chaine
	// FAUTE : str n'étant pas un tableau de pointeurs, delete [] str est suffisant (au contraire, ce fonctionnement n'est pas correct
//	for (char *cp = str ; *cp ; cp++) {
//		delete cp;
//	}
	// et la chaine elle meme
	// FAUTE : faire un delete [] car str est définie comme ça : char[]
	delete [] str;

}
