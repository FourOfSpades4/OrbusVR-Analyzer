#pragma once

#include <unordered_map>
#include <string>
#include <optional>
#include <expected>
#include <string_view>


enum class ConfigParserError {
	Ok,
	BadFile,
	BadSection,
	BadProperty,
};

class ConfigData {
public:
	ConfigData(ConfigData&& other);
	~ConfigData();
public:
	static constexpr std::string_view DEFAULT_SECTION = "global";

public:
	static std::expected<ConfigData, ConfigParserError> ParseFile(std::string_view path);
	static std::expected<ConfigData, ConfigParserError> ParseText(std::string_view config);

	std::optional<std::string_view> GetString(std::string_view section, std::string_view property);
	std::optional<int64_t> Geti64(std::string_view section, std::string_view property);

private:
	ConfigData();

private:
	static ConfigParserError ParseLine(std::string_view line, ConfigData* configData, std::string* currentSection);
	static ConfigParserError ParseSection(std::string_view line, ConfigData* configData, std::string* currentSection);
	static ConfigParserError ParseProperty(std::string_view line, ConfigData* configData, std::string* currentSection);

private:
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_Sections;
};
