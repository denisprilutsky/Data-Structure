// gcc list.c -o list -l ncurses
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int NumAll = 0; // ���������� ��������� �����

typedef struct _SList { // ������
	char Data;
	int Num;
	struct _SList *Last;
	struct _SList *Next;
} SList;

void textprint() {
	printw("|---------------------------------------------------------|\n");
	printw("\"1\" - add cell to List;\n");
	printw("\"2\" - delete cell List;\n");
	printw("\"3\" - exit.\n");
	printw("|---------------------------------------------------------|\n\n");
	printw("Enter the command:\n");
}

SList *push(SList *Cur, char InputData, int ReplCell) { // ���������� ��������
	if (NumAll == 0) { // ������� ������ ������� ������
		if (ReplCell != 1) printw("There are no cells!\n");
		SList *New = malloc(sizeof(SList));
		(*New).Last = NULL;		
		(*New).Next = NULL;
		(*New).Data = InputData;
		(*New).Num = 1;
		NumAll++; // ����������� � ������ ����� ���������
		return New;
	} else {
		if (ReplCell < NumAll) { // ���������� ����� ������ ����� �������������. �������� ������ ���������� ��������
			bool Direct = false; // ����� ����������� ������			
			while((*Cur).Num != ReplCell) {  // ����� �������� � ��������� �������
				if (Direct == true ) { // ������ �����������
					if ((*Cur).Next == NULL) {
						Direct = !Direct;
					} else Cur = (*Cur).Next;
				} else { // �������� �����������
					if ((*Cur).Last == NULL) {
						Direct = !Direct;
					} else Cur = (*Cur).Last;
				}							
			}	
			SList *New = malloc(sizeof(SList));
			(*New).Next = Cur; // 1
			Cur = (*Cur).Last;
			(*New).Last = Cur; // 2			
			(*Cur).Next = New; // 3
			Cur = (*New).Next;
			(*Cur).Last = New; // 4
			(*New).Data = InputData; // ������ ������
			(*New).Num = ReplCell; // ������ ����������� ������
			NumAll++; // ����������� � ������ ����� ���������
			(*Cur).Num++;
			while((*Cur).Next != NULL) { // ������������� � ����� ������
				Cur = (*Cur).Next;
				(*Cur).Num++;								
			}
			return New;
		} else { // ���� ����� ������ ��� ����� ������ ����� ������������ ���������, �� �������� ��������� ������� � ����� ������
			printw("This is no cell! Add in the End.\n");
			while((*Cur).Next != NULL) Cur = (*Cur).Next; // ����� ���������� ��������
			SList *New = malloc(sizeof(SList));
			(*Cur).Next = New;
			(*New).Last = Cur;
			(*New).Next = NULL;
			(*New).Data = InputData; // ������ ������
			NumAll++; // ����������� � ������ ����� ���������
			(*New).Num = NumAll; // ������ ������
			return New;
		}
	}
}

