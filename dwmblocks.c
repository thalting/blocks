#include "blocks.h"
#include <X11/Xlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SIGPLUS SIGRTMIN
#define SIGMINUS SIGRTMIN
#define LENGTH(X) (sizeof(X) / sizeof(X[0]))
#define MIN(a, b) ((a < b) ? a : b)
#define STATUSLENGTH (LENGTH(blocks) * CMDLENGTH + 1)

void sighandler(int num);
void getcmds(int time);
void getsigcmds(unsigned int signal);
void setupsignals();
void sighandler(int signum);
int getstatus(char *str, char *last);
void statusloop();
void termhandler();
void setroot();

static Display *dpy;
static int screen;
static Window root;

static char statusbar[LENGTH(blocks)][CMDLENGTH] = {0};
static char statusstr[2][STATUSLENGTH];
static int statusContinue = 1;

// opens process *cmd and stores output in *output
void getcmd(const Block *block, char *output) {
    strcpy(output, block->icon);
    FILE *cmdf = popen(block->command, "r");
    if (!cmdf)
        return;
    int i = strlen(block->icon);
    fgets(output + i, CMDLENGTH - i - delimLen, cmdf);
    i = strlen(output);
    if (i == 0) {
        // return if block and command output are both empty
        pclose(cmdf);
        return;
    }
    // only chop off newline if one is present at the end
    i = output[i - 1] == '\n' ? i - 1 : i;
    if (delim[0] != '\0')
        strncpy(output + i, delim, delimLen);
    else
        output[i++] = '\0';
    pclose(cmdf);
}

void getcmds(int time) {
    const Block *current;
    for (unsigned int i = 0; i < LENGTH(blocks); i++) {
        current = blocks + i;
        if ((current->interval != 0 && time % current->interval == 0) ||
            time == -1)
            getcmd(current, statusbar[i]);
    }
}

void getsigcmds(unsigned int signal) {
    const Block *current;
    for (unsigned int i = 0; i < LENGTH(blocks); i++) {
        current = blocks + i;
        if (current->signal == signal)
            getcmd(current, statusbar[i]);
    }
}

void setupsignals() {
    for (unsigned int i = 0; i < LENGTH(blocks); i++) {
        if (blocks[i].signal > 0)
            signal(SIGMINUS + blocks[i].signal, sighandler);
    }
}

int getstatus(char *str, char *last) {
    strcpy(last, str);
    str[0] = '\0';
    for (unsigned int i = 0; i < LENGTH(blocks); i++)
        strcat(str, statusbar[i]);
    str[strlen(str) - strlen(delim)] = '\0';
    return strcmp(str, last); // 0 if they are the same
}

void setroot() {
    if (!getstatus(statusstr[0],
                   statusstr[1])) // Only set root if text has changed.
        return;
    XStoreName(dpy, root, statusstr[0]);
    XFlush(dpy);
}

void statusloop() {
    setupsignals();
    int i = 0;
    getcmds(-1);
    while (1) {
        getcmds(i++);
        setroot();
        if (!statusContinue)
            break;
        sleep(1);
    }
}

void sighandler(int signum) {
    getsigcmds(signum - SIGPLUS);
    setroot();
}

void termhandler() { statusContinue = 0; }

int main(int argc, char **argv) {
    for (int i = 0; i < argc; i++) { // Handle command line arguments
        if (!strcmp("-d", argv[i]))
            strncpy(delim, argv[++i], delimLen);
    }

    dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "dwmblocks: Failed to open display\n");
        return 0;
    }
    screen = DefaultScreen(dpy);
    root = RootWindow(dpy, screen);

    delimLen = MIN(delimLen, strlen(delim));
    delim[delimLen++] = '\0';
    signal(SIGTERM, termhandler);
    signal(SIGINT, termhandler);
    statusloop();
    XCloseDisplay(dpy);
    return 0;
}
