// C4996 (scanf/gets) 경고를 무시합니다.
#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strcmp 함수 (단어 비교)
#include <ctype.h>   // tolower 함수 등 (현재 미사용)
#include <time.h>    // time 함수 (현재 미사용)
#include <windows.h> // Beep, PlaySound, Sleep, system("cls") 함수 사용
#include <conio.h>   // _getch 함수 사용 (엔터 없이 입력)

// BGM 재생을 위한 윈도우 멀티미디어 라이브러리 (LNK 오류 방지)
#pragma comment(lib, "winmm.lib")

// ====================================================
// 1. 게임 핵심 상수 정의
// ====================================================

// 캐릭터는 총 7단계 (실패 횟수 0~6)로 구성됩니다.
#define MAX_STAGES 7 
#define MAX_LINES 10 

// 총 퀴즈 개수 (10문제)
#define TOTAL_QUIZZES 10 

// ====================================================
// 2. 행맨 캐릭터 아스키 아트 데이터
// ====================================================

// 1. Pumpkin Style (호박)
const char* Classic_Char[MAX_STAGES][MAX_LINES] = {
    { // Stage 0
    "    +---+  ", "    |   |  ", "    |      ", "    |      ", "    |      ", "    |      ", "    ====="},
    { // Stage 1: 머리 (oVo)
    "    +---+  ", "    |   |  ", "    |  (oVo) ", // V가 11열
     "    |      ", "    |      ", "    |      ", "    ====="},
    { // Stage 2: 몸통 (|)
    "    +---+  ", "    |   |  ", "    |  (oVo) ",
     "    |    |   ", // 몸통 | at 11열
     "    |      ", "    |      ", "    ====="},
    { // Stage 3: 왼팔 (/)
    "    +---+  ", "    |   |  ", "    |  (oVo) ",
     "    |   /|   ", // 왼팔 / at 10열, 몸통 | at 11열
     "    |      ", "    |      ", "    ====="},
    { // Stage 4: 오른팔 (*)
    "    +---+  ", "    |   |  ", "    |  (oVo) ",
     "    |   /|* ", // 오른팔 * at 12열
     "    |      ", "    |      ", "    ====="},
    { // Stage 5: 왼다리 (/)
    "    +---+  ", "    |   |  ", "    |  (oVo) ",
     "    |   /|* ",
     "    |   /    ", // 왼다리 / at 10열
     "    |      ", "    ====="},
    { // Stage 6: 오른다리 (¿)
    "    +---+  ", "    |   |  ", "    |  (oVo) ",
     "    |   /|* ",
     "    |   /¿   ", // 오른다리 ¿ at 12열
     "    |      ", "    ====="}
};

// 2. Skeleton Style (해골)
const char* Boxy_Char[MAX_STAGES][MAX_LINES] = {
    { // Stage 0
    "    +---+  ", "    |   |  ", "    |      ", "    |      ", "    |      ", "    |      ", "    ======="},
    { // Stage 1: 머리 (o)
    "    +---+  ", "    |   |  ", "    |  (o)  ", // 해골 머리
     "    |      ", "    |      ", "    |      ", "    ======="},
    { // Stage 2: 몸통 (H)
    "    +---+  ", "    |   |  ", "    |  (o)  ",
     "    |   H  ", // 뼈 (갈비뼈)
     "    |      ", "    |      ", "    ======="},
     { // Stage 3: 왼팔 (~)
     "    +---+  ", "    |   |  ", "    |  (o)  ",
      "    |  ~H  ", // 뼈 팔
      "    |      ", "    |      ", "    ======="},
      { // Stage 4: 오른팔 (~)
      "    +---+  ", "    |   |  ", "    |  (o)  ",
       "    |  ~H~ ", // 뼈 팔
       "    |      ", "    |      ", "    ======="},
       { // Stage 5: 왼다리 (|)
       "    +---+  ", "    |   |  ", "    |  (o)  ",
        "    |  ~H~ ",
        "    |  |   ", // 뼈 다리
        "    |      ", "    ======="},
        { // Stage 6: 오른다리 (|) - 게임 오버!
        "    +---+  ", "    |   |  ", "    |  (o)  ",
         "    |  ~H~ ",
         "    |  | | ", // 뼈 다리
         "    |      ", "    ======="}
};

