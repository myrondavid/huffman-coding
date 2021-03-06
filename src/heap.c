#include <stdio.h>
#include <stdlib.h>
#include "../inc/heap.h"
#include "../inc/huffman_tree.h"

struct heap {
  int max_size;
  int size;
  Node **data;
};

void swap(Node **a, Node **b) {
  Node aux = **a;
  **a = **b;
  **b = aux;
}

Heap* create_heap(int size) {
  Heap *heap = (Heap*)malloc(sizeof(Heap));
  heap->data = (Node**)malloc(size * sizeof(Node*));
  heap->max_size = size;
  heap->size = 0;

  return heap;
}

int get_parent_index(Heap *heap, int i) {
  return i/2;
}

int get_left_index(Heap *heap, int i) {
  return 2*i;
}

int get_right_index(Heap *heap, int i) {
  return 2*i + 1;
}

int get_size(Heap *heap) {
  return heap->size;
}

void enqueue(Heap *heap, Node *value) {

  if (heap->size >= heap->max_size) {
    printf("Heap is full!");
  } else {
    heap->data[++heap->size] = value;
    int key_index = heap->size;
    int parent_index = get_parent_index(heap, heap->size);

    while (parent_index >= 1 && heap->data[key_index]->frequency < heap->data[parent_index]->frequency) {
      swap(&heap->data[key_index], &heap->data[parent_index]);
      key_index = parent_index;
      parent_index = get_parent_index(heap, key_index);
    }
  }
}

Node* dequeue(Heap *heap) {
  Node* value = heap->data[1];

  heap->data[1] = heap->data[heap->size];
  heap->size--;
  min_heapify(heap, 1);

  return value;
}

void min_heapify(Heap *heap, int i) {
  int lowest;
  int left_index = get_left_index(heap, i);
  int right_index = get_right_index(heap, i);

  if (left_index <= heap->size && heap->data[left_index]->frequency < heap->data[i]->frequency)
    lowest = left_index;
  else
    lowest = i;

  if (right_index <= heap->size && heap->data[right_index]->frequency < heap->data[lowest]->frequency)
    lowest = right_index;

  if (heap->data[i]->frequency != heap->data[lowest]->frequency) {
    swap(&heap->data[i], &heap->data[lowest]);
    min_heapify(heap, lowest);
  }
}
/* Recieves an item and its frequency, returns nodes */
Node* create_node(char item, int frequency){

  Node* newNode = (Node*)malloc(sizeof(Node));
  newNode->item = item;
  newNode->frequency = frequency;

  newNode->left = NULL;
  newNode->right = NULL;

  return newNode;

}
