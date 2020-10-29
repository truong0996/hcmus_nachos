#include "syscall.h"
#define MAX_LENGTH 32

int main()
{
	int srcFileId;
	int destFileId;
	int fileSize;
	int i; 
	char c;
	char src[MAX_LENGTH];
	char dest[MAX_LENGTH];
	
	PrintString("\n\t\t\t-----SAO CHEP FILE-----\n\n");
	PrintString(" - Nhap ten file nguon: ");
	ReadString(src, MAX_LENGTH);// doc vao ten file nguon
	
	PrintString(" - Nhap ten file dich: ");
	ReadString(dest, MAX_LENGTH); // doc vao ten file dich
	srcFileId = Open(src, 1); // de mo file nguon
	
	if (srcFileId != -1) //Kiem tra mo file thanh cong
	{
		destFileId = CreateFile(dest);
		Close(destFileId);
		
		destFileId = Open(dest, 0);
		if (destFileId != -1) //Kiem tra mo file thanh cong
		{
			fileSize = Seek(-1, srcFileId);
			
			Seek(0, srcFileId); 
			Seek(0, destFileId); 
			
			for (i=0; i < fileSize; i++) 
			{
				Read(&c, 1, srcFileId); 
				Write(&c, 1, destFileId);
			}
			
			PrintString(" -> Copy thanh cong.\n\n");
			Close(destFileId);
		}
		else
		{
			PrintString(" -> Tao file dich khong thanh cong!!!\n\n");
		}
		
		Close(srcFileId);
	}
	else
	{
		PrintString("Loi khi mo file");
	}
	
	return 0;
}
