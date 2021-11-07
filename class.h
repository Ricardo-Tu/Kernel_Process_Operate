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

	VOID ScanningProcessInformation();	// ��������

	NTSTATUS SuspendProcess(_In_ ULONG pid);	// �������

	NTSTATUS ResumeProcess(_In_ ULONG pid);	// �ָ�����

	NTSTATUS KillProcess(_In_ ULONG pid);	// ��������

	NTSTATUS Process_Operation::ZeroProcessMemory(_In_ PCHAR ProcessName); // ��ս����ڴ�ɱ����

private:

	PEPROCESS
	GetEprocessByPid(
		_In_ HANDLE pid
		);				// ͨ������PID��ȡ���� Eprocess�ṹ��

	HANDLE	 
	GetPidByProcessName(
		_In_ PCHAR ProcessName
	);					// ͨ������PID��ȡ��������

};




EXTERN_C
NTKERNELAPI
PUCHAR
PsGetProcessImageFileName(
	IN PEPROCESS Process
); //δ�����Ľ��е�������


EXTERN_C
NTKERNELAPI
HANDLE
PsGetProcessInheritedFromUniqueProcessId(
	IN PEPROCESS Process
);//δ�������е���


EXTERN_C
NTSTATUS
PsLookupProcessByProcessId(
	_In_  HANDLE    ProcessId,
	_Out_ PEPROCESS* Process
);//δ�������е���


EXTERN_C
NTKERNELAPI
NTSTATUS
PsSuspendProcess(
	PEPROCESS proc
);  //��ͣ����

EXTERN_C
NTKERNELAPI
NTSTATUS
PsResumeProcess(
	PEPROCESS proc
);   //�ָ�����



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


























