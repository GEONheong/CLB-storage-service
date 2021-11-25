#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifndef SWITCH_CASE_INIT
#define SWITCH_CASE_INIT
    #define SWITCH(X)   for (char* __switch_p__ = X, int __switch_next__=1 ; __switch_p__ ; __switch_p__=0, __switch_next__=1) {{
    #define CASE(X)         } if (!__switch_next__ || !(__switch_next__ = strcmp(__switch_p__, X))) {
    #define DEFAULT         } {
    #define END         }}
#endif

char *storageKind(char *ext){
    static char *storKind;
    SWITCH (ext)
        CASE("txt" || "doc" || "docx" || "html")
            strcpy(storKind,"docs");
            break;
        CASE("avi")
            strcpy(storKind,"video");
            break;
        CASE("mp3")
            strcpy(storKind,"sound");
            break;
        CASE("jpg")
            strcpy(storKind,"img");
            break;        
        DEFAULT
            printf("in DEFAULT\n");
    END

    return storKind;
}
