#ifndef BANK_SIMULATION_H
#define BANK_SIMULATION_H
#include "Queue.h"
#include "Event.h"

// 定义银行模拟系统数据类型
typedef struct BankSimulation {
    int total;      // 初始资金总额
    int closetime;        // 营业时间
    int current_time;		// 当前时间 
    int max_transaction_amount;     // 最大交易金额
    int min_arrival_interval;       // 相邻到达事件的间隔时间下界 
    int max_arrival_interval;       // 相邻到达事件的间隔时间上界 
    int min_transaction_time;       // 交易时间下界 
    int max_transaction_time;       // 交易时间上界 
    int total_customers;				// 总客户人数 
    CustomerQueue queue1;         // 第一个队列	
    CircularCustomerQueue queue2;         // 第二个队列
    EventTable events;    // 事件表
    int total_stay_time;   // 总逗留时间 
    double average_stay_time;  // 平均逗留时间
} BankSimulation;

// 定义日志级别
enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

// 银行业务 相关操作 
void initBankSimulation(BankSimulation* simulation);
void freeBankSimulation(BankSimulation* simulation);
void generateInitialArrivalCondition(BankSimulation* simulation);
void simulate(BankSimulation* simulation);
void checkQueue(BankSimulation* simulation,int raw_data);
void handleCustomers(BankSimulation* simulation,CustNode* cust);
void handleRemainingCustomersI(BankSimulation* simulation,CustomerQueue queue); 
void handleRemainingCustomersII(BankSimulation* simulation,CircularCustomerQueue queue); 
void printEventTable(const EventTable* et);
void displayBankSimulationProcess(const BankSimulation* simulation);

// 定义动态数据类型
typedef struct DynamicDataType {
    CustNode pool[MAX];
    int top;
} DynamicDataType;
// 全局变量声明 动态数据类型 
extern DynamicDataType globalDynamicDataType;
// 动态数据类型相关操作实现 
void initDynamicDataType(DynamicDataType* dynamicDataType);
CustNode* myMalloc(DynamicDataType* dynamicDataType);
void myFree(DynamicDataType* dynamicDataType, CustNode* node);

// 输入合法判断 
int judge(); 
// 日志记录
void logMessage(const char* message, enum LogLevel log_level); 
//清屏 
void clearScreen(); 

#endif  // BANK_SIMULATION_H
