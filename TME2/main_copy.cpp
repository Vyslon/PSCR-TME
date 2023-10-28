// TODO : rajouter O2 dans la compilation

#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>
#include <chrono>
#include <string>
#include <forward_list>
#include <string>
#include <unordered_map>

using namespace std;

template <typename K, typename V>
class HashMap {
public:
    class Entry {
    public:
        const K key;
        V value;

        Entry(const K key, V value) : key(key), value(value) {}
    };

    typedef std::vector<std::forward_list<Entry> > buckets_t;
    buckets_t buckets;

    class iterator {
    public:
        size_t index;
        typename std::forward_list<Entry>::iterator curr;

        buckets_t & buck; // ou pointeur de hashmap

        iterator(buckets_t & buck, size_t index, typename std::forward_list<Entry>::iterator curr) : buck(buck), index(index), curr(curr)  {}

        iterator & operator++() {
            // TODO : ERREUR 1 | buck size = 1 ?????
            ++curr;
            if (curr == buck[index].end())
            {
                ++index;
                for(; index < buck.size() && buck[index].empty(); index++) {/* NOP */}

                if (index < buck.size())
                    curr = buck[index].begin();
            }
            return *this;
        }

        bool operator != (const iterator & o)
        {
            return index != o.index || curr != o.curr || &buck != &o.buck;
        }

        Entry & operator*() {
            return  *curr ;
        }
    };


    iterator begin() {
        for (int index = 0; index < buckets.size(); index++)
        {
            if (!buckets[index].empty())
                return iterator(buckets, index, buckets[index].begin());
        }
        return end();
    }

    iterator end() {
        return iterator(buckets, buckets.size(), buckets[0].end());
    }



    HashMap(size_t size) : buckets(size) {}

    V* get(const K & key) {
        size_t h = std::hash<K>()(key)  % buckets.size();
        for (auto & ent : buckets[h]){
            if (ent.key == key) {
                return &ent.value;
            }
        }
        return nullptr;
    };

    bool put (const K & key, const V & value) {
        // todo : if taille actuelle >= 80% du nombre de buckets : grow
        if (size() >= 0.8 * buckets.size())
        {
            grow();
        }


        size_t h = std::hash<K>()(key) % buckets.size();
        for (auto & ent : buckets[h]){
            if (ent.key == key) {
                ent.value = value;
                return true;
            }
        }
        buckets[h].emplace_front(key, value);
        return false;
    }

    // nombre de buckets non vides
    size_t size() const {
        int res = 0;
        for (int i = 0; i < buckets.size(); ++i)
        {
            if (!buckets[i].empty())
                res++;
        }
        return std::max(res, 1);
    }

    // TODO : agrandir HashMap
    void grow() {
        HashMap tmp(2 * buckets.size());
        for (auto & b : buckets) {
            for (auto & e : b) {
                tmp.put(e.key, e.value);
            }
        }
        this->buckets = std::move(tmp.buckets);
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
    using namespace std::chrono;

    ifstream input = ifstream("./tmp/WarAndPeace.txt");
    //vector<pair<string, int>> mots;
    unordered_map<string, int> mots(20);

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

        // mots.put(word, 0);

        unordered_map<string, int>::iterator cell = mots.find(word);
        if (cell != mots.end())
        {
            // TODO : on n'est jamais dans ce cas là
            mots.insert_or_assign(word, cell->second + 1);
        }
        else
        {
            mots.emplace(word, 1);
        }

        // TODO : utiliser mes fonctions tout en haut

        // word est maintenant "tout propre"
        if (nombre_lu % 100 == 0)
            // on affiche un mot "propre" sur 100
            cout << nombre_lu << ": "<< word << endl;
        nombre_lu++;
    }
    input.close();

    cout << "Finished Parsing War and Peace" << endl;

    vector<pair<string, int>> vec;
    // todo q7 : vector<pair<string, int>> vec(mots.begin(), mots.end());
    for (auto & e : mots) {
        vec.emplace_back(e.first, e.second);
    }

    std::sort(vec.begin(), vec.end(), [] (const pair<string, int> & a, const pair<string, int> & b) {
        return a.second > b.second;
    });

    for (int i = 0; i < 10; ++i)
    {
        cout << "Mot : " << vec[i].first << " | Nombre d'occurences : " << vec[i].second << "\n";
    }
    cout << endl;

    vector<int> test = {1, 1 ,4, 5, 3 ,4 ,8,1};

    cout << "count : " << count(mots.begin(), mots.end()) << endl;
    cout << "count if equal : " << count_if_equal(test.begin(), test.end(), 1) << endl;

    unordered_map<int,forward_list<string>> freq;
    unordered_map<int,forward_list<string>>::iterator cellFreq;
    for (auto & e : mots)
    {
        cellFreq = freq.find(e.second);
        if (cellFreq != freq.end())
        {
            // TODO : on ajoute 1 élément dans la forwardlist
            cellFreq->second.push_front(e.first);
        }
        else
        {
            // TODO on met 1 seul élément dans la forward list
            //freq.emplace(e.second, forward_list<string>{e.first});
            //freq[e.second] = forward_list<const string>{e.first};
            forward_list<string> newList{e.first};
            freq.emplace(e.second, newList);

        }
    }

    int i = 0;
    for (auto & it : freq) {
        if (it.first == 34562)
        {
            cout << "fréquence : " << to_string(it.first) << " mots : " << endl;
            for (auto & oui : it.second)
            {
                cout << oui << endl;
            }
        }
        i++;
    }

    auto end = steady_clock::now();
    cout << "Parsing took "
         << duration_cast<milliseconds>(end - start).count()
         << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;

    cout << "Nombre de mots différents : " << vec.size() << endl;



    return 0;
}
