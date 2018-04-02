# rc-project
Simple HTTP web server @ Computer Networks course

## Server http concurent

### 1. Pachete librarii
PTHREAD
### 2. Cum se compileaza
#### CMAKE:
cmake_minimum_required(VERSION 3.9)
project(rc_p)

set(CMAKE_CXX_STANDARD 11)

add_executable(rc_p main.cpp)
target_link_libraries( rc_p pthread )

#### Make:
all:
	g++ main.cpp -lpthread -o s.exe
clean:
	rm -f *~ s.exe

### 3. Explicatii cum se foloseste proiectul ( nu usecasuri, nu cerinta)

-pornim serverul (localhost:portul/nume_fisier) specificat
-ne conectam cu un browser (acesta fiind clientul)
exemplu : http://127.0.0.1:8003/index.html
sau test.txt

serverul se inchide cu C-c si nu cu C-z pt a nu ramande portul deschis

### 4. Bibliografia

*Threading:
https://profs.info.uaic.ro/~computernetworks/files/NetEx/S12/ServerConcThread/servTcpConcTh2.c

*C manual:
http://www.cplusplus.com/reference/cstdio/sprintf/?kw=sprintf
http://www.cplusplus.com/reference/cstring/strchr/

https://www.tutorialspoint.com/http/http_responses.htm
https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm
https://fresh2refresh.com/c-programming/c-file-handling/fseek-seek_set-seek_cur-seek_end-functions-c/

*Format:
https://www.ntu.edu.sg/home/ehchua/programming/webprogramming/HTTP_Basics.html
http://www.jmarshall.com/easy/http/

https://stackoverflow.com/questions/15106102/how-to-use-c-stdostream-with-printf-like-formatting
http://www.boost.org/doc/libs/1_53_0/libs/format/doc/format.html

https://stackoverflow.com/questions/41286260/parse-http-request-line-in-c
https://stackoverflow.com/questions/7564033/difference-between-char-and-char
https://stackoverflow.com/questions/8266990/is-this-a-valid-http-response

*Errors:
https://ubuntuforums.org/showthread.php?t=646399
https://stackoverflow.com/questions/3413166/when-does-a-process-get-sigabrt-signal-6
