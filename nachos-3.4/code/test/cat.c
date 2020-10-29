#include "syscall.h"
#define MAX_LENGTH 32

int main()
{
	int fileId;
	int fileSize;
	char c; 
	char fileName[MAX_LENGTH];
	int i;

	PrintString("\n\t\t\t-----HIEN THI NOI DUNG FILE-----\n\n");
	PrintString(" - Nhap ten file can doc: ");
	
	//doc vao ten file
	ReadString(fileName, MAX_LENGTH);
	
	fileId= Open(fileName, 1); //ham Open de mo file 
	
	if (fileId != -1) //Kiem tra co loi khong
	{
		// do dai noi dung (fileSize)
		fileSize = Seek(-1, fileId);
		// dau tap tin
		Seek(0, fileId);
		
		PrintString(" -> Noi dung file:\n");
		for (i=0; i < fileSize; i++)
		{
			Read(&c, 1, fileId); //doc tung ki tu noi dung file
			PrintChar(c); //in tung ki tu ra man hinh
		}
		Close(fileId); //
	}
	else
	{
		PrintString(" -> Mo file khong thanh cong!!\n\n");
	}
	return 0;
}
