#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "Header.h"

#define TEST_AMOUNT 7

int main()
{
	vinyl *collection;
	int amount = 0;
	int choice = 0;
	bool created = false;
	bool initialized = false;

	printf("Welcome! Thank you for using this 'Vinyl Manager App'\n\n");

	securedNumberInput(amount, "Please enter the number of records in the collection");

	collection = createArray(amount);
	created = true;

	for (;;)
	{
		system("cls");
		printf("Please select the action to perform\n\n");
		printf("================================================================================\n\n");
		printf("\t\t\t\t\t  [The overall amount of records : %d]\n", amount);
		printf("1. Create new collection\n");
		printf("2. Initialize the collection manually\n");
		printf("3. Create test collection and initialize it with templates\n");
		printf("4. Sort the collection\n");
		printf("5. Delete an element from the collection\n");
		printf("6. Delete the collection\n");
		printf("7. View the collection\n");
		printf("0. Exit the program\n\n\n");
		printf("================================================================================\n\n");
		switch (_getch())
		{
		case '1':
		{
			if (created)
			{
				for (;;)
				{
					system("cls");
					printf("\aThe collection is already created. Are you sure, you want to delete it?\n\n");
					printf("Press '1' to continue action.\nPress '2' to reject ");
					switch (_getch())
					{
						case '1':
						{
							freeArray(collection, amount);
							system("cls");
							securedNumberInput(amount, "Please enter the number of records in the collection");
							collection = createArray(amount);
							break;
						}
						case '2':
						{
							break;
						}
						default: continue;
					}
					break;
				}
			}
			else
			{
				system("cls");
				securedNumberInput(amount, "Please enter the number of records in the collection");
				collection = createArray(amount);
				created = true;
			}
			break;
		}

		case '2':
		{
			system("cls");
			if (!created)
			{
				printf("\aThe collection is not created!\n");
				printf("Press any key to continue...");
				_getch();
				system("cls");
				break;
			}
			for (int i = 0; i < amount; i++)
			{
				printf("Record %d of %d\n", i + 1, amount);
				printf("Please enter the artist's name\n");
				initializeString(collection + i, ARTIST);

				printf("Please enter the record's name\n");
				initializeString(collection + i, ALBUM);

				initializeYear(collection + i);

				printf("Plese select the quality of the record\n\n");
				printf("1. Excelent\n2. Very Good\n3. Good\n4. Fair\n5. Poor\n6. Bad\n");
				initializeQuality(collection + i);

				printf("Please select the diameter of the record\n\n");
				printf("1. 12 inches\n2. 10 inches\n3. 7 inches\n");
				initializeDiameter(collection + i);
			}
			initialized = true;
			break;
		}

		case '3':
		{
			system("cls");
			if (created)
			{
				freeArray(collection, amount);
				system("cls");
			}

			amount = TEST_AMOUNT;
			collection = createArray(amount);
			created = true;

			strcpy_s(collection[0].artist, 14, "Judas Priest\n");
			strcpy_s(collection[0].album, 15,"British Steel\n");
			collection[0].year =  1980;
			collection[0].mark = VERY_GOOD;
			collection[0].measure = TWELVE_INCHES;

			strcpy_s(collection[1].artist, 11,"B.B. King\n");
			strcpy_s(collection[1].album, 22,"Riding with the King\n");
			collection[1].year = 2000;
			collection[1].mark = EXCELENT;
			collection[1].measure = TWELVE_INCHES;

			strcpy_s(collection[2].artist, 7,"AC/DC\n");
			strcpy_s(collection[2].album, 17,"Highway to Hell\n");
			collection[2].year = 1979;
			collection[2].mark = GOOD;
			collection[2].measure = TEN_INCHES;

			strcpy_s(collection[3].artist, 7,"AC/DC\n");
			strcpy_s(collection[3].album, 15,"Back in Black\n");
			collection[3].year = 1980;
			collection[3].mark = POOR;
			collection[3].measure = TWELVE_INCHES;

			strcpy_s(collection[4].artist, 15, "Frank Sinatra\n");
			strcpy_s(collection[4].album, 13, "That's Life\n");
			collection[4].year = 1966;
			collection[4].mark = FAIR;
			collection[4].measure = SEVEN_INCHES;

			strcpy_s(collection[5].artist, 13, "MAXIM DOSHI\n");
			strcpy_s(collection[5].album, 15, "I am the Best\n");
			collection[5].year = 2012;
			collection[5].mark = BAD;
			collection[5].measure = TWELVE_INCHES;

			strcpy_s(collection[6].artist, 17, "Michael Jackson\n");
			strcpy_s(collection[6].album, 10, "Thriller\n");
			collection[6].year = 1982;
			collection[6].mark = POOR;
			collection[6].measure = SEVEN_INCHES;

			initialized = true;

			printf("The test collection is successfully initialized!\n");
			printf("Press any key to continue...");
			_getch();
			system("cls");

			break;
		}

		case '4':
		{
			system("cls");
			if (!created || !initialized)
			{
				printf("\aThe collection is not created or initialized!\n");
				printf("Press any key to continue...");
				_getch();
				system("cls");
				break;
			}

			func_ptr criterion1;
			func_ptr criterion2;

			for (;;)
			{
				system("cls");
				printf("Please select the first sorting critirea:\n\n");
				printf("1. By Artist\n");
				printf("2. By Album\n");
				printf("3. By Year\n");
				printf("4. By Quality\n");
				printf("5. By Diameter\n");
				switch (_getch())
				{
				case '1':
				{
					criterion1 = artistSort;
					break;
				}
				case '2':
				{
					criterion1 = albumSort;
					break;
				}
				case '3':
				{
					criterion1 = yearSort;
					break;
				}
				case '4':
				{
					criterion1 = qualitySort;
					break;
				}
				case '5':
				{
					criterion1 = diameterSort;
					break;
				}
				default: continue;
				}
				break;
			}
			for (;;)
			{
				system("cls");
				printf("Please select the second sorting critirea:\n\n");
				printf("1. By Artist\n");
				printf("2. By Album\n");
				printf("3. By Year\n");
				printf("4. By Quality\n");
				printf("5. By Diameter\n");
				switch (_getch())
				{
				case '1':
				{
					criterion2 = artistSort;
					break;
				}
				case '2':
				{
					criterion2 = albumSort;
					break;
				}
				case '3':
				{
					criterion2 = yearSort;
					break;
				}
				case '4':
				{
					criterion2 = qualitySort;
					break;
				}
				case '5':
				{
					criterion2 = diameterSort;
					break;
				}
				default: continue;
				}
				if (criterion1 == criterion2)
				{
					system("cls");
					printf("\aYou have chosen two identical criteria! Plesase repeat the action\n");
					printf("Press any key to continue...");
					_getch();
					system("cls");
					continue;
				}
				break;
			}
			sortArray(collection, amount, criterion1, criterion2);
			break;
		}

		case '5':
		{
			system("cls");
			if (!created || !initialized)
			{
				printf("\aThe collection is not created or initialized!\n");
				printf("Press any key to continue...");
				_getch();
				system("cls");
				break;
			}
			else if (amount == 1)
			{
				freeArray(collection, amount);
				created = false;
				initialized = false;
				amount = 0;
				printf("The collection has been sucsessfully deleted!\n");
				printf("Press any key to continue...");
				_getch();
				system("cls");
				break;
			}
			else
			{
				printf("Hint:\n\n");
				outputArray(collection, amount);
				printf("Press any key to continue action");
				_getch();
				system("cls");
				securedNumberInput(choice, "Plese enter the number of the record you want to delete");
				system("cls");
				deleteElement(collection, amount, choice - 1);
				printf("The record has been sucsessfully deleted!\n");
				printf("Press any key to continue...");
				_getch();
				system("cls");
				break;
			}
		}

		case '6':
		{
			if (created)
			{
				system("cls");
				freeArray(collection, amount);
				created = false;
				initialized = false;
				amount = 0;
				printf("The collection has been sucsessfully deleted!\n");
			}
			else
			{
				system("cls");
				printf("The collection is alredy deleted.\n");
			}
			printf("Press any key to continue...");
			_getch();
			system("cls");
			break;
		}

		case '7':
		{
			system("cls");
			if (!created || !initialized)
			{
				printf("\aThe collection is not created or initialized!\n");
				printf("Press any key to continue...");
				_getch();
				system("cls");
			}
			else
			{
				outputArray(collection, amount);
				printf("Press any key to continue...");
				_getch();
				system("cls");
			}
			break;
		}

		case '0':
		{
			if (created)
			{
				freeArray(collection, amount);
			}
			return 0;
		}

		default: continue;
		}
	}
}