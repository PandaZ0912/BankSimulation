#ifndef EVENT_H
#define EVENT_H
#include "Union.h"

// 定义事件类型枚举
typedef enum {
    ARRIVAL,  // 客户到达事件
    DEPARTURE  // 客户离开事件
} EventType;

// 定义事件结点数据类型
typedef struct Event {
    int time;        // 事件发生时间
    EventType type;  // 事件类型
    CustNode* data;  // 相关数据（如客户结点）
    struct Event* next;  // 指向下一个事件结点的指针
} Event;

// 定义事件表数据类型
typedef struct EventTable {
    Event* head;
} EventTable;

// 函数原型声明
void initEventTable(EventTable* et);
void insertEvent(EventTable* et, Event event);
Event createEvent(int time, EventType type, CustNode* data);
Event* findARRIVALEvent(EventTable* et, CustNode* data);

#endif  // EVENT_H
