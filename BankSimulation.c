#include "BankSimulation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ANSI escape codes for terminal colors
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RED     "\x1b[31m"

// ��ʼ��ȫ�ֶ�̬�������ͱ���
DynamicDataType globalDynamicDataType;

int getRandomInt(int min, int max) {
    int randomValue = rand() % (max - min + 1) + min;
    return randomValue;
}

// ���ɳ�ʼ�������� 
void generateInitialArrivalCondition(BankSimulation* simulation) {
    
    srand(time(NULL));  // ��ʼ����������� 

    CustNode* first = myMalloc(&globalDynamicDataType);
    int time = getRandomInt(0, simulation->closetime/2); 
	first->Id = 1; 
    first->arrtime = time;
    first->durtime = getRandomInt(simulation->min_transaction_time, simulation->max_transaction_time);
    first->amount = getRandomInt(-simulation->max_transaction_amount, simulation->max_transaction_amount);
    first->state = UNFINISH;
    printf(ANSI_COLOR_YELLOW "Generated random value: %d", first->amount);
	printf(ANSI_COLOR_RESET "\n"); 
	first->next = NULL;
	
    //�ͻ����
	enqueue(&(simulation->queue1),first);
	
	//�¼������¼��� 
	Event firstArrivalEvent = createEvent(time,ARRIVAL,first);
	printf(ANSI_COLOR_GREEN"arrival Event Amount: %d\n",firstArrivalEvent.data->amount);
    insertEvent(&(simulation->events), firstArrivalEvent);
    
    //�ۼ��������� 
    simulation->total_customers = 1; 
    
    first = NULL;

    // ���ɺ��������¼�
    while (firstArrivalEvent.time < simulation->closetime) {
        
        //�ۼ��������� 
		simulation->total_customers++;
        // ����ǰһ�������¼���ʱ�����ɺ����ĵ����¼�
        int time = firstArrivalEvent.time + getRandomInt(simulation->min_arrival_interval, simulation->max_arrival_interval);
        if( time > simulation->closetime) break;
        CustNode* next = myMalloc(&globalDynamicDataType);
        next->Id = simulation->total_customers;
        next->arrtime = time;
        next->durtime = getRandomInt(simulation->min_transaction_time, simulation->max_transaction_time);
        next->amount = getRandomInt(-simulation->max_transaction_amount, simulation->max_transaction_amount);
        next->state = UNFINISH;
        printf(ANSI_COLOR_YELLOW "Generated random value: %d", next->amount);
		printf(ANSI_COLOR_RESET "\n"); 
		next->next = NULL; 
        //�ͻ���� 
        enqueue(&(simulation->queue1),next);
        //�¼������¼��� 
        Event nextArrivalEvent = createEvent(time,ARRIVAL,next);
        printf(ANSI_COLOR_GREEN"arrival Event Amount: %d\n",nextArrivalEvent.data->amount);
        insertEvent(&(simulation->events), nextArrivalEvent);
        
        next = NULL;
        
        firstArrivalEvent = nextArrivalEvent;
    }
    	
}

// ģ������ҵ��
void simulate(BankSimulation* simulation) {
	
	simulation->current_time = simulation->queue1.front->customer->arrtime;
	
    // �����¼�ֱ�����й���
    while (simulation->current_time < simulation->closetime) {
    	
        CustNode* now = dequeue(&(simulation->queue1));
        if(simulation->current_time<now->arrtime) simulation->current_time = now->arrtime;
        int raw_data = simulation->total;
		if(now->amount>=0){
			//���
			handleCustomers(simulation,now);
			if(simulation->current_time > simulation->closetime) break;
			//���ڶ������� 
			if(!isCircularCustomerQueueEmpty(&(simulation->queue2))){
				checkQueue(simulation,raw_data);
			}
		}else{
			//ȡ��
			int amount = (-1)*now->amount;
			if(amount<=simulation->total){
				//������
				handleCustomers(simulation,now);
			}else{
				//�޷����� 
				enqueueCircularCustomerQueue(&(simulation->queue2),now);
			}
		}
		if(isQueueEmpty(&simulation->queue1)) break;
		now = NULL;
    }
    
    // Ӫҵʱ������󣬴���ʣ���ڶ����еĿͻ�
    handleRemainingCustomersII(simulation,simulation->queue2);
    handleRemainingCustomersI(simulation,simulation->queue1);
    
    // ����ƽ������ʱ��
    if (simulation->total_customers > 0) {
        simulation->average_stay_time = (double)(simulation->total_stay_time / simulation->total_customers);
    }
}

