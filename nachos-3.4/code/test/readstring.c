#include <syscall.h>

int main()
{
	char str[10];
	ReadString(str, 10);
	PrintString(str);
}
