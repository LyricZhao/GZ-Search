/* 2018.05.06 ok */

# ifndef __DATABASEWRITER_H__
# define __DATABASEWRITER_H__

# include <string>
# include <stdlib.h>
# include <string.h>
# include <iostream>
# include <db_cxx.h>

# define AUTO_WRITE_LIMIT 100

void addContext(unsigned char *recvSTR);
int getUrlID(const std:: string &url);
void writeEntry(const CONTEXT &context);
void writeDataBase();
void checkContext();
void *dataBaseWriterENT(void *data);

# endif
