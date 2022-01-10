/**
 * @brief String helper functions
 * 
 * @file StringHelper.h
 * @author Daniel Lohmann
 * @date 2019-03-18
 */
#ifndef __STRINGHELPER_H_
#define __STRINGHELPER_H_

#ifdef __cplusplus
 extern "C" {
#endif

void strreverse(char* src_dest);
char* strtrim(const char* src);
char* strusertrim(const char* src, const char* trimChars, int trimCharsLength);
char** strsplit_customchars(const char* src, int* len, const char* trimChars, int lengthTrimChars);
char** strsplit(const char* src, int* len);
int strcountchar(char* src, char c);


#ifdef __cplusplus
 }
#endif
#endif /* __STRINGHELPER_H_ */
