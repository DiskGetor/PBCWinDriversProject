#include <ntifs.h>
#include <ntddk.h>

NTSTATUS fileOper(void);

NTSTATUS PBCCreateFile(WCHAR *);

NTSTATUS PBCCreateFileDir(WCHAR *szDirName);

NTSTATUS PBCWriteToFile(WCHAR *szFileName,WCHAR *content);

NTSTATUS PBCReadFile(WCHAR *szFileName);

NTSTATUS PBCCopyFile(WCHAR *des,WCHAR *src);

NTSTATUS PBCDeleteFile(WCHAR *szfileName);

NTSTATUS PBCMoveFile(WCHAR *des,WCHAR *src);

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	PBCDeleteFile(L"\\??\\c:\\PBC.txt");
	PBCDeleteFile(L"\\??\\c:\\PBCDir\\");
	PBCDeleteFile(L"\\??\\c:\\PBCCopied.txt");

	DbgPrint("p0sixB1ackcat:goodbye!!!\n");

	return;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject,PUNICODE_STRING pPath)
{
	DbgPrint("driver begian!!!\n");
	
	//����ûд�����������޷�ֹͣ������
	pDriverObject->DriverUnload = DriverUnload;

	fileOper();

	return STATUS_SUCCESS;
}

NTSTATUS fileOper(void)
{
	WCHAR *szDirName = L"\\??\\c:\\PBCDir\\";
	WCHAR *szFile1 = L"\\??\\c:\\PBC.txt";
	NTSTATUS status;
	//��C:\\��Ŀ¼�´���һ����ΪPBC.txt���ļ�
	status = PBCCreateFile(szFile1);

	if(!NT_SUCCESS(status))
	{
		DbgPrint("PBCCreateFile failed:%d!\n",status);
		return status;
	}
	status = PBCCreateFileDir(szDirName);

	if(!NT_SUCCESS(status))
	{
		DbgPrint("PBCCreateFileDir failed:%d!\n",status);
		return status;
	}

	status = PBCWriteToFile(szFile1,L"p0sixB1ackcat write:that's all right!");
	if(!NT_SUCCESS(status))
	{
		DbgPrint("PBCWriteToFile failed:%d!\n",status);
		return status;
	}

	status = PBCReadFile(szFile1);
	if(!NT_SUCCESS(status))
	{
		DbgPrint("PBCReadFile failed:%d!\n",status);
		return status;
	}

	//��ִ�п�������ʱ�������û��ʹ��L""�������͵��ַ���
	//������copy�����ڲ���des�ļ�ʧ��
	//ʹ��L""�������͵��ַ����ͺ���
	status = PBCCopyFile(L"\\??\\c:\\PBCCopied.txt",szFile1);
	if(!NT_SUCCESS(status))
	{
		DbgPrint("PBCCopyFile failed:%d!\n",status);
		return status;
	}

	status = PBCMoveFile(L"\\??\\c:\\PBCDir\\PBCCopied.txt",L"\\??\\c:\\PBCCopied.txt");
	if(!NT_SUCCESS(status))
	{
		return status;
	}

	return status;

}

NTSTATUS PBCCreateFile(WCHAR *szfileName)
{
	//�����������ΪUNICODE_STRING *���͵�ָ�룬���ҳ�ʼ��ΪNULL����ᷢ������
	UNICODE_STRING uFileName = {0x00};
	
	NTSTATUS status = STATUS_FAILED_DRIVER_ENTRY;
	HANDLE fileHandle = NULL;
	OBJECT_ATTRIBUTES fileObjectAttributes = {0};
	IO_STATUS_BLOCK ioBlock = {0};


	//��asicii������ַ���ת��Ϊunicode�����ַ���
	RtlInitUnicodeString(&uFileName,szfileName);

	InitializeObjectAttributes(&fileObjectAttributes,
		&uFileName,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL);

	status = ZwCreateFile(&fileHandle,
		GENERIC_WRITE,
		&fileObjectAttributes,
		&ioBlock,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
		FILE_OPEN_IF,
		FILE_SYNCHRONOUS_IO_NONALERT | FILE_NON_DIRECTORY_FILE,
		NULL,
		0);

	if(NT_SUCCESS(status))
	{
		ZwClose(fileHandle);
	}

	return status;
}

