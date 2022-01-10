/**
 * @brief String helper functions
 * 
 * @file StringHelper.c
 * @author Daniel Lohmann
 * @date 2019-03-18
 */
#include "StringHelper.h"
#include "limits.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"

/**
 * @brief Reverses the given string in the same memory
 */
void strreverse(char* str)
{
    if (str == NULL)
    {
        return;
    }

    int length = strlen(str);
    if (length == 0)
    {
        return;
    }

    char* ptrLow = str;
    char* ptrHigh = str + length - 1; // Do not point at '\0' therfore -1
    char tmp;

    while (ptrHigh > ptrLow)
    {
        tmp = *ptrHigh;
        *ptrHigh = *ptrLow;
        *ptrLow = tmp;

        ptrHigh--;
        ptrLow++;
    }

    return;
}

const char defaultTrimChars[] = {' ', '\n', '\r', '\0'};
const int defaultTrimCharsLength = 4;

bool iswhitespace(char c, const char* trimChars, int lengthTrimChars)
{
	for (int i = 0; i < lengthTrimChars; i++)
	{
		if (c == trimChars[i] || c == '\0')
		{
			return true;
		}
	}
	return false;
}

/**
 * @brief Returns a string which is trimmed
 */
char* strtrim(const char* src)
{
	return strusertrim(src, defaultTrimChars, defaultTrimCharsLength);
}

/**
 * @brief Returns a string which is trimmed
 * @param[in] src String to trim
 * @param[in] trimChars characters which should be removed be the trim operation
 * @param[in] lengthTrimChars Length of the list of chars which should be trimmed
 */
char* strusertrim(const char* src, const char* trimChars, int lengthTrimChars)
{
    int begin = -1;
    int end = INT_MAX;
    int iter = 0;
    while (src[iter] != '\0')
    {
        // Select characters to trim
        if (!iswhitespace(src[iter], trimChars, lengthTrimChars))
        {
            if (begin == -1)
            {
                begin = iter;
            }
            end = iter;
        }
        iter++;
    }
    if (begin == -1)
    {
        begin = 0;
    }
    if (end == INT_MAX)
    {
        end = -1;
    }

    char* newstr = calloc(end - begin + 2, sizeof(char));
    memcpy(newstr, src + begin, end - begin + 1);
    return newstr;
}


/**
 * @brief Split string into multiple strings
 * @param src String which should be split
 * @param len Pointer to a variable which will receive the length of the output array
 * @param trimChars Pointer to an arrays of chars which, is used to split the string
 * @param lengthtrimChars Length of the arrays of chars (Amount of chars), which is used to split the array
 * @return Pointer to an array of strings which contains the split strings. The pointer
 * 		   to the strings and the pointer to the array needs to be deleted
 */
char** strsplit_customchars(const char* src, int* len, const char* trimChars, int lengthTrimChars)
{
    if (src == NULL || trimChars == NULL)
    {
        return NULL;
    }

	*len = 0;
	int iter = 0;
	bool wasSplitChar = iswhitespace(src[iter], trimChars, lengthTrimChars);
	do
	{
		if (iswhitespace(src[iter], trimChars, lengthTrimChars) && !wasSplitChar)
		{
			(*len)++;
		}

		wasSplitChar = iswhitespace(src[iter], trimChars, lengthTrimChars);
	} while(src[iter++] != '\0');

	char** parts = (char**)calloc(*len, sizeof(char*));

	int begin = 0;
	int end = 0;
	int partIter = 0;
	iter = 0;
	do
	{
		if (iswhitespace(src[iter], trimChars, lengthTrimChars) && wasSplitChar)
		{
			begin = iter + 1;
		}

		if (iswhitespace(src[iter], trimChars, lengthTrimChars) && !wasSplitChar)
		{
			end = iter - 1;
			parts[partIter] = (char*)calloc(end - begin + 2, sizeof(char));
			memcpy(parts[partIter], src + begin, end - begin + 1);
			partIter++;
			begin = iter + 1;
		}

		wasSplitChar = iswhitespace(src[iter], trimChars, lengthTrimChars);
	} while(src[iter++] != '\0');

	return parts;
}

/**
 * @brief Split string into multiple strings
 * @param src String which should be split
 * @param len Pointer to a variable which will receive the length of the output array
 * @return Pointer to an array of strings which contains the split strings. The pointer
 * 		   to the strings and the pointer to the array needs to be deleted
 */
char** strsplit(const char* src, int* len)
{
	return strsplit_customchars(src, len, defaultTrimChars, defaultTrimCharsLength);
}

/**
 * @brief Count the number of the given character in a string
 */
int strcountchar(char* src, char c)
{
    int i = 0;
    for (i=0; src[i]; src[i]==c ? i++ : *src++);
    return i;
}
