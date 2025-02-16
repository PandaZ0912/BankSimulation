#include "Event.h"
#include "BankSimulation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ��ʼ���¼���
void initEventTable(EventTable* et) {
    et->head = NULL;
}

// �����¼�
Event createEvent(int time, EventType type, CustNode* data) {
    Event event;
    event.time = time;
    event.type = type;

    // ��̬�����ڴ�
    event.data = myMalloc(&globalDynamicDataType);
    //event.data = (CustNode*)malloc(sizeof(CustNode));
    if (event.data == NULL) {
        logMessage("CreateEvent memory allocation failed.", ERROR);
    }

    // �����ݸ��Ƶ��·�����ڴ���
    memcpy(event.data, data, sizeof(CustNode));
    event.next = NULL;
	
    return event;
}

// ���¼����շ���ʱ������¼��������¼����������
void insertEvent(EventTable* et, Event event) {
    // ����һ���µ��¼����
    Event* newEvent = (Event*)malloc(sizeof(Event));
    *newEvent = event;
    newEvent->next = NULL;

    // ����¼���Ϊ�գ��������¼�����ʱ��ȵ�һ���¼����磬����Ϊ��һ���¼�
    if (et->head == NULL || newEvent->time < et->head->time) {
        newEvent->next = et->head;
        et->head = newEvent;
        return;
    }

    // �����¼����ҵ����¼��Ĳ���λ��
    Event* currentEvent = et->head;
    while (currentEvent->next != NULL && currentEvent->next->time <= newEvent->time) {
        currentEvent = currentEvent->next;
    }

    // �������¼�
    newEvent->next = currentEvent->next;
    currentEvent->next = newEvent;
}

// Ѱ�ҵ����¼�
Event* findARRIVALEvent(EventTable* et, CustNode* data) {
    Event* head = et->head;
    while (head != NULL) {
        if (head->type == ARRIVAL && memcmp(head->data, data, sizeof(CustNode)) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}
