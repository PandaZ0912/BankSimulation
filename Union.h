#ifndef UNION_H
#define UNION_H

#define MAX 600
#define SIZE 50

// 定义客户交易状态 
typedef enum {
    COMPLETED,  
    UNFINISH  
} StateType;

typedef struct CustNode {
	int Id;
    int arrtime; // 到达时间 
    int durtime; // 交易时长 
    int amount; // 交易数额(+代表存款/还款，-代表取款/借款)
    StateType state; // 交易状态 
    struct CustNode* next;
} CustNode; 

#endif  // UNION_H
