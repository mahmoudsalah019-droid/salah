using UnrealBuildTool;

public class SMHQ_VirtualTarget : TargetRules
{
	public SMHQ_VirtualTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("SMHQ_Virtual");
	}
}
