#pragma once
template <typename T>
class Stack
{
private:
	T* arr;
	size_t capacity;
	size_t size;

	void resize();
	bool empty() const;
	void copyFrom(const Stack<T>& other);
	void free();
	void moveFrom(Stack<T>&& other);
	void validate(size_t index) const;

public:
	Stack();
	Stack(size_t cap);
	Stack(const Stack<T>& other);
	Stack& operator=(const Stack<T>& other);
	Stack(const Stack<T>&& other);
	Stack& operator=(Stack<T>&& other);
	~Stack();
	void pop();
	void push(const T& data);
	void push(T&& data);
	const T& peek() const;
};

template <typename T>
void Stack<T>::resize() {
	capacity *= 2;
	T* newArr = new T[capacity];

	for (size_t i = 0; i < size; i++) {
		newArr[i] = arr[i];
	}
	delete[] arr;
	arr = newArr;
}
template <typename T>
bool Stack<T>::empty() const {
	return size == 0;
}
template <typename T>
void Stack<T>::copyFrom(const Stack<T>& other) {
	size = other.size;
	capacity = other.capacity;
	arr = new T[capacity];

	for (size_t i = 0; i < size; i++) {
		arr[i] = other.arr[i];
	}
}
template <typename T>
void Stack<T>::free() {
	delete[] arr;
	arr = nullptr;
	size = capacity = 0;
}
template <typename T>
void Stack<T>::moveFrom(Stack<T>&& other) {
	arr = other.arr;
	size = other.size;
	capacity = other.capacity;
	other.arr = nullptr;
	other.size = other.capacity = 0;
}
template <typename T>
void Stack<T>::validate(size_t index) const {
	if (index >= size) {
		throw std::out_of_range("Invalid index!");
	}
}

template <typename T>
Stack<T>::Stack() {
	capacity = 8;
	arr = new T[capacity];
	size = 0;
}
template <typename T>
Stack<T>::Stack(size_t cap) {
	capacity = cap;
	arr = new T[capacity];
	size = 0;
}
template <typename T>
Stack<T>::Stack(const Stack<T>& other) {
	copyFrom(other);
}
template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T>& other) {
	if (this != &other) {
		free();
		copyFrom(other);
	}
	return *this;
}
template <typename T>
Stack<T>::Stack(const Stack<T>&& other) {
	moveFrom(std::move(other));
}
template <typename T>
Stack<T>& Stack<T>::operator=(Stack<T>&& other) {
	if (this != &other) {
		free();
		moveFrom(std::move(other));
	}
	return *this;
}
template <typename T>
Stack<T>::~Stack() {
	free();
}

template <typename T>
void Stack<T>::pop() {
	if (size == 0) {
		throw std::out_of_range("Stack is empty!");
	}
	size--;
}
template <typename T>
void Stack<T>::push(const T& data) {
	if (size == capacity) {
		resize();
	}
	arr[size++] = data;

}
template <typename T>
void Stack<T>::push(T&& data) {
	if (size == capacity) {
		resize();
	}
	arr[size++] = std::move(data);
}
template <typename T>
const T& Stack<T>::peek() const {
	if (size == 0) {
		throw std::out_of_range("Stack is empty!");
	}
	return arr[size - 1];
}