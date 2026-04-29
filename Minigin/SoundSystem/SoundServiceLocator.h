#ifndef SOUND_SERVICE_LOCATOR_H
#define SOUND_SERVICE_LOCATOR_H
#include <memory>

namespace mg
{
	class ISoundSystem;

	class SoundServiceLocator final
	{
	public:
		static ISoundSystem& Fetch();
		static void Register(std::unique_ptr<ISoundSystem> soundSystem);
		
		// No instances allowed, use class only statically
		SoundServiceLocator() = delete;

	private:
		static std::unique_ptr<mg::ISoundSystem> m_pInstance;
	};
}

#endif // !SOUND_SERVICE_LOCATOR_H