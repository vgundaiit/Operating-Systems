#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

int
main(int argc, char *argv[])
{
  int fd, no_of_bytes;

  if(argc <= 2){
    printf(1, "readtest: read error\n");
    exit();
  }

  if((fd = open(argv[1], 0)) < 0){
    printf(1, "readtest: cannot open %s\n", *((int*)argv[1]));
    exit();
  }

  no_of_bytes = read(fd, buf, atoi(argv[2]));
  printf(1, "Read %d bytes.\n", no_of_bytes);

  close(fd);

  exit();
}
