#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <dirent.h>
#include <string.h>

void bomb(char *text);
void listFiles(void);
void userProcess(void);

static int maxX, maxY, padRowPosition, padRows;
static WINDOW *pad;

int main()
{
    char text[] = "Greetings from ncurses commander\n";

    initscr();
    if (!has_colors())
        bomb("Unable to start colors\n");

    start_color();
    keypad(stdscr, true);
    scrollok(stdscr, true);

    getmaxyx(stdscr, maxY, maxX);
    addstr(text);
    refresh();
    getchar();
    listFiles();
    userProcess();
    endwin();
    return 0;
}

void bomb(char *text)
{
    addstr(text);
    addstr("Press any button\n");
    refresh();
    getchar();
    endwin();
    exit(1);
}

void listFiles(void)
{
    char cwd[PATH_MAX];
    DIR *dirp;
    struct dirent *entry;
    int filesNum = 0;
    int dirsNum = 0;
    int maxNameLen = 0;

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        dirp = opendir(cwd);
        if (dirp != NULL) {
            while((entry = readdir(dirp)) != NULL) {
                if (entry->d_type == DT_REG)
                    ++filesNum;
                else if (entry->d_type == DT_DIR)
                    ++dirsNum;
                if ((int)strlen(entry->d_name) > maxNameLen)
                    maxNameLen = (int)(strlen(entry->d_name));
            }
        }
    } else {
        bomb("Unable to find directory path");
    }

    pad = newpad(filesNum + dirsNum, maxNameLen);
    padRows = filesNum + dirsNum;
    if (pad == NULL)
        bomb("Unable to create pad\n");

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        dirp = opendir(cwd);
        if (dirp != NULL) {
            while((entry = readdir(dirp)) != NULL) {
                waddstr(pad, entry->d_name);
                waddch(pad, '\n');
            }
        }
    }

    prefresh(pad, 0, 0, 0, 0, maxY - 1, pad->_maxx);
}

void userProcess(void)
{
    int ch;

    while ((ch = getch()) != KEY_F(12)) {
        switch (ch) {
        case KEY_UP:
            ++padRowPosition;
            if ((padRows - padRowPosition) > maxY) {
                scrl(1);
                prefresh(pad, padRowPosition, 0, 0, 0, padRowPosition + 10, pad->_maxx);
            } else
                --padRowPosition;
            break;
        case KEY_DOWN:
            --padRowPosition;
            if (padRowPosition >=0) {
                scrl(-1);
                prefresh(pad, padRowPosition, 0, 0, 0, padRowPosition + 10, pad->_maxx);
            } else
                ++padRowPosition;
            break;
        default:
            break;
        }
    }
}
