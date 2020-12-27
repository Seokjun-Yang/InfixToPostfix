#include <stdio.h>        //입출력에 관한 선언이 있는 header file
#include <stdlib.h>        //C 표준 유틸리티 함수들을 모아놓은 header file
#define MAX 100        //스택의 최대크기 MAX=100 상수화
#define EOS 0        //EOS(End Of Stack)=0 상수화
int stack[MAX];        //stack 배열 생성
int pos = -1;        //stack 저장위치값을 가지는 pos 변수 초기화


int take() {        //stack 최상단 값을 반환하는 take함수
	if (pos < 0) {        //stack에 아무것도 저장되지 않은 경우
		printf("stack empty");        //'stack empty'를 출력
		return pos;        //pos 반환
	}
	
	return stack[pos--];        //stack[pos] 값을 반환한 후 pos를 감소시킴
}
char put(char token) {        //stack에 데이터를 저장하는 put함수
	if (MAX - 1 == pos) {        //stack이 가득 찬 경우
		printf("stack full");        //'stack full'을 출력
		return 0;        //0 반환
	}

	stack[++pos] = token;        //pos를 증가시킨 후 stack[pos]에 넘겨받은 데이터(token) 저장
}


int ID(char token) {        //데이터(token)가 숫자인지 알아보는 ID함수
	if ('0' <= token && token <= '9')        //token이 아스키코드 0과 9 사이에 위치해 있다면(숫자라면)
		return 1;        //1을 반환
	else        //숫자가 아닌 경우
		return 0;        //0을 반환
}
int IP(char token) {        //infix의 연산자 우선순위를 계산하는 IP함수
	switch (token) {
	case '(':
	case ')': return 10;        //token이 ( 또는 )인 경우, 10 반환(우선순위 가장 큼)
	case '*':
	case '/': return 5;        //token이 * 또는 /인 경우, 5 반환
	case '+':
	case '-': return 2;        //token이 + 또는 -인 경우, 2 반환(우선순위 가장 작음)
	}
}
int SP(char token) {        //stack에 저장된 연산자 우선순위를 계산하는 SP함수
	switch (token) {
	case '(': return 1;        //stack내부에서는 (의 우선순위가 가장 낮음, 1을 반환
	case ')': return 10;        //token이 )인 경우, 10을 반환
	case '*':
	case '/': return 5;        //token이 * 또는 /인 경우, 5 반환
	case '+':
	case '-': return 2;        //token이 + 또는 -인 경우, 2 반환
	default: return -1;        //위에 모두 해당되지 않는 경우, -1 반환
	}
}


void CV(char* infix, char* postfix) {        //infix에서 postfix로 변환하는 CV함수
	int cnt = 0;        //postfix에 순차적으로 저장해주기 위한 cnt변수 초기화
	char token;
	put(EOS);        //stack의 끝에 EOS 저장(stack[0]=EOS, pos=0)
	for (int i = 0; infix[i] != 0; i++) {        //infix에 저장된 내용이 없을 때까지 반복
		token = infix[i];        //간결한 코드를 위해 token에 infix[i]값 저장
		if (ID(token))        //token이 숫자인 경우,
			postfix[cnt++] = token;        //postfix에 token을 저장하고 cnt증가
		else {        //token이 숫자가 아닌 경우,
			if (token == ')') {        //token이 )인 경우, 괄호안의 연산자가 우선 실행되야 한다
				do {        //반복문 실행
					postfix[cnt++] = take();        //stack에 저장된 연산자를 take()함수에서 가져와 postfix에 저장 후, cnt 증가
				} while (postfix[cnt - 1] != '(');        //만약 증가 전 cnt가 괄호 시작이라면 반복 종료
				cnt--;        //cnt를 감소시켜줌으로써 다음 저장할 때 괄호 시작을 없앰
				continue;        //for문으로 돌아가서 다음 토큰저장 실행
			}
			while (SP(stack[pos]) >= IP(token)) {        //stack에 저장된 연산자가 infix의 연산자보다 우선순위가 클 동안 반복
				postfix[cnt++] = take();        //stack에 저장된 연산자를 postfix에 저장 후 cnt 증가
			}
			put(token);        //위의 과정이 모두 해당이 안되는 연산자(token)는 stack에 저장
		}

	}        //infix에 저장된 내용이 없을 때 반복문 종료
			
	do {        //반복문 실행
		postfix[cnt++] = take();        //stack에 저장된 연산자를 postfix에 저장 후 cnt증가
	} while (postfix[cnt - 1] != EOS);        //만약 증가 전 cnt가 EOS라면 반복 종료
}        //stack에 남아있는 모든 연산자를 postfix에 저장할 때까지 반복하고 종료


int EV(char* postfix) {        //postfix를 연산하는 EV함수
	char token, op1, op2;
	for (int i = 0; postfix[i] != 0; i++) {        //postfix에 저장된 내용이 없을 때까지 반복
		token = postfix[i];        //간편한 계산을 위해 token에 postfix[i]값 저장
		if (ID(token))        //token이 숫자인 경우,
			put(token - '0');        //stack에 저장, token-'0'은 문자로 된 숫자를 정수형으로 변환
		else {        //token이 숫자가 아닌 경우,
			op1 = take(), op2 = take();        //stack에 저장된 숫자를 꺼내서 op1, op2에 저장
			switch (token)        //token이 나타내는 연산자에 따라 다른 연산을 실행
			{        //op2가 먼저 저장된 숫자로, op2 op1으로 계산을 진행
			case '*': put(op2 * op1); break;        //연산 결과는 다음 연산을 위해 다시 stack에 저장한다
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
	}        //postfix에 저장된 내용이 없을 때는 빠져나옴. 모든 연산의 결과는 stack에 저장되어있음

	return take();        //stack에 저장된 연산 결과를 반환
}


int main() {

    char infix[50], postfix[50];        //infix와 postfix를 저장할 char형 배열선언
	puts("수식 입력 >>");        //'수식 입력 >>'을 출력
	gets_s(infix, sizeof(infix));        //수식을 입력받아 infix에 저장
	CV(infix, postfix);        //infix를 postfix로 변환
	printf("  postfix >> %s\n", postfix);        //postfix를 출력
	printf("  result >> %d\n", EV(postfix));        //postfix를 기반으로 계산결과 출력

	return 0;
}