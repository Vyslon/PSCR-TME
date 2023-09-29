#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>
#include <chrono>
#include <string>
#include <forward_list>
#include <string>


template <typename K, typename V>
class HashMap {
	class Entry {
		const K key;
		V value;
	};

	std::vector<std::forward_list<Entry>> buckets;

	public:
		HashMap(int size) {
			for (int i = 0; i < size; i++)
			{
				this->buckets.push_back(std::forward_list<Entry>());
			}
		}

		V* get(const K & key) {
			size_t h = std::hash<K>()(key)  % size();
			for (const auto & oui : buckets[h])
			{
				if (oui.key == key)
				{
					return oui.value;
				}
			}
			return nullptr;
		};

		 bool put (const K & key, const V & value) {
			 size_t h = std::hash<K>()(key) % size();
			 if (get(key) != nullptr)
			 {
				for (const auto & oui : buckets[h])
				{
					if (oui.key == key)
					{
						oui.value = value;
						return true;
					}
				}
			 }
			 else
			 {
				 buckets[h].emplace_front(key, value);
				 return false;
			 }
		 }

		 size_t size() const {
			 return buckets.size();
		 }
};


int main () {
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("./tmp/WarAndPeace.txt");
	//vector<pair<string, int>> mots;
	HashMap<string, int> mots;

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		bool trouver = false;

		//for (pair<string, int> & p : mots) {
		// on peut faire const auto &
		for (auto & p : mots) {
			if (p.first == word)
			{
				trouver = true;
				// Incrémenter valeur
				p.second++;
				break;
			}
		}

		int * valeur = mots.get(word);
		if (valeur != nullptr)
		{
			// Mot trouvé
			mots.put(word, valeur + 1);
		}
		else
		{
			// Mot non trouvé
			mots.put(word, 1);
		}

		// vérifier que le mot est présent dans le vecteur
		// TODO  : if (!trouver)
			// TODO  : mots.push_back(std::make_pair(word, 1));
			// meilleur : mots.emplace_back(word, 1);

		// word est maintenant "tout propre"
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		nombre_lu++;
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;

    cout << "Nombre de mots différents : " << mots.size() << endl;

    //for (pair<string, int> paire : mots)
    //{
    /* TODO
	if (paire.first == "war")
	{
		cout << "Nombre de fois war : " << mots.get("war") << endl;
	}
	else
	if (paire.first == "peace")
	{
		cout << "Nombre de fois peace : " << mots.get("peace") << endl;
	}
	else
	*/
	/*
	if (paire.first == "toto")
	{
		cout << "Nombre de fois toto : " << paire.second << endl;
	}
    } */


    return 0;
}

