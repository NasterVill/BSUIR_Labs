#define TRUE 1
#define DELAY 5
#define WIDTH 100
#define HEIGHT 25
#define CHILD_OFFSET_ROW 10
#define PARENT_OFFSET_ROW 8

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ncurses.h>
#include <time.h>

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_window);

int main(int argc, char *argv[])
{
    int child_status = -1;
    char symbol;
    pid_t pid;

    initscr();
    curs_set(0);

    noecho();
    halfdelay(DELAY);

    pid = fork();
    switch(pid)
    {
    case -1:
    {
        printw("Erorr while creating child process\n");
        refresh();

        flushinp();
        getch();

        exit(EXIT_FAILURE);
    }
    case 0:
    {
        WINDOW* childWindow = create_newwin(HEIGHT, WIDTH, 0, 0);

        while(symbol != '\n')
        {
            time_t seconds = time(NULL);
            struct tm* timeinfo = localtime(&seconds);

            wmove(childWindow, CHILD_OFFSET_ROW, 0);
            wprintw(childWindow, "Current Date and Time by Child_: %s", asctime(timeinfo));
            //wprintw(childWindow, " : %d", getpid());
            wrefresh(childWindow);

            flushinp();
            symbol = getch();
        }

        destroy_win(childWindow);

        exit(EXIT_SUCCESS);
        break;
    }
    default:
    {
        while(TRUE)
        {
            time_t seconds = time(NULL);
            struct tm* timeinfo = localtime(&seconds);

            move(PARENT_OFFSET_ROW, 0);
            printw("Current Date and Time by Parent: %s", asctime(timeinfo));
            refresh();

            if(waitpid(pid, &child_status, WNOHANG) > 0)
                break;

            napms(DELAY);
        }

        clear();
        if (WIFEXITED(child_status))
        {
            printw("Chiled exited with status of %d\n", WEXITSTATUS(child_status));
            printw("Press ENTER key, to continue...");
            refresh();
        }
        else
        {
            printw("Chiled exited with an error");
            refresh();
        }

        nocbreak();
        flushinp();
        getch();

        break;
    }
    }

   endwin();

   return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_window;

    local_window = newwin(height, width, starty, startx);

    box(local_window, 0 , 0);		// 0, 0 gives default characters
                                // for the vertical and horizontal
                                // lines

    wrefresh(local_window);		// Show that box

    return local_window;
}

void destroy_win(WINDOW *local_window)
{
    /* box(local_win, ' ', ' '); : This won't produce the desired
     * result of erasing the window. It will leave it's four corners
     * and so an ugly remnant of window.
     */

    wborder(local_window, ' ', ' ', ' ',' ',' ',' ',' ',' ');

    /* The parameters taken are
     * 1. win: the window on which to operate
     * 2. ls: character to be used for the left side of the window
     * 3. rs: character to be used for the right side of the window
     * 4. ts: character to be used for the top side of the window
     * 5. bs: character to be used for the bottom side of the window
     * 6. tl: character to be used for the top left corner of the window
     * 7. tr: character to be used for the top right corner of the window
     * 8. bl: character to be used for the bottom left corner of the window
     * 9. br: character to be used for the bottom right corner of the window
     */

    wrefresh(local_window);
    delwin(local_window);
}
