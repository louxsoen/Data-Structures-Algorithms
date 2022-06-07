#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IO.h"
#include "..//lib_type//types.h"

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static char* status = "대출가능";

int countFile() {
    FILE*   RFP;
    char    line[200];
    int     tolnum = 0;

    RFP = fopen(oriFILE, "rt");
    if(RFP == NULL) { puts("데이터 파일이 없습니다."); exit(1); }
    fgets(line, sizeof(line), RFP);
    tolnum = atoi(line);
    
    while(!feof(RFP)) {
        fgets(line, sizeof(line), RFP);
        tolnum++;
    }
    fclose(RFP);

    return tolnum;
}

void addNode(Book _book) {
    Node*   curNode = (Node*)malloc(sizeof(Node));
    curNode->book   = _book;
    curNode->pNext  = NULL;

    if(pHead == NULL)   pHead        = curNode;
    else                pTail->pNext = curNode;
    
    pTail = curNode;
}

void deleteEndString(char *buf) {
    int     i = 0;
    while(buf[i]) i++;      i--;
    while(buf[i] == ' ' || buf[i] == '\n') {
        buf[i] = '\0';
        i--;
    }
} 

void loadFile(int tolnum) {
    FILE*   RFP;
    Book    book;
    int     lenSTR;             // 길이 카운터의 버퍼 길이
    int     curNum = 0;
    char    line[500];
    char    *s1, *s2, *s3, *s4;

    RFP = fopen(oriFILE, "r+t");
    if(RFP == NULL) { puts("데이터 파일이 없습니다."); exit(1); }
    //while(!feof(RFP))

    for(int i = 0 ; i < tolnum ; i++) {
        fgets(line, sizeof(line), RFP);

        s1 = strtok(line, "\t");    if(s1 == NULL) s1 = "\0";   else deleteEndString(s1);//   strcpy(book.TITLE,  s1);
        s2 = strtok(NULL, "\t");    if(s2 == NULL) s2 = "\0";   else deleteEndString(s2);//   strcpy(book.AUTHOR, s2);
        s3 = strtok(NULL, "\t");    if(s3 == NULL) s3 = "\0";   else deleteEndString(s3);//   strcpy(book.YEAR,   s3);
        s4 = strtok(NULL, "\n");    if(s4 == NULL) s4 = "\0";   else deleteEndString(s4);//   strcpy(book.ISBN,   s4);
        //printf("%s\n%s\n%s\n%s\n\n", s1, s2, s3, s4);
        
        strcpy(book.TITLE,  s1);
        strcpy(book.AUTHOR, s2);
        strcpy(book.YEAR,   s3);
        strcpy(book.ISBN,   s4);

        addNode(book);
        //printf("%s\n%s\n%s\n%s\n\n", book.TITLE, book.AUTHOR, book.YEAR, book.ISBN);
    }
    fclose(RFP);
}

void showAllInfo() {
    Node    *curNode = pHead;

    if(pHead == NULL) {
        puts("현재 메모리에는 할당된 데이터가 없습니다. [-550 : HEAP FREE]");
        return;
    }
    while(curNode != NULL) {
        printf("[%s] %s %s %s %s\n", status, curNode->book.TITLE,curNode->book.AUTHOR,curNode->book.ISBN,curNode->book.YEAR);
        curNode = curNode->pNext;
    }
    puts("");
}

void showSingleInfo(Book *_book) {
    printf("책 제목\t\t%s\n", _book->TITLE);
    printf("책 저자\t\t%s\n", _book->AUTHOR);
    printf("발행일자\t%s\n", _book->YEAR);
    printf("ISBN 정보\t%s\n", _book->ISBN);
    printf("\n");
}
/*      노드 정보 전체 삭제 함수        */
void freeNodes() {
    Node    *curNode = pHead;       // 헤드를 현재 노드로 설정
    Node    *delNode = NULL;

    if(pHead == NULL)   return;     // 헤드가 빌 경우 함수 종료
    while(curNode != NULL) {        // 헤드가 비지 않을 경우 빌 때까지 반복
        delNode = curNode;          // 삭제할 노드를 현재 노드로 설정
        curNode = curNode->pNext;   // 현재 노드는 다음 연결 노드로 설정 
        free(delNode);              // 마지막 깔끔
    }
    pHead = pTail = NULL;           // 이거 안 해주면 더블프리
}

