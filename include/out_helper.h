#pragma once

#include <string>
#include <sstream>
#include <vector>

namespace out_helper
{
	namespace
	{
#ifdef _MSC_VER
		std::string exec(std::string_view cmd)
		{
			std::array<char, 128> buffer{};
			std::string result;
			std::shared_ptr<FILE> pipe(_popen(cmd.data(), "r"), _pclose);
			if (!pipe)
				throw std::runtime_error("popen() failed!");
			while (!feof(pipe.get()))
			{
				if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
					result += buffer.data();
			}
			return result;
		}

		bool can_color()
		{
			static int colourable = -1;
			if (colourable < 0)
			{
				colourable = 0;
				std::string result = exec("ver");
				int start_index = result.find("[Version ");
				int end_index = result.rfind("]");
				if (start_index > 0 && end_index > start_index)
				{
					std::istringstream version(result.substr(start_index + 9, end_index - start_index - 9));
					std::vector<int> versions;
					std::string s;
					while (getline(version, s, '.'))
					{
						versions.push_back(std::stoi(s));
					}

					if (versions.size() == 4)
					{
						int major_minor = (versions[0] * 100) + versions[1];
						int build = versions[2];
						int revision = versions[3];
						if (major_minor >= 1000)
						{
							if (build > 10586)
								colourable = 1;
							else if (build == 10586 && revision >= 11)
								colourable = 1;
						}
					}
				}
			}
			return colourable != 0;
		}
#else
		bool can_color()
		{
			return true;
		}
#endif
	}

	enum class Color
	{
		Ignore = -1,
		Black = 30,
		Red = 31,
		Green = 32,
		Yellow = 33,
		Blue = 34,
		Magenta = 35,
		Cyan = 36,
		White = 37
	};

	enum class Style
	{
		Ignore = -1,
		None = 0,
		Bold = 1,
		Underline = 4,
		Inverse = 7,
		BoldOff = 21,
		UnderlineOff = 24,
		InverseOff = 27
	};

	std::string format_color(Color foreground = Color::Ignore, Color background = Color::Ignore, Style style = Style::Ignore, Style otherStyle = Style::Ignore)
	{
		if (can_color())
		{
			if (foreground == Color::Ignore && background == Color::Ignore && style == Style::Ignore && otherStyle == Style::Ignore)
				return "\033[0m";
			else
			{
				std::string val = "\033[";
				std::string separator = "";
				if (foreground != Color::Ignore)
				{
					val += std::to_string((int)foreground);
					separator = ";";
				}
				if (background != Color::Ignore)
				{
					val += separator;
					val += std::to_string(((int)background) + 10);
					separator = ";";
				}
				if (style != Style::Ignore)
				{
					val += separator;
					val += std::to_string((int)style);
					separator = ";";
				}
				if (otherStyle != Style::Ignore)
				{
					val += separator;
					val += std::to_string((int)otherStyle);
				}
				val += "m";
				return val;
			}
		}
		else
			return "";
	}
}
