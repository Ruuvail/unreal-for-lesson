// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "TargetPracticeCPP/TargetPracticeTarget.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTargetPracticeTarget() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	TARGETPRACTICECPP_API UClass* Z_Construct_UClass_ATargetPracticeTarget();
	TARGETPRACTICECPP_API UClass* Z_Construct_UClass_ATargetPracticeTarget_NoRegister();
	UPackage* Z_Construct_UPackage__Script_TargetPracticeCPP();
// End Cross Module References
	void ATargetPracticeTarget::StaticRegisterNativesATargetPracticeTarget()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ATargetPracticeTarget);
	UClass* Z_Construct_UClass_ATargetPracticeTarget_NoRegister()
	{
		return ATargetPracticeTarget::StaticClass();
	}
	struct Z_Construct_UClass_ATargetPracticeTarget_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ATargetPracticeTarget_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_TargetPracticeCPP,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ATargetPracticeTarget_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "TargetPracticeTarget.h" },
		{ "ModuleRelativePath", "TargetPracticeTarget.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ATargetPracticeTarget_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ATargetPracticeTarget>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ATargetPracticeTarget_Statics::ClassParams = {
		&ATargetPracticeTarget::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_ATargetPracticeTarget_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ATargetPracticeTarget_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ATargetPracticeTarget()
	{
		if (!Z_Registration_Info_UClass_ATargetPracticeTarget.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ATargetPracticeTarget.OuterSingleton, Z_Construct_UClass_ATargetPracticeTarget_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ATargetPracticeTarget.OuterSingleton;
	}
	template<> TARGETPRACTICECPP_API UClass* StaticClass<ATargetPracticeTarget>()
	{
		return ATargetPracticeTarget::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ATargetPracticeTarget);
	ATargetPracticeTarget::~ATargetPracticeTarget() {}
	struct Z_CompiledInDeferFile_FID_Users_Ruvail_Documents_GitHub_enlast_unreal_for_lesson_TargetPracticeCPP_Source_TargetPracticeCPP_TargetPracticeTarget_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Ruvail_Documents_GitHub_enlast_unreal_for_lesson_TargetPracticeCPP_Source_TargetPracticeCPP_TargetPracticeTarget_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ATargetPracticeTarget, ATargetPracticeTarget::StaticClass, TEXT("ATargetPracticeTarget"), &Z_Registration_Info_UClass_ATargetPracticeTarget, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ATargetPracticeTarget), 1000932595U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Ruvail_Documents_GitHub_enlast_unreal_for_lesson_TargetPracticeCPP_Source_TargetPracticeCPP_TargetPracticeTarget_h_3042647289(TEXT("/Script/TargetPracticeCPP"),
		Z_CompiledInDeferFile_FID_Users_Ruvail_Documents_GitHub_enlast_unreal_for_lesson_TargetPracticeCPP_Source_TargetPracticeCPP_TargetPracticeTarget_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Ruvail_Documents_GitHub_enlast_unreal_for_lesson_TargetPracticeCPP_Source_TargetPracticeCPP_TargetPracticeTarget_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
