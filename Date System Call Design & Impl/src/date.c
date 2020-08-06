#include "types.h"
#include "user.h"
#include "date.h"

int main(int argc, char *argv[]){
    struct rtcdate r;

    if(date(&r)){
        printf(2,"date returned error");
        exit();
    }
    
    printf(1,"Current UTC date & time is %d:%d:%d %d-%d-%d\n",r.hour,r.minute,r.second,r.month,r.day, r.year);
    exit();
}
