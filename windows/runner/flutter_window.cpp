#include "flutter_window.h"

#include <optional>

#include "flutter/generated_plugin_registrant.h"

FlutterWindow::FlutterWindow(const flutter::DartProject& project)
    : project_(project) {}

FlutterWindow::~FlutterWindow() {}

bool FlutterWindow::OnCreate() {
  if (!Win32Window::OnCreate()) {
    return false;
  }

  RECT frame = GetClientArea();

  // 创建Flutter控制器
  flutter_controller_ = std::make_unique<flutter::FlutterViewController>(
      frame.right - frame.left, frame.bottom - frame.top, project_);

  // 确保控制器的基本设置成功
  if (!flutter_controller_->engine() || !flutter_controller_->view()) {
    return false;
  }

  // 注册插件
  RegisterPlugins(flutter_controller_->engine());
  
  // 将Flutter视图内容设置为窗口的子内容
  SetChildContent(flutter_controller_->view()->GetNativeWindow());

  // 确保窗口在创建时显示
  this->Show();

  // 设置下一个帧的回调
  flutter_controller_->engine()->SetNextFrameCallback([&]() {
    // 可以在这里添加每帧需要更新的逻辑
  });

  return true;
}

void FlutterWindow::OnDestroy() {
  // 清理Flutter控制器
  if (flutter_controller_) {
    flutter_controller_ = nullptr;
  }

  // 调用基类的OnDestroy
  Win32Window::OnDestroy();
}

LRESULT FlutterWindow::MessageHandler(HWND hwnd, UINT const message,
                                      WPARAM const wparam,
                                      LPARAM const lparam) noexcept {
  // 给Flutter及其插件一个处理窗口消息的机会
  if (flutter_controller_) {
    std::optional<LRESULT> result =
        flutter_controller_->HandleTopLevelWindowProc(hwnd, message, wparam,
                                                      lparam);
    if (result) {
      return *result;
    }
  }

  // 处理特定的消息
  switch (message) {
    case WM_FONTCHANGE:
      flutter_controller_->engine()->ReloadSystemFonts();
      break;
  }

  return Win32Window::MessageHandler(hwnd, message, wparam, lparam);
}
