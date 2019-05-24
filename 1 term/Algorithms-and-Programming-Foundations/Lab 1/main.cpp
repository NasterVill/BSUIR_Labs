#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <conio.h>
#include <iostream>
double power(double number, int power) // ������� ���������� ����� � ������������� �������
{
	int i;
	double num=number;					 //������ ���������� 'num', ����������� ��� ���������� ����� � ������������� �������//
	switch (power)
	{
		case 0:num=1; break;			 // ���� ������� = 0, �� ��������� = 1
		case 1:num=number; break;		 // ���� ������� = 1, �� ��������� ����� ��������� �����
		default: for (i=1; i<power; i++) // ���� ���������� � ������������� �������
		num=num*number;
	}
	return num;
}
double factorial(int value) // ������� �������� ����������
{
	int i;
	double result=1;			//������ ���������� 'result', ����������� ��� �������� ���������� �����//
	for (i=1; i<=value; i++)	// ���� �������� ����������
	{
		result=result*(double)i;
	}
	return result;
}
double module(double number) // ������ ���������� ������ �����
{
	if (number<0)
		number=number*(-1);
	return number;
}
double adduction(double angle) // ������� ���������� ���� � ��������� �� 0 �� ���� pi
{
	double pi=180;
	if (angle<0)
	{
		angle=angle*(-1);
	}
	while (angle>2*pi)
		angle=angle-(2*pi);
	return angle;
}
double function_cos(double angle, double precision) // ������� �������� �������� ����, ����������� � ��������, � ��������� ���������
{
	int n=(-1);			// ������ ����������, ������� ����� ���������� ����� ����������� � ���� ��������� ��� ������� "�������"
	double summand1;	// ����������, ������� ����� ������������� �������� ��������� � ���� ���������
	double summand2=0;	// ����������, ������� ����� ������������� �������� ���������� ����������
	do
	{
		n++;
		summand1=power((-1), n)*power(angle, 2*n)/factorial(2*n);
		summand2+=summand1;
	}while (module(summand1)>precision); // ��� ���������� �������� ���������� ��������, ������ ������� ���������� ���� ������������ �� ��������� ��������, ��������� � ����������
	return summand2;
}
int main()
 {
	 setlocale(0,"");
	 int digit_amount;					// ����� ������ ����� ������� � �������� ��������
	 int i;								// ���������� ��� ������ � ������
	 int choice;						// ����� ������ ���������
	 double angle;						// ����, �������� � ���������
	 double helping_angle;				// ��������������� ����, ���������� � ��������
	 double cos_angle;					// ������� ����
	 double precision=1;				// �������� �������� �������� (�����, � ������� ����� ������������ ��������� � ���� ���������)
	 double rad_coeff;					// �����������, �����������, ������� ��������� � 1 pi
	 double PI=M_PI;					// ����� pi
	 rad_coeff=180/PI;
	 do									// ������ ��������� ���������
	 {
		 do
		 {
			 system("cls");
			 printf ("������� ������� ��������� ������� ��� ���� : �������(1) ��� �������(2) ");
			 fflush(stdin);
		 }while(scanf_s("%d",&choice)!=1 || (choice!=1 && choice!=2)); // ���������� ����, ���� �� ������� 1 ��� 2
		 do
		 {
			 system("cls");
			 printf ("������� ��������� �������� ���� : ");
			 fflush(stdin);
		 }while(scanf_s("%12lf",&angle)!=1); 
		 if(choice==1) angle*=rad_coeff;	// ������� ���� � �������, ���� �� ��� ������ � ��������
		 angle=adduction(angle);			// ���������� ���� � �������� �� 0 �� 360 ��������
		 helping_angle=angle;				// ���������� �������� ���� � ��������
		 angle/=rad_coeff;					// ������� �������� ���� � �������
		 do
		 {
			 system("cls");
			 printf ("������� ������ �������� �������� �������a ���� \n(���������� ������ ����� �������, �� ������ 15):");
			 fflush(stdin);
		 }while(scanf_s("%d", &digit_amount)!=1 || digit_amount>15 || digit_amount<=0);
		 system("cls");
		 for (i=1; i<=digit_amount; i++)
		 {
			 precision/=10;	
		 }
		 cos_angle=function_cos(angle,precision); // ���������� �������� ����
		 printf("============================================================================\n\n");
		 printf("�������� ���������� ����: %lf ������ ��� %lf ��������",angle,helping_angle);
		 printf("\n\n������� ���� = %.*lf\n\n�������, ������������ ��� ������ ������������ ������� = %.*lf",digit_amount,cos_angle,digit_amount,cosl(angle));// �osl(angle) -- ������������ ������� �������� �������� ����
		 printf("\n\n============================================================================\n");
		 printf("\n����� ���������� ������ � ���������� ������� (1)\n���\n����� ����� ������� ����� ������ ������");
	 }while(_getch()=='1');	 // ������� ����������� ������ � ����������
	 return 0;
 }