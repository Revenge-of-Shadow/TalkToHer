template <typename T> class SimpleList {
  int size;
  T *elements;

public:
  SimpleList() : size(0), elements(nullptr) {}
  ~SimpleList() { delete[] elements; }

  int getSize() { return size; }

  int add(T element) {
    T *newElements = new T[size + 1];
    for (int i = 0; i < size; ++i) {
      newElements[i] = elements[i];
    }
    newElements[size] = element;
    delete[] elements;
    elements = newElements;

    return ++size;
  }

  T peek(int index) { return elements[index]; }
  T* getPtr(int index){ return &elements[index]; }
  T pop(int index) {
    T popped;
    T *newElements = new T[size - 1];
    for (int i = 0; i < size; ++i) {
      if (i == index)
        popped = elements[i];
      else if (i > index)
        newElements[i - 1] = elements[i];
      else
        newElements[i] = elements[i];
    }
    delete[] elements;
    elements = newElements;
    --size;

    return popped;
  }
};