//���ڶ������� 
void checkQueue(BankSimulation* simulation,int raw_data){
	CircularCustomerQueue queue = simulation->queue2;  
	int Id = queue.data[queue.front]->customer->Id;
	while(!isCircularCustomerQueueEmpty(&simulation->queue2)){
		CustNode* check = dequeueCircularCustomerQueue(&simulation->queue2);
		int Idd = queue.data[queue.front]->customer->Id;
		int amount = (-1)*check->amount;
		if(amount<=simulation->total){
			//������ 
			handleCustomers(simulation,check); 
		}else{
			//�������� 
			enqueueCircularCustomerQueue(&(simulation->queue2),check);
		} 
		if(simulation->total<=raw_data) break;
		if(Id == Idd) break;
		check = NULL;
	}
}

//����ҵ�� 
void handleCustomers(BankSimulation* simulation,CustNode* customer){
	//1.��������/���� 
	simulation->total += customer->amount;
	//2.����ʱ��
	simulation->current_time += customer->durtime;
	Event* arrivalEvent = findARRIVALEvent(&(simulation->events),customer);
	arrivalEvent->data->state = COMPLETED;
	//3.��¼�뿪�¼� 
	Event departureEvent;
	if(simulation->current_time>simulation->closetime) departureEvent = createEvent(simulation->closetime,DEPARTURE,arrivalEvent->data);
	else departureEvent = createEvent(simulation->current_time,DEPARTURE,arrivalEvent->data);
	// ����¼��͹˿��Ƿ����
    if (arrivalEvent != NULL && arrivalEvent->data != NULL) { 
        int stay_time = departureEvent.time - arrivalEvent->time;
        simulation->total_stay_time += stay_time;
        printf(ANSI_COLOR_GREEN"departure Event Amount: %d\n",departureEvent.data->amount); 
        insertEvent(&(simulation->events), departureEvent);
    } else {
    	logMessage("Error handling customer: Arrival event not found or invalid\n",ERROR);
    }
}

//����ʣ��˿� 
void handleRemainingCustomersI(BankSimulation* simulation,CustomerQueue queue){
	printCustomerQueue(&simulation->queue1);
	while (!isQueueEmpty(&queue)) {
        CustNode* customer = dequeue(&queue);
        Event* arrivalEvent = findARRIVALEvent(&(simulation->events), customer);

        // ����¼��͹˿��Ƿ����
       	if (arrivalEvent != NULL && arrivalEvent->data != NULL) {
       		// �����뿪�¼�
			Event departureEvent = createEvent(simulation->closetime, DEPARTURE, customer);
			insertEvent(&(simulation->events), departureEvent);
			printf(ANSI_COLOR_GREEN"departure Event Amount: %d",departureEvent.data->amount); 
			printf(ANSI_COLOR_RESET "\n");
            int stay_time = departureEvent.time - arrivalEvent->time;
            simulation->total_stay_time += stay_time;
            free(arrivalEvent->data);
            free(customer);
        } else {
            // ������������Ҫ��¼��־���ȡ������ʩ
            logMessage("Error handling customerI: Arrival event not found or invalid\n",ERROR);
        }
		   
		customer = NULL;
		arrivalEvent = NULL;
    }
    printf(ANSI_COLOR_RESET "end~\n");
}
//����ʣ��˿� 
void handleRemainingCustomersII(BankSimulation* simulation,CircularCustomerQueue queue){
	printCircularCustomerQueue(&simulation->queue2);
	while (!isCircularCustomerQueueEmpty(&queue)) {
        CustNode* customer = dequeueCircularCustomerQueue(&queue);
        Event* arrivalEvent = findARRIVALEvent(&(simulation->events), customer);
		
        // ����¼��͹˿��Ƿ����
        if (arrivalEvent != NULL && arrivalEvent->data != NULL) {
        	// �����뿪�¼�
			Event departureEvent = createEvent(simulation->closetime, DEPARTURE, customer);
			insertEvent(&(simulation->events), departureEvent);
			printf(ANSI_COLOR_GREEN"departure Event Amount: %d\n",departureEvent.data->amount); 
            int stay_time = departureEvent.time - arrivalEvent->time;
            simulation->total_stay_time += stay_time;
            free(arrivalEvent->data);
			free(customer);
        } else {
            // ������������Ҫ��¼��־���ȡ������ʩ
            logMessage("Error handling customerII: Arrival event not found or invalid\n",ERROR);
        }	   
		customer = NULL;
		arrivalEvent = NULL;
    }
}

