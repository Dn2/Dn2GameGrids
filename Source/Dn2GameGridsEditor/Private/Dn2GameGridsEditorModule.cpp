#include "Dn2GameGridsEditorModule.h"

#include "GameplayTagsManager.h"
#include "MaterialEditingLibrary.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Materials/Material.h"
#include "Materials/MaterialExpressionCustom.h"
#include "Materials/MaterialExpressionTextureCoordinate.h"
#include "Materials/MaterialExpressionConstant3Vector.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionVertexColor.h"
#include "UObject/SavePackage.h"
#if ENGINE_MAJOR_VERSION == 5
#include "UObject/ObjectSaveContext.h"
#endif


IMPLEMENT_GAME_MODULE(FDn2GameGridsEditorModule, Dn2GameGridsEditor);

void FDn2GameGridsEditorModule::StartupModule()
{
	/*
	*	Add GameplayTags that the plugin will always assume is present for making empty grids.
	*	Only remove or edit these if you're changing how ABoxGridActor works.
	*/
	UGameplayTagsManager::Get().AddNativeGameplayTag("Cell.Nav.Void");
	UGameplayTagsManager::Get().AddNativeGameplayTag("Cell.Nav.Blocked");
	
	// Try to load grid debug materials Dn2GameGrids/Content/Materials
	UObject* Asset = StaticLoadObject(UObject::StaticClass(), nullptr, TEXT("/Dn2GameGrids/Materials/M_BoxGridDebug.M_BoxGridDebug"));
	if (Asset == nullptr)
	{
		/*UE_LOG(LogTemp, Warning, TEXT("BoxGrid debug material not found! Creating it now.."));
		
		// Generate and save material to project
		FString AssetPath = FString(FPaths::ProjectPluginsDir() + "Dn2GameGrids/Content/Materials/");
		FString PackagePath = FString("/Dn2GameGrids/Content/Materials/M_BoxGridDebug");
		UPackage* Package = CreatePackage(*PackagePath);
	
		UE_LOG(LogTemp, Warning, TEXT("Path: %s"), *FPaths::ProjectPluginsDir());
	
		UMaterial* GenGridMat = NewObject<UMaterial>(Package, UMaterial::StaticClass(), *FString("M_BoxGridDebug"), EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);
		GenGridMat->SetShadingModel(MSM_Unlit);
		
		UMaterialExpressionScalarParameter* XNode = Cast<UMaterialExpressionScalarParameter>(UMaterialEditingLibrary::CreateMaterialExpression(GenGridMat,UMaterialExpressionScalarParameter::StaticClass()));
		XNode->ParameterName = FName("X");
		XNode->DefaultValue = 8.0f;
		
		UMaterialExpressionScalarParameter* YNode = Cast<UMaterialExpressionScalarParameter>(UMaterialEditingLibrary::CreateMaterialExpression(GenGridMat,UMaterialExpressionScalarParameter::StaticClass()));
		YNode->ParameterName = FName("Y");
		YNode->DefaultValue = 8.0f;
		
		UMaterialExpressionTextureCoordinate* CoordUVNode = Cast<UMaterialExpressionTextureCoordinate>(UMaterialEditingLibrary::CreateMaterialExpression(GenGridMat,UMaterialExpressionTextureCoordinate::StaticClass()));
					
		UMaterialExpressionConstant3Vector* Color1Node = Cast<UMaterialExpressionConstant3Vector>(UMaterialEditingLibrary::CreateMaterialExpression(GenGridMat,UMaterialExpressionConstant3Vector::StaticClass()));
		Color1Node->Constant = FLinearColor::Green;
		
		UMaterialExpressionVertexColor* VertexColor = Cast<UMaterialExpressionVertexColor>(UMaterialEditingLibrary::CreateMaterialExpression(GenGridMat,UMaterialExpressionVertexColor::StaticClass()));
		
		UMaterialExpressionCustom* CustomNode = Cast<UMaterialExpressionCustom>(UMaterialEditingLibrary::CreateMaterialExpression(GenGridMat,UMaterialExpressionCustom::StaticClass()));
		FCustomInput InUV;
		InUV.InputName = FName("UV");
		
		FCustomInput InX;
		InX.InputName = FName("X");
		
		FCustomInput InY;
		InY.InputName = FName("Y");
		
		CustomNode->Inputs[0].InputName = FName("color");
		
		CustomNode->Inputs.Add(InUV);
		CustomNode->Inputs.Add(InX);
		CustomNode->Inputs.Add(InY);
		
		FString ShaderCode = "float3 Color1 = float3(0.026241,0.043735,0.099899);";
		ShaderCode.Append("float3 Color2 = float3(0.031896,0.116971,0.439657);");
		ShaderCode.Append("float3 EdgeColor = float3(0.639216,0.141176,0.000000);");
		ShaderCode.Append("float2 Extents = float2(Y*0.5, X*0.5);");
		ShaderCode.Append("float2 CheckUV = frac(UV*Extents) - 0.5;");
		ShaderCode.Append("float mask = step(0.0, CheckUV.x * CheckUV.y);");
		ShaderCode.Append("float3 result = lerp(Color1,Color2,mask);");
		ShaderCode.Append("float2 EdgeUV = frac(UV*(Extents*2)) - 0.5;");
		ShaderCode.Append("float edge = step(0.48, EdgeUV.x);");
		ShaderCode.Append("edge += saturate(step(0.48, EdgeUV.y));");
		ShaderCode.Append("result = lerp(result, color, color.r);");
		ShaderCode.Append("return lerp(result,EdgeColor, edge);");
		
		CustomNode->Code = ShaderCode;

		UMaterialEditingLibrary::ConnectMaterialExpressions(CoordUVNode,"",CustomNode,"UV");
		UMaterialEditingLibrary::ConnectMaterialExpressions(XNode,"",CustomNode,"X");
		UMaterialEditingLibrary::ConnectMaterialExpressions(YNode,"",CustomNode,"Y");
		UMaterialEditingLibrary::ConnectMaterialExpressions(VertexColor,"",CustomNode,"color");
		UMaterialEditingLibrary::ConnectMaterialProperty(CustomNode,"",MP_EmissiveColor);
		//UMaterialEditingLibrary::RecompileMaterial(GenGridMat);
		
		if(GenGridMat && Package)
		{
			Package->PackageSavedEvent.AddRaw(this, &FDn2GameGridsEditorModule::OnGridPackageSaved);
			
			FAssetRegistryModule::AssetCreated(GenGridMat);
			Package->SetDirtyFlag(true);
			FString FilePath = FString::Printf(TEXT("%s%s%s"), *AssetPath, *FString("M_BoxGridDebug"), *FPackageName::GetAssetPackageExtension());
			UMaterialEditingLibrary::RecompileMaterial(GenGridMat);
			bool bSuccess = UPackage::SavePackage(Package, GenGridMat, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *FilePath);

			UE_LOG(LogTemp, Warning, TEXT("Saved Package: %s"), bSuccess ? TEXT("True") : TEXT("False"));
		}*/
	}
	else
	{
		
	}
	//FModuleManager::Get().LoadModuleChecked(FName("Dn2GameGrids"));
}


