#pragma once
#include <iostream>
#include <utility>

template <typename T>
class Vector
{
private:
	T* arr;
	size_t capacity;
	size_t size;

	void resize();
	bool empty() const;
	void copyFrom(const Vector<T>& other);
	void free();
	void moveFrom(Vector<T>&& other);
	bool validate(size_t index) const;

public:
	Vector();
	Vector(size_t cap);
	Vector& operator=(const Vector& other);
	Vector& operator=(Vector&& other); // move
	Vector(const Vector& other);
	Vector(Vector&& other); //move
	void push_back(const T& data);
	void push_back(const T&& data);
	void insert(const T& data, size_t index);
	void insert(const T&& data, size_t index);
	T& operator[](size_t index);
	const T& operator[](size_t index) const;
	void erase(size_t index);
	void erase(size_t first, size_t last);
	void pop_back();
	void clear();
	void swap(Vector& other); //
	size_t getSize() const;
	size_t getCap() const;
	void setSize(size_t s);
	void resize(size_t newCap);
	bool contains(const T& data) const;
	size_t find(const T& data) const;

	const static size_t nopos = -1; //return for not found
	~Vector();
};

template <typename T>
Vector<T>::Vector() {
	capacity = 8;
	arr = new T[capacity];
	size = 0;
}

template <typename T>
Vector<T>::Vector(size_t cap) {
	capacity = cap;
	arr = new T[capacity];
	size = 0;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
	if (this != &other) {
		free();
		copyFrom(other);
	}

	return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) {
	if (this != &other) {
		free();
		moveFrom(std::move(other));
	}

	return *this;
}

template <typename T>
Vector<T>::Vector(const Vector<T>& other) {
	copyFrom(other);
}

template <typename T>
Vector<T>::Vector(Vector<T>&& other) {
	moveFrom(std::move(other));
}

template <typename T>
void Vector<T>::push_back(const T& data) {
	if (size == capacity) {
		resize();
	}
	arr[size++] = data;
}

template <typename T>
void Vector<T>::push_back(const T&& data) {
	if (size == capacity) {
		resize();
	}
	arr[size++] = std::move(data);
}

template <typename T>
void Vector<T>::insert(const T& data, size_t index) {
	validate(index);
	size++;
	if (size == capacity) {
		resize();
	}
	for (size_t i = size - 1; i > index; i--) {
		arr[i] = arr[i - 1];
	}
	arr[index] = data;
}

template <typename T>
void Vector<T>::insert(const T&& data, size_t index) {
	validate(index);
	size++;
	if (size == capacity) {
		resize();
	}
	for (size_t i = size - 1; i > index; i--) {
		arr[i] = arr[i - 1];
	}
	arr[index] = std::move(data);
}

template <typename T>
T& Vector<T>::operator[](size_t index) {
	validate(index);
	return arr[index];
}

template <typename T>
const T& Vector<T>::operator[](size_t index) const {
	validate(index);
	return arr[index];
}

template <typename T>
bool Vector<T>::validate(size_t index) const {
	if (index >= size) {
		throw std::out_of_range("Invalid index!");
	}
	return true;
}

template <typename T>
void Vector<T>::erase(size_t index) {
	validate(index);
	if (size == 0) {
		throw std::out_of_range("Vector is empty!");
	}
	for (size_t i = index; i < size - 1; i++) {
		arr[i] = arr[i + 1];
	}
	size--;
}

template <typename T>
void Vector<T>::erase(size_t first, size_t last) {
	validate(first);
	validate(last);
	if (size == 0) {
		throw std::out_of_range("Vector is empty!");
	}
	for (size_t i = first; i < last; i++) {
		erase(i);
	}
}

template <typename T>
void Vector<T>::pop_back() {
	if (size == 0) {
		throw std::out_of_range("Vector is empty!");
	}
	size--;
}

template <typename T>
void Vector<T>::clear() {
	if (size == 0) {
		throw std::out_of_range("Vector is already empty!");
	}
	size = 0;
}

template <typename T>
void Vector<T>::swap(Vector& other) {
	size_t temp = capacity;
	capacity = other.capacity;
	other.capacity = temp;
	temp = size;
	size = other.size;
	other.size = temp;
	T* ptr = arr;
	arr = other.arr;
	other.arr = ptr;
}

template <typename T>
size_t Vector<T>::getSize() const {
	return size;
}

template <typename T>
size_t Vector<T>::getCap() const {
	return capacity;
}

template <typename T>
void Vector<T>::resize(size_t newCap) {
	capacity = newCap;
	T* newArr = new T[capacity];

	for (size_t i = 0; i < size; i++) {
		newArr[i] = arr[i];
	}
	delete[] arr;
	arr = newArr;
}

template <typename T>
bool Vector<T>::contains(const T& data) const {
	for (size_t i = 0; i < getSize(); i++) {
		if (arr[i] == data) {
			return true;
		}
	}
	return false;
}
template <typename T>
size_t Vector<T>::find(const T& data) const {
	for (size_t i = 0; i < getSize(); i++) {
		if (arr[i] == data) {
			return i;
		}
	}
	return nopos;
}

template <typename T>
void Vector<T>::setSize(size_t s) {
	size = s;
}


template <typename T>
Vector<T>::~Vector() {
	free();
}

template <typename T>
void Vector<T>::resize() {
	capacity *= 2;
	T* newArr = new T[capacity];

	for (size_t i = 0; i < size; i++) {
		newArr[i] = arr[i];
	}
	delete[] arr;
	arr = newArr;
}

template <typename T>
bool Vector<T>::empty() const {
	return size == 0;
}

template <typename T>
void Vector<T>::copyFrom(const Vector<T>& other) {
	size = other.size;
	capacity = other.capacity;
	arr = new T[capacity];

	for (size_t i = 0; i < size; i++) {
		arr[i] = other.arr[i];
	}
}

template <typename T>
void Vector<T>::free() {
	delete[] arr;
	arr = nullptr;
	size = capacity = 0;
}

template <typename T>
void Vector<T>::moveFrom(Vector<T>&& other) {
	arr = other.arr;
	size = other.size;
	capacity = other.capacity;
	other.arr = nullptr;
	other.size = other.capacity = 0;
}