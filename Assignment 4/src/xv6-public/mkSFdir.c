#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    printf(2, "Usage: mkSFdir files...\n");
    exit();
  }

  for(i = 1; i < argc; i++){
    if(mksfdir(argv[i]) < 0){
      printf(2, "mkSFdir: %s failed to create\n", argv[i]);
      break;
    }
  }

  exit();
}
