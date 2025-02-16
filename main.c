#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "BankSimulation.h"

// ��ʾ�˵�
void displayMenu() {
    printf("***********����ҵ��ģ��***********\n");
    printf("�༶��2022���������ѧ�뼼�����˰�\n");
    printf("ѧ�ţ�3222004895\n");
    printf("������������\n");
    printf("**********************************\n");
    printf("����������Ҫ���еĲ���\n");
    printf("1. �������в���\n");
    printf("2. ��ʼ����ز���\n");
    printf("3. ִ������ҵ��ģ��&��ʾ���\n");
    printf("4. �˳�\n");
    printf("**********************************\n");
    printf("��ѡ�������");
}


int main() {
	
	// ��¼����ʼ
	logMessage("Application started.", INFO); 
	
    // ���� BankSimulation �ṹ��
    BankSimulation* simulation = malloc(sizeof(BankSimulation));
    simulation->total = -1;
    if (simulation == NULL) {
        printf("BANKOVERFLOW.\n");
        return 0;
    }
	    
    // ���������г�ʼ����̬��������ȫ�ֱ���
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
	            // ���ն˶���ģ�����
			    printf("����������ӵ�еĿ��(Ԫ)��");
			    int total = judge();
			 
			    printf("������Ӫҵʱ��(����)��");
			    int closetime = judge();
			 
			    printf("���������ڵ����¼��ļ��ʱ����½�(����)��");
			    int min_arrival = 0;
			    while (min_arrival == 0 || min_arrival > closetime) {
			        if (min_arrival > closetime) printf("����Ӫҵʱ�䣬����������:");
			        min_arrival = judge();
			    }
			 
			    printf("���������ڵ����¼��ļ��ʱ����Ͻ�(����)��");
			    int max_arrival = 0;
			    while (max_arrival == 0 || max_arrival > closetime || max_arrival < min_arrival) {
			        if (max_arrival > closetime) printf("����Ӫҵʱ�䣬����������:");
			        if (max_arrival != 0 && max_arrival < min_arrival) printf("�Ͻ�С���½磬����������:");
			        max_arrival = judge();
			    }
			 
			    printf("�����뽻��ʱ����½�(����)��");
			    int min_transaction_time = 0;
			    while (min_transaction_time == 0 || min_transaction_time > closetime) {
			        if (min_transaction_time > closetime) printf("����Ӫҵʱ�䣬����������:");
			        min_transaction_time = judge();
			    }
			 
			    printf("�����뽻��ʱ����Ͻ�(����)��");
			    int max_transaction_time = 0;
			    while (max_transaction_time == 0 || max_transaction_time > closetime || max_transaction_time < min_transaction_time) {
			        if (max_transaction_time > closetime) printf("����Ӫҵʱ�䣬����������:");
			        if (max_transaction_time != 0 && max_transaction_time < min_transaction_time) printf("�Ͻ�С���½磬����������:");
			        max_transaction_time = judge();
			    }
			    
			    printf("����������׽��(Ԫ)��");
			    int amount = judge();
			    
                break;
	        case 2:
	        	if(flag == 0) {
					printf("����δ���ò��������Ƚ�������\n");
					break;
				}
				if(simulation == NULL) simulation = malloc(sizeof(BankSimulation));
				// ��ʼ�����к��¼��� 
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
	            // ������ʼ�� 
			    generateInitialArrivalCondition(simulation);
			    // ģ������ҵ��
				simulate(simulation);
	            break;
	        case 3:
	        	if(simulation->total == -1) {
					printf("����δ���ò��������Ƚ�������\n");
					break;
				}
				if(simulation->events.head == NULL) {
					printf("����δ���г�ʼ�������Ƚ�������\n");
					break;
				}
	            // ��ʾ���
			    printf("ƽ������ʱ�䣺%lf\n", simulation->average_stay_time);
			    // ��ӡ�¼���
			    printEventTable(&(simulation->events));
			    //�ͷ��ڴ� 
				freeBankSimulation(simulation);
				//��ָ���ÿ� 
				simulation = NULL;
	            break;
	        case 4:
	            // �˳�ѭ��
	            break;
	        default:
	            printf("��Ч��ѡ�����������롣\n");
		    }
		    // �ȴ��û���������
	        printf("�� Enter ������...");
	        getchar();
	        getchar();
	    
	} while (choice != 4);
    
    // ��¼������� 
	logMessage("Application end.", INFO); 

    return 0;
}
