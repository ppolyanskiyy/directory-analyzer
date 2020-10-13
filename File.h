#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>

enum CharData : char
{
	BREAK = '\n',
	SINGLE_COMMENT_PART = '/',
	FULL_COMMENT_PART = '*',
	QUOTE = '\"',
	SLASH = '\\',
};

const short int SIZE_COMMENT_PART = 2;

class File
{
public:
	File() = default;
	File(const std::filesystem::path&);

	void set_file_path(const std::filesystem::path&);

	std::filesystem::path get_file_path() const;

	int get_blank_line_count() const;
	int get_commented_line_count() const;
	int get_physical_line_count() const;
	int get_code_line_count() const;

	void AnalyzeFile();
	void PrintFileAnalysis(std::ostream&)const;

private:
	bool IsBeginOfComment(size_t, char, char);
	bool IsEndOfFullComment(size_t);

	void CodeLineCount();
	void CommentedLineCount();
	void PhysicalLineCount();

	void TotalLineCount();

	std::filesystem::path m_file_path;
	std::string m_file_data{};
	size_t m_file_data_size{};
	int m_blank_line_count{},
		m_commented_line_count{},
		m_physical_line_count{},
		m_code_line_count{};
};


File::File(const std::filesystem::path& file_path)
{
	set_file_path(file_path);
}

void File::set_file_path(const std::filesystem::path& file_path)
{
	if (file_path.has_extension()) {
		m_file_path = file_path;
	}
	else {
		throw std::exception((file_path.string() + "Isn't file").c_str());
	}
}

std::filesystem::path File::get_file_path() const
{
	return m_file_path;
}

int File::get_blank_line_count() const
{
	return m_blank_line_count;
}

int File::get_commented_line_count() const
{
	return m_commented_line_count;
}

int File::get_physical_line_count() const
{
	return m_physical_line_count;
}

inline int File::get_code_line_count() const
{
	return m_code_line_count;
}

void File::AnalyzeFile()
{
	std::ifstream file(m_file_path, std::ios_base::in);
	if (file.is_open())
	{
		m_file_data = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
		m_file_data.shrink_to_fit();
		m_file_data_size = m_file_data.size();

		TotalLineCount();
		m_blank_line_count = m_physical_line_count -  m_code_line_count;

		file.close();
	}
	else throw std::exception("Unable to open file");
}

void File::PrintFileAnalysis(std::ostream&output) const
{
	output << m_file_path << "\n";
	output << "Count of commented lines		: " << m_commented_line_count << "\n";
	output << "Count of blank lines			: " << m_blank_line_count << "\n";
	output << "Count of code lines			: " << m_code_line_count << "\n";
	output << "Count of physical lines		: " << m_physical_line_count << "\n\n";
	
}

bool File::IsBeginOfComment(size_t index, char first_part, char second_part = SINGLE_COMMENT_PART)
{
	return first_part == m_file_data[index] &&
		second_part == m_file_data[index + 1] &&
		index < m_file_data_size;
}

bool File::IsEndOfFullComment(size_t index)
{
	return FULL_COMMENT_PART == m_file_data[index] &&
		SINGLE_COMMENT_PART == m_file_data[index + 1] &&
		index < m_file_data_size;
}

void File::PhysicalLineCount()
{
	m_physical_line_count = static_cast<int>(std::count_if(begin(m_file_data), end(m_file_data),
		[](const char& ch) {return BREAK == ch; }));

	++m_physical_line_count;
}

void File::CodeLineCount()
{
	size_t letter_index{};

	while (letter_index < m_file_data_size)
	{
		if (!iswspace(m_file_data[letter_index]))
		{
			while (BREAK != m_file_data[++letter_index]  && letter_index < m_file_data_size) {}
			++m_code_line_count;
		}
		else ++letter_index;
	}	
}

void File::CommentedLineCount()
{
	bool is_at_the_same_line = false;
	bool is_end_of_current_comment = false;

	size_t letter_index{};
	while (letter_index < m_file_data_size)
	{
		// To avoid counting :  string sample = "//not comment /*this too*/"
		if (SLASH == m_file_data[letter_index] && QUOTE == m_file_data[letter_index + 1] && letter_index < m_file_data_size )
		{
			while (QUOTE != m_file_data[++letter_index] && letter_index < m_file_data_size) {}
			++letter_index;
		}
		// Count "//" comments
		else if (IsBeginOfComment(letter_index, SINGLE_COMMENT_PART))
		{
			while (m_file_data[++letter_index] != BREAK && letter_index < m_file_data_size) {}
			++m_commented_line_count;
			++letter_index;
		}

		// Count "/*" comments
		else if (IsBeginOfComment(letter_index, SINGLE_COMMENT_PART, FULL_COMMENT_PART))
		{
			is_at_the_same_line = true;
			is_end_of_current_comment = false;

			++m_commented_line_count;
			letter_index += SIZE_COMMENT_PART;

			while (BREAK != m_file_data[letter_index])
			{
				if (IsEndOfFullComment(letter_index))
				{
					is_end_of_current_comment = true;
					letter_index += SIZE_COMMENT_PART;
				}
				else ++letter_index;
			}

			if (!is_end_of_current_comment)
			{
				++letter_index;
				while (!IsEndOfFullComment(letter_index))
				{
					if (BREAK == m_file_data[letter_index])
					{
						is_at_the_same_line = false;
						++m_commented_line_count;
					}
					++letter_index;
				}
				if (IsEndOfFullComment(letter_index)) ++m_commented_line_count;
				letter_index += SIZE_COMMENT_PART;
			}
		}
		else ++letter_index;
	}
}

void File::TotalLineCount()
{
	CodeLineCount();
	PhysicalLineCount();
	CommentedLineCount();
	m_file_data.clear();
}
