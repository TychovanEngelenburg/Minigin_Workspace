#include "ImGUI/imgui_CacheTester.h"
#include <array>
#include <vector>
#include <chrono>
#include "imgui.h"
#include "ImGUI/imgui_plot.h"

#include <algorithm>


// TODO: look into templated testing?

void dae::imgui_CacheTester::ThrashTheCacheWithGameObject3D()
{
	const size_t size = 1 << 20; // ~1 million objects
	std::vector<GameObject3DAlt> buffer(size);

	for (size_t currentStep{}; currentStep < m_stepSizes.size(); ++currentStep)
	{
		long long elapsedTime{};

		for (int sample = 0; sample < sampleCount; ++sample)
		{
			auto startTime = std::chrono::high_resolution_clock::now();

			for (auto it = buffer.begin(); it != buffer.end(); it += static_cast<size_t>(m_stepSizes[currentStep]))
			{
				it->ID *= 2;
			}

			auto endTime = std::chrono::high_resolution_clock::now();
			elapsedTime += std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
		}

		m_resultsGameObject[currentStep] = (float)(elapsedTime / sampleCount);
	}

	m_showGameObject = true;
}


void dae::imgui_CacheTester::ThrashTheCacheWithGameObject3DAlt()
{
	const size_t size = 1 << 20; // ~1 million objects
	std::vector<GameObject3DAlt> buffer(size);

	for (size_t currentStep{}; currentStep < m_stepSizes.size(); ++currentStep)
	{
		long long elapsedTime{};

		for (int sample{}; sample < sampleCount; ++sample)
		{
			auto startTime = std::chrono::high_resolution_clock::now();

			for (auto it = buffer.begin(); it != buffer.end(); it += static_cast<size_t>(m_stepSizes[currentStep]))
			{
				it->ID *= 2;
			}

			auto endTime = std::chrono::high_resolution_clock::now();
			elapsedTime += std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
		}

		m_resultsGameObjectAlt[currentStep] = (float)(elapsedTime / sampleCount);
	}

	m_showGameObjectAlt = true;
}

void dae::imgui_CacheTester::RenderCacheTestWindow()
{
    ImGui::Begin("Thrash the cache!");

    ImGui::InputInt("Samples", &sampleCount);
    if (sampleCount < 1)
        sampleCount = 1;

    if (ImGui::Button("Thrash using GameObject3D"))
        ThrashTheCacheWithGameObject3D();

    if (ImGui::Button("Thrash using 3DAlt"))
        ThrashTheCacheWithGameObject3DAlt();

    ImGui::Separator();



    const int count = static_cast<int>(m_stepSizes.size());
    static ImU32 colors[] =
    {
        IM_COL32(255, 0, 0, 255),
        IM_COL32(0, 255, 0, 255)
    };

    // Gameobject graph
    if (m_showGameObject && !m_resultsGameObject.empty())
    {
        const float* ys[] = { m_resultsGameObject.data() };

        auto maxY = *std::max_element(m_resultsGameObject.begin(), m_resultsGameObject.end());
        auto yPadding = maxY * .1f;

        ImGui::PlotConfig conf{};
        conf.values.xs = m_stepSizes.data();
        conf.values.count = count;
        conf.grid_x.show = true;
        conf.grid_y.show = true;
        conf.tooltip.show = true;
        conf.line_thickness = 2.f;

        conf.values.ys_list = ys;
        conf.values.ys_count = 1;
        conf.values.colors = colors;

        conf.scale.min = 0.f;
        conf.scale.max = maxY + yPadding;

        conf.frame_size = ImVec2(500, 200);

        ImGui::Plot("Results GameObject3D", conf);
    }

    // GameobjectAlt graph
    if (m_showGameObjectAlt && !m_resultsGameObjectAlt.empty())
    {
        const float* ys[] = { m_resultsGameObjectAlt.data() };

        auto maxY = *std::max_element(m_resultsGameObject.begin(), m_resultsGameObject.end());
        auto yPadding = maxY * .1f;

        ImGui::PlotConfig conf{};
        conf.values.xs = m_stepSizes.data();
        conf.values.count = count;
        conf.grid_x.show = true;
        conf.grid_y.show = true;
        conf.tooltip.show = true;
        conf.line_thickness = 2.f;

        conf.values.ys_list = ys;
        conf.values.ys_count = 1;
        conf.values.colors = &colors[1];

        conf.scale.min = 0.f;
        conf.scale.max = maxY + yPadding;

        conf.frame_size = ImVec2(500, 200);

        ImGui::Plot("Results GameObject3DAlt!", conf);
    }

    // Combined graph
    if (m_showGameObject && m_showGameObjectAlt &&
        !m_resultsGameObject.empty() &&
        !m_resultsGameObjectAlt.empty())
    {
        std::array<const float*, 2> ys =
        {
            m_resultsGameObject.data(),
            m_resultsGameObjectAlt.data()
        };

        auto maxY = *std::max_element(m_resultsGameObject.begin(), m_resultsGameObject.end());
        auto yPadding = maxY * .1f;

        ImGui::PlotConfig conf{};
        conf.values.xs = m_stepSizes.data();
        conf.values.count = count;
        conf.grid_x.show = true;
        conf.grid_y.show = true;
        conf.tooltip.show = true;
        conf.line_thickness = 2.f;

        conf.values.ys_list = ys.data();
        conf.values.ys_count = 2;
        conf.values.colors = colors;

        conf.scale.min = 0.f;
        conf.scale.max = maxY + yPadding;

        conf.frame_size = ImVec2(500, 300);

        ImGui::Plot("Comparison", conf);
    }

    ImGui::End();
}

dae::imgui_CacheTester::imgui_CacheTester()
	: m_showGameObject{false}
	, m_showGameObjectAlt{false}
	, m_resultsGameObject{}
	, m_resultsGameObjectAlt{}
{
	m_resultsGameObject.resize(m_stepSizes.size());
	m_resultsGameObjectAlt.resize(m_stepSizes.size());
}
