#pragma once

enum quality
{
	EXCELENT,
	VERY_GOOD,
	GOOD,
	FAIR,
	POOR,
	BAD
};

enum diameter
{
	TWELVE_INCHES,
	TEN_INCHES,
	SEVEN_INCHES
};

enum discription
{
	ARTIST,
	ALBUM
};

struct vinyl
{
	char* artist;
	char* album;
	int year;
	quality mark;
	diameter measure;
};

typedef int(*func_ptr)(vinyl record_1, vinyl record_2);

vinyl* createArray(int size);
void initializeString(vinyl* record, discription type);
void initializeYear(vinyl* record);
void initializeQuality(vinyl* record);
void initializeDiameter(vinyl* record);
void printQuality(vinyl record);
void printDiameter(vinyl record);
int artistSort(vinyl record_1, vinyl record_2);
int albumSort(vinyl record_1, vinyl record_2);
int yearSort(vinyl record_1, vinyl record_2);
int diameterSort(vinyl record_1, vinyl record_2);
int qualitySort(vinyl record_1, vinyl record_2);
bool comparison_result(vinyl record_1, vinyl record_2, func_ptr compare_1, func_ptr compare_2);
void outputArray(vinyl* arr, int size);
void sortArray(vinyl* arr, int size, func_ptr compare_1, func_ptr compare_2);
void deleteElement(vinyl* arr, int &size, int position);
void freeArray(struct vinyl* arr, int size);
void securedNumberInput(int &number, char* input_message);
void emergencyExit();