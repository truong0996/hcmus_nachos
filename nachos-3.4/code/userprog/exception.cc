// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------


// Doi thanh ghi Program counter cua he thong ve sau 4 byte de tiep tuc nap lenh
void IncreasePC()
{
	int counter = machine->ReadRegister(PCReg);
   	machine->WriteRegister(PrevPCReg, counter);
    	counter = machine->ReadRegister(NextPCReg);
    	machine->WriteRegister(PCReg, counter);
   	machine->WriteRegister(NextPCReg, counter + 4);
}


// Input: - User space address (int)
// - Limit of buffer (int)
// Output:- Buffer (char*)
// Purpose: Copy buffer from User memory space to System memory space
char* User2System(int virtAddr,int limit)
{
 int i;// index
 int oneChar;
 char* kernelBuf = NULL;
 kernelBuf = new char[limit +1];//need for terminal string
 if (kernelBuf == NULL)
 	return kernelBuf;
 memset(kernelBuf,0,limit+1);
 //printf("\n Filename u2s:");
 for (i = 0 ; i < limit ;i++)
 {
	 machine->ReadMem(virtAddr+i,1,&oneChar);
	 kernelBuf[i] = (char)oneChar;
	 //printf("%c",kernelBuf[i]);
	 if (oneChar == 0)
	 break;
 }
 return kernelBuf;
}

// Input: - User space address (int)
// - Limit of buffer (int)
// - Buffer (char[])
// Output:- Number of bytes copied (int)
// Purpose: Copy buffer from System memory space to User memory space
int System2User(int virtAddr,int len,char* buffer)
{
 if (len < 0) return -1;
 if (len == 0)return len;
 int i = 0;
 int oneChar = 0 ;
 do{
	 oneChar= (int) buffer[i];
	 machine->WriteMem(virtAddr+i,1,oneChar);
	 i ++;
 }while(i < len && oneChar != 0);
 return i;
}


void
ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);
	switch (which)
	{
		case SyscallException:
		  switch (type)
		  {
		    case SC_Halt:
			DEBUG('a', "Shutdown, initiated by user program.\n");
			interrupt->Halt();
			break;

		    case SC_ReadChar:
		    {
			//Input: Khong co
			//Output: Duy nhat 1 ky tu (char)
			//Cong dung: Doc mot ky tu tu nguoi dung nhap
			int maxBytes = 255;
			char* buffer = new char[255];
			int numBytes = gSynchConsole->Read(buffer, maxBytes);

			if(numBytes > 1) //Neu nhap nhieu hon 1 ky tu thi khong hop le
			{
			  printf("Chi duoc nhap duy nhat 1 ky tu!");
			  DEBUG('a', "\nERROR: Chi duoc nhap duy nhat 1 ky tu!");
			  machine->WriteRegister(2, 0);
			}
			else if(numBytes == 0) //Ky tu rong
			{
			  printf("Ky tu rong!");
			  DEBUG('a', "\nERROR: Ky tu rong!");
			  machine->WriteRegister(2, 0);
			}
			else
			{
			  //Chuoi vua lay co dung 1 ky tu, lay ky tu o index = 0, return vao thanh ghi R2
			  char c = buffer[0];
			  machine->WriteRegister(2, c);
			}

			delete buffer;
			IncreasePC();
			//return;
			break;
		}

		      
 		    default:
		      printf("\n Unexpected user mode exception (%d %d)", which, type);
		      interrupt->Halt();
		  }


		case NoException:
			return;
		case PageFaultException:
			DEBUG('a', "\n No valid translation found");
			interrupt->Halt();
			break;
		case ReadOnlyException:
			DEBUG('a', "\n Write attempted to page marked read-only");
			interrupt->Halt();
			break;
		case BusErrorException:
			DEBUG('a', "\n Translation resulted invalid physical address");
			interrupt->Halt();
			break;
		case AddressErrorException:
			DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space");
			interrupt->Halt();
			break;
		case OverflowException:
			DEBUG('a', "\nInteger overflow in add or sub.");
			interrupt->Halt();
			break;
		case IllegalInstrException:
			DEBUG('a', "\n Unimplemented or reserved instr.");
			interrupt->Halt();
			break;
		case NumExceptionTypes:
			DEBUG('a', "\n Number exception types");
			interrupt->Halt();
			break;
	}
}
