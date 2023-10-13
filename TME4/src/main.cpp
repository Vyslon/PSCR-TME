#include "Banque.h"

using namespace std;

void worker(pr::Banque & bq) {
    int i, j, m;
    ::srand(::time(nullptr));
    for (int a = 0; a < 1000; a++)
    {
        i = ::rand() % 10 ;
        j = ::rand() % 10 ;
        m = ::rand() % 100 + 1 ;
        bq.transfert(i, j, m);
        // todo : this_thread::sleep_for (chrono::milliseconds(::rand() % 21));
    }
}

const int NB_THREAD = 10;
int main () {
    vector<thread> threads;
    vector<thread> comptables;
    int solde_attendu = 10 * 500;
    pr::Banque bq(10, 500);

	// TODO : creer des threads qui font ce qui est demandé
    // mettre une graine aléatoire pour rand
    for (int ai = 0; ai < NB_THREAD; ai++)
    {
        threads.emplace_back(worker, std::ref(bq));
        if (ai % 2 == 0)
            comptables.emplace_back(&pr::Banque::comptabiliser, &bq, solde_attendu);
    }

	for (auto & t : threads) {
		t.join();
	}

    for (auto & t2 : comptables) {
        t2.join();
    }

	// TODO : tester solde = NB_THREAD * JP
	return 0;
}
