#include <stdio.h>
#include <string.h>

#include "quiz.h"

int play(quiz_t *quiz) 
{
    char *res = fetch(API_URL);
    if (res == NULL) return -1;
    
    int parse_res = parse(quiz, res);
    if (parse_res == -1) return -1;
    if (parse_res == -2) return -2;
    

    quiz->n++;
    quiz->max += 8;

    int available_points = 8;

    printf("\n%s\n\n", quiz->question);

    for (int i = 0; i < 4; i++) {
	printf("[%c] %s\n", 'a' + i, quiz->choices[i]);
    }

    do {
	printf("(%dpt) > ", available_points);
	fflush(stdout);
	

	char user_input = getchar();
	if (user_input == EOF) return 1;

	getchar();

	if (user_input - 'a' == quiz->answer_index) {
	    quiz->score += available_points;
	    printf("Congratulation, answer [%c] is correct. Your current score is %d/%d points.\n",
		   user_input, quiz->score, quiz->max);

	    break;
	}
	

	available_points /= 2;
	printf("Answer [%c] is wrong. Try again.\n", user_input);
    } while (1);

    
    return 0;
}
