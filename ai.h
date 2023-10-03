#ifndef AI_H
#define AI_H

/* ai.c */
void destrand( entity * );
void desthome( entity * );
void deststraight( entity * );
int makemonst( int, char, int, int, int, int );
void spawnbullet( entity * );
void killmonster( entity * );
void biteattack( entity *, entity * );
void vacattack( entity *, entity * );
int clearshot( entity *, entity * );
void findtarg( entity *, int );

#endif /* AI_H */
