#ifndef INFO_H
#define INFO_H

extern void info(const char *msg, const char *file, int line);
extern void infoe(const char *msg, const char *file, int line);
extern void infoee(const char *msg, const char *file, int line);

#define INFO(message) \
	info((message), __FILE__, __LINE__)

#define INFOE(message) \
	infoe((message), __FILE__, __LINE__)

#define INFOEE(message) \
	infoee((message), __FILE__, __LINE__)

#endif /*INFO_H*/
