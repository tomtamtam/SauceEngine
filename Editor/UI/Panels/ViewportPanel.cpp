#include "ViewportPanel.h"

#include "Editor/UI/Core/Context.h"

#include "Editor/UI/Panels/Panel.h"

ViewportPanel::ViewportPanel(EditorContext &ctx)
	: Panel(ctx, "AssetBrowserPanel")
{}

void ViewportPanel::DrawItems(uint32_t width, uint32_t height)
{
    glm::mat4 view = {1.0f};
    glm::mat4 proj = {1.0f};

    view = glm::lookAt(m_Ctx.cam.transform.Translation, m_Ctx.cam.transform.Translation + m_Ctx.cam.transform.Rotation, m_Ctx.cam.cam.Up);
    proj = glm::perspective(glm::radians(m_Ctx.cam.cam.FOV), (float)width / (float)height, F_NEAR, F_FAR);

    auto meshView = m_Ctx.scene->m_Registry.view<Sauce::MeshInstance>();
    for(auto entity : meshView)
    {
        if(!m_Ctx.scene->m_Registry.get<Sauce::MeshInstance>(entity).IsVisible) continue;
        auto meshId = m_Ctx.scene->m_Registry.get<Sauce::MeshInstance>(entity).meshId;
        m_Ctx.renderer->Submit(m_Ctx.scene->m_Registry.get<Sauce::TransformComponent>(entity), view, proj, *m_Ctx.scene->p_AssetSystem->p_Meshes.at(meshId).mesh);
    }
}

void ViewportPanel::UpdateInput()
{
   float x = 0, y = 0, z = 0;

   m_Ctx.cam.cam.FOV -= 2000 * m_Ctx.dt * m_Ctx.inputSystem->GetMouseScrollY();
   if(m_Ctx.inputSystem->GetKeyDown(GLFW_KEY_W))
   {
       z -= 2 * m_Ctx.dt;
   }
   if(m_Ctx.inputSystem->GetKeyDown(GLFW_KEY_S))
   {
      z += 2 * m_Ctx.dt;
   }
   if(m_Ctx.inputSystem->GetKeyDown(GLFW_KEY_DOWN))
   {
       y -= 2 * m_Ctx.dt;
   }
   if(m_Ctx.inputSystem->GetKeyDown(GLFW_KEY_UP))
   {
      y += 2 * m_Ctx.dt;
   }
   if(m_Ctx.inputSystem->GetKeyDown(GLFW_KEY_D))
   {
       x += 2 * m_Ctx.dt;
   }
   if(m_Ctx.inputSystem->GetKeyDown(GLFW_KEY_A))
   {
       x -= 2 * m_Ctx.dt;
   }
   m_Ctx.cam.transform.Translate({x, y, z});

   if(m_Ctx.inputSystem->GetMouseButtonJustPressed(GLFW_MOUSE_BUTTON_RIGHT))
   {
       m_MousePos = m_Ctx.inputSystem->GetMousePos();
       m_PrevMousePos = m_Ctx.inputSystem->GetMousePos();
   }
   if(m_Ctx.inputSystem->GetMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
   {
       m_MousePos = m_Ctx.inputSystem->GetMousePos();
       m_Ctx.cam.transform.Rotation.x += (m_MousePos.x - m_PrevMousePos.x) * m_Ctx.dt * 10;
       m_Ctx.cam.transform.Rotation.y -= (m_MousePos.y - m_PrevMousePos.y) * m_Ctx.dt * 10;
       m_PrevMousePos = m_MousePos;
   }
}

void ViewportPanel::OnImGui()
{
   ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
   ImGui::Begin("Viewport", &open, ImGuiWindowFlags_NoScrollbar);
       ImGui::PopStyleVar();

       ImVec2 regionSize = ImGui::GetContentRegionAvail();
       ImVec2 originScreen = ImGui::GetCursorScreenPos();

       if(m_Ctx.frameBuffer->GetWidth() != (int)regionSize.x || m_Ctx.frameBuffer->GetHeight() != (int)regionSize.y)
       {
           m_Ctx.frameBuffer->Resize((int)regionSize.x, (int)regionSize.y);
       }

       m_Ctx.frameBuffer->Bind();
           glViewport(0, 0, (int)regionSize.x, regionSize.y);
           m_Ctx.renderer->Clear({0.5f, 0.2f, 0.6f, 1.0f});
           if(ImGui::IsWindowFocused()) UpdateInput();
           DrawItems((int)regionSize.x, (int)regionSize.y);
       m_Ctx.frameBuffer->Unbind();

       glViewport(0, 0, m_Ctx.renderer->GetWinWidth(), m_Ctx.renderer->GetWinHeight());
       ImTextureID texId = (ImTextureID)(intptr_t)m_Ctx.frameBuffer->GetColorAttachment();
       ImGui::Image(texId, regionSize, ImVec2(0, 1), ImVec2(1, 0));
   ImGui::End();
}

void ViewportPanel::OnUpdate()
{
}
