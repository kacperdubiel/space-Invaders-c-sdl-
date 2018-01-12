#include "ranking.h"

#include <stdio.h>
#include <string.h>

Ranking ranking[RANKING_TOP];
char forbidden_symbols[FORBIDDEN_SYMBOLS][5] = {
    " ", "ę", "ó", "ł", "ś", "ą", "ż", "ź", "ć", "ń",
    "€", "Ę", "Ó", "Ł", "Ś", "Ą", "Ż", "Ź", "Ć", "Ń"
};

void readRanking(){
    FILE *file = fopen(RANKING_FILE, "r");

    if(file){
        printf("READING FILE: %s\n", RANKING_FILE);
        for(int i = 0; i<RANKING_TOP;i++){
            char scanName[11];
            if(fscanf(file, "%d %10s", &ranking[i].score, scanName) == EOF){
                ranking[i].score = 0;
                strcpy(scanName, "...");
            }

            strcpy(ranking[i].name, scanName);
            printf("READING %d LINE FROM %s: %d %s\n", i+1, RANKING_FILE, ranking[i].score, ranking[i].name);
        }
        fclose(file);
    }else{
        printf("ERROR: FILE NOT FOUND!\n");
        resetRanking();
    }

}

void resetRanking(){
    FILE *file = fopen(RANKING_FILE, "w");
    if(file){
        printf("CREATED FILE: %s\n", RANKING_FILE);
        for(int i = 0; i<RANKING_TOP;i++)
            fprintf(file, "%d %s\n", 0, "...");
        fclose(file);
    }
    readRanking();
}

void sortRanking(){
    for(int i = 0; i<RANKING_TOP-1; i++){
        int flag = 0;
        for(int j = 0; j<RANKING_TOP-1; j++){
            if(ranking[j].score<ranking[j+1].score){
                Ranking temp = ranking[j];
                ranking[j] = ranking[j+1];
                ranking[j+1] = temp;
                flag++;
            }
        }
        if(!flag)
            break;
    }
    printf("RANKING SORTED!\n");
}

void saveRanking(){
    FILE *file = fopen(RANKING_FILE, "w");
    if(file){
        for(int i = 0; i<RANKING_TOP;i++){
            fprintf(file, "%d %s\n", ranking[i].score, ranking[i].name);
        }
        fclose(file);
        printf("RANKING SAVED TO: %s\n", RANKING_FILE);
    }
}

//debug
void printRanking(){
    printf("RANKING:\n");
    for(int i = 0; i<RANKING_TOP; i++){
        printf("[%d]: %d %s\n", i+1, ranking[i].score, ranking[i].name);
    }
}


