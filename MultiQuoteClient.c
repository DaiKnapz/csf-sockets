//
//  MultiQuoteClient
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

int main(int argc, const char *argv[])
{
    //Input validation
    if (argc != 2 || !strcmp(argv[1], "help"))
    {
        exit_usage(argv[0]);
    }

    //Open connection
    int sockFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //Validation, checks +ve value returned
    if (sockFD < 0)
    {
        fprintf(stderr, "Error: Socket could not be initialised, please try again!");
        exit(-2);
    }

    //Setup address
    struct sockaddr_in sockaddrIn;
    memset(&sockaddrIn, 0, sizeof(sockaddrIn));
    sockaddrIn.sin_family = AF_INET;
    sockaddrIn.sin_port = htons(kMULTIQUOTEPORT);
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
    char received[BUFFSIZE];
    int nRecieved = 0;
    int continues;

    do
    {
        continues = 1;
        do
        {
            nRecieved = (int) read(sockFD, (void *) received, BUFFSIZE);
            if (nRecieved > 0)
            {
                int i;
                received[nRecieved] = '\0';
                for (i = 0; i < nRecieved; i++)
                {
                    if (received[i] == 10)
                    {
                        continues = 0;
                        break;
                    }
                }
            }
            else
            {
                fprintf(stderr, "Error: No data recieved\n ");
                break;
            }
            printf("%s", received);
        } while (continues);
        printf("\n");


        char response[BUFFSIZE];
        printf("Your response: ");
        fgets(response,BUFFSIZE, stdin);
        response[strlen(response) -1] = '\0';
        sprintf(response, "%s\r\n", response);
        write(sockFD, response, strlen(response));
        if (strcmp(response, "CLOSE\r\n") == 0) {
            nRecieved = (int) read(sockFD, (void *) received, BUFFSIZE);
            received[nRecieved] = '\0';
            break;
        }
    } while (nRecieved > 0);
    printf("%s", received);
    close(sockFD);
    return 0;
}

void exit_usage(const char *programName)
{
    fprintf(stderr,
            "USAGE: %s server_name \n\nWhere server_name is the address (URL/IP address) of the server you wish to connect to\n",
            programName);
    exit(1);
}