//�ͷ���Դ
void freeBankSimulation(BankSimulation* simulation) {
    // �ͷ��¼���
    Event* currentEvent = simulation->events.head;
    while (currentEvent != NULL) {
        Event* temp = currentEvent;
        currentEvent = currentEvent->next;
        myFree(&globalDynamicDataType,temp->data);
        free(temp);
    }
    simulation->events.head = NULL;
    // �ͷ� simulation �ṹ����ڴ�
    free(simulation);
}	

//��ӡ�¼���
void printEventTable(const EventTable* et) {
    if (et == NULL || et->head == NULL) {
        printf("Event table is empty.\n");
        return;
    }

    printf("Event Table:\n");
    printf("Time\tType\t\tData\t\t\tId\tDeal\n");

    Event* currentEvent = et->head;

    while (currentEvent != NULL) {
        printf("%d\t", currentEvent->time);

        // Set color based on event type
        if (currentEvent->type == ARRIVAL) {
            printf(ANSI_COLOR_YELLOW "ARRIVAL\t\tCustomer Arrival\t");
        } else if(currentEvent->data->state == COMPLETED){
			printf(ANSI_COLOR_GREEN "DEPARTURE\tCustomer Departure\t");
		} else {
            printf(ANSI_COLOR_RED "DEPARTURE\tCustomer Departure\t");
        }

        CustNode* now = currentEvent->data;
        printf("%d\t",now->Id);
        printf("%d\t" ANSI_COLOR_RESET "\n", now->amount);

        currentEvent = currentEvent->next;
    }
}



// ��ʼ����̬��������
void initDynamicDataType(DynamicDataType* dynamicDataType) {
    dynamicDataType->top = 0;

    // ��ʼ����̬��ջ
    for (int i = 0; i < MAX - 1; ++i) {
        dynamicDataType->pool[i].next = &(dynamicDataType->pool[i + 1]);
    }
    dynamicDataType->pool[MAX - 1].next = NULL;
}

// �����ڴ�
CustNode* myMalloc(DynamicDataType* dynamicDataType) {
    if (dynamicDataType->top == MAX) {
        // ջ�����޷�����
        return NULL;
    }

    CustNode* allocatedNode = &(dynamicDataType->pool[dynamicDataType->top]);
    dynamicDataType->top++;

    return allocatedNode;
}

// �ͷ��ڴ�
void myFree(DynamicDataType* dynamicDataType, CustNode* node) {
    if (dynamicDataType->top > 0) {
        dynamicDataType->top--;
        node->next = &(dynamicDataType->pool[dynamicDataType->top]);
    }
}

// �ж�����Ϸ��ԣ�ȷ������������ǷǸ���
int judge() {
    int num = 0;
    char input[10];

    while (1) {
        scanf("%s", input);
        int result = sscanf(input, "%d", &num);

        if (result == 1 && num >= 0) {
            return num;
        }

        printf("�����������������루Ҫ��Ǹ���������");
    }
}

void displayBankSimulationProcess(const BankSimulation* simulation) {
    printf("��ǰʱ�䣺%d\n", simulation->current_time);
    printf("�ܿ�%d\n", simulation->total);
	printf("�ܶ���ʱ�䣺%d\n",simulation->total_stay_time);
	printf("������������%d\n",simulation->total_customers);
}

// ��¼��־�ĺ���
void logMessage(const char* message, enum LogLevel log_level) {
    // ����־�ļ�
    FILE* logFile = fopen("app.log", "a");
    if (logFile == NULL) {
        printf("Error opening log file.\n");
        return;
    }

    // ��ȡ��ǰʱ��
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

    // ���ʱ�����־��Ϣ���ļ�
    fprintf(logFile, "[%s] ", buffer);
    switch (log_level) {
        case DEBUG:
            fprintf(logFile, "[DEBUG] ");
            break;
        case INFO:
            fprintf(logFile, "[INFO] ");
            break;
        case WARNING:
            fprintf(logFile, "[WARNING] ");
            break;
        case ERROR:
            fprintf(logFile, "[ERROR] ");
            break;
    }
    fprintf(logFile, "%s\n", message);

    // �ر���־�ļ�
    fclose(logFile);
}

// ��������
void clearScreen() {
    // ���ݲ���ϵͳѡ��ͬ����������
	#ifdef _WIN32
	    system("cls");
	#else
	    system("clear");
	#endif
}
