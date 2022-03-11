# SIMD
使用SIMD指令集实现`C`语言标准库`<string.h>`中的一些函数
1. - [ ] void* memchr(const void* str, int c, size_t size)
2. - [ ] int memcmp(const void* str1, const void* str2, size_t size)
3. - [X]	void* memcpy(void* dst, const void* src, size_t size)
4. - [ ]	void* memmove(void* dst, const void* src, size_t size)
5. - [ ]	void* memset(void* str, int c, size_t size)
6. - [ ]	char* strcat(char* dst, const char* src)
7. - [ ]	char* strncat(char* dst, const char* src, size_t size)
8. - [ ]	char* strchr(const char* str, int c)
9. - [X]	int strcmp(const char* str1, const char* str2)
10. - [ ]	int strncmp(const char* str1, const char* str2, size_t size)
11. - [ ]	int strcoll(const char* str1, const char* str2)
12. - [ ]	char* strcpy(char* dst, const char* src)
13. - [ ]	char* strncpy(char* dst, const char* src, size_t size)
14. - [ ]	size_t strcspn(const char* str1, const char* str2)
15. - [ ]	char* strerror(int errnum)
16. - [X]	size_t strlen(const char* str)
17. - [ ]	char* strpbrk(const char* str1, const char* str2)
18. - [ ]	char* strrchr(const char* str, int c)
19. - [ ]	size_t strspn(const char* str1, const char* str2)
20. - [ ]	char* strstr(const char* haystack, const char* needle)
21. - [ ]	char* strtok(char* str, const char* delim)
22. - [ ]	size_t strxfrm(char* dst, const char* src, size_t size)


### 编译环境
- centos 8
- C++23
- gcc 11.2.0

**使用了AVX512指令集，编译前请先查看主机CPU是否支持此指令集。**
