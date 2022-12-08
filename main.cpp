#include "common.h"


Process_Operation *MyClass;

VOID	UnloadMyDriver(
	_In_	PDRIVER_OBJECT	pDriverObject
)
{
	UNREFERENCED_PARAMETER(pDriverObject);

	//MyClass->ResumeProcess(5220);

	delete MyClass;

	KdPrint(("Unload...\n"));
}


EXTERN_C NTSTATUS DriverEntry(
	_In_	PDRIVER_OBJECT	pDriverObject,
	_In_	PUNICODE_STRING	pRegistryPath
)
{
	UNREFERENCED_PARAMETER(pRegistryPath);

	MyClass = new Process_Operation;

	//MyClass->ScanningProcessInformation();

	//MyClass->SuspendProcess(5220);

	//MyClass->KillProcess(6084);

	MyClass->ZeroProcessMemory("calc.exe");

	pDriverObject->DriverUnload = UnloadMyDriver;
	
	return STATUS_SUCCESS;
}
