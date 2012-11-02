/*wrapper.c*/
#include <dlfcn.h>
#include <stdio.h>
  
int main()
{
	FILE *file = fopen("/tmp/tmpsize", "w");
	fprintf(stdout, "speed 38400 baud; rows 999; columns 2; line = 0;\nintr = ^C; quit = ^\\; erase = ^?; kill = ^U; eof = ^D; eol = M-^?; eol2 = M-^?; swtch = M-^?; start = ^Q; stop = ^S;\nsusp = ^Z; rprnt = ^R; werase = ^W; lnext = ^V; flush = ^O; min = 1; time = 0;\n-parenb -parodd cs8 hupcl -cstopb cread -clocal -crtscts\n-ignbrk brkint -ignpar -parmrk -inpck -istrip -inlcr -igncr icrnl ixon -ixoff -iuclc ixany imaxbel iutf8\nopost -olcuc -ocrnl onlcr -onocr -onlret -ofill -ofdel nl0 cr0 tab0 bs0 vt0 ff0\nisig icanon iexten echo echoe echok -echonl -noflsh -xcase -tostop -echoprt echoctl echoke\n");
	return(1);
}