SList *pull(SList *Cur, int DelCell) { // �������� ��������
	if (NumAll == 0) { // ��� ��������� ��� ��������
		if (DelCell != 1) printw("There are no cells for delete!\n");
		return Cur;
	} else {
		if ((DelCell < NumAll) && (NumAll != 1)) { // �������� ������ ����� �������������. �������� ������ ���������� ��������
			if (DelCell == 1) { // �������� ������� �������� � ������������ ������
				bool Direct = false; // ����� ����������� ������			
				while((*Cur).Num != DelCell) {  // ����� �������� � ��������� �������
					if (Direct == true ) { // ������ �����������
						if ((*Cur).Next == NULL) {
							Direct = !Direct;
						} else Cur = (*Cur).Next;
					} else { // �������� �����������
						if ((*Cur).Last == NULL) {
							Direct = !Direct;
						} else Cur = (*Cur).Last;
					}							
				}	
				Cur = (*Cur).Next;
				free((*Cur).Last);
				(*Cur).Last = NULL;
				NumAll--; // ��������� �� ������ ����� ���������
				(*Cur).Num--;
				while((*Cur).Next != NULL) { // ������������� � ����� ������
					Cur = (*Cur).Next;
					(*Cur).Num--;								
				}
			} else { // �������� �������� ����� ������������� ����������
				bool Direct = false; // ����� ����������� ������			
				while((*Cur).Num != DelCell) {  // ����� �������� � ��������� �������
					if (Direct == true ) { // ������ �����������
						if ((*Cur).Next == NULL) {
							Direct = !Direct;
						} else Cur = (*Cur).Next;
					} else { // �������� �����������
						if ((*Cur).Last == NULL) {
							Direct = !Direct;
						} else Cur = (*Cur).Last;
					}
				}
				(*(*Cur).Last).Next = (*Cur).Next;
				(*(*Cur).Next).Last = (*Cur).Last;
				free(Cur);
				NumAll--; // ��������� �� ������ ����� ���������
				(*Cur).Num--;
				while((*Cur).Next != NULL) { // ������������� � ����� ������
					Cur = (*Cur).Next;
					(*Cur).Num--;								
				}
			}
			return Cur;
		} else { // ���� ����� ������ ��� ����� ������ ����� ������������ ���������, �� ������� ��������� ������� ������
			if (NumAll == 1) { // ���� ������� ���� �������, �� ���������� ������ � ������� NULL
				free(Cur);
				NumAll--;
				return NULL;
			}
			printw("This is no cell! Delete the End Cell.\n");
			while((*Cur).Next != NULL) Cur = (*Cur).Next; // ����� ���������� ��������
			Cur = (*Cur).Last;
			free((*Cur).Next);
			(*Cur).Next = NULL;
			NumAll--; // ����������� � ������ ����� ���������
			return Cur;
		}
	}
}

int printlist(SList *Cur) { // ����� ���� ���������
	if (NumAll != 0) {
		while((*Cur).Last != NULL) { // ����� ������� ��������
			Cur = (*Cur).Last; // �������� ���������� �������
		}
		printw("\nPrint List:\n");
		if (NumAll > 1) {
			do { // ����� ���� ���������;
				printw("Cell #%i with data: \"%C\"; \n", (*Cur).Num, (*Cur).Data);
				Cur = (*Cur).Next;
			} while((*Cur).Next != NULL);
		}
		printw("Cell #%i with data: \"%C\"; \n\n", (*Cur).Num, (*Cur).Data);
	}
	return 0;
}

int main(int NumTerm, char **InTerm) {
	int NumCell = 0; // ����� ���������� ������
	char NewData; // �������� ������ ��� ����� ������
	SList *LP = NULL; // ��������� ������
	LP = (SList*) malloc(sizeof(SList)); // �������� ������������ ��� ��������� ������
	initscr(); // ������������� ncurses.h
	clear(); // ������� ���������
	while(true) {
		textprint(); // ����� ���������� � ��������
		switch (getch()) { // ���� �������
			case '1' : // ���������� � ������
				clear(); // ������� ���������
				printw("Enter the Data:\n");
				scanw("%C", &NewData);
				clear(); // ������� ���������;
				printw("Enter the Number Cell:\n");
				scanw("%i", &NumCell);
				LP = push(LP, NewData, NumCell);
				clear();
				printlist(LP); // ����� ����� ������
				break;
			case '2' : // �������� �������� �� ������
				clear();
				printw("Enter the Number Cell for delete:\n");
				scanw("%i", &NumCell);
				LP = pull(LP, NumCell);
				clear();
				printlist(LP); // ����� ����� ������
				break;
			case '3' : // �����
				endwin(); // ��������������� ncurses.h
				return 0;
				break;
			case '4' : // ����� ������
				clear(); // ������� ���������
				printlist(LP); // ����� ����� ������
				break;
			default :
				printw("�ommand is missing.\n");
				break;
		}
	};
}