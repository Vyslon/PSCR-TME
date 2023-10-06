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
	public:
		class Entry {
			public:
				const K key;
				V value;
		};

		typedef std::vector<std::forward_list<Entry>> buckets_t;


		class iterator {
		public:
			size_t index;
			typename std::forward_list<Entry>::iterator curr;

			buckets_t & buck; // ou pointeur de hashmap

			iterator(buckets_t buck, size_t index, std::forward_list<Entry>::iterator curr) : buck(buck), index(index), curr(curr) {}

			// Gérer débordement ?
			void operator++() {
				++curr;
				if (curr == buck[index].end()){
					index++;
					while(index < buck.size() && buck[index].empty())
						index++;

					if (index < buck.size())
						curr = buck[index].begin();
				}
			}

			Entry & operator*() {
				return  *curr ;
			}

			bool operator!=(const iterator & other) {
				return curr != other.curr;
			}
		};


		iterator begin() {
			if (buckets.size() == 0)
				return end();

			size_t ind = 0;
			for (; ind < buckets.size(); ++ind) {
				if (!buckets[ind].empty())
					break;
			}
			return iterator(buckets, ind, buckets[ind].begin());
		}


		iterator end() {
			// TODO : modifier
			return iterator(buckets, 0,  buckets[0].end());
		}

		buckets_t buckets;

		HashMap(size_t size) : buckets(size) {}

		V* get(const K & key) {
			size_t h = std::hash<K>()(key)  % size();
			 for (auto & ent : buckets[h]){
				 if (ent.key == key) {
					 return &ent.value;
				 }
			 }
			 return nullptr;
		};

		 bool put (const K & key, const V & value) {
			 size_t h = std::hash<K>()(key) % size();
			 for (auto & ent : buckets[h]){
				 if (ent.key == key) {
					 ent.value = value;
					 return true;
				 }
			 }
			 buckets[h].emplace_front(key, value);
			 return false;
		 }

		 size_t size() const {
			 return buckets.size();
		 }

		 void grow() {
			 HashMap tmp(2 * buckets.size());
			 for (auto & b : buckets) {
				 for (auto & e : b) {
					 tmp.put(e.key, e.value);
				 }
			 }
			 this.buckets = tmp.buckets;
		 }
};


template <typename iterator>
size_t count (iterator begin, iterator end) {
	int i = 0;
	for (iterator it = begin; it != end; ++it)
	{
		++i;
	}
	return i;
}

template <typename iterator, typename T>
size_t count_if_equal (iterator begin, iterator end, const T & val)
{
	int i = 0;
	for (iterator it = begin; it != end; ++it)
	{
		if (*it == val)
			++i;
	}
	return i;
}

template <typename iterator>
iterator prochainItListeNonVide(iterator begin, iterator end) {
	for (auto & it = begin + 1; it != end; it++) {
		if (*it != nullptr) {
			return it;
		}
	}
	return nullptr;
}



int main () {
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("./tmp/WarAndPeace.txt");
	//vector<pair<string, int>> mots;
	HashMap<string, int> mots(20);

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
			if (p.key == word)
			{
				trouver = true;
				// Incrémenter valeur
				p.value++;
				break;
			}
		}

		int * valeur = mots.get(word);
		if (valeur != nullptr)
		{
			// Mot trouvé
			mots.put(word, *valeur + 1);
		}
		else
		{
			// Mot non trouvé
			mots.put(word, 1);
		}


		// word est maintenant "tout propre"
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		nombre_lu++;
	}
	input.close();

	// TODO :
	/*
	 * vector<pair<string, int>> vec;
	 * for (Entry & e : mots {
	 * 	vec.emplace_back(e.key, e.value
	 * 	}
	 * */

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

