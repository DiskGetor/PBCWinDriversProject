#include <ntddk.h>
#include <ntstrsafe.h>

int main()
{
	//��ջ�ϸ�UNICODE_STRING��buffer����ռ�
	UNICODE_STRING uStrWithStack = {0};//��ʼ��һ��UNICODE_STRING
	WCHAR buffer[512] = L"Hello world";
	uStrWithStack.Buffer = buffer;
	uStrWithStack.Length = (wcslen(L"Hello world")) * sizeof(WCHAR);
	uStrWithStack.MaximumLength = sizeof(buffer);

	//�Ӷ��ϸ�UNICODE_STRING�����ڴ�
	UNICODE_STRING uStrWithHeap = {0};
	uStrWithHeap.MaximumLength = MAX_PATH * sizeof(WCHAR);
	uStrWithHeap.Buffer = ExAllocatePoolWithTag(PagedPool,uStrWithHeap.MaximumLength);
	uStrWithHeap.Length = (wcslen(L"Hello world") * sizeof(WCHAR));
	if(uStrWithHeap.Buffer == NULL)
	{
		return -1;
	}
	
	RtlZeroMemory(uStrWithHeap,uStrWithHeap.MaximumLength);
	RtlCopyMemory(uStrWithHeap.Buffer,L"Hello world",wcslen(L"Hello world"));
	DbgPrint("%ws\n",&uStrWithHeap);
	ExFreePool(uStrWithHeap);


	//���õĲ�������
	UNICODE_STRING uStr = {0};
	WCHAR *szStr = L"Hello world";
	UNICODE_STRING uStr1 = {0};
	//ʹ��RtlInitUnicodeString��û�н�szStr�е��ַ���������uStr��
	//����uStr.Bufferֱ��ָ����szStrָ����ַ������ھ�̬����
	RtlInitUnicodeString(&uStr,szStr);
	//��uStr1ƴ�ӵ�uStr�ĺ��棬������������UNICODE_STRING����ָ��
	RtlAppendUnicodeStringToString(&uStr,&uStr1);


	//����ͨunicode������ַ���ƴ�ӵ�UNICODE_STRING�����ַ����ĺ���
	RtlAppendUnicodeToString(&uStr,L"nopnopnop...");

	//��uStr1������uStr
	RtlCopyUnicodeString(&uStr,&uStr1);

	//�Ƚ������ַ����Ƿ���ȣ���ȷ���0�����һ��������ʾ�Ƿ���Դ�Сд
	RtlCompareUnicodeString(&uStr,&uStr1,TRUE);

	char *astr = "ascii str";
	//��astr�ַ���ת��Ϊunicode������ַ�����ת��֮���ŵ�uStr�У�uStr���
	//����ַ������ڴ������ϵͳ���䣬Ҳ���Գ���Ա�Լ�ָ��
	//�����������������ΪTRUE����ô����ϵͳ�Ӷ��ϸ�uStr�����ڴ棬ʹ���������ExFreeMemory
	//FALSE����Ҫ����Ա�Լ�����
	RtlAnsiStringToUnicodeString(&uStr,astr,TRUE);

	//��ȫ�ĺ���������������---#include <ntstrsafe.h>
	RtlUnicodeStringInit(&uStr,szStr);
	
	RtlUnicodeStringCopy(&uStr,&uStr1);

	RtlUnicodeStringCat(&uStr,&uStr1);

}