#pragma once

#include <string>
#include <unordered_map>
#include <memory>

enum class Language {
    English,
    Chinese
};

class Localization {
private:
    static std::unique_ptr<Localization> instance;
    Language currentLanguage;
    std::unordered_map<std::string, std::unordered_map<Language, std::string>> translations;

    Localization();

public:
    static Localization& getInstance();
    
    void setLanguage(Language lang);
    Language getLanguage() const { return currentLanguage; }
    
    const char* getText(const std::string& key) const;
    
    // 根据系统locale自动检测语言
    static Language detectSystemLanguage();
    
    // 便捷宏
    #define TR(key) Localization::getInstance().getText(key)
    
private:
    void initTranslations();
};
