//
// Correction
//

// todo : iterator
iterator & operator++() {
    ++lit;
    if (lit == buck[index].end())
        ++index;
        for (; index < buck.size() && buck[index].empty(); index++)
        {}

        if (index < buck.size())
            lit = buck[index].begin();
    return *this;
}

iterator & operator++(int) {
	auto b4 = *this;
	++(*this);
	return b4;
}

bool operator != (const iterator & o) {
    // à ajouter?  :
    /*
     * if (index == buck.size() && o.index == index) {return false}
     * else {
     * => mettre la ligne return actuelle
     * }
     * */
    return index != o.index || lit != o.lit || &buck != &o.buck;
}

Entry & operator*() {
    return &lit;
}

// todo : dans notre classe map
iterator begin() {

    for (int index = 0; index < buck.size(); index++) {
        if (!bucket[index].empty())
            return iterator(index, buck[index].begin())
    }
    return end();
}

iterator end() {
    return iterator(buckets, buckets.size(), buckets[0].end())
}
