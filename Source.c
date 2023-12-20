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
	printf("�������� �������:\n1. �����\n2. �����\n");
	scanf_s("%d", &t);;
	unsigned char A1[n];
	unsigned short int A2[n];
	if (t == 1) {
		printf("������� ������ �: ");
		for (i = 0; i < n; i++) {
			scanf_s("%d", &input);
			A1[i] = (unsigned char)input;
		}
		printf("������ �� ���������:\n");
		printf("�������� ������� ���������: ");
		for (i = 0; i < n; i++) print_bin((unsigned long long)A1[i], sizeof(char));
		printf("\n����������������� ������� ���������: ");
		for (i = 0; i < n; i++) printf("%X ", A1[i]);
	}
	else {
		printf("������� ������ �: ");
		for (i = 0; i < n; i++) scanf_s("%hu", A2 + i);
		printf("������ �� ���������:\n");
		printf("�������� ������� ���������: ");
		for (i = 0; i < n; i++) print_bin((unsigned long long)A2[i], sizeof(short int));
		printf("\n����������������� ������� ���������: ");
		for (i = 0; i < n; i++) printf("%X ", A2[i]);
	}
	printf("\n������� �������: ");
	while (getchar() != '\n');
	short int sample2 = (short int)scanbin();
	char sample1 = (char)sample2;
	__asm {
		mov ecx, n;// ������������� ���������� ���������� � �����
		lea esi, A2;// ���������� � esi ������ ������� ����
		cmp t, 1;// ���������� t c 1
		jne SL;//���� t!=1 ��������� � ����� SL
		lea esi, A1;// ���������� � esi ������ ������� ������
	B:
		mov al, [esi];// ���������� � al ������� �������
		mov bl, al;// �������� al � bl
		mov dl, al;// �������� al � dl
		AND al, 01100000b;//�������� ������� ���� 6:5
		shr al, 5;// �������� �� 5 ������� ������
		AND bl, 00011000b;//�������� ������� ���� 4:3
		shr bl, 3;//�������� �� 3 ������ ������
		XOR bl, sample1//����������� ���  ������� � �������� ���� 4:3
		cmp al, bl;//���������� al � bl
		jne CLEAR7;//���� �� ����� ���������� ������� ���
		XOR dl, 10000000b;//����������� ������� ���
		jmp FIN1;//�������������� ������� � ����� FIN1
	CLEAR7:
		AND dl, 01111111b;//���������� ������� ���
	FIN1:
		mov[esi], dl;// �������� ���������� ����� � ������
		inc esi;// ����������� esi �� 1
		loop B;// �� ecx �������� 1 � ���� ecx!=0 ��������� � ����� B
		jmp FIN; // ��������� � ����� �������
	SL:
		mov ax, [esi];// ���������� � ax ������� �������
		mov bx, ax;// �������� ax � bx
		mov dx, ax;// �������� ax � dx
		AND ax, 0000000001100000b;//�������� ������� ���� 6:5
		shr ax, 5;// �������� �� 5 ������� ������
		AND bx, 0000000000011000b;//�������� ������� ���� 4:3
		shr bx, 3;// �������� �� 3 ������� ������
		XOR bx, sample2;//����������� ��� ������� � �������� ���� 4:3
		cmp ax, bx;//���������� ax � bx
		jne CLEAR15;//���� �� ����� ���������� ������� ���
		XOR dx, 1000000000000000b;//����������� ������� ���
		jmp FIN2;//�������������� ������� � ����� FIN2
	CLEAR15:
		AND dx, 0111111111111111b;//���������� ������� ���
	FIN2:
		mov[esi], dx;// �������� ���������� ����� � ������
		add esi, 2;// ����������� esi �� 1
		loop SL;// �� ecx �������� 1 � ���� ecx!=0 ��������� � ����� SL
	FIN: nop;//������ ������� ����� �������
	}
	if (t == 1) {
		printf("������ ����� ���������:\n");
		printf("�������� ������� ���������: ");
		for (i = 0; i < n; i++) print_bin((unsigned long long)A1[i], sizeof(char));
		printf("\n����������������� ������� ���������: ");
		for (i = 0; i < n; i++) printf("%X ", A1[i]);
	}
	else {
		printf("������ ����� ���������:\n");
		printf("�������� ������� ���������: ");
		for (i = 0; i < n; i++) print_bin((unsigned long long)A2[i], sizeof(short int));
		printf("\n����������������� ������� ���������: ");
		for (i = 0; i < n; i++) printf("%X ", A2[i]);
	}
	getch();
	return 0;
}
