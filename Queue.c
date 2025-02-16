#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BankSimulation.h"

// 初始化队列
void initQueue(CustomerQueue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
    logMessage("InitQueue successful.", INFO);
}

// 入队操作
void enqueue(CustomerQueue* queue, CustNode* customer) {
    // 创建新的队列结点
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        logMessage("EnQueue memory allocation failed.", ERROR);
        return;
    }

    // 设置结点信息
    newNode->customer = customer;
    //printf("Enqueue amount:%d\t",newNode->customer->amount);
    newNode->next = NULL;

    // 如果队列为空，更新 front 和 rear 指针
    if (isQueueEmpty(queue)) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        // 否则更新队尾结点的 next 指针
        queue->rear->next = newNode;
        // 更新队尾指针
        queue->rear = newNode;
    }
    
    // 记录入队成功的信息
    logMessage("EnQueue successful.", INFO);
}

// 出队操作
CustNode* dequeue(CustomerQueue* queue) {
    if (isQueueEmpty(queue)) {
    	logMessage("Dequeue operation on an empty queue.", WARNING);
        return NULL;
    }

    // 保存头部结点
    QueueNode* frontNode = queue->front;
    // 保存客户结点
    CustNode* customer = frontNode->customer;
			
    // 更新队列头部指针
    queue->front = frontNode->next;

    // 如果队列只有一个结点，更新队尾指针为 NULL
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    // 将队头结点的 next 置为 NULL
    frontNode->next = NULL;

    // 释放头部结点的内存
    free(frontNode);
    frontNode = NULL;
    
    // 记录出队成功的信息
    logMessage("Dequeue successful.", INFO);
    
    return customer;
}

// 检查队列是否为空
int isQueueEmpty(CustomerQueue* queue) {
    return queue->front == NULL;
}

// 初始化循环队列 
void initCircularCustomerQueue(CircularCustomerQueue* queue) {
    queue->data = (QueueNode**)malloc(sizeof(QueueNode*) * MAX);
    if (queue->data == NULL) {
        logMessage("InitCircularCustomerQueue memory allocation failed.", ERROR);
        return;
    }
    queue->front = 0;
    queue->rear = 0;
    queue->size = MAX;
    // 记录初始化成功的信息
	logMessage("InitCircularCustomerQueue successful.", INFO);
}

// 判断循环队列是否为空 
int isCircularCustomerQueueEmpty(const CircularCustomerQueue* queue) {
    return queue->front == queue->rear;
}

// 判断循环队列是否已满 
int isCircularCustomerQueueFull(const CircularCustomerQueue* queue) {
    return (queue->rear + 1) % queue->size == queue->front;
}

// 入队 
void enqueueCircularCustomerQueue(CircularCustomerQueue* queue, CustNode* element) {
    if ((queue->rear+1)%queue->size == queue->front) {
        logMessage("CirQueue is full. Enqueue operation failed.",ERROR);
        return;
    }

    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        logMessage("CirEnQueue memory allocation failed.", ERROR);
        return;
    }

    newNode->customer = element;
    newNode->next = NULL;

    queue->data[queue->rear] = newNode;
	queue->rear = (queue->rear + 1) % queue->size;
	// 记录入队成功的信息
	logMessage("CirEnQueue successful.", INFO);
}

//出队 
CustNode* dequeueCircularCustomerQueue(CircularCustomerQueue* queue) {
    if (isCircularCustomerQueueEmpty(queue)) {
    	logMessage("CirQueue is empty. Dequeue operation failed.",ERROR);
        return NULL;
    }

    QueueNode* nodeToRemove = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->size;

    CustNode* element = nodeToRemove->customer;
    free(nodeToRemove);
    nodeToRemove = NULL;
	
	// 记录出队成功的信息
	logMessage("CirDequeue successful.", INFO);
    return element;
}

// 打印普通队列中的顾客情况
void printCustomerQueue(CustomerQueue* queue) {
    if (isQueueEmpty(queue)) {
        printf("Customer Queue is empty.\n");
        return;
    }

    printf("Customer Queue:\n");
    QueueNode* current = queue->front;

    while (current != NULL) {
        CustNode* customer = current->customer;
        printf("Customer ID: %d, Arrival Time: %d, Duration: %d, Amount: %d\n",
               customer->Id, customer->arrtime, customer->durtime, customer->amount);

        current = current->next;
    }
}

// 打印循环队列中的顾客情况
void printCircularCustomerQueue(CircularCustomerQueue* queue) {
    if (isCircularCustomerQueueEmpty(queue)) {
        printf("Circular Customer Queue is empty.\n");
        return;
    }

    printf("Circular Customer Queue:\n");
    int current = queue->front;

    while (current != queue->rear) {
        CustNode* customer = queue->data[current]->customer;
        printf("Customer ID: %d, Arrival Time: %d, Duration: %d, Amount: %d\n",
               customer->Id, customer->arrtime, customer->durtime, customer->amount);

        current = (current + 1) % queue->size;
    }
}
