#pragma once

#include <filesystem>

namespace gle
{
/**
 * @brief Read all content from a file into a string
 * @param filepath The filepath
 * @returns String containing the file's contents
 * @note Based on https://stackoverflow.com/questions/116038/how-do-i-read-an-entire-file-into-a-stdstring-in-c#116220
 */
std::string ReadWholeFile(std::filesystem::path const &filepath);
} // namespace gle
