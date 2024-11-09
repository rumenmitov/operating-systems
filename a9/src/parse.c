#include <time.h>
#include <stdio.h>
#include <jansson.h>
#include <string.h>


#include "quiz.h"
#include "errors.h"


static void __null_terminated_copy(char **dest, json_t **src) 
{
    size_t len = strlen(json_string_value(*src));
    
    *dest = (char*) malloc(len);
    if (*dest == NULL) die("malloc()");

    strncpy(*dest,
	    json_string_value(*src),
	    len);

    (*dest)[len] = '\0';
}


static int __parse_init(json_t **root, char *msg) 
{
    json_error_t err;
    
    *root = json_loads(msg, 0, &err);
    if (root == NULL) {
	fprintf(stderr, "JSON parser init failed\n");
	return -1;
    }
    
    json_t *response_code = json_object_get(*root, "response_code");
    if (!json_is_integer(response_code)) {
	json_decref(*root);
	fprintf(stderr, "Couldn't parse response code = %s\n", msg);
	
	return -1;
    }

    if (json_integer_value(response_code) != 0) {
	json_decref(*root);
	fprintf(stderr, "Invalid response code = %lli\n", json_integer_value(response_code));
	
	return -2;
    }

    return 0;
}


static int __parse_results(json_t **first_result, json_t **root)
{
    json_t *results = json_object_get(*root, "results");
    if (!json_is_array(results)) {
	fprintf(stderr, "Couldn't parse array\n");
	
	return -1;
    }
    

    *first_result = json_array_get(results, 0);
    if (!json_is_object(*first_result)) {
	fprintf(stderr, "Couldn't parse result\n");
	
	return -1;
    }

    return 0;
}


static int __parse_question(json_t **question, json_t **result, quiz_t *quiz) 
{
    *question = json_object_get(*result, "question");
    if (!json_is_string(*question)) {
	fprintf(stderr, "Couldn't parse question\n");
	
	return -1;
    }

    __null_terminated_copy(&quiz->question, question);

    return 0;
}


static int __parse_answer(json_t **correct, json_t **result, quiz_t *quiz) 
{
    *correct = json_object_get(*result, "correct_answer");
    if (!json_is_string(*correct)) {
	fprintf(stderr, "Couldn't parse answer\n");
	
	return -1;
    }

    __null_terminated_copy(&quiz->answer, correct);

    srand(time(NULL));
    quiz->answer_index = rand() % 4;

    __null_terminated_copy(&quiz->choices[quiz->answer_index], correct);	

    return 0;
}


static int __parse_incorrect(json_t **incorrect, json_t **result, quiz_t *quiz) 
{
    *incorrect = json_object_get(*result, "incorrect_answers");
    if (!json_is_array(*incorrect)) {
	fprintf(stderr, "Couldn't parse incorrect choices array\n");
	
	return -1;
    }
   
    int incorrect_len = json_array_size(*incorrect);

    for (int j = 0; j < incorrect_len; j++) {
	json_t *incorrect_idx = json_array_get(*incorrect, j);
	if (!json_is_string(incorrect_idx)) {
	    fprintf(stderr, "Couldn't parse incorrect choice\n");
	    
	    return -1;
	}

	int choice_idx = j >= quiz->answer_index ? j + 1 : j;

	__null_terminated_copy(&quiz->choices[choice_idx], &incorrect_idx);
    }

    return 0;
}


int parse(quiz_t *quiz, char *msg) 
{
    json_t *root    = NULL,
	*result     = NULL,
	*question   = NULL,
	*correct    = NULL,
	*incorrect  = NULL;
    

    int init_res = __parse_init(&root, msg);
    free(msg);

    if (init_res == -1) return -1;
    if (init_res == -2) return -2;


    if (__parse_results(&result, &root) == -1) {
	json_decref(root);
	return -1;
    }

    if (__parse_question(&question, &result, quiz) == -1) {
	json_decref(root);
	return -1;
    }
    
    
    if (__parse_answer(&correct, &result, quiz) == -1) {
	json_decref(root);
	return -1;
    }
    

    if (__parse_incorrect(&incorrect, &result, quiz) == -1) {
	json_decref(root);
	return -1;
    }
    
   
    json_decref(root);
    return 0;
}

