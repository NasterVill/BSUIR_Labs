#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "Header.h"

#define MAX_LENGTH 29

vinyl* createArray(int size)
{
	vinyl *arr;
	arr = (vinyl*)malloc(size * sizeof(vinyl));
	if (arr == nullptr)
	{
		emergencyExit();
	}
	for (int i = 0; i < size; i++)
	{
		arr[i].artist = (char*)calloc(MAX_LENGTH + 1, sizeof(char));
		if (arr[i].artist == nullptr)
		{
			emergencyExit();
		}
		arr[i].album = (char*)calloc(MAX_LENGTH + 1, sizeof(char));
		if (arr[i].album == nullptr)
		{
			emergencyExit();
		}
	}
	return arr;
}

void initializeString(vinyl* record, discription type)
{
	rewind(stdin);
	switch (type)
	{
		case ARTIST :
		{
			fgets(record->artist, MAX_LENGTH, stdin);

			if (strlen(record->artist) == MAX_LENGTH - 1)
			{
				*((record->artist) + MAX_LENGTH - 1) = '\n';
			}
			break;
		}
		case ALBUM :
		{
			fgets(record->album, MAX_LENGTH, stdin);

			if (strlen(record->album) == MAX_LENGTH - 1)
			{
				*((record->album) + MAX_LENGTH - 1) = '\n';
			}
			break;
		}
	}
	system("cls");
}

void initializeYear(vinyl* record)
{
	int year = 1860;
	int check = 1;

	do
	{
		printf("Please enter the recording year\n");
		if (!check || year < 1859)
		{
			printf("\aError! Please repeat the input\n");
		}
		rewind(stdin);
		check = scanf_s("%d", &year);
		system("cls");
	} while (!check || year < 1859);		// 1859 - год изготовления первой грампластинки

	record->year = year;
}

void initializeQuality(vinyl* record)
{
	for (;;)
	{
		switch (_getch())
		{
		case '1':
		{
			record->mark = EXCELENT;
			break;
		}
		case '2':
		{
			record->mark = VERY_GOOD;
			break;
		}
		case '3':
		{
			record->mark = GOOD;
			break;
		}
		case '4':
		{
			record->mark = FAIR;
			break;
		}
		case '5':
		{
			record->mark = POOR;
			break;
		}
		case '6':
		{
			record->mark = BAD;
			break;
		}
		default: continue;
		}
		break;
	}

	system("cls");
}

void initializeDiameter(vinyl* record)
{
	for (;;)
	{
		switch (_getch())
		{
		case '1':
		{
			record->measure = TWELVE_INCHES;
			break;
		}
		case '2':
		{
			record->measure = TEN_INCHES;
			break;
		}
		case '3':
		{
			record->measure = SEVEN_INCHES;
			break;
		}
		default: continue;
		}
		break;
	}

	system("cls");
}

void outputArray(vinyl* arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		printf("No. %d\n\nArtist: %s", i + 1, arr[i].artist);
		printf("Album: %s", arr[i].album);
		printf("Year: %d\n", arr[i].year);
		printQuality(arr[i]);
		printDiameter(arr[i]);
	}
}

void printQuality(vinyl record)
{
	printf("Quality: ");
	switch (record.mark)
	{
		case EXCELENT:
		{
			printf("Excelent\n");
			break;
		}
		case VERY_GOOD:
		{
			printf("Very Good\n");
			break;
		}
		case GOOD:
		{
			printf("Good\n");
			break;
		}
		case FAIR:
		{
			printf("Fair\n");
			break;
		}
		case POOR:
		{
			printf("Poor\n");
			break;
		}
		case BAD:
		{
			printf("Bad\n");
			break;
		}
	}
}

void printDiameter(vinyl record)
{
	printf("Diameter: ");
	switch (record.measure)
	{
		case TWELVE_INCHES:
		{
			printf("Twelve inches\n\n");
			break;
		}
		case TEN_INCHES:
		{
			printf("Ten inches\n\n");
			break;
		}
		case SEVEN_INCHES:
		{
			printf("Seven inches\n\n");
			break;
		}
	}
}

void freeArray(vinyl* arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		free(arr[i].artist);
		free(arr[i].album);
	}
	free(arr);
	arr = nullptr;
}

void deleteElement(vinyl* arr, int &size, int position)
{
	vinyl temp;

	for (int i = position; i < size - 1; i++)
	{
		temp = arr[i + 1];
		arr[i + 1] = arr[i];
		arr[i] = temp;
	}

	free(arr[size - 1].artist);
	free(arr[size - 1].album);
	size--;
	arr = (vinyl*)realloc(arr, size*(sizeof(vinyl)));
}

int artistSort(vinyl record_1, vinyl record_2)
{
	return strcmp(record_1.artist, record_2.artist);
}

int albumSort(vinyl record_1, vinyl record_2)
{
	return strcmp(record_1.album, record_2.album);
}

int yearSort(vinyl record_1, vinyl record_2)
{
	if (record_1.year > record_2.year) return 1;
	if (record_1.year == record_2.year) return 0;
	return -1;
}

int qualitySort(vinyl record_1, vinyl record_2)
{
	if (record_1.mark > record_2.mark) return 1;
	if (record_1.mark == record_2.mark) return 0;
	return -1;
}

int diameterSort(vinyl record_1, vinyl record_2)
{
	if (record_1.measure > record_2.measure) return 1;
	if (record_1.measure == record_2.measure) return 0;
	return -1;
}

bool comparison_result(vinyl record_1, vinyl record_2, func_ptr compare_1, func_ptr compare_2)
{
	int result = compare_1(record_1, record_2);
	if (result > 0)
		return true;
	if (result == 0 && compare_2(record_1, record_2) > 0)
		return true;
	return false;
}

void sortArray(vinyl* arr, int size, func_ptr compare_1, func_ptr compare_2)
{
	vinyl temp;
	int i, j;

	for (i = 1; i < size; ++i)
	{
		j = i - 1;
		temp = arr[i];
		while (j >= 0 && comparison_result(arr[j], temp, compare_1, compare_2))
		{
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = temp;
	}
}

void securedNumberInput(int &number, char* input_message)
{
	int check = 1;
	number = 1;
	do
	{
		printf("%s:\n", input_message);
		if (!check || number <= 0)
		{
			printf("\aError! Please repeat the input\n");
		}
		rewind(stdin);
		check = scanf_s("%d", &number);
		system("cls");
	} while (!check || number <= 0);
}

void emergencyExit()
{
	system("cls");
	printf("The memory has not been allocated!\nPress any key to exit the programm...");
	_getch();
	exit(1);
}