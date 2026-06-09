#ifndef HUD_Manager_H
#define HUD_Manager_H


#include <Minigin/Components/Component.h>

#include <vector>

class PlayerUI;
class HUDManager : public mg::Component
{
public:
	void Start() override;

	HUDManager(mg::GameObject& owner);

	~HUDManager() override;
	HUDManager(const HUDManager&) = delete;
	HUDManager& operator=(const HUDManager&) = delete;
	HUDManager(HUDManager&&) noexcept = delete;
	HUDManager& operator=(HUDManager&&) noexcept = delete;

private:
	std::vector<PlayerUI*> m_pPlayerInfoElements;
};

#endif //  !HUD_Manager_H