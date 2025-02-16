#ifndef BANK_SIMULATION_H
#define BANK_SIMULATION_H
#include "Queue.h"
#include "Event.h"

// ��������ģ��ϵͳ��������
typedef struct BankSimulation {
    int total;      // ��ʼ�ʽ��ܶ�
    int closetime;        // Ӫҵʱ��
    int current_time;		// ��ǰʱ�� 
    int max_transaction_amount;     // ����׽��
    int min_arrival_interval;       // ���ڵ����¼��ļ��ʱ���½� 
    int max_arrival_interval;       // ���ڵ����¼��ļ��ʱ���Ͻ� 
    int min_transaction_time;       // ����ʱ���½� 
    int max_transaction_time;       // ����ʱ���Ͻ� 
    int total_customers;				// �ܿͻ����� 
    CustomerQueue queue1;         // ��һ������	
    CircularCustomerQueue queue2;         // �ڶ�������
    EventTable events;    // �¼���
    int total_stay_time;   // �ܶ���ʱ�� 
    double average_stay_time;  // ƽ������ʱ��
} BankSimulation;

// ������־����
enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

// ����ҵ�� ��ز��� 
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

// ���嶯̬��������
typedef struct DynamicDataType {
    CustNode pool[MAX];
    int top;
} DynamicDataType;
// ȫ�ֱ������� ��̬�������� 
extern DynamicDataType globalDynamicDataType;
// ��̬����������ز���ʵ�� 
void initDynamicDataType(DynamicDataType* dynamicDataType);
CustNode* myMalloc(DynamicDataType* dynamicDataType);
void myFree(DynamicDataType* dynamicDataType, CustNode* node);

// ����Ϸ��ж� 
int judge(); 
// ��־��¼
void logMessage(const char* message, enum LogLevel log_level); 
//���� 
void clearScreen(); 

#endif  // BANK_SIMULATION_H
