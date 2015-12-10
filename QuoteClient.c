//
//  QuoteClient
//
//  Created by <David Knapman>
//  Username: <psydrk>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>

#define kQUOTEPORT      1717
#define kMULTIQUOTEPORT 1818
#define BUFFSIZE        512

void exit_usage(const char *programName);

int main(int argc, const char * argv[])
{
    //Input validation
    if(argc != 2 || !strcmp(argv[1], "help")) {
        exit_usage(argv[0]);
    }

    //Open connection
    int sockFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //Validation, checks +ve value returned
    if (sockFD < 0) {
        fprintf(stderr, "Error: Socket could not be initialised, please try again!");
        exit(-2);
    }

    //Setup address
    struct sockaddr_in sockaddrIn;
    memset(&sockaddrIn, 0, sizeof(sockaddrIn));
    sockaddrIn.sin_family = AF_INET;
    sockaddrIn.sin_port = htons(kQUOTEPORT);
    //Setup host
    struct hostent *host;
    host = gethostbyname(argv[1]);
    memcpy(&sockaddrIn.sin_addr, host->h_addr, host->h_length);

    //Connect to server
    int connectValue = connect(sockFD, (struct sockaddr *)&sockaddrIn, sizeof(sockaddrIn));
    if (connectValue!=0){
        //Connection failed, exit program
        fprintf(stderr, "Error: Could not establish connection! Error no: %d\n", errno);
        exit(-1);
    }
    char received[BUFFSIZE + 1];
    int nRecieved = 0;
    int continues = 1;
    nRecieved = (int) read(sockFD, (void *) received, BUFFSIZE);
    printf("%d\n", nRecieved);

    do {
        if (nRecieved>0) {
            int i;
            received[nRecieved] = '\0';
            for(i=0; i<nRecieved; i++){
                if (received[i] == 10) {
                    continues = 0;
                    break;
                }
            }
        }
        else {
            fprintf(stderr, "Error: ");
            break;
        }
        printf("%s\n", received);
    } while(continues);



    close(sockFD);
    return 0;
}

void exit_usage(const char *programName)
{
    fprintf(stderr, "USAGE: %s server_name \n\nWhere server_name is the address (URL/IP address) of the server you wish to connect to\n", programName);
    exit(1);
}
