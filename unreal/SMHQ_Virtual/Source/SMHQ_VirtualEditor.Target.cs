using UnrealBuildTool;

public class SMHQ_VirtualEditorTarget : TargetRules
{
	public SMHQ_VirtualEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("SMHQ_Virtual");
	}
}
