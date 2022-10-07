typedef struct {
    const char *icon;
    const char *command;
    unsigned int interval;
    unsigned int signal;
} Block;

// Modify this file to change what commands output to your statusbar, and
// recompile using the make command.
static const Block blocks[] = {
    /*Icon Command Update Interval Update Signal*/
    {"", "echo", 0, 0},
    {"", "mprisst", 1, 0},
    {"", "volst", 0, 10},
    {"", "datest", 5, 0},
    {"", "echo", 0, 0},
};

// sets delimeter between status commands. NULL character ('\0') means no
// delimeter.
static char delim[] = " ┃ ";
static unsigned int delimLen = 5;

#define CMDLENGTH 50
