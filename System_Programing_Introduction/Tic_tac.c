#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define MAXPOS 9

char board[MAXPOS];
pid_t childpid;

/* parent receives child's choice */
char child_mark = ' ';

/* ---------- PARENT HANDLER ---------- */
void parent_handler(int sig)
{
    if(sig == SIGUSR1) child_mark = 'X';
    else child_mark = 'O';
}

/* ---------- CHILD HANDLERS ---------- */
void child_move(int sig)
{
    time_t t = time(NULL);
    if(t % 2 == 0)
        kill(getppid(), SIGUSR1);  // want X
    else
        kill(getppid(), SIGUSR2);  // want O
}

void child_over(int sig)
{
    _exit(0);
}

/* find empty box from r,r+1,... */
int findpos(int r)
{
    for(int i=0;i<MAXPOS;i++){
        int k = (r+i)%MAXPOS;
        if(board[k]==' ') return k;
    }
    return -1;
}

/* simple win-check */
int win(char m)
{
    int w[8][3]={{0,1,2},{3,4,5},{6,7,8},
                 {0,3,6},{1,4,7},{2,5,8},
                 {0,4,8},{2,4,6}};
    for(int i=0;i<8;i++)
        if(board[w[i][0]]==m && board[w[i][1]]==m && board[w[i][2]]==m)
            return 1;
    return 0;
}

int full()
{
    for(int i=0;i<MAXPOS;i++)
        if(board[i]==' ') return 0;
    return 1;
}

int main()
{
    for(int i=0;i<MAXPOS;i++) board[i]=' ';

    srand(time(NULL));

    struct sigaction sa;
    sa.sa_handler = parent_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags=0;

    sigaction(SIGUSR1,&sa,NULL);
    sigaction(SIGUSR2,&sa,NULL);

    childpid = fork();

    if(childpid==0){
        struct sigaction c1,c2;
        c1.sa_handler = child_move;
        sigemptyset(&c1.sa_mask);
        c1.sa_flags=0;

        c2.sa_handler = child_over;
        sigemptyset(&c2.sa_mask);
        c2.sa_flags=0;

        sigaction(SIGUSR1,&c1,NULL);
        sigaction(SIGUSR2,&c2,NULL);

        while(1) pause();
    }

    /* --------- Parent first move (P1,P2) --------- */
    int r = rand()%MAXPOS;
    time_t t = time(NULL);
    char m = (t%2==0)?'X':'O';
    int p = findpos(r);
    board[p] = m;
    printf("Parent placed %c at %d\n",m,p);

    /* ask child to play */
    kill(childpid, SIGUSR1);

    while(1)
    {
        pause();   // wait for SIGUSR1/SIGUSR2 from child

        /* child_mark now contains X or O */
        int r2 = rand()%MAXPOS;
        int pos = findpos(r2);
        if(pos==-1) break;

        board[pos] = child_mark;
        printf("Child move mark %c at %d\n",child_mark,pos);

        if(win(child_mark)){
            printf("Child wins!\n");
            kill(childpid, SIGUSR2);
            break;
        }
        if(full()){
            printf("Draw\n");
            kill(childpid, SIGUSR2);
            break;
        }

        /* parent next move (P1,P2) */
        r = rand()%MAXPOS;
        t = time(NULL);
        m = (t%2==0)?'X':'O';
        p = findpos(r);
        board[p] = m;
        printf("Parent placed %c at %d\n",m,p);

        if(win(m)){
            printf("Parent wins!\n");
            kill(childpid, SIGUSR2);
            break;
        }
        if(full()){
            printf("Draw\n");
            kill(childpid, SIGUSR2);
            break;
        }

        /* ask child again */
        kill(childpid, SIGUSR1);
    }

    return 0;
}
