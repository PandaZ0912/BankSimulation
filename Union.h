#ifndef UNION_H
#define UNION_H

#define MAX 600
#define SIZE 50

// ����ͻ�����״̬ 
typedef enum {
    COMPLETED,  
    UNFINISH  
} StateType;

typedef struct CustNode {
	int Id;
    int arrtime; // ����ʱ�� 
    int durtime; // ����ʱ�� 
    int amount; // ��������(+������/���-����ȡ��/���)
    StateType state; // ����״̬ 
    struct CustNode* next;
} CustNode; 

#endif  // UNION_H
