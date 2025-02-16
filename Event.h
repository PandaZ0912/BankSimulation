#ifndef EVENT_H
#define EVENT_H
#include "Union.h"

// �����¼�����ö��
typedef enum {
    ARRIVAL,  // �ͻ������¼�
    DEPARTURE  // �ͻ��뿪�¼�
} EventType;

// �����¼������������
typedef struct Event {
    int time;        // �¼�����ʱ��
    EventType type;  // �¼�����
    CustNode* data;  // ������ݣ���ͻ���㣩
    struct Event* next;  // ָ����һ���¼�����ָ��
} Event;

// �����¼�����������
typedef struct EventTable {
    Event* head;
} EventTable;

// ����ԭ������
void initEventTable(EventTable* et);
void insertEvent(EventTable* et, Event event);
Event createEvent(int time, EventType type, CustNode* data);
Event* findARRIVALEvent(EventTable* et, CustNode* data);

#endif  // EVENT_H
