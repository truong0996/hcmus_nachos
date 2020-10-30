#include "syscall.h"
#define MAX_LENGTH 32

int main()
{
	char src[MAX_LENGTH];
	PrintString("\n\t\t\t-----SAO CHEP FILE-----\n\n");
	PrintString(" - Nhap ten file nguon: ");
	ReadString(src, MAX_LENGTH);// doc vao ten file nguon
	Create(src);
	PrintString(" - Tao file thanh cong\n\n");
	
}
