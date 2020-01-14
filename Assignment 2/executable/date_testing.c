#include "types.h"
#include "user.h"
#include "date.h"

int main(int argc, char *argv[]){
    struct rtcdate r;
 
    if(date(&r)){
        printf(1,"date returned error");         
    }
    else{
        printf(1,"date works perfectly fine with struct rtcdate argument\n");
    }


    /*
    //All these throw errors at compile time itself
    // struct rtcdate *rp;

    // int a = date(rp);

    // if(a == -1){
    //     printf(1,"date works perfectly fine with uninitialized argument");
    // }

    // int r1;

    // if(date(&r1) == -1){
    //     printf(1,"date correctly throwing error with int argument");
    // }

    // if(date() == -1){
    //     printf(1,"date correctly throwing error with no arguments");
    // }
    */
    
    exit();
}
