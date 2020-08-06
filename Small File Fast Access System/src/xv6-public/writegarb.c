#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define EXACTLY_52 "exactly_52"
#define MORE_THAN_52 "more_than_52"

int main(int argc, char *argv[])
{
    int n, fd;
    char * message = "Machines take me by surprise with great frequency.\n";

    if (argc < 2)
    {
        printf(2, "Usage: writegarb files...\n");
        exit();
    }

    if ((fd = open(argv[1], O_RDWR)) < 0)
    {
        printf(2, "writegarb: failed to open file %s\n", argv[1]);
        exit();
    }

    if(argc > 2){
        if(strcmp(argv[2], EXACTLY_52) == 0){
            message = "Machines take me by surprise with great frequency .\n";
        }else if(strcmp(argv[2], MORE_THAN_52) == 0){
            message = "Machines take me by surprise with great frequency testing.\n";
        }else{
            printf(2, "writegarb: A valid second argument must be 'exactly_52' or 'more_than_52'");
            exit();
        }
    }

    printf(1, "Write size : %d\n", strlen(message) * sizeof(char));
    if((n = write(fd, message, strlen(message) * sizeof(char))) < 0){
        printf(1, "writegarb : failed to write\n");
    }

    exit();
}