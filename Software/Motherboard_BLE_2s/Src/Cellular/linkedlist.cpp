/**
 * @file linkedlist.c
 *
 *  Created on: Feb 15, 2019
 *      Author: Joschka Wirges
 */

#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include <string.h>
#include "main.h"

at_resp* head;


/**
 * @brief creates a linked list and fills the first list element
 * http://www.zentut.com/c-tutorial/c-linked-list/
 * @param newline (char []) data
 * @param pointer to the next list element
 * @retvalue new list element
 */
at_resp* create(char newline[], at_resp* next) {
	at_resp* new_at_resp = (at_resp*) malloc(sizeof(at_resp));
	if (new_at_resp == NULL) {
		printf("Error creating a new at_resp.\r\n");
	}
	new_at_resp->length = strlen(newline);
	strcpy(new_at_resp->line, newline);
	new_at_resp->next = next;
	return new_at_resp; //returns NULL if there was an error
}

/**
 * @brief points the next pointer of the new at_resp
 * http://www.zentut.com/c-tutorial/c-linked-list/
 * @param *head (at_resp) pointer to the head at_resp of the list
 * @param newline (char []) data
 * @retvalue new head pointer (at_resp)
 */
at_resp* prepend(at_resp* head, char newline[]) {
	at_resp* new_at_resp = create(newline, head);
	head = new_at_resp;
	return head;
}

/**
 *  @brief counts the number of elements currently in the list
 *  http://www.zentut.com/c-tutorial/c-linked-list/
 *  @param *head (at_resp) pointer to the head at_resp of the list
 *  @retvalue c (uint8_t) number of elements
 */
uint8_t count(at_resp *head) {
	at_resp *cursor = head;
	uint8_t c = 0;
	while (cursor != NULL) {
		c++;
		cursor = cursor->next;
	}
	return c;
}

/**
 * @brief traverses through the list
 * http://www.zentut.com/c-tutorial/c-linked-list/
 * @param *head (at_resp) pointer to the head at_resp of the list
 * @param f (callback)
 */
void traverse(at_resp* head, callback f) {
	at_resp* cursor = head;
	while (cursor != NULL) {
		f(cursor);
		cursor = cursor->next;
	}
}

/**
 * @brief appends a new at_resp element to the end of the list
 * http://www.zentut.com/c-tutorial/c-linked-list/
 * @param *head (at_resp) pointer to the head at_resp of the list
 * @param newline (char []) data
 * @retvalue new head pointer (at_resp)
 */
at_resp* append(at_resp* head, char newline[]) {
	/* go to the last at_resp */
	at_resp *cursor = head;
	while (cursor->next != NULL)
		cursor = cursor->next;
	/* create a new at_resp */
	at_resp* new_at_resp = create(newline, NULL);
	cursor->next = new_at_resp;

	return head;
}

/**
 * @brief prints a list element
 * http://www.zentut.com/c-tutorial/c-linked-list/
 * @param *n (at_resp) pointer to element to be printed
 */
void display(at_resp* n) {
	if (n != NULL) {
		for (uint8_t i = 0; i < (n->length); i++) {
			printf("%c", n->line[i]);
		}
		printf("\r\n");
	}
}

/**
 * @brief frees all memory and deletes the list
 * http://www.zentut.com/c-tutorial/c-linked-list/
 * @param *head (at_resp) head pointer of the list
 */
void dispose(at_resp *head) {
	at_resp *cursor, *tmp;

	if (head != NULL) {
		cursor = head->next;
		head->next = NULL;
		while (cursor != NULL) {
			tmp = cursor->next;
			free(cursor);
			cursor = tmp;
		}
	}
}
