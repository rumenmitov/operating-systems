#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "quiz.h"
#include "errors.h"

static void sig_action(int signum, siginfo_t *siginfo, void *quiz_ptr);

static void sig_install(void)
{
    struct sigaction sa;

    sa.sa_sigaction = sig_action;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO | SA_RESTART;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
	die("sigaction()");
    }
}


static void __clean(quiz_t *quiz) 
{
    if (quiz->answer) {
	free(quiz->answer);
	quiz->answer = NULL;
    }
    

    if (quiz->question) {
	free(quiz->question);
	quiz->question = NULL;
    }

    if (quiz->max == 0) return;

    for (int i = 0; i < 4; i++) {
	if (quiz->choices[i]) {
	    free(quiz->choices[i]);
	    quiz->choices[i] = NULL;
	}
    }
}


static void __quit(quiz_t *quiz) 
{
    printf("\n\n");
    printf("Thanks for playing today.\n");
    printf("Your final score is %d/%d points.\n\n", quiz->score, !quiz->max ? 0 : quiz->max - 8);

    __clean(quiz);
}


static void sig_action(int signum, siginfo_t *siginfo, void *quiz_ptr) 
{
    quiz_t *quiz = (quiz_t*) quiz_ptr;
    (void) siginfo;

    if (signum == SIGINT) {
	__quit(quiz);
	exit(EXIT_SUCCESS);
    };
}


int main(int argc, char *argv[])
{
    sig_install();
    
    quiz_t quiz = {
	.n = 0,
	.score = 0,
	.max = 0,
	.answer_index = 0
    };

    quiz.question = NULL;
    quiz.answer   = NULL;

    for (int i = 0; i < 4; i++) {
	quiz.choices[i] = NULL;
    }
    

    while (1) {
	int res = play(&quiz);
	if (res == -1) {
	    die2("quiz: failed to run");
	}

	if (res == 1) {
	    break;
	}
	

	__clean(&quiz);
    }

    __quit(&quiz);

    return EXIT_SUCCESS;
}
