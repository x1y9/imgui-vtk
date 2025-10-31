#include "Localization.h"
#include <locale>
#include <cstring>

std::unique_ptr<Localization> Localization::instance = nullptr;

Localization::Localization() : currentLanguage(detectSystemLanguage()) {
    initTranslations();
}

Localization& Localization::getInstance() {
    if (!instance) {
        instance = std::unique_ptr<Localization>(new Localization());
    }
    return *instance;
}

void Localization::setLanguage(Language lang) {
    currentLanguage = lang;
}

const char* Localization::getText(const std::string& key) const {
    auto it = translations.find(key);
    if (it != translations.end()) {
        auto langIt = it->second.find(currentLanguage);
        if (langIt != it->second.end()) {
            return langIt->second.c_str();
        }
    }
    // 如果找不到翻译，返回key本身
    return key.c_str();
}

Language Localization::detectSystemLanguage() {
    try {
        // 尝试获取系统locale
        std::locale loc("");
        std::string localeName = loc.name();
        
        // 检查locale字符串
        if (localeName.find("zh") != std::string::npos || 
            localeName.find("Chinese") != std::string::npos) {
            return Language::Chinese;
        }
    }
    catch (...) {
        // 如果获取locale失败，使用环境变量
        const char* lang = std::getenv("LANG");
        if (lang) {
            std::string langStr(lang);
            if (langStr.find("zh") != std::string::npos) {
                return Language::Chinese;
            }
        }
    }
    
    // 默认返回英文
    return Language::English;
}

void Localization::initTranslations() {
    // 格式: {key, {English, Chinese}}
    const std::pair<std::string, std::pair<const char*, const char*>> translationData[] = {
        // 窗口标题
        {"app.title",           {"Dear ImGui VTK Viewer Example", "ImGui VTK 查看器示例"}},
        {"window.hello",        {"Hello, world!", "你好，世界！"}},
        {"window.another",      {"Another Window", "另一个窗口"}},
        {"window.vtk1",         {"VTK Viewer 1", "VTK 查看器 1"}},
        {"window.vtk2",         {"VTK Viewer 2", "VTK 查看器 2"}},
        {"window.chart",        {"Chart Window", "图表窗口"}},
        {"window.table",        {"Table Window", "表格窗口"}},
        {"window.performance",  {"Performance Settings", "性能设置"}},
        
        // 菜单
        {"menu.file",           {"File", "文件"}},
        {"menu.view",           {"View", "视图"}},
        {"menu.language",       {"Language", "语言"}},
        
        // 性能设置
        {"perf.vsync",          {"Enable VSync", "启用垂直同步"}},
        {"perf.fps",            {"FPS", "帧率"}},
        {"perf.frametime",      {"Frame Time", "帧时间"}},
        
        // VTK背景
        {"vtk.bg.black",        {"VTK Background: Black", "VTK 背景：黑色"}},
        {"vtk.bg.red",          {"VTK Background: Red", "VTK 背景：红色"}},
        {"vtk.bg.green",        {"VTK Background: Green", "VTK 背景：绿色"}},
        {"vtk.bg.blue",         {"VTK Background: Blue", "VTK 背景：蓝色"}},
        {"vtk.bg.alpha",        {"Background Alpha", "背景透明度"}},
        
        // 通用按钮
        {"button.close",        {"Close", "关闭"}},
        {"button.open",         {"Open", "打开"}},
        
        // HelloWindow
        {"hello.text",          {"This is some useful text.", "这是一些有用的文本。"}},
        {"hello.demo",          {"Demo Window", "演示窗口"}},
        {"hello.button",        {"Button", "按钮"}},
        {"hello.counter",       {"counter", "计数器"}},
        {"hello.clearcolor",    {"clear color", "清除颜色"}},
    };
    
    // 批量初始化
    for (const auto& item : translationData) {
        translations[item.first] = {
            {Language::English, item.second.first},
            {Language::Chinese, item.second.second}
        };
    }
}
