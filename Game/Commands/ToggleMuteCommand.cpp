#include "ToggleMuteCommand.h"

#include <Minigin/Audio/SoundServiceLocator.h>
#include <Minigin/Audio/ISoundSystem.h>

void ToggleMuteCommand::Execute()
{
	mg::SoundServiceLocator::Fetch().ToggleMute();
}
