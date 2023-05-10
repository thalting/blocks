# blocks
Modular status bar for %anything% written in c.

# usage
To use blocks first run 'make' and then install it with 'sudo make install'.
After that you can put blocks in your xinitrc or other startup script to have it start with dwm or other window manager.

# modifying blocks
The statusbar is made from text output from commandline programs.
Blocks are added and removed by editing the blocks.h header file.
By default the blocks.h header file is created the first time you run make which copies the default config from blocks.def.h.
This is so you can edit your status bar commands and they will not get overwritten in a future update.
