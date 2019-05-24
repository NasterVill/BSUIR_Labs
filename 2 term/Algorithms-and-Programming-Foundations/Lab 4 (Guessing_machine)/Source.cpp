#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "Header.h"

int main()
{
	bool move_on;
	bool push_type;
	NODE* ptr = nullptr;
	NODE* root = nullptr;
	STATISTICS* player = nullptr;
	ARRAY_INFO* unit = nullptr;
	FILE* tree_file;
	FILE* spreadsheet;
	char str[NAME_LENGTH] = { 0 };

	fopen_s(&tree_file, "Tree.txt", "r");
	if (!check_File(tree_file))
	{
		emergency_Exit();
	}
	root = create_Tree(tree_file);
	if (root == nullptr)
	{
		fclose(tree_file);
		emergency_Exit();
	}
	fclose(tree_file);

	fopen_s(&spreadsheet, "Spreadsheet.txt", "r");	// �������� ������� �������
	if (!check_File(spreadsheet))
	{
		emergency_Exit();
	}
	unit = load_Spredsheet(spreadsheet);
	if (unit == nullptr)
	{
		delete_Tree(root);
		fclose(tree_file);
		fclose(spreadsheet);
		emergency_Exit();
	}

	rewind(stdin);									// ��������� ���� ����� ������
	printf("Please enter your username\n");
	fgets(str, NAME_LENGTH, stdin);
	str[strlen(str) - 1] = '\0';

	fclose(spreadsheet);

	player = find_Player(unit, str);				// ����� ������ � �������
	if (player == nullptr)							// � ���������� ��� ����, � ������ ��� ���������
	{
		delete_Tree(root);
		free_Player_Ms(unit);
		emergency_Exit();
	}

	while(true)
	{
		system("cls");
		output_menu(player->user_name);
		switch (_getch())
		{
			case '1':
			{
				ptr = root;
				while (true)
				{

					system("cls");
					printf("%s?\n\n0. No \n1. Yes", ptr->string);

					accept_Pushtype(push_type);						// ��������� ����� ������������ (0 - ������, 1 - �������)

					move_on = push_Pointer(ptr, push_type);			// ������� ���������,
					if (move_on) continue;							// ���� �� ����� �� �� �����
					else											
					{
						if (!push_type) guess_Failed(player, ptr);	// ����� ���������� ������������, ��� �� �������, ���� ����� 0 (���)

						else guess_Succed();						// ���, ���� ����� 1 (��), �� ��������� ��������
					}
					break;

				}
				break;
			}

			case '2':
			{
				system("cls");

				spreadsheet = create_Spreadsheet(unit);				// ���������� ���������
				free_Player_Ms(unit);								// ������� ������
				fclose(spreadsheet);

				fopen_s(&spreadsheet, "Spreadsheet.txt", "r");		// ��������� �������� ����� 
				if (!check_File(spreadsheet))						// ��� ������ � ����� �������
				{
					delete_Tree(root);
					emergency_Exit();
				}

				unit = load_Spredsheet(spreadsheet);
				if (unit == nullptr)
				{
					delete_Tree(root);
					fclose(tree_file);
					fclose(spreadsheet);
					emergency_Exit();
				}

				rewind(stdin);
				memset(str, '\0', NAME_LENGTH);
				printf("Please enter your username\n");
				fgets(str, NAME_LENGTH, stdin);
				str[strlen(str) - 1] = '\0';

				fclose(spreadsheet);

				player = find_Player(unit, str);
				if (player == nullptr)
				{
					delete_Tree(root);
					free_Player_Ms(unit);
					emergency_Exit();
				}

				break;
			}

			case '0':
			{
				tree_file = create_Tree_File(root);
				delete_Tree(root);
				fclose(tree_file);
				
				spreadsheet = create_Spreadsheet(unit);
				free_Player_Ms(unit);
				fclose(spreadsheet);

				return 0;
			}

		}
	}
}