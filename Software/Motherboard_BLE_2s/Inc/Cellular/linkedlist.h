/**
 * @file linkedlist.h
 *
 *  Created on: Feb 15, 2019
 *      Author: Joschka Wirges
 */
#include <stdint.h>

/**
 * @brief type at_resp is a linked list element storing at responses of unknown number of lengths and characters per line
 * http://www.zentut.com/c-tutorial/c-linked-list/
 * @param length (uint8_t) stores the length of the current elements line (as in: number of characters)
 * @param next (struct at_resp*) is the pointer to the next elementof at_resp
 * @param line[] (char) stores the characters of the current line
 */
typedef struct at_resp{
	uint8_t length;
	struct at_resp* next;
	char line[100];
}at_resp;

typedef void (*callback)(at_resp* line);



/* Function Prototypes */
at_resp* create(char newline[], at_resp* next);
at_resp* prepend(at_resp* head,char newline[]);
uint8_t count(at_resp *head);
void traverse(at_resp* head,callback f);
at_resp* append(at_resp* head, char newline[]);
void display(at_resp* n);
void dispose(at_resp *head);

extern at_resp* head;
