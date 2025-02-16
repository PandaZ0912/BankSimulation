#ifndef QUEUE_H
#define QUEUE_H
#include "Union.h"

typedef struct QueueNode {
    CustNode* customer; // 客户结点
    struct QueueNode* next; // 指向下一个队列结点的指针
} QueueNode;

typedef struct {
    QueueNode* front; // 队头指针 
    QueueNode* rear; // 队尾指针 
} CustomerQueue;

typedef struct {
    QueueNode** data;  // 队列数据数组的指针
    int front;         // 队头
    int rear;          // 队尾
    int size;          // 队列大小
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
