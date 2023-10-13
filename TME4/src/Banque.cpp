#include "Banque.h"

#include <iostream>

using namespace std;

namespace pr {

void Banque::transfert(size_t deb, size_t cred, unsigned int val) {
    unique_lock<recursive_mutex> test(m);
	Compte & debiteur = comptes[deb];
	Compte & crediteur = comptes[cred];
    if (deb < cred)
    {
        debiteur.getMutex().lock();
        crediteur.getMutex().lock();
    }
    else if (deb > cred)
    {
        crediteur.getMutex().lock();
        debiteur.getMutex().lock();
    }
    else
    {
        return;
    }

	if (debiteur.debiter(val)) {
		crediteur.crediter(val);
	}
    debiteur.getMutex().unlock();
    crediteur.getMutex().unlock();
}
size_t Banque::size() const {
	return comptes.size();
}
bool Banque::comptabiliser (int attendu) const {
    unique_lock<recursive_mutex> test(m);
	int bilan = 0;
	int id = 0;
	for (const auto & compte : comptes) {
		if (compte.getSolde() < 0) {
			cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
		}
		bilan += compte.getSolde();
		id++;
	}
	if (bilan != attendu) {
		cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
	}
	return bilan == attendu;
}
}
