#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "BankSimulation.h"

// 显示菜单
void displayMenu() {
    printf("***********银行业务模拟***********\n");
    printf("班级：2022级计算机科学与技术伏羲班\n");
    printf("学号：3222004895\n");
    printf("姓名：钟雯敏\n");
    printf("**********************************\n");
    printf("请输入您需要进行的操作\n");
    printf("1. 设置银行参数\n");
    printf("2. 初始化相关参数\n");
    printf("3. 执行银行业务模拟&显示结果\n");
    printf("4. 退出\n");
    printf("**********************************\n");
    printf("请选择操作：");
}


int main() {
	
	// 记录程序开始
	logMessage("Application started.", INFO); 
	
    // 创建 BankSimulation 结构体
    BankSimulation* simulation = malloc(sizeof(BankSimulation));
    simulation->total = -1;
    if (simulation == NULL) {
        printf("BANKOVERFLOW.\n");
        return 0;
    }
	    
    // 在主函数中初始化动态数据类型全局变量
	initDynamicDataType(&globalDynamicDataType);
	
	int flag = 0;
 	int choice;
	do {
		clearScreen();
	    displayMenu();
	    choice = judge();
			
	    switch (choice) {
	        case 1:
	        	flag = 1;
	            // 从终端读入模拟参数
			    printf("请输入银行拥有的款额(元)：");
			    int total = judge();
			 
			    printf("请输入营业时间(分钟)：");
			    int closetime = judge();
			 
			    printf("请输入相邻到达事件的间隔时间的下界(分钟)：");
			    int min_arrival = 0;
			    while (min_arrival == 0 || min_arrival > closetime) {
			        if (min_arrival > closetime) printf("超出营业时间，请重新输入:");
			        min_arrival = judge();
			    }
			 
			    printf("请输入相邻到达事件的间隔时间的上界(分钟)：");
			    int max_arrival = 0;
			    while (max_arrival == 0 || max_arrival > closetime || max_arrival < min_arrival) {
			        if (max_arrival > closetime) printf("超出营业时间，请重新输入:");
			        if (max_arrival != 0 && max_arrival < min_arrival) printf("上界小于下界，请重新输入:");
			        max_arrival = judge();
			    }
			 
			    printf("请输入交易时间的下界(分钟)：");
			    int min_transaction_time = 0;
			    while (min_transaction_time == 0 || min_transaction_time > closetime) {
			        if (min_transaction_time > closetime) printf("超出营业时间，请重新输入:");
			        min_transaction_time = judge();
			    }
			 
			    printf("请输入交易时间的上界(分钟)：");
			    int max_transaction_time = 0;
			    while (max_transaction_time == 0 || max_transaction_time > closetime || max_transaction_time < min_transaction_time) {
			        if (max_transaction_time > closetime) printf("超出营业时间，请重新输入:");
			        if (max_transaction_time != 0 && max_transaction_time < min_transaction_time) printf("上界小于下界，请重新输入:");
			        max_transaction_time = judge();
			    }
			    
			    printf("请输入最大交易金额(元)：");
			    int amount = judge();
			    
                break;
	        case 2:
	        	if(flag == 0) {
					printf("您还未设置参数，请先进行设置\n");
					break;
				}
				if(simulation == NULL) simulation = malloc(sizeof(BankSimulation));
				// 初始化队列和事件表 
			    simulation->total = total;
				simulation->closetime = closetime;
				simulation->min_arrival_interval = min_arrival;
				simulation->max_arrival_interval = max_arrival;
				simulation->min_transaction_time = min_transaction_time;
				simulation->max_transaction_time = max_transaction_time;
				simulation->max_transaction_amount = amount;
			    simulation->current_time = 0;
			    simulation->total_customers = 0;
			    simulation->total_stay_time = 0.0;
			    simulation->average_stay_time = 0.0;
				initQueue(&(simulation->queue1));
			    initCircularCustomerQueue(&(simulation->queue2));	 
			    initEventTable(&(simulation->events));
	            // 条件初始化 
			    generateInitialArrivalCondition(simulation);
			    // 模拟银行业务
				simulate(simulation);
	            break;
	        case 3:
	        	if(simulation->total == -1) {
					printf("您还未设置参数，请先进行设置\n");
					break;
				}
				if(simulation->events.head == NULL) {
					printf("您还未进行初始化，请先进行设置\n");
					break;
				}
	            // 显示结果
			    printf("平均逗留时间：%lf\n", simulation->average_stay_time);
			    // 打印事件表
			    printEventTable(&(simulation->events));
			    //释放内存 
				freeBankSimulation(simulation);
				//将指针置空 
				simulation = NULL;
	            break;
	        case 4:
	            // 退出循环
	            break;
	        default:
	            printf("无效的选择，请重新输入。\n");
		    }
		    // 等待用户输入后继续
	        printf("按 Enter 键继续...");
	        getchar();
	        getchar();
	    
	} while (choice != 4);
    
    // 记录程序结束 
	logMessage("Application end.", INFO); 

    return 0;
}