// 3. Scarecrow Style (허수아비)
const char* Stickman_Char[MAX_STAGES][MAX_LINES] = {

    { // Stage 0

    "    _____", "    | / |", "    | /  ", "    |    ", "    |    ", "    |    ", "  /-----\\"},

    { // Stage 1: 머리 (%)

    "    _____", "    | / |", "    | / @",

     "    |    ", "    |    ", "    |    ", "  /-----\\"},

    { // Stage 2: 몸통 (|)

    "    _____", "    | / |", "    | / @",

     "    |   |", "    |   |", "    |    ", "  /-----\\"},

    { // Stage 3: 왼팔 (/)

    "    _____", "    | / |", "    | / @",

     "    |  /|  ", "    |   |  ", "    |    ", "  /-----\\"},

    { // Stage 4: 오른팔 (￢)

    "    _____", "    | / |", "    | / @",

     "    |  /|￢ ", "    |   |  ", "    |    ", "  /-----\\"},

    { // Stage 5: 왼다리 (/)

    "    _____", "    | / |", "    | / @",

     "    |  /|￢ ", "    |   |  ", "    |  /   ", "  /-----\\"},

    { // Stage 6: 오른다리 (¡)

    "    _____", "    | / |", "    | / @", // [사장님 요청] (%) -> @로 복구

     "    |  /|￢ ", "    |   |  ", "    |  /¡", "  /-----\\"}

};


// ====================================================
// 3. 게임 퀴즈 데이터 (10문제)
// ====================================================

// 문제 문장 (빈칸은 *** 로 표시)
const char* quiz_sentences[TOTAL_QUIZZES] = {
    "With great power comes *** responsibility.", // 1
    "To be or not to ***.",                      // 2
    "Life is *** of ups and downs.",           // 3
    "Live as if you were to *** tomorrow.",      // 4
    "Past is *** past.",                         // 5 
    "Until death, it is all ***.",           // 6 
    "No sweat, No sw**t",                        // 7
    "Seeing is ***.",                            // 8
    "Knowledge is ***.",                         // 9
    "Time is ***."                               // 10
};

// 정답 단어 (소문자)
const char* quiz_answers[TOTAL_QUIZZES] = {
    "great",      // 1
    "be",         // 2
    "full",       // 3
    "die",        // 4
    "just",       // 5
    "life",       // 6 
    "ee",         // 7
    "believing",  // 8
    "power",      // 9
    "gold"        // 10
};

// [추가] 문제별 힌트
const char* quiz_hints[TOTAL_QUIZZES] = {
    "힌트: 스파이더맨 명대사",
    "힌트: 햄릿",
    "힌트: '가득찬'",
    "힌트: '인간이 두려워 하는 것'",
    "힌트: '스윙스회사'",
    "힌트: '지금 살고 있는 것'",
    "힌트: 'e'",
    "힌트: '믿는 것'",
    "힌트: '힘'",
    "힌트: 노란색 덩어리"
};


// ====================================================
// 4. 함수 원형 선언
// ====================================================
void show_intro();
void draw_hangman(int character_choice, int stage);
void select_character();
void clear_input_buffer();
void play_game();

// ====================================================
// 5. 함수 구현
// ====================================================

/**
 * @brief 게임 시작 시 사용자에게 게임 규칙을 설명합니다.
 */
void show_intro() {
    printf("==========================================\n");
    printf("             [ 게임 설명 ]\n");
    printf("==========================================\n");
    printf("이 게임은 컴퓨터가 내는 %d개의 문장 빈칸을 맞추는 게임입니다.\n\n", TOTAL_QUIZZES);
    printf(" [패배 조건 1]\n");
    printf(" 단어를 6번 틀려 캐릭터가 모두 그려지면 **패배**합니다.\n\n");
    printf(" [패배 조건 2 / 생존 조건]\n");
    printf(" 6번 틀리기 전에 %d문제를 모두 풀면,\n", TOTAL_QUIZZES);
    printf(" 맞춘 개수가 절반(6개) 이상일 경우 **생존**합니다.\n");
    printf(" 절반을 맞추지 못하면(5개 이하) **패배**합니다.\n");
    printf("==========================================\n");

    printf("\n [ 계속하려면 아무 키나 누르세요... ]");

    // 엔터 한 번으로 넘어가게 수정
    int temp_char_start = getchar();
    (void)temp_char_start; // 변수 사용 경고 방지

    system("cls"); // 화면 지우기 (Windows 전용)
}


/**
 * @brief 선택된 캐릭터와 현재 실패 단계(stage)에 맞는 아스키 아트를 출력합니다.
 * @param character_choice 사용자가 선택한 캐릭터 (1, 2, 3)
 * @param stage 현재까지 틀린 횟수 (0 ~ 6)
 */
void draw_hangman(int character_choice, int stage) {
    if (stage < 0 || stage >= MAX_STAGES) return;

    const char** current_char_set;

    switch (character_choice) {
    case 1: current_char_set = (const char**)Classic_Char[stage]; break;
    case 2: current_char_set = (const char**)Boxy_Char[stage]; break;
    case 3: current_char_set = (const char**)Stickman_Char[stage]; break;
    default: current_char_set = (const char**)Classic_Char[stage]; break;
    }

    // 캐릭터 그림 7줄 출력
    for (int i = 0; i < 7; i++) {
        if (current_char_set[i]) {
            printf("%s\n", current_char_set[i]);
        }
    }
}

