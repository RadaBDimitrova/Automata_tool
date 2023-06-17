#pragma once
template <typename T, typename D, typename K>
class Triple {
private:
	T first;
	D rel;
	K second;

public:
	Triple() = default;
	Triple(const T& first, const D& rel, const K& second);
	const T& getFirst() const;
	const D& getRel() const;
	const K& getSecond() const;

	void setFirst(const T& newValue);
	void setRel(const D& newValue);
	void setSecond(const K& newValue);
};

template <typename T, typename D, typename K>
Triple<T, D, K>::Triple(const T& first, const D& rel, const K& second) : first(first), rel(rel), second(second) {}

template <typename T, typename D, typename K>
const T& Triple<T, D, K>::getFirst() const {
	return first;
}

template <typename T, typename D, typename K>
const D& Triple<T, D, K>::getRel() const {
	return rel;
}

template <typename T, typename D, typename K>
const K& Triple<T, D, K>::getSecond() const {
	return second;
}

template <typename T, typename D, typename K>
void Triple<T, D, K>::setFirst(const T& newValue) {
	first = newValue;
}

template <typename T, typename D, typename K>
void Triple<T, D, K>::setRel(const D& newValue) {
	rel = newValue;
}

template <typename T, typename D, typename K>
void Triple<T, D, K>::setSecond(const K& newValue) {
	second = newValue;
}