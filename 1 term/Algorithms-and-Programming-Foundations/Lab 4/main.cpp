#include "functions_header.h"
#include <stdio.h>
#include <locale.h>
#include <conio.h>
#include <stdlib.h>
int main()
{
	setlocale(0,"");
	int *ms;
	int size=0;
	int scanf_check=1;  //���������� ��� �������� ���������� ������� scanf_s
	printf("\t      � ������� ������������!\n\n ����� ������ ������ � ����������, ������� (1)\n\t\t      ���\n ����� �����, ������� ����� ������ �������");
	if(_getch()!='1')
		{
			return 0;
		}
	for(;;)
	{
	do
		{
			system("cls");
			if (!scanf_check) printf("\a������ ��������� ����\n\n");
			puts(" ������� ����� ��������� �������:");
			fflush(stdin);
			scanf_check=scanf_s("%d",&size);
			if(size<=0 || size>101000) scanf_check=0;
		}while(!scanf_check);
	ms=create_array(size);
	system("cls");
	printf("\t�������� ��� ������������� �������\n\n");
	printf("===================================================================================================\n\n");
	printf("(1) : ������������� ������� � ����������\n\n");
	printf("(2) : ������������� ������� ���������� �������\n\n");
	printf("===================================================================================================\n\n");
	for(;;)
	{
		switch(_getch())
		{
		case '1': 
			{
				system("cls");
				keyboard_init(ms, size);
				system("cls");
				printf (" ������ ������� ��������\n\n ����� ���������� ������, ������� ����� �������");
				_getch();
				break;
			}
		case '2':
			{
				system("cls");
				random_init(ms, size);
				printf (" ������ ������� ��������\n\n ����� ���������� ������, ������� ����� �������");
				_getch();
				break;
			}
		default : continue;
		}		// ����� switch
		break;
	}			// ����� ����� ������ ���� �������������
	for(;;)
	{
		int special=0;				// ������ ������� (�����������/������������/���������)
		int number_of_special=0;	// ����� �����������/������������ ���������
		int choice=0;				// ���������� ��� ������ ���� ����������
		system("cls");
		printf("\n\t�������� ��������:\n\n");
		printf("===================================================================================================\n\n");
		printf("(1) : �������� ������� (����� �� �����) \t\t\t(����� ��������� ������� [%d])\n",size);
		printf("(2) : ����� � ����� ������������� �������� � �������\n");
		printf("(3) : ����� � ����� ������������ �������� � �������\n");
		printf("(4) : ���������� �������\n");
		printf("(5) : �������� ���������, ����������� � �������� ���������\n");
		printf("(6) : �������� ������� � �������� ������\n\n");
		printf("===================================================================================================\n\n");
		printf("����� �����, ������� (0)");
		switch(_getch())
		{
		case '0':
			{
				size=delete_array(ms);
				return 0;
			}
		case '1': 
			{
				system("cls");
				output(ms, size);
				printf ("\n\n ����� ���������� ������, ������� ����� �������");
				_getch();
				continue;
			}
		case '2':
			{
				system("cls");
				find_max(ms, size, &number_of_special, &special);
				printf("������������ ������� ������� = [%d] ����� ������������ �������� = [%d]",special,number_of_special);
				printf ("\n\n ����� ���������� ������, ������� ����� �������");
				_getch();
				continue;
			}
		case '3':
			{
				system("cls");
				find_min(ms, size, &number_of_special, &special);
				printf("����������� ������� ������� = [%d] ����� ����������� �������� = [%d]",special,number_of_special);
				printf ("\n\n ����� ���������� ������, ������� ����� �������");
				_getch();
				continue;
			}
		case '4':
			{
				system("cls");
				do
				{
					system("cls");
					printf("���������� �� (1) �������� (2) ����������� ");
					fflush(stdin);
				}while(!scanf_s("%d",&choice) || (choice!=1 && choice!=2));
				sort (ms, size, choice);
				system("cls");
				printf (" ������ ������� ������������\n\n ����� ���������� ������, ������� ����� �������");
				_getch();
				continue;
			}
		case '5':
			{
				do
				{
					system("cls");
					printf("������� �������, ������� �� ������� ������� :");
				}while(!scanf_s("%d", &special));
				system("cls");
				size=delete_element(ms, size, special);
				if (size==0)
				{
					delete_array(ms);
					printf (" ������ ������� ������\n\n ������� ����� �������, ����� ������� ����� ������");
					_getch();
					break;
				}
				else 
				{
					ms=(int*)realloc(ms, size*sizeof(int));   // ���������� ����� �������
					printf (" ��������� �������� ������� �������\n\n ����� ���������� ������, ������� ����� �������");
					_getch();
					continue;
				}
			}
			case '6':
			{
				size=delete_array(ms);
				break;
			}
			case '7':
			{
				system("cls");
				shellSort(ms, size);
				printf (" ������ ������� ������������\n\n ����� ���������� ������, ������� ����� �������");
				_getch();
				continue;
			}
			default : continue;
		}
		break;
		}	// ����� ����� ����������� ��� �������� (��������, �������� ���������, ����������, ��������)
	}		// ����� ����� ������ � ��������  (��������, ������������� � ����������������� ��������)
}			// ����� ���� main