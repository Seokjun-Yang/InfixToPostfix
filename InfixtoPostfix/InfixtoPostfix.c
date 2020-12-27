#include <stdio.h>        //����¿� ���� ������ �ִ� header file
#include <stdlib.h>        //C ǥ�� ��ƿ��Ƽ �Լ����� ��Ƴ��� header file
#define MAX 100        //������ �ִ�ũ�� MAX=100 ���ȭ
#define EOS 0        //EOS(End Of Stack)=0 ���ȭ
int stack[MAX];        //stack �迭 ����
int pos = -1;        //stack ������ġ���� ������ pos ���� �ʱ�ȭ


int take() {        //stack �ֻ�� ���� ��ȯ�ϴ� take�Լ�
	if (pos < 0) {        //stack�� �ƹ��͵� ������� ���� ���
		printf("stack empty");        //'stack empty'�� ���
		return pos;        //pos ��ȯ
	}
	
	return stack[pos--];        //stack[pos] ���� ��ȯ�� �� pos�� ���ҽ�Ŵ
}
char put(char token) {        //stack�� �����͸� �����ϴ� put�Լ�
	if (MAX - 1 == pos) {        //stack�� ���� �� ���
		printf("stack full");        //'stack full'�� ���
		return 0;        //0 ��ȯ
	}

	stack[++pos] = token;        //pos�� ������Ų �� stack[pos]�� �Ѱܹ��� ������(token) ����
}


int ID(char token) {        //������(token)�� �������� �˾ƺ��� ID�Լ�
	if ('0' <= token && token <= '9')        //token�� �ƽ�Ű�ڵ� 0�� 9 ���̿� ��ġ�� �ִٸ�(���ڶ��)
		return 1;        //1�� ��ȯ
	else        //���ڰ� �ƴ� ���
		return 0;        //0�� ��ȯ
}
int IP(char token) {        //infix�� ������ �켱������ ����ϴ� IP�Լ�
	switch (token) {
	case '(':
	case ')': return 10;        //token�� ( �Ǵ� )�� ���, 10 ��ȯ(�켱���� ���� ŭ)
	case '*':
	case '/': return 5;        //token�� * �Ǵ� /�� ���, 5 ��ȯ
	case '+':
	case '-': return 2;        //token�� + �Ǵ� -�� ���, 2 ��ȯ(�켱���� ���� ����)
	}
}
int SP(char token) {        //stack�� ����� ������ �켱������ ����ϴ� SP�Լ�
	switch (token) {
	case '(': return 1;        //stack���ο����� (�� �켱������ ���� ����, 1�� ��ȯ
	case ')': return 10;        //token�� )�� ���, 10�� ��ȯ
	case '*':
	case '/': return 5;        //token�� * �Ǵ� /�� ���, 5 ��ȯ
	case '+':
	case '-': return 2;        //token�� + �Ǵ� -�� ���, 2 ��ȯ
	default: return -1;        //���� ��� �ش���� �ʴ� ���, -1 ��ȯ
	}
}


void CV(char* infix, char* postfix) {        //infix���� postfix�� ��ȯ�ϴ� CV�Լ�
	int cnt = 0;        //postfix�� ���������� �������ֱ� ���� cnt���� �ʱ�ȭ
	char token;
	put(EOS);        //stack�� ���� EOS ����(stack[0]=EOS, pos=0)
	for (int i = 0; infix[i] != 0; i++) {        //infix�� ����� ������ ���� ������ �ݺ�
		token = infix[i];        //������ �ڵ带 ���� token�� infix[i]�� ����
		if (ID(token))        //token�� ������ ���,
			postfix[cnt++] = token;        //postfix�� token�� �����ϰ� cnt����
		else {        //token�� ���ڰ� �ƴ� ���,
			if (token == ')') {        //token�� )�� ���, ��ȣ���� �����ڰ� �켱 ����Ǿ� �Ѵ�
				do {        //�ݺ��� ����
					postfix[cnt++] = take();        //stack�� ����� �����ڸ� take()�Լ����� ������ postfix�� ���� ��, cnt ����
				} while (postfix[cnt - 1] != '(');        //���� ���� �� cnt�� ��ȣ �����̶�� �ݺ� ����
				cnt--;        //cnt�� ���ҽ��������ν� ���� ������ �� ��ȣ ������ ����
				continue;        //for������ ���ư��� ���� ��ū���� ����
			}
			while (SP(stack[pos]) >= IP(token)) {        //stack�� ����� �����ڰ� infix�� �����ں��� �켱������ Ŭ ���� �ݺ�
				postfix[cnt++] = take();        //stack�� ����� �����ڸ� postfix�� ���� �� cnt ����
			}
			put(token);        //���� ������ ��� �ش��� �ȵǴ� ������(token)�� stack�� ����
		}

	}        //infix�� ����� ������ ���� �� �ݺ��� ����
			
	do {        //�ݺ��� ����
		postfix[cnt++] = take();        //stack�� ����� �����ڸ� postfix�� ���� �� cnt����
	} while (postfix[cnt - 1] != EOS);        //���� ���� �� cnt�� EOS��� �ݺ� ����
}        //stack�� �����ִ� ��� �����ڸ� postfix�� ������ ������ �ݺ��ϰ� ����


int EV(char* postfix) {        //postfix�� �����ϴ� EV�Լ�
	char token, op1, op2;
	for (int i = 0; postfix[i] != 0; i++) {        //postfix�� ����� ������ ���� ������ �ݺ�
		token = postfix[i];        //������ ����� ���� token�� postfix[i]�� ����
		if (ID(token))        //token�� ������ ���,
			put(token - '0');        //stack�� ����, token-'0'�� ���ڷ� �� ���ڸ� ���������� ��ȯ
		else {        //token�� ���ڰ� �ƴ� ���,
			op1 = take(), op2 = take();        //stack�� ����� ���ڸ� ������ op1, op2�� ����
			switch (token)        //token�� ��Ÿ���� �����ڿ� ���� �ٸ� ������ ����
			{        //op2�� ���� ����� ���ڷ�, op2 op1���� ����� ����
			case '*': put(op2 * op1); break;        //���� ����� ���� ������ ���� �ٽ� stack�� �����Ѵ�
			case '/': 
				if (op1 == 0) {
					printf("do not divide by zero");
					break;
				}
				put(op2 / op1); break;
			case '+': put(op2 + op1); break;
			case '-': put(op2 - op1); break;
			}
		}
	}        //postfix�� ����� ������ ���� ���� ��������. ��� ������ ����� stack�� ����Ǿ�����

	return take();        //stack�� ����� ���� ����� ��ȯ
}


int main() {

    char infix[50], postfix[50];        //infix�� postfix�� ������ char�� �迭����
	puts("���� �Է� >>");        //'���� �Է� >>'�� ���
	gets_s(infix, sizeof(infix));        //������ �Է¹޾� infix�� ����
	CV(infix, postfix);        //infix�� postfix�� ��ȯ
	printf("  postfix >> %s\n", postfix);        //postfix�� ���
	printf("  result >> %d\n", EV(postfix));        //postfix�� ������� ����� ���

	return 0;
}