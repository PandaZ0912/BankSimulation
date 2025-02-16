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

// 初始化全局动态数据类型变量
DynamicDataType globalDynamicDataType;

int getRandomInt(int min, int max) {
    int randomValue = rand() % (max - min + 1) + min;
    return randomValue;
}

// 生成初始到达条件 
void generateInitialArrivalCondition(BankSimulation* simulation) {
    
    srand(time(NULL));  // 初始化随机数种子 

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
	
    //客户入队
	enqueue(&(simulation->queue1),first);
	
	//事件插入事件表 
	Event firstArrivalEvent = createEvent(time,ARRIVAL,first);
	printf(ANSI_COLOR_GREEN"arrival Event Amount: %d\n",firstArrivalEvent.data->amount);
    insertEvent(&(simulation->events), firstArrivalEvent);
    
    //累计人数增加 
    simulation->total_customers = 1; 
    
    first = NULL;

    // 生成后续到达事件
    while (firstArrivalEvent.time < simulation->closetime) {
        
        //累计人数增加 
		simulation->total_customers++;
        // 根据前一个到达事件的时间生成后续的到达事件
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
        //客户入队 
        enqueue(&(simulation->queue1),next);
        //事件插入事件表 
        Event nextArrivalEvent = createEvent(time,ARRIVAL,next);
        printf(ANSI_COLOR_GREEN"arrival Event Amount: %d\n",nextArrivalEvent.data->amount);
        insertEvent(&(simulation->events), nextArrivalEvent);
        
        next = NULL;
        
        firstArrivalEvent = nextArrivalEvent;
    }
    	
}

// 模拟银行业务
void simulate(BankSimulation* simulation) {
	
	simulation->current_time = simulation->queue1.front->customer->arrtime;
	
    // 处理事件直到银行关门
    while (simulation->current_time < simulation->closetime) {
    	
        CustNode* now = dequeue(&(simulation->queue1));
        if(simulation->current_time<now->arrtime) simulation->current_time = now->arrtime;
        int raw_data = simulation->total;
		if(now->amount>=0){
			//存款
			handleCustomers(simulation,now);
			if(simulation->current_time > simulation->closetime) break;
			//检查第二个队列 
			if(!isCircularCustomerQueueEmpty(&(simulation->queue2))){
				checkQueue(simulation,raw_data);
			}
		}else{
			//取款
			int amount = (-1)*now->amount;
			if(amount<=simulation->total){
				//可满足
				handleCustomers(simulation,now);
			}else{
				//无法满足 
				enqueueCircularCustomerQueue(&(simulation->queue2),now);
			}
		}
		if(isQueueEmpty(&simulation->queue1)) break;
		now = NULL;
    }
    
    // 营业时间结束后，处理剩余在队列中的客户
    handleRemainingCustomersII(simulation,simulation->queue2);
    handleRemainingCustomersI(simulation,simulation->queue1);
    
    // 计算平均逗留时间
    if (simulation->total_customers > 0) {
        simulation->average_stay_time = (double)(simulation->total_stay_time / simulation->total_customers);
    }
}

//检查第二个队列 
void checkQueue(BankSimulation* simulation,int raw_data){
	CircularCustomerQueue queue = simulation->queue2;  
	int Id = queue.data[queue.front]->customer->Id;
	while(!isCircularCustomerQueueEmpty(&simulation->queue2)){
		CustNode* check = dequeueCircularCustomerQueue(&simulation->queue2);
		int Idd = queue.data[queue.front]->customer->Id;
		int amount = (-1)*check->amount;
		if(amount<=simulation->total){
			//可满足 
			handleCustomers(simulation,check); 
		}else{
			//不可满足 
			enqueueCircularCustomerQueue(&(simulation->queue2),check);
		} 
		if(simulation->total<=raw_data) break;
		if(Id == Idd) break;
		check = NULL;
	}
}

//处理业务 
void handleCustomers(BankSimulation* simulation,CustNode* customer){
	//1.银行入账/出账 
	simulation->total += customer->amount;
	//2.更新时间
	simulation->current_time += customer->durtime;
	Event* arrivalEvent = findARRIVALEvent(&(simulation->events),customer);
	arrivalEvent->data->state = COMPLETED;
	//3.记录离开事件 
	Event departureEvent;
	if(simulation->current_time>simulation->closetime) departureEvent = createEvent(simulation->closetime,DEPARTURE,arrivalEvent->data);
	else departureEvent = createEvent(simulation->current_time,DEPARTURE,arrivalEvent->data);
	// 检查事件和顾客是否存在
    if (arrivalEvent != NULL && arrivalEvent->data != NULL) { 
        int stay_time = departureEvent.time - arrivalEvent->time;
        simulation->total_stay_time += stay_time;
        printf(ANSI_COLOR_GREEN"departure Event Amount: %d\n",departureEvent.data->amount); 
        insertEvent(&(simulation->events), departureEvent);
    } else {
    	logMessage("Error handling customer: Arrival event not found or invalid\n",ERROR);
    }
}

