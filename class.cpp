#include "common.h"

_Use_decl_annotations_
PEPROCESS
Process_Operation::GetEprocessByPid(
	_In_ HANDLE pid
)
{
	//根据PID 返回PEPROCESS
	PEPROCESS pEpro = NULL;

	NTSTATUS status = STATUS_UNSUCCESSFUL;

	status = PsLookupProcessByProcessId(pid, &pEpro);

	if (NT_SUCCESS(status))
		return pEpro;

	return NULL;
}


_Use_decl_annotations_
HANDLE 
Process_Operation::GetPidByProcessName(
	_In_ PCHAR ProcessName
)
{
	PEPROCESS pCurrentEprocess = NULL;

	HANDLE pid = 0;

	for (int i = 0; i < 2147483647; i += 4)
	{
		pCurrentEprocess = GetEprocessByPid((HANDLE)i);

		if (pCurrentEprocess != NULL)
		{

			pid = PsGetProcessId(pCurrentEprocess);

			if (strstr((PCCHAR)PsGetProcessImageFileName(pCurrentEprocess), ProcessName) != NULL)
			{
				ObDereferenceObject(pCurrentEprocess); //解引用

				KdPrint(("This <%s> Process ID is <%d>!\r\n", ProcessName, (LONG)pid));

				return pid;
			}
			ObDereferenceObject(pCurrentEprocess); //解引用
		}
	}

	KdPrint(("Not Found %s Process!\r\n",ProcessName));

	return (HANDLE)-1;  // 失败返回 0xffffffff
}


/*
*   遍历进程操作函数
*/
_Use_decl_annotations_ VOID Process_Operation::ScanningProcessInformation()
{
	PEPROCESS pCurrentEprocess = NULL;

	KdPrint(("Start Read Process Information!\n"));

	for (LONG i = 0; i < 0x80000000; i += 4)
	{

		pCurrentEprocess = GetEprocessByPid((HANDLE)i);

		if (pCurrentEprocess != NULL)
		{

			KdPrint(("进程名字为: %s 进程PID = %lld 进程的父Pid = %lld\r\n",
				PsGetProcessImageFileName(pCurrentEprocess),
				(ULONG64)PsGetProcessId(pCurrentEprocess),
				(ULONG64)PsGetProcessInheritedFromUniqueProcessId(pCurrentEprocess)));

			ObDereferenceObject(pCurrentEprocess);   //解引用

		}
	}

	KdPrint(("Read Process Information End!\n"));

}


/*
*
*   挂起进程函数
*
*	参数1： 目标进程PID
*
*/
_Use_decl_annotations_ NTSTATUS Process_Operation::SuspendProcess(
	_In_ ULONG pid
)
{
	PEPROCESS pCurrentEprocess = NULL;

	NTSTATUS status = STATUS_SUCCESS;

	pCurrentEprocess = GetEprocessByPid(reinterpret_cast<HANDLE>(pid));

	if (pCurrentEprocess != NULL)
	{
		status = PsSuspendProcess(pCurrentEprocess);

		if (!NT_SUCCESS(status))
		{
			KdPrint(("Suspend Failed!\n"));

			return status;
		}

		KdPrint(("Suspend %s process success!\r\n",
			PsGetProcessImageFileName(
				this->GetEprocessByPid(reinterpret_cast<HANDLE>(pid))
			)
			));

		ObDereferenceObject(pCurrentEprocess);

	}


	return status;

}


/*
*   恢复进程函数
*
*	参数1： 目标进程PID
*/
_Use_decl_annotations_ NTSTATUS Process_Operation::ResumeProcess(
	_In_ ULONG pid
)
{
	PEPROCESS	pCurrentEprocess = NULL;

	NTSTATUS	status = STATUS_SUCCESS;

	pCurrentEprocess = GetEprocessByPid(reinterpret_cast<HANDLE>(pid));

	if (pCurrentEprocess != NULL)
	{

		status = PsResumeProcess(pCurrentEprocess);

		if (!NT_SUCCESS(status))
		{
			KdPrint(("Resume Failed!\n"));

			return status;
		}

		KdPrint(("Resume Process %s Success!\r\n",
			PsGetProcessImageFileName(
				this->GetEprocessByPid(reinterpret_cast<HANDLE>(pid))
			)
			));

		ObDereferenceObject(pCurrentEprocess);

	}

	return status;
}


_Use_decl_annotations_ NTSTATUS Process_Operation::KillProcess(
	_In_ ULONG pid
)
{
	NTSTATUS status = STATUS_SUCCESS;

	HANDLE ProcessHandle = NULL;

	OBJECT_ATTRIBUTES obj;

	CLIENT_ID cid = { 0 };

	InitializeObjectAttributes(&obj, NULL, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);

	cid.UniqueProcess = reinterpret_cast<HANDLE>(pid);

	cid.UniqueThread = 0;

	status = ZwOpenProcess(&ProcessHandle, GENERIC_ALL, &obj, &cid);

	if (NT_SUCCESS(status))
	{

		KdPrint(("Kill Process %s!\r\n",
			PsGetProcessImageFileName(
				this->GetEprocessByPid(reinterpret_cast<HANDLE>(pid))
			)
			));

		ZwTerminateProcess(ProcessHandle, 0);

		ZwClose(ProcessHandle);

	}

	ZwClose(ProcessHandle);

	return status;

}


_Use_decl_annotations_ NTSTATUS Process_Operation::ZeroProcessMemory(
	_In_ PCHAR ProcessName
)
{
	HANDLE pid = GetPidByProcessName(ProcessName);

	PEPROCESS proc = NULL;

	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;

	PKAPC_STATE pApcState = NULL;

	NTSTATUS status = STATUS_SUCCESS;

	PsLookupProcessByProcessId((HANDLE)pid, &proc);

	if (proc == 0)
	{

		KdPrint(("Look up process id fail!\n"));

		return STATUS_UNSUCCESSFUL;

	}

	//KeAttachProcess(proc);
	//KeDetachProcess()  等都已经过时.所以使用新的
	pApcState = (PKAPC_STATE)ExAllocatePoolWithTag(NonPagedPool, sizeof(PKAPC_STATE), '1111');

	if (NULL == pApcState)
	{
		ObDereferenceObject(proc);

		return STATUS_UNSUCCESSFUL;
	}
	__try 
	{
		KeStackAttachProcess(proc, pApcState);

		//KeAttachProcess(proc);

		for (int i = 0x10000; i < 0x20000000; i += PAGE_SIZE)
		{
			__try
			{
				RtlZeroMemory((PVOID)i, PAGE_SIZE);
			}
			__except (1)
			{
				;  //内部处理异常
			}
		}
		KeUnstackDetachProcess(pApcState);

		//KeDetachProcess();

		ObDereferenceObject(proc);

		return status;
	}
	__except (1)
	{
		KdPrint(("KillProcess By Zero Memory fail!\r\n"));

		KeUnstackDetachProcess(pApcState);

		ObDereferenceObject(proc);
	}
	return STATUS_UNSUCCESSFUL;
}


_IRQL_requires_max_(DISPATCH_LEVEL)
void* __cdecl operator new(size_t size) {
	if (size == 0) {
		size = 1;
	}
	return ExAllocatePoolWithTag(NonPagedPool, size, 'newC');
}


_IRQL_requires_max_(DISPATCH_LEVEL)
void __cdecl operator delete(void* p, SIZE_T size) {
	UNREFERENCED_PARAMETER(size);
	if (p) {
		ExFreePoolWithTag(p, 'delC');
	}
}
