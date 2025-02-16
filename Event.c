#include "Event.h"
#include "BankSimulation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 初始化事件表
void initEventTable(EventTable* et) {
    et->head = NULL;
}

// 创建事件
Event createEvent(int time, EventType type, CustNode* data) {
    Event event;
    event.time = time;
    event.type = type;

    // 动态分配内存
    event.data = myMalloc(&globalDynamicDataType);
    //event.data = (CustNode*)malloc(sizeof(CustNode));
    if (event.data == NULL) {
        logMessage("CreateEvent memory allocation failed.", ERROR);
    }

    // 将数据复制到新分配的内存中
    memcpy(event.data, data, sizeof(CustNode));
    event.next = NULL;
	
    return event;
}

// 将事件按照发生时间插入事件表，保持事件表的有序性
void insertEvent(EventTable* et, Event event) {
    // 创建一个新的事件结点
    Event* newEvent = (Event*)malloc(sizeof(Event));
    *newEvent = event;
    newEvent->next = NULL;

    // 如果事件表为空，或者新事件发生时间比第一个事件还早，插入为第一个事件
    if (et->head == NULL || newEvent->time < et->head->time) {
        newEvent->next = et->head;
        et->head = newEvent;
        return;
    }

    // 遍历事件表找到新事件的插入位置
    Event* currentEvent = et->head;
    while (currentEvent->next != NULL && currentEvent->next->time <= newEvent->time) {
        currentEvent = currentEvent->next;
    }

    // 插入新事件
    newEvent->next = currentEvent->next;
    currentEvent->next = newEvent;
}

// 寻找到达事件
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
