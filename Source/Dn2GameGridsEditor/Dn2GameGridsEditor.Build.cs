using UnrealBuildTool;
	 
public class Dn2GameGridsEditor: ModuleRules
{
	public Dn2GameGridsEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "Dn2GameGrids", "Slate", "SlateCore","GameplayTags", "MaterialEditor", "Json", "JsonUtilities" });
		
		PublicDependencyModuleNames.AddRange(new string[] { "UnrealEd", "AssetTools" });
	}
}