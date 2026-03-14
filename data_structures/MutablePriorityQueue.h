/*
 * MutablePriorityQueue.h
 * A simple implementation of mutable priority queues, required by Dijkstra algorithm.
 *
 * Created on: 17/03/2018
 *      Author: João Pascoal Faria
 */

#ifndef DA_TP_CLASSES_MUTABLEPRIORITYQUEUE
#define DA_TP_CLASSES_MUTABLEPRIORITYQUEUE

#include <vector>

/**
 * class T must have: (i) accessible field int queueIndex; (ii) operator< defined.
 */

template <class T>
class MutablePriorityQueue {
    std::vector<T *> H;
    void heapifyUp(unsigned i);
    void heapifyDown(unsigned i);
    inline void set(unsigned i, T * x);
public:
    MutablePriorityQueue();
    void insert(T * x);
    T * extractMin();
    void decreaseKey(T * x);
    bool empty();
};

// Index calculations
#define parent(i) ((i) / 2)
#define leftChild(i) ((i) * 2)

template <class T>
MutablePriorityQueue<T>::MutablePriorityQueue() {
    H.push_back(nullptr);
    // indices will be used starting in 1
    // to facilitate parent/child calculations
}

template <class T>
bool MutablePriorityQueue<T>::empty() {
    return H.size() == 1;
}

template <class T>
T* MutablePriorityQueue<T>::extractMin() {
    auto x = H[1]; // Extrair para x o valor mínimo
    H[1] = H.back(); // Colocar o último valor da queue no local onde o nó será extraído
    H.pop_back(); // Remover último elemento
    // Se a queue tem mais de um elemento (se size 1 já estaria no local correto)
    // Efetuar heapifyDown do primeiro nó (que foi recentemente movido) → neste caso seria o maior nó da heap
    if(H.size() > 1) heapifyDown(1);
    x->queueIndex = 0; // Atualizar o valor do nó com um 0 para que este sja considerado como já não fazendo parte da queue
    return x;
}

template <class T>
void MutablePriorityQueue<T>::insert(T *x) {
    H.push_back(x); // Adicionar o valor ao final da queue para fazer heapifyUp
    heapifyUp(H.size()-1); // Fazer heapify up do último nó da queue (o que foi recentemente adicionado)
}

template <class T>
void MutablePriorityQueue<T>::decreaseKey(T *x) {
    heapifyUp(x->queueIndex); // Caso a distância tenha sido atualizada sinalizamos que é necessária uma reordenação
    // Dado que neste caso esta sinalização ocorre quando a distância é reduzida, temos de fazer heapifyUp
    // Trazendo esse nó mais para o topo desta → dado que aqui estamos a tratar com uma minHeap
}

template <class T>
void MutablePriorityQueue<T>::heapifyUp(unsigned i) {
    auto x = H[i];
    while (i > 1 && *x < *H[parent(i)]) {
        set(i, H[parent(i)]);
        i = parent(i);
    }
    set(i, x);
}

template <class T>
void MutablePriorityQueue<T>::heapifyDown(unsigned i) {
    auto x = H[i];
    while (true) {
        unsigned k = leftChild(i);
        if (k >= H.size())
            break;
        if (k+1 < H.size() && *H[k+1] < *H[k])
            ++k; // right child of i
        if ( ! (*H[k] < *x) )
            break;
        set(i, H[k]);
        i = k;
    }
    set(i, x);
}

template <class T>
void MutablePriorityQueue<T>::set(unsigned i, T * x) {
    H[i] = x; // Coloca no índice i o nó
    x->queueIndex = i; // Atualiza dentro do nó a sua queue position
}

#endif /* DA_TP_CLASSES_MUTABLEPRIORITYQUEUE */