#ifndef INFO_H
#define INFO_H

void info(const char *, const char *, int);
void info_error(const char *, const char *, int);
void infoee(const char *, const char *, int);

#define INFO(message) \
	info((message), __FILE__, __LINE__)

#define INFOE(message) \
	infoe((message), __FILE__, __LINE__)

#define INFOEE(message) \
	infoee((message), __FILE__, __LINE__)

#endif /*INFO_H*/
