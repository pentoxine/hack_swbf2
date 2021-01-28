#include "includes.h"

namespace Renderer
{
	ImFont* m_pFont = NULL;

	RenderView* pRenderer;
	bool isPlayerVisible = false;

	uint32_t redColor = 0xffff1f1f;
	uint32_t blueColor = 0xff7ea6ff;
	uint32_t greenColor = 0xff0c9d00;

	void Initialize()
	{
		std::cout << "Init renderer" << std::endl;
		// Shitty stuff
		ImGuiIO& io = ImGui::GetIO();

		// TODO ? custom font
		io.Fonts->AddFontDefault();
		m_pFont = io.Fonts->AddFontDefault();
	}

	void BeginScene()
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::Begin("##Backbuffer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);
	}

	void DrawScene()
	{
		// Color example: 0xfff54266

		if (!IsValidPtr(pRenderView)) return ;

		if (!IsValidPtr(pLocalPlayer)) return ;

		for (auto pPlayer : pClientArray->clients)
		{
			if (!IsValidPtr(pPlayer)) continue;

			if (pPlayer == pLocalPlayer) continue;

			ClientSoldier* pLocalSoldier = pLocalPlayer->controlledControllable;
			if (!IsValidPtr(pLocalSoldier)) continue;

			ClientSoldier* pSoldier = pPlayer->controlledControllable;
			if (!IsValidPtr(pSoldier)) continue;

			if (!IsValidPtr(pSoldier->clientSolderHealthComponent)) continue;
			if (pSoldier->clientSolderHealthComponent->health <= 0) continue;

			ClientSoldierPrediction* LocalSoldierPrediction = pLocalSoldier->clientSoldierPrediction;
			if (!IsValidPtr(LocalSoldierPrediction)) continue;

			ClientSoldierPrediction* PlayerSoldierPrediction = pSoldier->clientSoldierPrediction;
			if (!IsValidPtr(PlayerSoldierPrediction)) continue;

			uint32_t *Chosencolor;
			if (pPlayer->team == pLocalPlayer->team) { Chosencolor = &greenColor; }
			else { pSoldier->occluded ? Chosencolor = &blueColor : Chosencolor = &redColor; }

			DirectX::XMFLOAT3 head = PlayerSoldierPrediction->pos;
			head.y += pSoldier->heightoffset + .25;
			DirectX::XMFLOAT3 foot = PlayerSoldierPrediction->pos;

			if (utils::WorldToScreen(foot) && utils::WorldToScreen(head))
			{
				float heightoffset = utils::DistanceVec3(foot, head);
				if (settings::ESP::enabled) {
					if (pPlayer->team != pLocalPlayer->team && settings::ESP::ennemies)
					{
						float factor = (heightoffset / 5);

						DirectX::XMFLOAT3 m2 = DirectX::XMFLOAT3(head.x - factor, head.y, 0);
						DirectX::XMFLOAT3 m1 = DirectX::XMFLOAT3(head.x + factor, head.y, 0);
						DirectX::XMFLOAT3 m3 = DirectX::XMFLOAT3(foot.x - factor, foot.y, 0);
						DirectX::XMFLOAT3 m4 = DirectX::XMFLOAT3(foot.x + factor, foot.y, 0);

						RenderLine(ImVec2(m1.x, m1.y), ImVec2(m2.x, m2.y), *Chosencolor);
						RenderLine(ImVec2(m2.x, m2.y), ImVec2(m3.x, m3.y), *Chosencolor);
						RenderLine(ImVec2(m3.x, m3.y), ImVec2(m4.x, m4.y), *Chosencolor);
						RenderLine(ImVec2(m4.x, m4.y), ImVec2(m1.x, m1.y), *Chosencolor);
					}
				}
				
			}
		}
	}

	void EndScene()
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->PushClipRectFullScreen();

		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);
	}

	float RenderText(const std::string& text, const ImVec2& position, float size, uint32_t color, bool center)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (color >> 24) & 0xff;
		float r = (color >> 16) & 0xff;
		float g = (color >> 8) & 0xff;
		float b = (color) & 0xff;

		std::stringstream stream(text);
		std::string line;

		float y = 0.0f;
		int i = 0;

		while (std::getline(stream, line))
		{
			ImVec2 textSize = m_pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

			if (center)
			{
				window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());

				window->DrawList->AddText(m_pFont, size, { position.x - textSize.x / 2.0f, position.y + textSize.y * i }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), line.c_str());
			}
			else
			{
				window->DrawList->AddText(m_pFont, size, { (position.x) + 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				window->DrawList->AddText(m_pFont, size, { (position.x) - 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				window->DrawList->AddText(m_pFont, size, { (position.x) + 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				window->DrawList->AddText(m_pFont, size, { (position.x) - 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());

				window->DrawList->AddText(m_pFont, size, { position.x, position.y + textSize.y * i }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), line.c_str());
			}

			y = position.y + textSize.y * (i + 1);
			i++;
		}

		return y;
	}

	void RenderLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (color >> 24) & 0xff;
		float r = (color >> 16) & 0xff;
		float g = (color >> 8) & 0xff;
		float b = (color) & 0xff;

		window->DrawList->AddLine(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), thickness);
	}

	void RenderCircle(const ImVec2& position, float radius, uint32_t color, float thickness, uint32_t segments)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (color >> 24) & 0xff;
		float r = (color >> 16) & 0xff;
		float g = (color >> 8) & 0xff;
		float b = (color) & 0xff;

		window->DrawList->AddCircle(position, radius, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), segments, thickness);
	}

	void RenderCircleFilled(const ImVec2& position, float radius, uint32_t color, uint32_t segments)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (color >> 24) & 0xff;
		float r = (color >> 16) & 0xff;
		float g = (color >> 8) & 0xff;
		float b = (color) & 0xff;

		window->DrawList->AddCircleFilled(position, radius, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), segments);
	}

	void RenderRect(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags, float thickness)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (color >> 24) & 0xFF;
		float r = (color >> 16) & 0xFF;
		float g = (color >> 8) & 0xFF;
		float b = (color) & 0xFF;

		window->DrawList->AddRect(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags, thickness);
	}

	void RenderRectFilled(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (color >> 24) & 0xFF;
		float r = (color >> 16) & 0xFF;
		float g = (color >> 8) & 0xFF;
		float b = (color) & 0xFF;

		window->DrawList->AddRectFilled(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags);
	}

	void RenderImage(ID3D11ShaderResourceView* pTexture, const ImVec2& from, const ImVec2& to, uint32_t color)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (color >> 24) & 0xFF;
		float r = (color >> 16) & 0xFF;
		float g = (color >> 8) & 0xFF;
		float b = (color) & 0xFF;

		window->DrawList->AddImage(pTexture, from, to, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }));
	}

	void RenderImageRounded(ID3D11ShaderResourceView* pTexture, const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (color >> 24) & 0xFF;
		float r = (color >> 16) & 0xFF;
		float g = (color >> 8) & 0xFF;
		float b = (color) & 0xFF;

		window->DrawList->AddImageRounded(pTexture, from, to, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags);
	}
}

