#pragma once
#include "File.h"
#include <thread>
#include <mutex>

const size_t MAIN_THREAD = 1;

class DirectoryAnalyzer
{
public:
	DirectoryAnalyzer() = default;
	DirectoryAnalyzer(const std::filesystem::path&);

	void set_directory_path(const std::filesystem::path& directory_path);

	int get_total_blank_line_count()const;
	int get_total_commented_line_count()const;
	int get_total_code_line_count()const;
	int get_total_physical_line_count()const;
	int get_processed_files_count()const;

	void AnalyzeDirectory();
	void PrintDirectoryAnalysis(std::ostream&)const;

private:
	bool IsRightExtension(const std::filesystem::path&);
	void CalculateDirectoryAnalysis(size_t, size_t);

	std::filesystem::path m_directory_path;
	std::vector<File> m_files;
	int m_total_blank_line_count{},
		m_total_commented_line_count{},
		m_total_code_line_count{},
		m_total_physical_line_count{},
		m_processed_files_count{};
	std::mutex m_mutex;
};


DirectoryAnalyzer::DirectoryAnalyzer(const std::filesystem::path& directory_path)
{
	set_directory_path(directory_path);
}

void DirectoryAnalyzer::set_directory_path(const std::filesystem::path& directory_path)
{
	if (std::filesystem::is_directory(directory_path))
	{
		m_directory_path = directory_path;
	}
	else throw std::exception((directory_path.string() + " Isn't directory").c_str());
}

int DirectoryAnalyzer::get_total_blank_line_count() const
{
	return m_total_blank_line_count;
}

int DirectoryAnalyzer::get_total_commented_line_count() const
{
	return m_total_commented_line_count;
}

int DirectoryAnalyzer::get_total_code_line_count() const
{
	return m_total_code_line_count;
}

int DirectoryAnalyzer::get_total_physical_line_count() const
{
	return m_total_physical_line_count;
}

int DirectoryAnalyzer::get_processed_files_count() const
{
	return m_processed_files_count;
}

void DirectoryAnalyzer::AnalyzeDirectory()
{
	for (const auto& entry : std::filesystem::recursive_directory_iterator(m_directory_path))
	{
		if (IsRightExtension(static_cast<std::filesystem::path>(entry)))
		{
			m_files.push_back(File(entry.path()));
			++m_processed_files_count;
		}
	}
	const size_t max_threads = std::thread::hardware_concurrency();
	const size_t step = m_processed_files_count / max_threads;

	size_t begin = 0;

	std::vector<std::thread>file_threads(max_threads - MAIN_THREAD); // const size_t MAIN_THREAD = 1

	for (auto& thread : file_threads)
	{
		thread = std::thread(&DirectoryAnalyzer::CalculateDirectoryAnalysis, this, begin, begin + step);
		begin += step;
	}

	CalculateDirectoryAnalysis(begin, m_processed_files_count);

	for (auto& thread : file_threads) { thread.join(); }
}

bool DirectoryAnalyzer::IsRightExtension(const std::filesystem::path& file)
{
	std::string extension = file.extension().string();
	return ".cpp" == extension || ".c" == extension || ".h" == extension || ".hpp" == extension;
}

void DirectoryAnalyzer::CalculateDirectoryAnalysis(size_t begin, size_t end)
{
	for (size_t i = begin; i < end; ++i)
	{
		m_files[i].AnalyzeFile();

		std::lock_guard<std::mutex> quard(m_mutex);	

		m_total_blank_line_count += m_files[i].get_blank_line_count();
		m_total_commented_line_count += m_files[i].get_commented_line_count();
		m_total_code_line_count += m_files[i].get_code_line_count();
		m_total_physical_line_count += m_files[i].get_physical_line_count();
	}
}

void DirectoryAnalyzer::PrintDirectoryAnalysis(std::ostream& output)const
{
	output << "Total count of commented lines		: " << m_total_commented_line_count << "\n";
	output << "Total count of blank lines			: " << m_total_blank_line_count << "\n";
	output << "Total count of code lines			: " << m_total_code_line_count << "\n";
	output << "Total count of physical lines		: " << m_total_physical_line_count << "\n";
	output << "Count of processed files				: " << m_processed_files_count << "\n\n\n";

	for (auto& file : m_files)
	{
		file.PrintFileAnalysis(output);
	}
}
