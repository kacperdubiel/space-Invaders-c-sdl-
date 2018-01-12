#ifndef RANKING_H_INCLUDED
#define RANKING_H_INCLUDED

#define RANKING_FILE "ranking.txt"
#define RANKING_TOP 5
#define RANKING_NAME_CHARS 11 //1 na koniec stringa
#define FORBIDDEN_SYMBOLS 20

typedef struct _Ranking{
    int score;
    char name[RANKING_NAME_CHARS];
} Ranking;

extern Ranking ranking[RANKING_TOP];
extern char forbidden_symbols[FORBIDDEN_SYMBOLS][5];

void readRanking();
void resetRanking();
void sortRanking();
void saveRanking();

//debug
void printRanking();

#endif // RANKING_H_INCLUDED
