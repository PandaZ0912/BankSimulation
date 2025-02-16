#ifndef QUEUE_H
#define QUEUE_H
#include "Union.h"

typedef struct QueueNode {
    CustNode* customer; // �ͻ����
    struct QueueNode* next; // ָ����һ�����н���ָ��
} QueueNode;

typedef struct {
    QueueNode* front; // ��ͷָ�� 
    QueueNode* rear; // ��βָ�� 
} CustomerQueue;

typedef struct {
    QueueNode** data;  // �������������ָ��
    int front;         // ��ͷ
    int rear;          // ��β
    int size;          // ���д�С
} CircularCustomerQueue;

void initQueue(CustomerQueue* queue);
void enqueue(CustomerQueue* queue, CustNode* customer);
CustNode* dequeue(CustomerQueue* queue);
int isQueueEmpty(CustomerQueue* queue);
void printCustomerQueue(CustomerQueue* queue);

void initCircularCustomerQueue(CircularCustomerQueue* queue);
int isCircularCustomerQueueEmpty(const CircularCustomerQueue* queue);
int isCircularCustomerQueueFull(const CircularCustomerQueue* queue);
void enqueueCircularCustomerQueue(CircularCustomerQueue* queue, CustNode* element);
CustNode* dequeueCircularCustomerQueue(CircularCustomerQueue* queue);
void printCircularCustomerQueue(CircularCustomerQueue* queue);

#endif  // QUEUE_H
