#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct _SSTACK { // Стек;
    char DATA;
    struct _SSTACK *NEXT;
} SSTACK;

void textprint() {
	printw("'1 <character>' - add to stack;\n");
	printw("'2' - delete last element;\n");
	printw("'3' - show stack;\n");
	printw("'4' - exit.\n");
	printw("Enter the command:\n");
}

SSTACK *push(SSTACK *_SP, char NEWDATA) {
	SSTACK *_STACK = malloc(sizeof(SSTACK));
	(*_STACK).DATA = NEWDATA;
	(*_STACK).NEXT = _SP;
	_SP = _STACK;
	return _SP;
}

SSTACK *pull(SSTACK *_SP) { // Удаление элемента;
	free(_SP);
	_SP = (*_SP).NEXT;
	return _SP;
}

int main(int NUMPARAMS, char **INDATETERM) {
	SSTACK *SP = NULL; // Указатель стека;
	SP = (SSTACK*) malloc(sizeof(SSTACK));
	SSTACK *SAVE; // Сохранение стека для удаления;
	SAVE = (SSTACK*) malloc(sizeof(SSTACK));
	SSTACK *SAVENULL; // Сохранение нулевого значения стека для сравнения;
	SAVENULL = (SSTACK*) malloc(sizeof(SSTACK));
	SAVENULL = SP;
	char INCOMMAND; // Символ команды;
	char INDATA; //
	initscr(); // Инициализация ncurses.h;
	while(true)  {
		textprint(); // Вывод информации о командах;
		INCOMMAND = getch(); // Ввод команды;
		clear(); // Очистка терминала;
		switch (INCOMMAND) {
			case '1' : // Добавление в стек;				
				scanw("%C", &INDATA);
				printw("Data push: %C\n", INDATA);
				SP = push(SP, INDATA); //printw("SP = %d\n", *SP);
				break;		
			case '2' : // Удаление последнего элемента из стека;
				SP = pull(SP);
				printw("Last element: %C\n", (*SP).DATA);
				break;			
			case '3' : // Вывод стека;
				SAVE = SP;
				while(SP != SAVENULL) {
					printw("%C\n", (*SP).DATA);
					SP = (*SP).NEXT;				
				}
				break;			
			case '4' : // Выход из программы;
				endwin();
				return 0;
				break;
			default :
				printw("Сommand is missing.\n");
				break;			
		}
	};
}
