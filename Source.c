#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#define n 5
void print_bin(long long unsigned int k, long unsigned int size)
{
	long unsigned int i;
	for (i = size * 8; i > 0; i--) {
		printf("%d", (k & ((unsigned long long)1 << (i - 1))) ? 1 : 0);
	}
	printf(" ");
}
int scanbin() {
	char* string = malloc(8 * sizeof(int) + 1);
	gets(string);
	int ret = 0, i = (int)strlen(string) - 1, pow = 1;
	do {
		ret += pow * (string[i] - '0');
		i--;
		pow *= 2;
	} while (i >= 0);
	return ret;
}
int main() {
	setlocale(LC_ALL, "rus");
	int i, input, t;
	printf("Элементы массива:\n1. Байты\n2. Слова\n");
	scanf_s("%d", &t);;
	unsigned char A1[n];
	unsigned short int A2[n];
	if (t == 1) {
		printf("Введите массив А: ");
		for (i = 0; i < n; i++) {
			scanf_s("%d", &input);
			A1[i] = (unsigned char)input;
		}
		printf("Массив до изменения:\n");
		printf("Двоичная система счисления: ");
		for (i = 0; i < n; i++) print_bin((unsigned long long)A1[i], sizeof(char));
		printf("\nШестнадцатеричная система счисления: ");
		for (i = 0; i < n; i++) printf("%X ", A1[i]);
	}
	else {
		printf("Введите массив А: ");
		for (i = 0; i < n; i++) scanf_s("%hu", A2 + i);
		printf("Массив до изменения:\n");
		printf("Двоичная система счисления: ");
		for (i = 0; i < n; i++) print_bin((unsigned long long)A2[i], sizeof(short int));
		printf("\nШестнадцатеричная система счисления: ");
		for (i = 0; i < n; i++) printf("%X ", A2[i]);
	}
	printf("\nВведите образец: ");
	while (getchar() != '\n');
	short int sample2 = (short int)scanbin();
	char sample1 = (char)sample2;
	__asm {
		mov ecx, n;// Устанваливаем количество повторений в цикле
		lea esi, A2;// Записываем в esi начало массива слов
		cmp t, 1;// сравниваем t c 1
		jne SL;//если t!=1 переходим к метке SL
		lea esi, A1;// Записываем в esi начала массива байтов
	B:
		mov al, [esi];// Записываем в al элемент массива
		mov bl, al;// Копируем al в bl
		mov dl, al;// Копируем al в dl
		AND al, 01100000b;//Выделяем битовое поле 6:5
		shr al, 5;// Сдвигаем на 5 разряда вправо
		AND bl, 00011000b;//Выделяем битовое поле 4:3
		shr bl, 3;//Сдвигаем на 3 разряд вправо
		XOR bl, sample1//исключающее или  образца и битового поля 4:3
		cmp al, bl;//Сравниваем al и bl
		jne CLEAR7;//Если не равны сбрасываем старший бит
		XOR dl, 10000000b;//Инвертируем старший бит
		jmp FIN1;//Принудительный переход к метке FIN1
	CLEAR7:
		AND dl, 01111111b;//Сбрасываем старший бит
	FIN1:
		mov[esi], dl;// Помещаем измененное число в массив
		inc esi;// Увеличиваем esi на 1
		loop B;// Из ecx вычитаем 1 и если ecx!=0 переходим к метке B
		jmp FIN; // Переходим к концу вставки
	SL:
		mov ax, [esi];// Записываем в ax элемент массива
		mov bx, ax;// Копируем ax в bx
		mov dx, ax;// Копируем ax в dx
		AND ax, 0000000001100000b;//Выделяем битовое поле 6:5
		shr ax, 5;// Сдвигаем на 5 разряда вправо
		AND bx, 0000000000011000b;//Выделяем битовое поле 4:3
		shr bx, 3;// Сдвигаем на 3 разряда вправо
		XOR bx, sample2;//исключающее или образца и битового поля 4:3
		cmp ax, bx;//Сравниваем ax и bx
		jne CLEAR15;//Если не равны сбрасываем старший бит
		XOR dx, 1000000000000000b;//Инвертируем старший бит
		jmp FIN2;//Принудительный переход к метке FIN2
	CLEAR15:
		AND dx, 0111111111111111b;//Сбрасываем старший бит
	FIN2:
		mov[esi], dx;// Помещаем измененное число в массив
		add esi, 2;// Увеличиваем esi на 1
		loop SL;// Из ecx вычитаем 1 и если ecx!=0 переходим к метке SL
	FIN: nop;//Пустая команда конец вставки
	}
	if (t == 1) {
		printf("Массив после изменения:\n");
		printf("Двоичная система счисления: ");
		for (i = 0; i < n; i++) print_bin((unsigned long long)A1[i], sizeof(char));
		printf("\nШестнадцатеричная система счисления: ");
		for (i = 0; i < n; i++) printf("%X ", A1[i]);
	}
	else {
		printf("Массив после изменения:\n");
		printf("Двоичная система счисления: ");
		for (i = 0; i < n; i++) print_bin((unsigned long long)A2[i], sizeof(short int));
		printf("\nШестнадцатеричная система счисления: ");
		for (i = 0; i < n; i++) printf("%X ", A2[i]);
	}
	getch();
	return 0;
}
