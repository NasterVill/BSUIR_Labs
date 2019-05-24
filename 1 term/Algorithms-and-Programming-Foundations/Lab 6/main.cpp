#include "sorts.h"
#include <stdio.h>
#include <conio.h>
#include <locale.h>

int main()
{
	setlocale(LC_ALL,"RUS");
	int* ms, size;
	do
	{
		fflush(stdin);
		printf("������� ����������� ������� � ���������� : ");
	}while(!scanf("%d", &size) || size<1);
	system("cls");
	ms=(int*)malloc(size*sizeof(int));
	assert(ms != nullptr);
	randInit(ms, size);
	printf("���� �������� ������� ������������, ������ ���������� ������� �����������");
	printf("\n\n���������� :\n\n����� ��������� : %d", size);
	printf("\n\n����� � �������� hoar %f", timeCheck(ms, size, hoarSort));
	printf("\n\n����� � �������� optimizedHoar %f", timeCheck(ms, size, optimizedHoar));
	printf("\n\n����� � �������� shell %f", timeCheck(ms, size, shellSort));
	printf("\n\n����� � �������� insert %f", timeCheck(ms, size, insertSort));
	printf("\n\n����� � �������� selection %f", timeCheck(ms, size, selectionSort));
	printf("\n\n����� � �������� shaker %f", timeCheck(ms, size, shakerSort));
	printf("\n\n����� � �������� bubble %f", timeCheck(ms, size, bubbleSort));
	_getch();
	free(ms);
	return 0;
}