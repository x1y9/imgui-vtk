# 多语言支持

## 功能

- 🇬🇧 English / 🇨🇳 中文
- 自动检测系统语言
- 运行时切换语言

## 使用方法

### 在代码中使用翻译

```cpp
#include "Localization.h"

// 使用TR宏
ImGui::Begin(TR("window.hello"));
ImGui::Button(TR("button.close"));
ImGui::Text("%s: %.1f", TR("perf.fps"), fps);
```

### 添加新翻译

在 `Localization.cpp` 的 `initTranslations()` 中添加：

```cpp
translations["your.key"] = {
    {Language::English, "English Text"},
    {Language::Chinese, "中文文本"}
};
```

### 切换语言

在"性能设置"窗口点击"Language"菜单选择语言，或代码中调用：

```cpp
Localization::getInstance().setLanguage(Language::Chinese);
```

## 已翻译的界面

- 窗口标题（VTK查看器、性能设置等）
- 性能设置（VSync、帧率、帧时间）
- VTK背景颜色按钮
- 语言切换菜单