void searchTitle() {
    char    searchKey[256];
    Node    *curNode = pHead;
    memset(searchKey, 0, sizeof(searchKey));

    //FILE *fp = fopen("result.txt","w");
    
    FILE*   RFP;
    RFP = fopen(oriFILE, "r+t");
    if(RFP == NULL) { puts("데이터 파일이 없습니다."); exit(1); }

    int test_cnt = 0;
    printf("검색할 단어를 입력하세요 >> ");
    scanf("%[^\n]s", searchKey);

    char *s1;
    system("clear");
    puts("[ 자료구조론 8팀 라이브러리 검색 결과 ]");
    printf("\"%s\"에 대한 검색 결과입니다.\n", searchKey);
    
    while(curNode != NULL) {
        //int     lenSTR;             // 길이 카운터의 버퍼 길이
        char    line[500];
        fseek(RFP,0,SEEK_SET);
        while(!feof(RFP)) {
            fgets(line, sizeof(line), RFP);
            s1 = strtok(line, "\t ");
            while (s1 != NULL) {
                if(strcmp(s1,searchKey) == 0) {
                    test_cnt++;
                    //fprintf(fp,"%d. \"%s\"에 대한 검색 결과입니다.\n", test_cnt,searchKey);
                    //fprintf(fp,"%s\n%s\n%s\n%s\n", curNode->book.TITLE,curNode->book.AUTHOR,curNode->book.ISBN,curNode->book.YEAR);
                    showSingleInfo(&(curNode->book));
                    //return;
                }
                s1 = strtok(NULL, " "); 
            }
            curNode = curNode->pNext;
        }
        printf("%d\n",test_cnt);
        if(test_cnt == 0) {
            printf("\n\"%s\"에 대한 검색 결과가 없습니다.\n", searchKey);
        }
    }
    fclose(RFP);
    //fclose(fp);
    return;
}

int levenshtein(char *s1, char *s2) {
    unsigned int s1len, s2len, x, y, lastdiag, olddiag;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int column[s1len + 1];
    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++)
    {
        column[0] = x;
        for (y = 1, lastdiag = x - 1; y <= s1len; y++)
        {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y - 1] + 1, lastdiag + (s1[y - 1] == s2[x - 1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }
    return column[s1len];
}

int main() {
    //int     dataNum = countFile();
    loadFile(48); // 리드
    showAllInfo();
    searchTitle();
    freeNodes();    // wnrdj....
    
}
/*
https://stackoverflow.com/questions/8370468/debugging-segmentation-faults-on-a-mac

해야할 것
1. 21번째 메모리 덤프 원인 알아내기
2. 검색 알고리즘 강화 KMP
3. ISBN 자르고 검색 기능(?)
4. 사용자 JSON만들고 대출 여부 만들기
5. 도서 예약
*/


/*

    for(int i = 0 ; i < 21 ; i++) {
    fgets(line, sizeof(line), RFP);
    ptH = strtok(line, "\t"); deleteEndString(ptH); lenSTR = strlen(ptH); book.TITLE  = (char*)malloc((lenSTR + 1) * 10); strcpy(book.TITLE  , ptH);
    ptH = strtok(NULL, "\t"); deleteEndString(ptH); lenSTR = strlen(ptH); book.AUTHOR = (char*)malloc((lenSTR + 1) * 10); strcpy(book.AUTHOR , ptH); 
    ptH = strtok(NULL, "\t"); deleteEndString(ptH); lenSTR = strlen(ptH); book.YEAR   = (char*)malloc((lenSTR + 1) * 10); strcpy(book.YEAR   , ptH);
    ptH = strtok(NULL, "\n"); deleteEndString(ptH); lenSTR = strlen(ptH); book.ISBN   = (char*)malloc((lenSTR + 1) * 10); strcpy(book.ISBN   , ptH);
    
    //printf("%s\n%s\n%s\n%s\n", book.TITLE, book.AUTHOR, book.ISBN, book.YEAR);
    addNode(book);
    memset(line, 0, sizeof(line));
    }   
    */
