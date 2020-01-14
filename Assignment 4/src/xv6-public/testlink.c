#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{

    char *sfile =  "./sdir/sfile";
    char *nfile = "./ndir/nfile";

    //create a small directory
    if(mksfdir("sdir") < 0){
        printf(2, "testlink: failed to create small dir\n");
    }else{
        printf(1, "testlink: Created small dir sdir\n");
    }

    //Creare a normal directory
    if(mkdir("ndir") < 0){
        printf(2, "mkdir:  failed to create ndir\n");
    }else{
        printf(1, "testlink: Created noraml dir ndir\n");
    }

    if(open("./sdir/sfile", O_CREATE | O_RDWR) < 0){
      printf(2, "touch: failed to create small file \n");
    }else{
        printf(1, "testlink: Created small file at /sdir/sfile\n");
    }


    printf(1, "testlink: Linking small file at /sdir/sfile to /ndir/nfile\n");
    if(link(sfile,nfile) < 0){
        printf(2, "Linking issue\n");
    }
    else{
        printf(2, "Linking done\n");
    }
        
    exit();
}