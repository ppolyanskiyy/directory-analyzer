#include <ctime>
#include "DirectoryAnalyzer.h"	

int main()
{
	std::ofstream results(R"(results.txt)");
	std::filesystem::path directory(R"(Enter Directory)");

	if (results.is_open())
	{
		try
		{
			clock_t before = clock();
			DirectoryAnalyzer analyzer(directory);
			analyzer.AnalyzeDirectory();
			analyzer.PrintDirectoryAnalysis(results);
			clock_t after = clock();
			clock_t result_time = after - before;
			results << "Speed -> " << result_time << " ms.\n";
			std::cout << "Speed -> " << result_time << " ms.\n";
			results.close();
		}
		catch (std::exception& ex)
		{
			std::cerr << ex.what() << "\n";
		}
		catch (...)
		{
			std::cerr << "Error\n";
		}
	}
	else
	{
		std::cerr << "Unable to open file\n";
	}



	return 0;
}