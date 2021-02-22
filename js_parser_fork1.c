#include <stdio.h>
#include <stdlib.h>

#define DOUBLE_QUOTE 34
#define SINGLE_QUOTE 39
#define STAR 42
#define FORWARD_SLASH 47
#define BACKSLASH 92
#define BACKTICK 96
#define CARET 94
#define PRECENT 37
#define NEWLINE '\n'

int in_single_line = 0;
int in_multi_line = 0;
int in_string = 0;
int in_tick = 0;
int slashes = 0;
int stars = 0;
int new_tick = 1;
int backslashes = 0;


int in_any_string();
int in_any_comment();
int in_code();
void handle_quote();
void handle_tick();
void handle_star();
void handle_slash();
void handle_newline();
void reset_slashes_and_stars();
void handle_char(char c);
void read_from_file(char * file_name);
void handle_backslash();
int main(int argc, char * argv[]);

int in_any_string(){
    return in_string || in_tick;
}

int in_any_comment(){
    return in_multi_line || in_single_line;
}

//returns 1 if in code
//0 if in string or comment
int in_code(){
    return !in_any_comment() && !in_any_string();
}

void handle_string_char(int * in_tick_type){
    if(in_code()){
        *(in_tick_type) = 1;
    }else if(*(in_tick_type)){
        *(in_tick_type) = 0;
        new_tick = 1;
        printf("\n");
    }
}

void handle_star(){
    if(in_code() && slashes == 1){
        in_multi_line = 1;
    }
    stars = 1;
}

//only handle if in code context
//not in quote
void handle_slash(){
    if(in_code() && slashes == 1){
        in_single_line = 1;
    }else if(in_multi_line && stars == 1){
        in_multi_line = 0;
    }
    slashes = 1;
}

//anytime a \n is created it cant be in a single line
void handle_newline(){
    if(in_single_line){
        in_single_line = 0;
    }
}

void reset_slashes_and_stars(){
    slashes = 0;
    stars = 0;
}

void handle_backslash(){

    if(in_any_string()){
        backslashes = 1;
    }

}
void change_state(char c){

    switch(c){
        case DOUBLE_QUOTE:
            //handle_quote();
            handle_string_char(&in_string);
            reset_slashes_and_stars();
            break;
        case SINGLE_QUOTE:// 39 = ' 
            //handle_tick(c);
            handle_string_char(&in_tick);
            reset_slashes_and_stars();
            break;
        case STAR:
            handle_star();
            break;
        case FORWARD_SLASH:
            handle_slash();
            break;
        case NEWLINE:
            handle_newline();
            reset_slashes_and_stars();
            break;
        case BACKSLASH: /* 92 = \  */
            handle_backslash();
            reset_slashes_and_stars();
        default:
            reset_slashes_and_stars();
    }

}

void handle_char(char c){

    if(!backslashes){
        change_state(c);
    }else{
        backslashes = 0;
    }

    if(in_any_string()){
        if(!new_tick){
            printf("%c", c);
        }else{
            new_tick = 0;
        }
    }

}


void read_from_file(char * file_name){
  
    register int c;
    FILE *file;

    file = fopen(file_name, "r");

    if(file == 0){
        printf("file error: js_parser\n");
        fclose(file);
        exit(0);
    }

    while((c = getc(file)) != EOF){
        handle_char(c);
    }
    
    fclose(file);

}

int main(int argc, char * argv[]){

    char * file_name;

    if(argc != 2){
        printf("js_parser.c: usage: <file>\n" );
        exit(0);
    }

    file_name = argv[1];
    read_from_file(file_name);
   
}
