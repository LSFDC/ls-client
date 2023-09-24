#ifndef __NMCONEW_H_7E9B922A_A6D1_4865_93DD_69B59E27CF1C__
#define __NMCONEW_H_7E9B922A_A6D1_4865_93DD_69B59E27CF1C__

#ifdef _NM_DLL_EXPORT
#define DllDecl	__declspec( dllexport )
#else
#define DllDecl	__declspec( dllimport )
#endif

extern "C"
{
	/*
	uFuncCode		: ȣ���ϴ� CNMFunc�� �ڵ�
	pCallingData	: Serialize�� ȣ�� ������
	ppReturnData	: ����� �������� ������ ������, ���۰� ������� ��� �����Ҵ��ؼ� ���ϵ�
	uReturnDataLen	: ȣ��� ��� ������ ������
					���Ͻ� ����� ���Ͽ� ����� �����ǻ�����
	����			: ���� ������ ũ�Ⱑ ���� ���� �����Ϳ� ���Ͽ� ���� ���
					���� �޸𸮸� �Ҵ��� ����� �����Ѵ�. �̶� ppReturnData
					���� ó�� ȣ��ÿ� �޶����� �Ǹ�, �̷��� ���� �Ҵ�Ǿ�
					�Ѿ �޸𸮴� NMCO_MemoryFree�� �̿��� ������ �־�� �Ѵ�.
	*/
	BOOL DllDecl NMCO_CallNMFunc(	IN		UINT32	uFuncCode,
									IN		BYTE*	pCallingData,
									IN OUT	BYTE**	ppReturnData,
									IN OUT	UINT32&	uReturnDataLen
									);

	/*
	uEncVersion		: pCallingData�� Encrypt ���� ����
	������ �����ʹ� ��
	*/
	BOOL DllDecl NMCO_CallNMFunc2(	IN		UINT32	uFuncCode,
									IN		BYTE*	pCallingData,
									IN		UINT8	uSerializerVersion,
									IN OUT	BYTE**	ppReturnData,
									IN OUT	UINT32&	uReturnDataLen
									);

	void DllDecl NMCO_MemoryFree( IN LPVOID pBuffer );

	//
	//	��� ��ġ ���θ� �����մϴ�
	//	(�⺻�����δ� ��ġ�� �մϴ�)
	//
	BOOL DllDecl NMCO_SetPatchOption( IN BOOL bPatch );

	//
	//	ģ�� ���� ��� ���θ� �����մϴ�
	//	(�⺻�����δ� ���� �ֽ��ϴ�)
	//
	BOOL DllDecl NMCO_SetUseFriendModuleOption( IN BOOL bUserFriendModule );

	//
	//	��ġ �� ���, ���� ������ �����մϴ�
	//	(�⺻�����δ� �ѱ� ������ �����˴ϴ�)
	//
	BOOL DllDecl NMCO_SetVersionFileUrlA( IN LPCSTR pszVersionUrl );
	BOOL DllDecl NMCO_SetVersionFileUrlW( IN LPCWSTR pszVersionUrl );

#if defined( UNICODE ) || defined( _UNICODE )
#define NMCO_SetVersionFileUrl	NMCO_SetVersionFileUrlW
#else
#define NMCO_SetVersionFileUrl	NMCO_SetVersionFileUrlA
#endif

	//
	//	NGM �� �̿��ؼ� ��ġ �մϴ�
	//	(�⺻�����δ� ���� �ֽ��ϴ�)
	//
	BOOL DllDecl NMCO_SetUseNGMOption( IN BOOL bUseNGM );

	//
	//	�޽����� ������ ������ �����մϴ�
	//
	//		-	�⺻�����δ� �ѱ����� �����ϵ��� �����Ǿ� �ֽ��ϴ�
	//
	BOOL DllDecl NMCO_SetLocale( IN UINT32 uLocaleID );

	//
	//	�޽����� ������ ���� & ������ �����մϴ�
	//
	//		-	�⺻�����δ� �ѱ� & �⺻�������� �����ϵ��� �����Ǿ� �ֽ��ϴ�
	//
	BOOL DllDecl NMCO_SetLocaleAndRegion( IN NMLOCALEID uLocaleID, IN NMREGIONCODE uRegionCode );
}

#endif	//	#ifndef __NMCONEW_H_7E9B922A_A6D1_4865_93DD_69B59E27CF1C__