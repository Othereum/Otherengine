#include <filesystem>
#include <iostream>


int main(int argc, char* argv[]) try
{
    if (argc < 2)
        throw std::invalid_argument{"no input files"};

    for (auto i = 1; i < argc; ++i)
        try
        {
            const std::filesystem::path path = argv[i];

            if (!is_regular_file(path))
                throw std::invalid_argument{"not a file"};

            std::cout << path.extension() << '\n';
        }
        catch (const std::exception& e)
        {
            std::cout << "error: " << argv[i] << ": " << e.what() << '\n';
        }
}
catch (const std::exception& e)
{
    std::cout << "fatal error: " << e.what() << '\n';
}
