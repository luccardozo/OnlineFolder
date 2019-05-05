#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <sys/inotify.h>
#include "../../include/common/common.h"

void serializePacket(packet inPacket, char* serialized, int size) {
    return;
}

void deserializePacket(packet outPacket, char* serialized, int size) {
    return;
}
/*
TODO: Avisar o servidor que aconteceu uma mudança e tratar a mesma...
TODO: Criar uma tread no servidor que fica esperando esse aviso do cliente para com ele.
*/
void *inotifyWatcher(void *pathToWatch){
    int length;
    int fd;
    int wd;
    char buffer[BUF_LEN];

    fd = inotify_init();

        if ( fd < 0 ) {
        perror( "inotify_init" );
    }

    wd = inotify_add_watch( fd, (char *) pathToWatch, 
                            IN_CLOSE_WRITE | IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO);

    while (1) {
        int i = 0;
        length = read( fd, buffer, BUF_LEN );

        if ( length < 0 ) {
            perror( "read" );
        }  

        while ( i < length ) {
            struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
            if ( event->len ) {
                if ( event->mask & IN_CREATE ) {
                    if ( event->mask & IN_ISDIR ) {
                        printf( "The directory %s was created in %s.\n", event->name,(char *) pathToWatch);       
                    }
                    else {
                        printf( "The file %s was created in %s.\n", event->name,(char *) pathToWatch);
                    }
                }
                else if ( event->mask & IN_DELETE ) {
                    if ( event->mask & IN_ISDIR ) {
                        printf( "The directory %s was deleted in %s.\n", event->name,(char *) pathToWatch);       
                    }
                    else {
                        printf( "The file %s was deleted in %s.\n", event->name,(char *) pathToWatch);
                    }
                }
                else if ( event->mask & IN_MODIFY ) {
                    if ( event->mask & IN_ISDIR ) {
                        printf( "The directory %s was modified in %s.\n", event->name,(char *) pathToWatch );
                    }
                    else {
                        printf( "The file %s was modified in %s.\n", event->name,(char *) pathToWatch);
                    }
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }
    ( void ) inotify_rm_watch( fd, wd );
    ( void ) close( fd );
}

int checkAndCreateDir(char *pathName){
    struct stat sb;
    //printf("%s",strcat(pathcomplete, userName));
    if (stat(pathName, &sb) == 0 && S_ISDIR(sb.st_mode)){
        // usuário já tem o diretório com o seu nome
        return 0;
    }
    else{
        if (mkdir(pathName, 0777) < 0){
            //....
            return -1;
        }
        // diretório não existe
        else{
            printf("Creating %s directory...\n", pathName);
            return 0;
        }
    }
}