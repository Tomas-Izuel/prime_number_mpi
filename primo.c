#include <stdbool.h>

bool es_primo(int num){
    if (num < 2){
        return false;
    }
    if (num < 4){
        return true;
    }
    for(int i = 2; i < num; i++){
        if (num % i == 0){
            return false;
        }
    }
    return true;
}