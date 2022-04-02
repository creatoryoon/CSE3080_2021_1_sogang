#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*
* 실패 함수로 검색 하고자 하는 패턴(str)을 전처리
* pi[i]에는 0~i번째까지 문자를 살펴봤을 때, 접두사와 접미사가 같아지는 최대 길이
*/
int* Preprocessing(char * str) {
	int m = strlen(str);
	int* pi = (int*)malloc(sizeof(int) * m);
	pi[0] = 0;
	int j = 0;	//현재 관리하고 있는 구간의 접두사와 접미사가 같은 최대 길이
	for (int i = 1; i < m; i++) {
		while (j > 0 && str[i] != str[j])	//만약 현재 관리한 구간의 양 끝문자가 서로 다른 경우 이전에 저장되어 있는 값들과 비교하여
											//가장 긴 공통 접두사/접미사의 길이를 찾음
			j = pi[j - 1];

		if (str[i] == str[j]) {	//만약 양 끝문자가 같다면 접두사/접미사의 길이를 1 증가시켜주며, 해당 값을 저장
			pi[i] = j + 1;
			++j;
		}
		else 
			pi[i] = 0;	//그 외의 경우에는 공통 접두사/접미사가 없으므로 0을 저장
	}
	return pi;
}
/*
*	KMP 함수는 문자열 s와 패턴 p를 매개변수로 받아, 문자열 s에서 패턴 p를 찾은 결과를 반환
*	이때 cnt는 과제에서 정의한 반복문이 실행되는 횟수
*/
int KMP(char * s, char * p, int * cnt) {
	int n = strlen(s);	//n은 문자열 s의 길이
	int m = strlen(p);	//p는 패턴 p의 길이
	int * pi = Preprocessing(p);	//패턴 p를 실패 함수를 통해 구한 값을 pi에 저장
	int j = 0;	//패턴 p의 위치를 관리할 변수
	for (int i = 0; i < n; i++) {
		++(*cnt);
		while (j > 0 && s[i] != p[j]) {	//문자열 s와 패턴 p를 비교하는 도중 다른 부분을 찾았다면
			j = pi[j - 1];	//실패 함수를 이용해 구한 값을 기준으로 j를 이동
			++(*cnt);
		}
		if (s[i] == p[j]) {	//만약 두 위치의 문자가 같은 경우
			if (j == m - 1)	//패턴의 끝 문자와 비교한 경우 패턴을 찾은 것이므로 1을 반환
				return 1;

			else	//아니라면 다음 문자를 비교하기 위해 인덱스를 1 증가시킴
				++j;
		}
	}
	return 0;	//찾지 못한 경우 0 반환
}
/*
*	Naive 함수는 문자열 s와 패턴 p를 매개변수로 받아, 문자열 s에서 패턴 p를 찾은 결과를 반환(O(NM)) N: 문자열의 길이, M: 패턴의 길이
*	이때 cnt는 과제에서 정의한 반복문이 실행되는 횟수
*/
int Naive(char* s, char* p, int* cnt) {
	int n = strlen(s);	//n에는 str의 길이를 저장
	int m = strlen(p);	//m에는 pat의 길이를 저장
	for (int i = 0; i + m <= n; ++i) {	//만약 비교를 시작하는 문자의 위치가 패턴보다 작다면 더 이상 패턴을 찾을 수 없으므로 반복문 종료
		int chk = 1;	//패턴을 찾았는지 확인할 변수
		for (int j = 0; j < m; ++j) {	//패턴의 길이만큼 반복
			++(*cnt);
			if (s[i + j] != p[j]) {	//만약 비교하는 두 문자가 서로 다르다면
				chk = 0;	//chk를 0으로 만들어서 탐색에 실패했음을 확인
			}
		}
		if (chk == 1)	//만약 chk가 1인 경우 해당 패턴을 찾은 것이므로 1 반환
			return 1;
	}
	return 0;	//찾지 못한 경우 0 반환
}
int main() {
	const char fileName = "input.txt";
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("Can't open file");
		return 0;
	}
	char s[1000], p[1000];
	fscanf(fp, "%s %s", s, p);

	fclose(fp);

	int cnt1 = 0, cnt2 = 0;
	int res1 = Naive(s, p, &cnt1);
	int res2 = KMP(s, p, &cnt2);

	if (res1 == 1)
		printf("YES ");
	else
		printf("NO ");
	printf("%d\n", cnt1);

	if (res2 == 1)
		printf("YES ");
	else
		printf("NO ");
	printf("%d", cnt2);
	
}
