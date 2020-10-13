#include "pch.h"
#include "CppUnitTest.h"
#include"..\Task3\File.h"
#include"..\Task3\DirectoryAnalyzer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(ProjectAnalyzerUnitTests)
	{
		File file_to_test;
		DirectoryAnalyzer directory_to_test;

	public:
		TEST_METHOD_INITIALIZE(Init)
		{
			file_to_test.set_file_path("D:/Learning/SoftServe Projects/Task3/Task3/to_test.txt");
			file_to_test.AnalyzeFile();

			directory_to_test.set_directory_path("D:/Learning/SoftServe Projects/Task3/Task3/");
			directory_to_test.AnalyzeDirectory();
		}

		BEGIN_TEST_METHOD_ATTRIBUTE(FileBlankLineCountTest)
			TEST_OWNER(L"Andriy Polyanskiy")
			TEST_PRIORITY(3)
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(FileBlankLineCountTest)
		{
			int blank = file_to_test.get_blank_line_count();
			Assert::AreEqual(9, blank);
		}

		BEGIN_TEST_METHOD_ATTRIBUTE(FileCommentedLineCountTest)
			TEST_OWNER(L"Andriy Polyanskiy")
			TEST_PRIORITY(3)
			END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(FileCommentedLineCountTest)
		{
			int commented = file_to_test.get_commented_line_count();
			Assert::AreEqual(13, commented);
		}

		BEGIN_TEST_METHOD_ATTRIBUTE(FilePhysicalBlankLineCountTest)
			TEST_OWNER(L"Andriy Polyanskiy")
			TEST_PRIORITY(2)
			END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(FilePhysicalBlankLineCountTest)
		{
			int physical = file_to_test.get_physical_line_count();
			Assert::AreEqual(24, physical);

		}

		BEGIN_TEST_METHOD_ATTRIBUTE(FileCodeLineCountTest)
			TEST_OWNER(L"Andriy Polyanskiy")
			TEST_PRIORITY(2)
			END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(FileCodeLineCountTest)
		{
			int code = file_to_test.get_code_line_count();
			Assert::AreEqual(15, code);

		}

		BEGIN_TEST_METHOD_ATTRIBUTE(DirectoryTotalCodeLineCountTest)
			TEST_OWNER(L"Andriy Polyanskiy")
			TEST_PRIORITY(1)
			END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(DirectoryTotalCodeLineCountTest)
		{
			int total_code = directory_to_test.get_total_code_line_count();
			Assert::AreEqual(345, total_code);

		}

		BEGIN_TEST_METHOD_ATTRIBUTE(DirectoryTotalBlankLineCountTest)
			TEST_OWNER(L"Andriy Polyanskiy")
			TEST_PRIORITY(2)
			END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(DirectoryTotalBlankLineCountTest)
		{
			int total_blank = directory_to_test.get_total_blank_line_count();
			Assert::AreEqual(74, total_blank);

		}

		BEGIN_TEST_METHOD_ATTRIBUTE(DirectoryTotalCommentedLineCountTest)
			TEST_OWNER(L"Andriy Polyanskiy")
			TEST_PRIORITY(2)
			END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(DirectoryTotalCommentedLineCountTest)
		{
			int total_commented = directory_to_test.get_total_commented_line_count();
			Assert::AreEqual(5, total_commented);

		}

		BEGIN_TEST_METHOD_ATTRIBUTE(DirectoryTotalPhysicalLineCountTest)
			TEST_OWNER(L"Andriy Polyanskiy")
			TEST_PRIORITY(1)
			END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(DirectoryTotalPhysicalLineCountTest)
		{
			int total_physical = directory_to_test.get_total_physical_line_count();
			Assert::AreEqual(419, total_physical);
		}	
	};
}
