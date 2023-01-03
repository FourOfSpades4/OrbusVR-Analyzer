#include "Tools/ConfigParser.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>
#include <charconv>

static std::string_view TrimString(std::string_view s)
{
	while (s.starts_with(' ')) {
		s.remove_prefix(1);
	}

	while (s.ends_with(' ')) {
		s.remove_suffix(1);
	}

	return s;
}

ConfigData::ConfigData()
{
}

ConfigData::ConfigData(ConfigData&& other)
{
	m_Sections = std::move(other.m_Sections);
}

ConfigData::~ConfigData()
{
}

std::expected<ConfigData, ConfigParserError> ConfigData::ParseFile(std::string_view path)
{
	// Is this the best way to read files in C++? Probably not. But I am too lazy to care.
	std::stringstream stream;
	std::ifstream input_file(path.data());

	if (!input_file.is_open() || input_file.bad()) {
		return std::unexpected(ConfigParserError::BadFile);
	}

	stream << input_file.rdbuf();

	return ParseText(stream.str());
}

std::expected<ConfigData, ConfigParserError> ConfigData::ParseText(std::string_view config)
{
	std::string currentSection(ConfigData::DEFAULT_SECTION);
	ConfigData configData;

	size_t curIndex = 0;
	while ((curIndex = config.find('\n')) != std::string::npos) {
		std::string_view line = config.substr(0, curIndex);

		if (!line.empty()) {
			ConfigParserError ret = ParseLine(line, &configData, &currentSection);

			if (ret != ConfigParserError::Ok) {
				return std::unexpected(ret);
			}
		}

		config = config.substr(curIndex + 1);
	}

	if (!config.empty()) {
		ConfigParserError ret = ParseLine(config, &configData, &currentSection);

		if (ret != ConfigParserError::Ok) {
			return std::unexpected(ret);
		}
	}


	return configData;
}

ConfigParserError ConfigData::ParseLine(std::string_view line, ConfigData* configData, std::string* currentSection)
{
	line = TrimString(line);

	if (line.empty()) {
		return ConfigParserError::Ok;
	}

	// When the first character is a ; the line is ignored
	if (line.starts_with(';')) {
		return ConfigParserError::Ok;
	}
	else if (line.starts_with('[')) {
		ConfigParserError ret = ParseSection(line, configData, currentSection);

		if (ret != ConfigParserError::Ok) {
			return ret;
		}
	}
	else {
		ConfigParserError ret = ParseProperty(line, configData, currentSection);

		if (ret != ConfigParserError::Ok) {
			return ret;
		}
	}

	return ConfigParserError::Ok;
}

ConfigParserError ConfigData::ParseSection(std::string_view line, ConfigData* configData, std::string* currentSection)
{
	size_t ending_bracket = line.find_last_of(']');

	if (ending_bracket == std::string::npos) {
		return ConfigParserError::BadSection;
	}

	std::string_view section_name = line.substr(1, ending_bracket - 1);

	if (section_name.empty()) {
		return ConfigParserError::BadSection;
	}

	*currentSection = std::move(section_name);

	return ConfigParserError::Ok;
}

ConfigParserError ConfigData::ParseProperty(std::string_view line, ConfigData* configData, std::string* currentSection)
{
	size_t equal_sign = line.find('=');

	if (equal_sign == std::string::npos) {
		return ConfigParserError::BadProperty;
	}

	std::string_view identifier = TrimString(line.substr(0, equal_sign - 1));
	std::string_view value = TrimString(line.substr(equal_sign + 1));

	configData->m_Sections[*currentSection][std::string(identifier)] = value;

	return ConfigParserError::Ok;
}

std::optional<std::string_view> ConfigData::GetString(std::string_view section, std::string_view property)
{
	auto prop = m_Sections.find(section.data());
	if (prop != m_Sections.end()) {
		auto ident = prop->second.find(property.data());
		if (ident != prop->second.end()) {
			return ident->second;
		}

		return std::nullopt;
	}

	return std::nullopt;
}

std::optional<int64_t> ConfigData::Geti64(std::string_view section, std::string_view property)
{
	auto as_string = GetString(section, property);

	if (as_string.has_value()) {
		std::string_view unwrapped = as_string.value();

		int64_t converted;
		auto result = std::from_chars(unwrapped.data(), unwrapped.data() + unwrapped.length(), converted);
		
		if (result.ec == std::errc::invalid_argument) { 
			return std::nullopt;
		}

		return converted;
	}

	return std::nullopt;
}