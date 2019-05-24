#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <locale.h>
#include <iostream>
#include <stdlib.h>
int main()
{
	setlocale(0,"");
	int dimension_value=0;				// ����������� ���������� �������
	int primes_amount=0;				// ����� ��������� ������� ������� �����
	int prime=0;						// ����������, ������������ ������� �����
	int i=0;
	int modulo=0;						// ������� �� �������
	int natural_number=0;				// ���������� ��� �������� ����������� �����
	int *ms;							// ��������� �� ������ ������� �����
	int line=0;
	int column=0;
	int dynamic_dimension=0;            // ������� ���������� ������ ������ / ������� ����
	int mat_edge=0;						// ������� ���������� ������ ����� / ������� �����
	int **mat;							// ������� (��������� �� ��������� �� ������)
		do								// ���� �������� �����
		{
			system("cls");
			puts("������� ����������� ������� (�� 1 �� 10)");
			fflush(stdin);
		}while(!scanf_s("%d", &dimension_value) || dimension_value<=0 || dimension_value>10 );
		system("cls");
		primes_amount=dimension_value*dimension_value; // ������ ����� ��������� �������
		ms=(int*)malloc(primes_amount*sizeof(int));
		for(i=0;i<primes_amount;i++)				   // ���� ���������� ������� �������� �������
		{
			prime++;
			for (natural_number=2;prime>natural_number/2;natural_number++) // �������� ����, ������� �� �����
			{
				modulo=prime%natural_number;
				if (!modulo) break;
			}
			if (!modulo && prime!=natural_number)  
				{
					i--;
					continue;
				}
			ms[i]=prime;
		}
		mat=(int**)calloc(dimension_value,sizeof(int*));			// ��������� ������ ��� ������ ����� (���������� �� ���)
		for(line=0;line<dimension_value;line++)						// ���� ��������� ������ 
			mat[line]=(int*)calloc(dimension_value,sizeof(int));	// ��� ������ ������
		i=0;
		line=0;
		dynamic_dimension=dimension_value;
		dynamic_dimension++;
		while (dynamic_dimension>0)		 // ���� ���������� �������
		{
			if (dynamic_dimension>2)
				{
					dynamic_dimension--; // ������ �������
				}
			if (dynamic_dimension==1)    // ���� ����������� ������� 1�1, �� ������ ��������� 1 �������
			{
				mat[line][column]=ms[i];
				break;
			}
			for (;column<dynamic_dimension;column++, i++) // ���������� ������ �����
				mat[line][column]=ms[i];
			column--;									  // ������������ �� ��������� ������� �������� ������� �������
			line++;										  // ���������� �� 1 ������ ����
			for (;line<dynamic_dimension;line++, i++)	  // ���������� ������� ����
				mat[line][column]=ms[i];
			line--;										  // ������������ �� ��������� ������� ������� ������ ������
			column--;                                     // ���������� �� 1 ������� �����
			for (;column>=mat_edge;column--, i++)		  // ���������� ������ ������
				mat[line][column]=ms[i];
			column++;									  // ������������ � ������� ����� �������
			line--;										  // ���������� �� 1 ������ �����
			if (dynamic_dimension==2) break;			  // ���� ����������� ������� 2�2, �� ���� �����������
			for (;line>(mat_edge);line--, i++)			  // ���������� ������� �����
				mat[line][column]=ms[i];
			line++;										  // ������������ �� ������� ������� ������
			column++;									  // ���������� �� 1 ������� ������
			mat_edge++;									  // ���������� ���������, �������������� ���������� ������� ������ ������ � �������� ������ �������
		}
		printf("\n �������[%d]�[%d]:\n",dimension_value,dimension_value);
		for (line=0;line<dimension_value;line++)		   // ���� ������ ������� �� �����
		{
			printf("\n\n| ",dimension_value);
			for (column=0;column<dimension_value;column++)
			{
				printf("%4d", mat[line][column]);
				if (column==dimension_value-1) printf("   |");
			}
		}
		for(line=0;line<dimension_value;line++)
			free(mat[line]);
		free(mat);
		free(ms);
		_getch();
		return 0;
}