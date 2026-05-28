#include "Minigin/SoundSystem/SoundServiceLocator.h"
#include "Minigin/SoundSystem/NullSoundSystem.h"

std::unique_ptr<mg::ISoundSystem> mg::SoundServiceLocator::m_pInstance = std::make_unique<mg::NullSoundSystem>();

mg::ISoundSystem& mg::SoundServiceLocator::Fetch()
{
	if (!m_pInstance)
	{
		Register(nullptr);
	}

	return *m_pInstance;
}

void mg::SoundServiceLocator::Register(std::unique_ptr<ISoundSystem> soundSystem)
{
	if (soundSystem == nullptr)
	{
		m_pInstance = std::make_unique<NullSoundSystem>();
	}
	else
	{
		m_pInstance = std::move(soundSystem);
	}
}