NTSTATUS PBCCreateFileDir(WCHAR *szDirName)
{
	NTSTATUS status;
	UNICODE_STRING uDirName = {0};
	HANDLE dirHandle;
	OBJECT_ATTRIBUTES dirAttributes;
	IO_STATUS_BLOCK ioBlock;
	RtlInitUnicodeString(&uDirName,szDirName);

	InitializeObjectAttributes(&dirAttributes,
		&uDirName,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL);

	status = ZwCreateFile(&dirHandle,
		GENERIC_WRITE |GENERIC_READ,
		&dirAttributes,
		&ioBlock,
		NULL,
		FILE_ATTRIBUTE_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN_IF,
		FILE_DIRECTORY_FILE |FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0);
	if(NT_SUCCESS(status))
	{
		ZwClose(dirHandle);
	}
	return status;
}

NTSTATUS PBCWriteToFile(WCHAR *szFileName,WCHAR *content)
{
	HANDLE fileHandle = NULL;
	UNICODE_STRING uFileName = {0x00};
	OBJECT_ATTRIBUTES fileAttributes = {0x00};
	IO_STATUS_BLOCK ioBlock = {0x00};
	NTSTATUS status;
	PUCHAR buffer;

	RtlInitUnicodeString(&uFileName,szFileName);
	
	InitializeObjectAttributes(&fileAttributes,
		&uFileName,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL);
	
	//����ҲҪ���ļ�
	status = ZwCreateFile(&fileHandle,
		GENERIC_WRITE,
		&fileAttributes,
		&ioBlock,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_WRITE,
		FILE_OPEN_IF,
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0);
	
	if(!NT_SUCCESS(status))
	{
		return status;
	}

	buffer = ExAllocatePoolWithTag(PagedPool,1024,'BCP');
	if(!buffer)
	{
		ZwClose(fileHandle);
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	RtlZeroMemory(buffer,1024);

	RtlCopyMemory(buffer,content,wcslen(content) * sizeof(WCHAR));

	status = ZwWriteFile(fileHandle,
		NULL,
		NULL,
		NULL,
		&ioBlock,
		buffer,
		1024,
		NULL,
		NULL);

	ZwClose(fileHandle);

	ExFreePool(buffer);

	return status;

}

NTSTATUS PBCReadFile(WCHAR *szFileName)
{
	NTSTATUS status;
	HANDLE fileHandle = NULL;
	UNICODE_STRING uFileName = {0x00};
	IO_STATUS_BLOCK ioBlock = {0x00};
	OBJECT_ATTRIBUTES objAttributes = {0x00};
	FILE_STANDARD_INFORMATION fileInformation = {0x00};
	PVOID outputBuffer = NULL;
	

	RtlInitUnicodeString(&uFileName,szFileName);

	InitializeObjectAttributes(&objAttributes,
		&uFileName,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL);

	status = ZwCreateFile(&fileHandle,
		GENERIC_READ,
		&objAttributes,
		&ioBlock,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0);

	if(!NT_SUCCESS(status))
	{
		return status;
	}

	//������Ҫ�Ȼ�ȡ��fileInformation��information���ͻ��кܶ���
	//����ָ������ΪstandardInformation,����ͨ����������ṹ�ĳ�Ա��ȡ�ļ�size
	//��ѯ���ļ��ı�׼����
	status = NtQueryInformationFile(fileHandle,
		&ioBlock,
		&fileInformation,
		sizeof(FILE_STANDARD_INFORMATION),
		FileStandardInformation);
	if(!NT_SUCCESS(status))
	{
		//ִ�е������ʾ�ļ��Ѿ�����ǰ���̴򿪲�ռ�У����Է���ʱҪ�رվ��������ͬ��
		goto ret;
	}

	outputBuffer = ExAllocatePoolWithTag(PagedPool,fileInformation.EndOfFile.QuadPart,'DAER');

	if(!outputBuffer)
	{
		//������ʾ��ǰ������Դ��������صĴ���
		status = STATUS_INSUFFICIENT_RESOURCES;
		goto ret;
	}

	status = ZwReadFile(fileHandle,
		NULL,
		NULL,
		NULL,
		&ioBlock,
		outputBuffer,
		(ULONG)fileInformation.EndOfFile.QuadPart,
		NULL,
		NULL);

	//test
	DbgPrint("fileInformation.endoffile.quadpart is %d\nsizeof(outputbuffer) is %d\n",
		fileInformation.EndOfFile.QuadPart,sizeof(outputBuffer));
	DbgPrint("read:%ws\n",outputBuffer);
	
	//ʹ�����ֶ������ǳ����ExAllocatePoolWithTag�󣬱������ͷ��ڴ�
	ExFreePool(outputBuffer);

ret:
	//�رվ��
	ZwClose(fileHandle);
	
	return status;
}

NTSTATUS PBCCopyFile(WCHAR *des,WCHAR *src)
{
	NTSTATUS iostatus;
	UNICODE_STRING uDesFileName = {0x00};
	UNICODE_STRING uSrcFileName = {0x00};
	OBJECT_ATTRIBUTES objDesAttributes = {0x00};
	OBJECT_ATTRIBUTES objSrcAttributes= {0x00};
	HANDLE DesFileHandle = NULL;
	HANDLE SrcFileHandle = NULL;
	IO_STATUS_BLOCK ioStatusBlock = {0x00};
	PVOID buffer = NULL;
	LARGE_INTEGER byteOffset = {0x00};
	ULONG length = 0;


	RtlInitUnicodeString(&uDesFileName,des);
	RtlInitUnicodeString(&uSrcFileName,src);
	
	InitializeObjectAttributes(&objSrcAttributes,
		&uSrcFileName,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL);
	InitializeObjectAttributes(&objDesAttributes,
		&uDesFileName,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL);
	iostatus = ZwCreateFile(&SrcFileHandle,
		FILE_READ_DATA | FILE_READ_ATTRIBUTES,
		&objSrcAttributes,
		&ioStatusBlock,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ | FILE_SHARE_DELETE,
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0);

	if(!NT_SUCCESS(iostatus))
	{
		return iostatus;
	}
	
	iostatus = ZwCreateFile(&DesFileHandle,
		FILE_WRITE_ATTRIBUTES | FILE_WRITE_DATA,
		&objDesAttributes,
		&ioStatusBlock,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN_IF,
		FILE_SYNCHRONOUS_IO_NONALERT | FILE_NON_DIRECTORY_FILE,
		NULL,
		0);
	if(!NT_SUCCESS(iostatus))
	{
		ZwClose(SrcFileHandle);
		return iostatus;
	}

	buffer = ExAllocatePoolWithTag(PagedPool,1024,'YPOC');
	if(!buffer)
	{
		ZwClose(SrcFileHandle);
		ZwClose(DesFileHandle);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	while(1)
	{
		iostatus = ZwReadFile(SrcFileHandle,
			NULL,
			NULL,
			NULL,
			&ioStatusBlock,
			buffer,
			PAGE_SIZE,
			&byteOffset,
			NULL);
		if(!NT_SUCCESS(iostatus))
		{
			if(iostatus == STATUS_END_OF_FILE)
			{
				iostatus = STATUS_SUCCESS;	
			}
			break;
		}

		length = (ULONG)ioStatusBlock.Information;

		iostatus = ZwWriteFile(DesFileHandle,
			NULL,
			NULL,
			NULL,
			&ioStatusBlock,
			buffer,
			length,
			&byteOffset,
			NULL);
		if(!NT_SUCCESS(iostatus))
		{
			break;
		}
		byteOffset.QuadPart += length;

	}

	ExFreePool(buffer);
	buffer = NULL;
	ZwClose(SrcFileHandle);
	ZwClose(DesFileHandle);
	return iostatus;

}

NTSTATUS PBCDeleteFile(WCHAR *szfileName)
{
	UNICODE_STRING ufileName = {0x00};
	OBJECT_ATTRIBUTES objAttributes = {0x00};
	HANDLE fileHandle = NULL;
	IO_STATUS_BLOCK ioStatusBlock = {0x00};
	FILE_DISPOSITION_INFORMATION disInfo = {0x00};
	NTSTATUS ioStatus;

	RtlInitUnicodeString(&ufileName,szfileName);

	InitializeObjectAttributes(&objAttributes,
		&ufileName,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL);
	
	ioStatus = ZwCreateFile(&fileHandle,
		DELETE | SYNCHRONIZE | FILE_WRITE_DATA,
		&objAttributes,
		&ioStatusBlock,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_DELETE |FILE_SHARE_READ | FILE_SHARE_WRITE,
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT | FILE_DELETE_ON_CLOSE,
		NULL,
		0);
	if(!NT_SUCCESS(ioStatus))
	{
		if(ioStatus == STATUS_ACCESS_DENIED)
		{
			ioStatus = ZwCreateFile(&fileHandle,
				FILE_WRITE_DATA | FILE_READ_DATA | SYNCHRONIZE,
				&objAttributes,
				&ioStatusBlock,
				NULL,
				FILE_ATTRIBUTE_NORMAL,
				FILE_SHARE_READ |FILE_SHARE_WRITE |FILE_SHARE_DELETE,
				FILE_OPEN,
				FILE_SYNCHRONOUS_IO_NONALERT,
				NULL,
				0);
			if(NT_SUCCESS(ioStatus))
			{
				FILE_BASIC_INFORMATION fileInformation = {0x00};
				ioStatus = ZwQueryInformationFile(fileHandle,
					&ioStatusBlock,
					&fileInformation,
					sizeof(FILE_BASIC_INFORMATION),
					FileBasicInformation);
				if(!NT_SUCCESS(ioStatus))
				{
					DbgPrint("ZwQueryInformationFile(%wZ) failed:%d\n",&ufileName,ioStatus);
				}
				//ȥ���ļ���ֻ������
				fileInformation.FileAttributes = FILE_ATTRIBUTE_NORMAL;

				ioStatus = ZwSetInformationFile(fileHandle,
					&ioStatusBlock,
					&fileInformation,
					sizeof(FILE_BASIC_INFORMATION),
					FileBasicInformation);

				if(!NT_SUCCESS(ioStatus))
				{
					DbgPrint("ZwSetInformationFile(%wZ) failed:%d\n",&ufileName,ioStatus);
				}

				ZwClose(fileHandle);

				ioStatus = ZwCreateFile(&fileHandle,
					DELETE | FILE_READ_DATA | FILE_WRITE_DATA,
					&objAttributes,
					&ioStatusBlock,
					NULL,
					FILE_ATTRIBUTE_NORMAL,
					FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
					FILE_OPEN,
					FILE_SYNCHRONOUS_IO_NONALERT |FILE_DELETE_ON_CLOSE,
					NULL,
					0);
			}
		}
		if(!NT_SUCCESS(ioStatus))
		{
			DbgPrint("ZwCreateFile(%wZ) failed:%d\n",&ufileName,ioStatus);
			return ioStatus;
		}
	}
	disInfo.DeleteFile = TRUE;
	ioStatus = ZwSetInformationFile(fileHandle,
		&ioStatusBlock,
		&disInfo,
		sizeof(FILE_DISPOSITION_INFORMATION),
		FileDispositionInformation);

	if(!NT_SUCCESS(ioStatus))
	{
		DbgPrint("ZwSetInformationFile(%wZ) falied:%d\n",&ufileName,ioStatus);
	}
	
	//�����˹رգ���ֹ�ļ���һֱռ��
	ZwClose(fileHandle);

	return ioStatus;
	
}

NTSTATUS PBCMoveFile(WCHAR *des,WCHAR *src)
{
	NTSTATUS ioStatus;
	ioStatus = PBCCopyFile(des,src);
	if(!NT_SUCCESS(ioStatus))
	{
		DbgPrint("PBCMoveFile copy file(%wZ) to %wZ failed:%d",&src,&des,ioStatus);
		return ioStatus;
	}
	ioStatus = PBCDeleteFile(src);

	return ioStatus;
}