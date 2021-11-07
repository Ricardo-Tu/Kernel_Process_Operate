#pragma once


class	Process_Operation
{
public:
	Process_Operation()
	{
	}
	~Process_Operation()
	{
	}

	VOID ScanningProcessInformation();	// 遍历进程

	NTSTATUS SuspendProcess(_In_ ULONG pid);	// 挂起进程

	NTSTATUS ResumeProcess(_In_ ULONG pid);	// 恢复进程

	NTSTATUS KillProcess(_In_ ULONG pid);	// 结束进程

	NTSTATUS Process_Operation::ZeroProcessMemory(_In_ PCHAR ProcessName); // 清空进程内存杀进程

private:

	PEPROCESS
	GetEprocessByPid(
		_In_ HANDLE pid
		);				// 通过进程PID获取进程 Eprocess结构体

	HANDLE	 
	GetPidByProcessName(
		_In_ PCHAR ProcessName
	);					// 通过进程PID获取进程名称

};




EXTERN_C
NTKERNELAPI
PUCHAR
PsGetProcessImageFileName(
	IN PEPROCESS Process
); //未公开的进行导出即可


EXTERN_C
NTKERNELAPI
HANDLE
PsGetProcessInheritedFromUniqueProcessId(
	IN PEPROCESS Process
);//未公开进行导出


EXTERN_C
NTSTATUS
PsLookupProcessByProcessId(
	_In_  HANDLE    ProcessId,
	_Out_ PEPROCESS* Process
);//未公开进行导出


EXTERN_C
NTKERNELAPI
NTSTATUS
PsSuspendProcess(
	PEPROCESS proc
);  //暂停进程

EXTERN_C
NTKERNELAPI
NTSTATUS
PsResumeProcess(
	PEPROCESS proc
);   //恢复进程



EXTERN_C
NTKERNELAPI 
VOID NTAPI 
KeAttachProcess(
	PEPROCESS Process
);

EXTERN_C
NTKERNELAPI 
VOID NTAPI 
KeDetachProcess();





_IRQL_requires_max_(DISPATCH_LEVEL)
void* __cdecl operator new(size_t size);

_IRQL_requires_max_(DISPATCH_LEVEL)
void __cdecl operator delete(void* p, SIZE_T size);


























