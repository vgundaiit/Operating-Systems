#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    printf(2, "Usage: touch files...\n");
    exit();
  }

  for(i = 1; i < argc; i++){
    if(open(argv[i], O_CREATE | O_RDWR) < 0){
      printf(2, "touch: failed to create file %s\n", argv[i]);
      break;
    }
  }

  exit();
}