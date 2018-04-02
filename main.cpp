/* servTCPConcTh2.c - Exemplu de server TCP concurent care deserveste clientii
   prin crearea unui thread pentru fiecare client.
   Asteapta un numar de la clienti si intoarce clientilor numarul incrementat.
	Intoarce corect identificatorul din program al thread-ului.


   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

//parse
#include <iostream>
#include <fstream>
using namespace std;


/* portul folosit */
#define PORT 8003

/* codul de eroare returnat de anumite apeluri */
//extern int errno;//deja declarat int *(void) acum int (*)()

typedef struct thData{
    int idThread; //id-ul thread-ului tinut in evidenta de acest program
    int cl; //descriptorul intors de accept
}thData;

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);


int main ()
{
    struct sockaddr_in server;	// structura folosita de server
    struct sockaddr_in from;
    int nr;		//mesajul primit de trimis la client

    int sd;		//descriptorul de socket
    int pid;
    pthread_t th[100];    //Identificatorii thread-urilor care se vor crea
    int i=0;


    /* crearea unui socket */
    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("[server]Eroare la socket().\n");
        return errno;
    }
    /* utilizarea optiunii SO_REUSEADDR */
    int on=1;
    setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

    /* pregatirea structurilor de date */
    bzero (&server, sizeof (server));
    bzero (&from, sizeof (from));

    /* umplem structura folosita de server */
    /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;
    /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    /* utilizam un port utilizator */
    server.sin_port = htons (PORT);

    /* atasam socketul */
    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
        perror ("[server]Eroare la bind().\n");
        return errno;
    }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen (sd, 2) == -1)
    {
        perror ("[server]Eroare la listen().\n");
        return errno;
    }
    /* servim in mod concurent clientii...folosind thread-uri */
    while (1)
    {
        int client;
        thData * td; //parametru functia executata de thread
        unsigned int length = sizeof (from);

        printf ("[server]Asteptam la portul %d...\n",PORT);
        fflush (stdout);

        //client= malloc(sizeof(int));
        /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
        if ( (client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
        {
            perror ("[server]Eroare la accept().\n");
            continue;
        }

        /* s-a realizat conexiunea, se astepta mesajul */

        int idThread; //id-ul threadului
        int cl; //descriptorul intors de accept

        td=(struct thData*)malloc(sizeof(struct thData));
        td->idThread=i++;
        td->cl=client;

        pthread_create(&th[i], NULL, &treat, td);

    }//while
};
static void *treat(void * arg)
{
    struct thData tdL;
    tdL= *((struct thData*)arg);
    printf ("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
    fflush (stdout);
    pthread_detach(pthread_self());
    raspunde((struct thData*)arg);
    /* am terminat cu acest client, inchidem conexiunea */
    close ((intptr_t)arg);
    return(NULL);

};


void raspunde(void *arg)
{
    int nr, i=0;

    char msg[1000];		//mesajul primit de la client
    char msgrasp[1000]=" ";        //mesaj de raspuns pentru client
    //char raw_file[1000];

    struct thData tdL;
    tdL= *((struct thData*)arg);

    bzero(msg,1000);///??
        if (read (tdL.cl, msg, sizeof(msg)) <= 0)
    {
        printf("[Thread %d]\n",tdL.idThread);
        perror ("Eroare la read() de la client.\n");
    }
        printf ("[Thread %d]Mesajul a fost receptionat...\n\n",tdL.idThread);

    /*Print mesaj primit*/
    //printf ("%s",msg);

    /*pregatim mesajul de raspuns */
    bzero(msgrasp,1000);


    //parse("GET /path/script.cgi?field1=value1&field2=value2 HTTP/1.1");

    const char* p_msg = msg;

    /* Find out where everything is */
    const char *start_of_path = strchr(p_msg, ' ') + 2; // 2 pentru a nu lua "/" din path

    //const char *start_of_query = strchr(start_of_path, '?');
    //if (strchr(start_of_path, '?')==NULL)
    const char *start_of_query = strchr(start_of_path, ' ');

    /* Get the right amount of memory */
    char path[start_of_query - start_of_path];

    /* Copy the strings into our memory */
    strncpy(path, start_of_path,  start_of_query - start_of_path);

    /* Null terminators (because strncpy does not provide them) */
    //path[sizeof(path)] = '\0';

    /*Print */

    printf("PATH IS = %s", path);

    const char* opt_body="<html><body><h1>Hello, World!</h1></body></html>";

//    sprintf("<html><body><h1>Hello, World!</h1>\"\n"
//                    "<h2> PATH IS = %s </h2>\"\n"
//                    "</body></html>", path);

//    strcat(opt_body,"<html><body><h1>Hello, World!</h1><h2> PATH IS = ");
//    strcat(opt_body, path);
//    strcat(opt_body,"</h2></body></html>");

    const char * m404= "<html><head>\n"
            "<title>404 Not Found</title>\n"
            "</head><body>\n"
            "<h1>Not Found</h1>\n"
            "<p>The requested URL/file was not found on this server.</p>\n"
            "</body></html>";

    char raw_file[1000];
    bzero(raw_file,1000);

    //citire fisier
    // VARIANTA C++
//    ifstream f;//("index.html");
//    int raw_file_l=0;
//    f.open(path);
    // if(f.is_open()) {
//        while (!f.eof()) {
//            f >> raw_file[raw_file_l];
//            raw_file_l++;
//        }
//
//        raw_file[raw_file_l] = NULL;

    // VARIANTA C
    FILE * f ;
    if(f=fopen(path,"r"))
    {
        fseek(f, 0, SEEK_END);
        int len = ftell(f);
        rewind(f);
        char *raw_file;
        fread(raw_file, len, 1, f);
        raw_file[len+1]='\0';
        printf("am citit din fisier:\n%s\n", raw_file);

        fclose(f);

        sprintf(msgrasp,
                "HTTP/1.1 200 OK\r\n"
                        "Server: WebServer\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: %d\r\n"
                        "Connection: close\r\n"
                        "\r\n"
                ,0
                 +strlen(opt_body)+strlen(msg)+strlen(raw_file)
        );
        strcat(msgrasp, opt_body);
        strcat(msgrasp, raw_file);
        strcat(msgrasp,msg);
    }
    else {
        printf("nu s-a putut deschide, fisierul nu exista probabil");
        perror("perrrrrrr");
        sprintf(msgrasp,
                "HTTP/1.1 404 Not Found\r\n"
                        "Server: WebServer\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: %d \r\n"
                "Connection: close\r\n"
                "\r\n"
                ,0
                 +strlen(opt_body)+strlen(m404)+strlen(msg)
        );
        strcat(msgrasp, opt_body);
        strcat(msgrasp,m404);
        strcat(msgrasp,msg);
    }

    printf("##########################%s",raw_file);

//    while(fgets(raw_file,1000,fd))
//    {
//       printf("%s", raw_file);
//    }
//    fclose(fd);
//    printf("%s",raw_file);



    //printf("lung body=%d\n",strlen(opt_body));
    //printf("lung msg primit=%d\n",strlen(msg));

    /* returnam mesajul clientului */
    if (write (tdL.cl, msgrasp, sizeof(msgrasp)) <= 0)
    {
        printf("[Thread %d] ",tdL.idThread);
        perror ("[Thread]Eroare la write() catre client.\n");
    }
    else
        printf ("[Thread %d]Mesajul a fost trasmis cu succes.\n %s",tdL.idThread,msgrasp);

}