void FDn2GameGridsEditorModule::ShutdownModule()
{
	
}

void FDn2GameGridsEditorModule::PostLoadCallback()
{
	//IModuleInterface::PostLoadCallback();
	UObject* Asset = StaticLoadObject(UObject::StaticClass(), nullptr, TEXT("/Dn2GameGrids/Materials/M_BoxGridDebug.M_BoxGridDebug"));
	if (Asset == nullptr)
	{
		CreateAndSaveMaterialPackage();
	}

}

void FDn2GameGridsEditorModule::CreateAndSaveMaterialPackage()
{
	UE_LOG(LogTemp, Warning, TEXT("BoxGrid debug material not found! Creating it now.."));
		
	// Generate and save material to project
	FString AssetPath = FString(FPaths::ProjectPluginsDir() + "Dn2GameGrids/Content/Materials/");
	FString PackagePath = FString("/Dn2GameGrids/Content/Materials/M_BoxGridDebug");
	UPackage* Package = CreatePackage(*PackagePath);
	
	UE_LOG(LogTemp, Warning, TEXT("Path: %s"), *FPaths::ProjectPluginsDir());
	
	UMaterial* GenGridMat = NewObject<UMaterial>(Package, UMaterial::StaticClass(), *FString("M_BoxGridDebug"), EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);
	GenGridMat->SetShadingModel(MSM_Unlit);
		
	UMaterialExpressionScalarParameter* XNode = Cast<UMaterialExpressionScalarParameter>(UMaterialEditingLibrary::CreateMaterialExpression(GenGridMat,UMaterialExpressionScalarParameter::StaticClass()));
	XNode->ParameterName = FName("X");
	XNode->DefaultValue = 8.0f;
		
	UMaterialExpressionScalarParameter* YNode = Cast<UMaterialExpressionScalarParameter>(UMaterialEditingLibrary::CreateMaterialExpression(GenGridMat,UMaterialExpressionScalarParameter::StaticClass()));
	YNode->ParameterName = FName("Y");
	YNode->DefaultValue = 8.0f;
		
	UMaterialExpressionTextureCoordinate* CoordUVNode = Cast<UMaterialExpressionTextureCoordinate>(UMaterialEditingLibrary::CreateMaterialExpression(GenGridMat,UMaterialExpressionTextureCoordinate::StaticClass()));
					
	UMaterialExpressionConstant3Vector* Color1Node = Cast<UMaterialExpressionConstant3Vector>(UMaterialEditingLibrary::CreateMaterialExpression(GenGridMat,UMaterialExpressionConstant3Vector::StaticClass()));
	Color1Node->Constant = FLinearColor::Green;
	
	UMaterialExpressionVertexColor* VertexColor = Cast<UMaterialExpressionVertexColor>(UMaterialEditingLibrary::CreateMaterialExpression(GenGridMat,UMaterialExpressionVertexColor::StaticClass()));
	
	UMaterialExpressionCustom* CustomNode = Cast<UMaterialExpressionCustom>(UMaterialEditingLibrary::CreateMaterialExpression(GenGridMat,UMaterialExpressionCustom::StaticClass()));
	FCustomInput InUV;
	InUV.InputName = FName("UV");
	
	FCustomInput InX;
	InX.InputName = FName("X");
	
	FCustomInput InY;
	InY.InputName = FName("Y");
	
	CustomNode->Inputs[0].InputName = FName("color");
	
	CustomNode->Inputs.Add(InUV);
	CustomNode->Inputs.Add(InX);
	CustomNode->Inputs.Add(InY);
	
	FString ShaderCode = "float3 Color1 = float3(0.026241,0.043735,0.099899);";
	ShaderCode.Append("float3 Color2 = float3(0.031896,0.116971,0.439657);");
	ShaderCode.Append("float3 EdgeColor = float3(0.639216,0.141176,0.000000);");
	ShaderCode.Append("float2 Extents = float2(X*0.5, Y*0.5);");
	ShaderCode.Append("float2 CheckUV = frac(UV*Extents) - 0.5;");
	ShaderCode.Append("float mask = step(0.0, CheckUV.x * CheckUV.y);");
	ShaderCode.Append("float3 result = lerp(Color1,Color2,mask);");
	ShaderCode.Append("float2 EdgeUV = frac(UV*(Extents*2)) - 0.5;");
	ShaderCode.Append("float edge = step(0.48, EdgeUV.x);");
	ShaderCode.Append("edge += saturate(step(0.48, EdgeUV.y));");
	ShaderCode.Append("result = lerp(result, color, color.r);");
	ShaderCode.Append("return lerp(result,EdgeColor, edge);");
	
	CustomNode->Code = ShaderCode;

	UMaterialEditingLibrary::ConnectMaterialExpressions(CoordUVNode,"",CustomNode,"UV");
	UMaterialEditingLibrary::ConnectMaterialExpressions(XNode,"",CustomNode,"X");
	UMaterialEditingLibrary::ConnectMaterialExpressions(YNode,"",CustomNode,"Y");
	UMaterialEditingLibrary::ConnectMaterialExpressions(VertexColor,"",CustomNode,"color");
	UMaterialEditingLibrary::ConnectMaterialProperty(CustomNode,"",MP_EmissiveColor);
	//UMaterialEditingLibrary::RecompileMaterial(GenGridMat);
	
	if(GenGridMat && Package)
	{
#if ENGINE_MAJOR_VERSION == 4
		Package->PackageSavedEvent.AddRaw(this, &FDn2GameGridsEditorModule::OnGridPackageSaved);
#endif
#if ENGINE_MAJOR_VERSION == 5
		Package->PackageSavedWithContextEvent.AddRaw(this, &FDn2GameGridsEditorModule::OnGridPackageSaved);
#endif
		
		FAssetRegistryModule::AssetCreated(GenGridMat);
		Package->SetDirtyFlag(true);
		FString FilePath = FString::Printf(TEXT("%s%s%s"), *AssetPath, *FString("M_BoxGridDebug"), *FPackageName::GetAssetPackageExtension());
		UMaterialEditingLibrary::RecompileMaterial(GenGridMat);
		
#if ENGINE_MAJOR_VERSION == 4
		bool bSuccess = UPackage::SavePackage(Package, GenGridMat, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *FilePath);
#endif
#if ENGINE_MAJOR_VERSION == 5
		FSavePackageArgs Args;
		Args.SaveFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
		bool bSuccess = UPackage::SavePackage(Package, GenGridMat, *FilePath, Args);
#endif
		
		
		UE_LOG(LogTemp, Warning, TEXT("Saved Package: %s"), bSuccess ? TEXT("True") : TEXT("False"));
		
#if ENGINE_MAJOR_VERSION == 4 || ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION < 2
		auto clicked = FMessageDialog::Open( EAppMsgType::Ok, FText::FromString("Material M_BoxGridDebug was created in plugin dir. A one time editor restart is required to start using Dn2GameGrids. \n \n You can manually restart the editor on your own."),nullptr);
#else
		auto clicked = FMessageDialog::Open( EAppMsgType::Ok, FText::FromString("Material M_BoxGridDebug was created in plugin dir. A one time editor restart is required to start using Dn2GameGrids. \n \n You can manually restart the editor on your own."),FText::FromString("Editor Restart"));
#endif
		if (clicked == EAppReturnType::Ok)
		{
			FUnrealEdMisc::Get().RestartEditor(true);
		}

	}
}


#if ENGINE_MAJOR_VERSION == 4
void FDn2GameGridsEditorModule::OnGridPackageSaved(const FString& PackageFileName, UObject* Outer)
#endif
#if ENGINE_MAJOR_VERSION == 5
void FDn2GameGridsEditorModule::OnGridPackageSaved(const FString& PackageFileName, UPackage* Package, FObjectPostSaveContext PackageName)
#endif
{
	UE_LOG(LogTemp, Warning, TEXT("Saved Package: %s"), *PackageFileName);
	//FModuleManager::Get().LoadModuleChecked(FName("Dn2GameGrids"));
}