/**
 * @brief (버그 수정용) scanf, fgets 사용 후 입력 버퍼에 남은 \n(엔터)를 비웁니다.
 */
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 전역 변수로 사용자가 선택한 캐릭터 저장
int selected_char = 1;

/**
 * @brief [수정] 사용자에게 3가지 캐릭터를 보여주고 '1', '2', '3'만 입력받도록 수정
 */
void select_character() {
    char input_buffer[100];
    int choice = 0;
    int is_valid_input = 0;

    while (is_valid_input == 0) {
        system("cls"); // 화면 깨끗하게
        printf("==========================================\n");
        printf("     행맨(HANGMAN) 게임 캐릭터 선택\n");
        printf("==========================================\n\n");

        // 캐릭터 미리보기 출력 (최종 모습)
        printf(" ----- [1] 호박 (Pumpkin) -----\n");
        draw_hangman(1, MAX_STAGES - 1);
        printf("\n");
        printf(" ----- [2] 해골 (Skeleton) -----\n");
        draw_hangman(2, MAX_STAGES - 1);
        printf("\n");
        printf(" ----- [3] 허수아비 (Scarecrow) -----\n");
        draw_hangman(3, MAX_STAGES - 1);
        printf("\n");

        printf("==========================================\n");
        printf(" [1] 호박 (Pumpkin)\n");
        printf(" [2] 해골 (Skeleton)\n");
        printf(" [3] 허수아비 (Scarecrow)\n");

        printf("\n 캐릭터를 선택하세요 (1, 2, 3): ");

        // [수정] scanf -> fgets로 변경
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            continue; // 입력 오류 시 다시
        }

        // [수정] 입력받은 문자열이 정확히 "1", "2", "3" (엔터 포함)인지 확인
        if (strcmp(input_buffer, "1\n") == 0) {
            choice = 1;
            is_valid_input = 1;
        }
        else if (strcmp(input_buffer, "2\n") == 0) {
            choice = 2;
            is_valid_input = 1;
        }
        else if (strcmp(input_buffer, "3\n") == 0) {
            choice = 3;
            is_valid_input = 1;
        }
        else {
            // [수정] 엔터만 치거나, 다른 문자 입력 시
            printf("\n 잘못된 입력입니다. 1, 2, 3 중 하나를 정확히 입력하세요.\n");
            printf(" (잠시 후 다시 선택합니다...)\n");
            Sleep(1500); // 1.5초 대기
        }
    } // end while

    // 유효한 입력이 들어옴
    selected_char = choice;
    printf("\n ==> %d번 캐릭터가 선택되었습니다!\n", selected_char);

    printf("\n [ 잠시 후 게임을 시작합니다... ]\n");
    Sleep(1000); // 1초 대기
    system("cls"); // 화면 지우기
}


/**
 * @brief 메인 게임 로직을 수행합니다. (10문제 진행)
 */
