// gcc list.c -o list -l ncurses
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int NumAll = 0; // Количество созданных ячеек

typedef struct _SList { // Список
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

SList *push(SList *Cur, char InputData, int ReplCell) { // Добавление элемента
	if (NumAll == 0) { // Создаем первый элемент списка
		if (ReplCell != 1) printw("There are no cells!\n");
		SList *New = malloc(sizeof(SList));
		(*New).Last = NULL;		
		(*New).Next = NULL;
		(*New).Data = InputData;
		(*New).Num = 1;
		NumAll++; // Прибавление к общему числу элементов
		return New;
	} else {
		if (ReplCell < NumAll) { // Добавление новой ячейки между существующими. Проверка номера введенного элемента
			bool Direct = false; // Смена направления поиска			
			while((*Cur).Num != ReplCell) {  // Поиск элемента с введенным номером
				if (Direct == true ) { // Прямое направление
					if ((*Cur).Next == NULL) {
						Direct = !Direct;
					} else Cur = (*Cur).Next;
				} else { // Обратное направление
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
			(*New).Data = InputData; // Запись данных
			(*New).Num = ReplCell; // Запись порядкового номера
			NumAll++; // Прибавление к общему числу элементов
			(*Cur).Num++;
			while((*Cur).Next != NULL) { // Пронумеровать с новой ячейки
				Cur = (*Cur).Next;
				(*Cur).Num++;								
			}
			return New;
		} else { // Если номер больше или равен общему числу существующих элементов, то добавить введенный элемент в конец списка
			printw("This is no cell! Add in the End.\n");
			while((*Cur).Next != NULL) Cur = (*Cur).Next; // Поиск последнего элемента
			SList *New = malloc(sizeof(SList));
			(*Cur).Next = New;
			(*New).Last = Cur;
			(*New).Next = NULL;
			(*New).Data = InputData; // Запись данных
			NumAll++; // Прибавление к общему числу элементов
			(*New).Num = NumAll; // Запись номера
			return New;
		}
	}
}

SList *pull(SList *Cur, int DelCell) { // Удаление элемента
	if (NumAll == 0) { // Нет элементов для удаления
		if (DelCell != 1) printw("There are no cells for delete!\n");
		return Cur;
	} else {
		if ((DelCell < NumAll) && (NumAll != 1)) { // Удаление ячейки между существующими. Проверка номера введенного элемента
			if (DelCell == 1) { // Удаление первого элемента в существующем списке
				bool Direct = false; // Смена направления поиска			
				while((*Cur).Num != DelCell) {  // Поиск элемента с введенным номером
					if (Direct == true ) { // Прямое направление
						if ((*Cur).Next == NULL) {
							Direct = !Direct;
						} else Cur = (*Cur).Next;
					} else { // Обратное направление
						if ((*Cur).Last == NULL) {
							Direct = !Direct;
						} else Cur = (*Cur).Last;
					}							
				}	
				Cur = (*Cur).Next;
				free((*Cur).Last);
				(*Cur).Last = NULL;
				NumAll--; // Вычитание из общего числа элементов
				(*Cur).Num--;
				while((*Cur).Next != NULL) { // Пронумеровать с новой ячейки
					Cur = (*Cur).Next;
					(*Cur).Num--;								
				}
			} else { // Удаление элемента между существующими элементами
				bool Direct = false; // Смена направления поиска			
				while((*Cur).Num != DelCell) {  // Поиск элемента с введенным номером
					if (Direct == true ) { // Прямое направление
						if ((*Cur).Next == NULL) {
							Direct = !Direct;
						} else Cur = (*Cur).Next;
					} else { // Обратное направление
						if ((*Cur).Last == NULL) {
							Direct = !Direct;
						} else Cur = (*Cur).Last;
					}
				}
				(*(*Cur).Last).Next = (*Cur).Next;
				(*(*Cur).Next).Last = (*Cur).Last;
				free(Cur);
				NumAll--; // Вычитание из общего числа элементов
				(*Cur).Num--;
				while((*Cur).Next != NULL) { // Пронумеровать с новой ячейки
					Cur = (*Cur).Next;
					(*Cur).Num--;								
				}
			}
			return Cur;
		} else { // Если номер больше или равен общему числу существующих элементов, то удалить последний элемент списка
			if (NumAll == 1) { // Если остался один элемент, то освободить память и вернуть NULL
				free(Cur);
				NumAll--;
				return NULL;
			}
			printw("This is no cell! Delete the End Cell.\n");
			while((*Cur).Next != NULL) Cur = (*Cur).Next; // Поиск последнего элемента
			Cur = (*Cur).Last;
			free((*Cur).Next);
			(*Cur).Next = NULL;
			NumAll--; // Прибавление к общему числу элементов
			return Cur;
		}
	}
}

int printlist(SList *Cur) { // Вывод всех элементов
	if (NumAll != 0) {
		while((*Cur).Last != NULL) { // Поиск первого элемента
			Cur = (*Cur).Last; // Копируем предыдущий элемент
		}
		printw("\nPrint List:\n");
		if (NumAll > 1) {
			do { // Вывод всех элементов;
				printw("Cell #%i with data: \"%C\"; \n", (*Cur).Num, (*Cur).Data);
				Cur = (*Cur).Next;
			} while((*Cur).Next != NULL);
		}
		printw("Cell #%i with data: \"%C\"; \n\n", (*Cur).Num, (*Cur).Data);
	}
	return 0;
}

int main(int NumTerm, char **InTerm) {
	int NumCell = 0; // Номер замещаемой ячейки
	char NewData; // Вводимые данные для новой ячейки
	SList *LP = NULL; // Указатель списка
	LP = (SList*) malloc(sizeof(SList)); // Выделяем пространство для указателя списка
	initscr(); // Инициализация ncurses.h
	clear(); // Очистка терминала
	while(true) {
		textprint(); // Вывод информации о командах
		switch (getch()) { // Ввод команды
			case '1' : // Добавление в список
				clear(); // Очистка терминала
				printw("Enter the Data:\n");
				scanw("%C", &NewData);
				clear(); // Очистка терминала;
				printw("Enter the Number Cell:\n");
				scanw("%i", &NumCell);
				LP = push(LP, NewData, NumCell);
				clear();
				printlist(LP); // Вывод всего списка
				break;
			case '2' : // Удаление элемента из списка
				clear();
				printw("Enter the Number Cell for delete:\n");
				scanw("%i", &NumCell);
				LP = pull(LP, NumCell);
				clear();
				printlist(LP); // Вывод всего списка
				break;
			case '3' : // Выход
				endwin(); // Деинициализация ncurses.h
				return 0;
				break;
			case '4' : // Вывод списка
				clear(); // Очистка терминала
				printlist(LP); // Вывод всего списка
				break;
			default :
				printw("Сommand is missing.\n");
				break;
		}
	};
}