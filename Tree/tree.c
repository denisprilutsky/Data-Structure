// gcc tree.c -o tree -l ncurses
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define Test 2

int NumAll; // Количество созданных ячеек

void textprint() {
	printw("|---------------------------------------------------------|\n");
	printw("\"1\" - add cell to The Tree;\n");
	printw("\"2\" - delete first cell of The Tree;\n");
	printw("\"3\" - exit.\n");
	printw("|---------------------------------------------------------|\n\n");
	printw("Enter the command:\n");
}

void treeprint( int *PrintTree ) {
	int Layer = 1;
	int i;
	printw("\n");
	for( i = 1; i <= NumAll; i++ ) {
		if( i <= Layer ) printw("%i	", *(PrintTree+i));
		else {
			printw("\n");
			Layer = Layer * 2 + 1;
			i--;
		}
	}
	printw("\n\n");
}

void swap( int *First, int *Second ) {
	int Temp = *(First);
	*(First) = *(Second);
	*(Second) = Temp;
}

int *push( int *AddTree, int NewData ) {
	NumAll++;	
	*(AddTree+NumAll) = NewData;
	if( NumAll == 1 ) {
		return AddTree;
	} else {
		int i = NumAll;
		int j = NumAll - 1;
		while( j > 0 ) {
			if( // Нахождение родителя
				( (i / j == 2) && ( (i % j == 0) || (i % j == 1) ) )
				||
				( (i / j == 3) && (i % j == 0) ) // Исключение для 3 и 1 элемента массива
			   ) {
				if( *(AddTree+i) > *(AddTree+j) ) { // Сравнение с родителем
					swap( (AddTree+i), (AddTree+j) ); // Смена мест
					i = j;
				} else return AddTree; // Если меньше, то возврат указателя
			}
			j--;
		}
		return AddTree;
	}
}

int *pull( int *DelTree ) {
	if( NumAll == 1 ) {
		*(DelTree+1) = 0;
		NumAll--;
		return DelTree;
	} else {
		int i = 1;
		*(DelTree+1) = *(DelTree+NumAll);
		*(DelTree+NumAll) = 0;
		NumAll--;
		while( i < NumAll ) {
			if( (i * 2) < NumAll ) { // Проверка на существование левой ветки
				if( (i * 2 + 1) < NumAll ) { // Проверка на существование правой ветки
					if( *(DelTree+(i*2)) > *(DelTree+(i*2+1)) ) { // Левая ветка больше правой
						if ( *(DelTree+(i*2)) > *(DelTree+i) ) { // Смена мест, если потомок больше родителя
							swap((DelTree+i), (DelTree+(i*2))); // Смена мест
							i = i * 2;
						} else break;
					} else { // Правая ветка больше левой
						if( *(DelTree+(i*2+1)) > *(DelTree+i) ) { // Смена мест, если потомок больше родителя
							swap( (DelTree+i), (DelTree+(i*2+1)) ); // Смена мест
							i = i * 2 + 1;
						} else break;
					}
				} else {
					if( *(DelTree+(i*2)) > *(DelTree+i) ) { // Смена мест, если потомок больше родителя
						swap((DelTree+i), (DelTree+(i*2))); // Смена мест
						i = i * 2;
					} else break;
				}
			} else break;
		}
		return DelTree;
	}
	return DelTree;
}

int main( int NumTerm, char **InTerm ) {
	int *Tree = NULL; // Дерево
	char *NewData; // Вводимые данные для новой ячейки
	int i = 0;
	NumAll = 0;
#if Test==1
	printf("Test 1\n");
	for( i = 1; i < (NumTerm - 1); i++ ) {
		printf("%i\n", atoi(InTerm[i]));
		Tree = (int*) realloc( Tree, (sizeof(int) * (i)) );
		Tree = push( Tree, (int)atoi(InTerm[i]) );
	}
	treeprint(Tree);
	getch();
#elif Test==2
	printf("Test 2\n");
	printf("NumTerm: %i \n", (NumTerm - 1));
	Tree = (int*) malloc( sizeof(int) * (NumTerm - 1) );
	for( i = 1; i < (NumTerm - 1); i++ ) {
		printf("%i\n", atoi(InTerm[i]));
		push( Tree, (int)atoi(InTerm[i]) );
	}
	treeprint(Tree);
	getch();
#else
	initscr(); // Инициализация ncurses.h
	clear(); // Очистка терминала
	while(true) {
		textprint(); // Вывод информации о командах
		switch (getch()) { // Ввод команды
			case '1' : // Добавление в список
				clear(); // Очистка терминала
				printw("Enter the Data Of Number:\n");
				refresh();
				getstr(NewData);
				//printw("%i\n", NewData);
				i++;
				Tree = (int*) realloc( Tree, (sizeof(int) * (i)) );
				Tree = push( Tree, (int)atoi(NewData) );
				clear();
				treeprint(Tree); // Вывод всего дерева
				break;
			case '2' : // Удаление элемента из списка
				clear();
				if( NumAll != 0 ) {
					Tree = pull(Tree);
					i--;
					Tree = (int*) realloc( Tree, (sizeof(int) * (i)) );
				}
				treeprint(Tree); // Вывод всего дерева
				break;
			case '3' : // Выход
				clear();
				free(Tree);
				endwin(); // Деинициализация ncurses.h
				return 0;
				break;
			default :
				clear(); // Очистка терминала
				printw("\nСommand is missing.\n");
				break;
		}
	};
#endif
}