void play_game() {
    int current_quiz = 0;   // 현재 문제 번호
    int wrong_guesses = 0;  // 틀린 횟수 (0 ~ 6)
    int correct_answers = 0; // 맞춘 문제 수
    char user_input[100];   // 사용자 단어 입력 버퍼

    // [수정] 승리 조건 (6개)를 루프 시작 전에 정의
    int survival_threshold = TOTAL_QUIZZES / 2; // 10 / 2 = 5

    printf("\n\n#################################################\n");
    printf("#           [ 문장 행맨 게임 시작 ]             #\n");
    printf("#     빈칸(***)에 들어갈 단어를 맞추세요!       #\n");
    printf("#################################################\n");

    // [수정] 6개를 맞추면 루프가 중단되도록 조건 추가 (correct_answers <= survival_threshold)
    while (current_quiz < TOTAL_QUIZZES && wrong_guesses < MAX_STAGES - 1 && correct_answers <= survival_threshold) {

        system("cls"); // 매 턴마다 화면 깨끗하게

        printf("\n\n--- [ 문제 %d / %d ] (맞춘 개수: %d / 틀린 횟수: %d / 6) ---\n",
            current_quiz + 1, TOTAL_QUIZZES, correct_answers, wrong_guesses);

        printf("\n  문제: %s\n", quiz_sentences[current_quiz]);
        printf("\n  %s\n", quiz_hints[current_quiz]); // [추가] 힌트 출력
        draw_hangman(selected_char, wrong_guesses);

        printf("\n  빈칸(*** 또는 sw**t)에 들어갈 단어를 입력하세요: ");

        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            printf("\n입력 오류입니다.\n");
            break;
        }

        size_t len = strlen(user_input);
        if (len > 0 && user_input[len - 1] == '\n') {
            user_input[len - 1] = '\0';
        }

        if (strlen(user_input) == 0) {
            printf("\n  [X] 땡! 아무것도 입력하지 않았습니다.\n");
            printf("  [!] 정답은 '%s' 입니다.\n", quiz_answers[current_quiz]); // [수정] 정답 알려주기

            Beep(261, 300);
            Beep(261, 300);

            wrong_guesses++;
            current_quiz++;

            printf("\n  [ 정답 확인 후 다음 문제로 넘어갑니다... (1초) ]\n");
            Sleep(1000);
            continue;
        }

        if (strcmp(user_input, quiz_answers[current_quiz]) == 0) {
            // [O] 정답
            printf("\n  [O] 정답입니다! '%s' 맞습니다!\n", user_input);

            Beep(523, 120);
            Beep(659, 120);
            Beep(784, 150);

            correct_answers++;
            current_quiz++;
            printf("\n  [ 잠시 후 다음 문제로 넘어갑니다... (1초) ]\n");
            Sleep(1000);
        }
        else {
            // [X] 오답
            printf("\n  [X] 땡! '%s'가 아닙니다...\n", user_input);
            printf("  [!] 정답은 '%s' 입니다.\n", quiz_answers[current_quiz]); // [수정] 정답 알려주기

            Beep(261, 300);
            Beep(261, 300);

            wrong_guesses++;
            current_quiz++;
            printf("\n  [ 정답 확인 후 다음 문제로 넘어갑니다... (1초) ]\n");
            Sleep(1000);
        }

    } // end while loop

    // ===================================
    // 최종 결과 판정
    // ===================================
    system("cls");

    printf("\n\n==========================================\n");
    printf("              [ 게임 결과 ]\n");
    printf("==========================================\n");

    // [수정] 승리/패배 판정 로직을 '조기 승리'가 가능하도록 수정

    // [조건 1] 6번 틀려서 게임 오버 (최우선 패배 조건)
    if (wrong_guesses == MAX_STAGES - 1) {
        printf("          게임 오버... 6번 틀렸습니다. \n");
        printf("               *** 패 배 ***\n");

        if (current_quiz > 0) {
            printf("  마지막 문제 정답: %s\n", quiz_answers[current_quiz - 1]);
        }

        printf("==========================================\n");
        draw_hangman(selected_char, MAX_STAGES - 1);
    }
    // [조건 2] 6개 이상 맞춘 경우 (생존)
    else if (correct_answers > survival_threshold) { // 6~10개 맞춤
        printf("         %d개의 문제를 맞춰 생존했습니다! \n", correct_answers);
        printf("               *** 생 존 ***\n");
        printf("  (총 %d개 정답, %d개 오답)\n", correct_answers, wrong_guesses);
        draw_hangman(selected_char, wrong_guesses);
    }
    // [조건 3] 10문제를 다 풀었지만, 5개 이하로 맞춘 경우 (패배)
    else if (current_quiz == TOTAL_QUIZZES) {
        printf("          게임 오버... 절반을 맞추지 못했습니다. \n");
        printf("               *** 패 배 ***\n");
        printf("  (총 %d개 정답, %d개 오답)\n", correct_answers, wrong_guesses);
        printf("==========================================\n");
        draw_hangman(selected_char, wrong_guesses);
    }
}

// ====================================================
// 6. 메인 함수 (게임 전체 루프)
// ====================================================
int main() {

    PlaySound(L"bgm.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);

    int play_again = 1;

    while (play_again == 1) {

        show_intro();

        select_character();

        play_game();

        // 재시작 여부 묻기
        printf("\n\n==========================================\n");
        printf("          다시 하시겠습니까?\n");
        printf("==========================================\n");
        printf(" [1] 네 (캐릭터 선택부터 다시)\n");
        printf(" [2] 아니오 (게임 종료)\n");
        printf("\n 선택 (1 또는 2): ");

        int choice_ch = 0;
        while (choice_ch != '1' && choice_ch != '2') {
            choice_ch = _getch(); // '1' 또는 '2' 키 입력 대기
        }

        if (choice_ch == '1') {
            play_again = 1;
        }
        else {
            play_again = 0; // 루프 종료
        }

        system("cls");

    } // end while(play_again)


    // 게임 종료
    printf("\n\n[ 게임을 종료합니다. 이용해주셔서 감사합니다. ]\n");

    PlaySound(NULL, 0, 0);

    Sleep(1500);

    return 0;
}