//处理剩余顾客 
void handleRemainingCustomersI(BankSimulation* simulation,CustomerQueue queue){
	printCustomerQueue(&simulation->queue1);
	while (!isQueueEmpty(&queue)) {
        CustNode* customer = dequeue(&queue);
        Event* arrivalEvent = findARRIVALEvent(&(simulation->events), customer);

        // 检查事件和顾客是否存在
       	if (arrivalEvent != NULL && arrivalEvent->data != NULL) {
       		// 生成离开事件
			Event departureEvent = createEvent(simulation->closetime, DEPARTURE, customer);
			insertEvent(&(simulation->events), departureEvent);
			printf(ANSI_COLOR_GREEN"departure Event Amount: %d",departureEvent.data->amount); 
			printf(ANSI_COLOR_RESET "\n");
            int stay_time = departureEvent.time - arrivalEvent->time;
            simulation->total_stay_time += stay_time;
            free(arrivalEvent->data);
            free(customer);
        } else {
            // 错误处理，可能需要记录日志或采取其他措施
            logMessage("Error handling customerI: Arrival event not found or invalid\n",ERROR);
        }
		   
		customer = NULL;
		arrivalEvent = NULL;
    }
    printf(ANSI_COLOR_RESET "end~\n");
}
//处理剩余顾客 
void handleRemainingCustomersII(BankSimulation* simulation,CircularCustomerQueue queue){
	printCircularCustomerQueue(&simulation->queue2);
	while (!isCircularCustomerQueueEmpty(&queue)) {
        CustNode* customer = dequeueCircularCustomerQueue(&queue);
        Event* arrivalEvent = findARRIVALEvent(&(simulation->events), customer);
		
        // 检查事件和顾客是否存在
        if (arrivalEvent != NULL && arrivalEvent->data != NULL) {
        	// 生成离开事件
			Event departureEvent = createEvent(simulation->closetime, DEPARTURE, customer);
			insertEvent(&(simulation->events), departureEvent);
			printf(ANSI_COLOR_GREEN"departure Event Amount: %d\n",departureEvent.data->amount); 
            int stay_time = departureEvent.time - arrivalEvent->time;
            simulation->total_stay_time += stay_time;
            free(arrivalEvent->data);
			free(customer);
        } else {
            // 错误处理，可能需要记录日志或采取其他措施
            logMessage("Error handling customerII: Arrival event not found or invalid\n",ERROR);
        }	   
		customer = NULL;
		arrivalEvent = NULL;
    }
}

//释放资源
void freeBankSimulation(BankSimulation* simulation) {
    // 释放事件表
    Event* currentEvent = simulation->events.head;
    while (currentEvent != NULL) {
        Event* temp = currentEvent;
        currentEvent = currentEvent->next;
        myFree(&globalDynamicDataType,temp->data);
        free(temp);
    }
    simulation->events.head = NULL;
    // 释放 simulation 结构体的内存
    free(simulation);
}	

//打印事件表
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



// 初始化动态数据类型
void initDynamicDataType(DynamicDataType* dynamicDataType) {
    dynamicDataType->top = 0;

    // 初始化静态链栈
    for (int i = 0; i < MAX - 1; ++i) {
        dynamicDataType->pool[i].next = &(dynamicDataType->pool[i + 1]);
    }
    dynamicDataType->pool[MAX - 1].next = NULL;
}

// 分配内存
CustNode* myMalloc(DynamicDataType* dynamicDataType) {
    if (dynamicDataType->top == MAX) {
        // 栈满，无法分配
        return NULL;
    }

    CustNode* allocatedNode = &(dynamicDataType->pool[dynamicDataType->top]);
    dynamicDataType->top++;

    return allocatedNode;
}

// 释放内存
void myFree(DynamicDataType* dynamicDataType, CustNode* node) {
    if (dynamicDataType->top > 0) {
        dynamicDataType->top--;
        node->next = &(dynamicDataType->pool[dynamicDataType->top]);
    }
}

// 判断输入合法性，确保输入的整数是非负数
int judge() {
    int num = 0;
    char input[10];

    while (1) {
        scanf("%s", input);
        int result = sscanf(input, "%d", &num);

        if (result == 1 && num >= 0) {
            return num;
        }

        printf("输入有误，请重新输入（要求非负整数）：");
    }
}

void displayBankSimulationProcess(const BankSimulation* simulation) {
    printf("当前时间：%d\n", simulation->current_time);
    printf("总款额：%d\n", simulation->total);
	printf("总逗留时间：%d\n",simulation->total_stay_time);
	printf("逗留总人数：%d\n",simulation->total_customers);
}

// 记录日志的函数
void logMessage(const char* message, enum LogLevel log_level) {
    // 打开日志文件
    FILE* logFile = fopen("app.log", "a");
    if (logFile == NULL) {
        printf("Error opening log file.\n");
        return;
    }

    // 获取当前时间
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

    // 输出时间和日志消息到文件
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

    // 关闭日志文件
    fclose(logFile);
}

// 清屏函数
void clearScreen() {
    // 根据操作系统选择不同的清屏命令
	#ifdef _WIN32
	    system("cls");
	#else
	    system("clear");
	#endif
}
