#include "TerminalPlugin.h"
#include "Modules/ModuleManager.h"
#include "TerminalUISubsystem.h"

IMPLEMENT_MODULE(FTerminalPluginModule, TerminalPlugin)

void FTerminalPluginModule::StartupModule()
{
    UE_LOG(LogTemp, Log, TEXT("[Terminal] Module Startup"));
    // Petite référence pour éviter tout stripping
    ensure(UTerminalUISubsystem::StaticClass() != nullptr);
}
void FTerminalPluginModule::ShutdownModule()
{
    UE_LOG(LogTemp, Log, TEXT("[Terminal] Module Shutdown"));
}
