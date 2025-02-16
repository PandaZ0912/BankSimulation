#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BankSimulation.h"

// ��ʼ������
void initQueue(CustomerQueue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
    logMessage("InitQueue successful.", INFO);
}

// ��Ӳ���
void enqueue(CustomerQueue* queue, CustNode* customer) {
    // �����µĶ��н��
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        logMessage("EnQueue memory allocation failed.", ERROR);
        return;
    }

    // ���ý����Ϣ
    newNode->customer = customer;
    //printf("Enqueue amount:%d\t",newNode->customer->amount);
    newNode->next = NULL;

    // �������Ϊ�գ����� front �� rear ָ��
    if (isQueueEmpty(queue)) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        // ������¶�β���� next ָ��
        queue->rear->next = newNode;
        // ���¶�βָ��
        queue->rear = newNode;
    }
    
    // ��¼��ӳɹ�����Ϣ
    logMessage("EnQueue successful.", INFO);
}

// ���Ӳ���
CustNode* dequeue(CustomerQueue* queue) {
    if (isQueueEmpty(queue)) {
    	logMessage("Dequeue operation on an empty queue.", WARNING);
        return NULL;
    }

    // ����ͷ�����
    QueueNode* frontNode = queue->front;
    // ����ͻ����
    CustNode* customer = frontNode->customer;
			
    // ���¶���ͷ��ָ��
    queue->front = frontNode->next;

    // �������ֻ��һ����㣬���¶�βָ��Ϊ NULL
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    // ����ͷ���� next ��Ϊ NULL
    frontNode->next = NULL;

    // �ͷ�ͷ�������ڴ�
    free(frontNode);
    frontNode = NULL;
    
    // ��¼���ӳɹ�����Ϣ
    logMessage("Dequeue successful.", INFO);
    
    return customer;
}

// �������Ƿ�Ϊ��
int isQueueEmpty(CustomerQueue* queue) {
    return queue->front == NULL;
}

// ��ʼ��ѭ������ 
void initCircularCustomerQueue(CircularCustomerQueue* queue) {
    queue->data = (QueueNode**)malloc(sizeof(QueueNode*) * MAX);
    if (queue->data == NULL) {
        logMessage("InitCircularCustomerQueue memory allocation failed.", ERROR);
        return;
    }
    queue->front = 0;
    queue->rear = 0;
    queue->size = MAX;
    // ��¼��ʼ���ɹ�����Ϣ
	logMessage("InitCircularCustomerQueue successful.", INFO);
}

// �ж�ѭ�������Ƿ�Ϊ�� 
int isCircularCustomerQueueEmpty(const CircularCustomerQueue* queue) {
    return queue->front == queue->rear;
}

// �ж�ѭ�������Ƿ����� 
int isCircularCustomerQueueFull(const CircularCustomerQueue* queue) {
    return (queue->rear + 1) % queue->size == queue->front;
}

// ��� 
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
	// ��¼��ӳɹ�����Ϣ
	logMessage("CirEnQueue successful.", INFO);
}

//���� 
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
	
	// ��¼���ӳɹ�����Ϣ
	logMessage("CirDequeue successful.", INFO);
    return element;
}

// ��ӡ��ͨ�����еĹ˿����
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

// ��ӡѭ�������еĹ˿����
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
