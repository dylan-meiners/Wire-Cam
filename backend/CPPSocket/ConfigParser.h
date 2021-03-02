#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

static const char * NULL_TERMINATOR_C_STR = "\0";

class ConfigParser {

    public:

        ConfigParser(const std::string pathToConfigFile) {

            std::cout << ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl;
            std::cout << std::endl;
            std::cout << "Reading in config file at \"" << pathToConfigFile << "\"..." << std::endl;
            std::stringstream buffer;
            buffer << std::ifstream(pathToConfigFile).rdbuf();
            std::string rawStr = buffer.str();
            m_raw = rawStr.c_str();
            std::cout << "Found the following in the config file: " << std::endl << std::endl;
            std::cout << "===========================================================================================================" << std::endl;
            std::cout << rawStr << std::endl;
            std::cout << "===========================================================================================================" << std::endl << std::endl;

            bool searching = true;
            bool lookingForEndl = true;
            const char * ptrStart = m_raw;
            const char * ptr = m_raw;
            int chars = 0;
            char * bufferTwo;
            int length = strlen(m_raw);
            while (searching) {
                
                if (*ptr == '=') {

                    chars = ptr - ptrStart;
                    bufferTwo = (char *)malloc(chars * sizeof(char) + 1);
                    memcpy(bufferTwo, ptrStart, chars * sizeof(char));
                    memcpy(bufferTwo + chars, NULL_TERMINATOR_C_STR, sizeof(char));
                    options.push_back(std::string(bufferTwo));
                    free(bufferTwo);
                    ptrStart += chars + 1;
                    ++ptr;
                    ptrStart = ptr;
                    ++ptr;
                    while (lookingForEndl) {

                        if (*ptr == '\n') {

                            chars = ptr - ptrStart;
                            bufferTwo = (char *)malloc(chars * sizeof(char) + 1);
                            memcpy(bufferTwo, ptrStart, chars * sizeof(char));
                            memcpy(bufferTwo + chars, NULL_TERMINATOR_C_STR, sizeof(char));
                            configChoices.push_back(std::string(bufferTwo));
                            free(bufferTwo);
                            ptrStart += chars + 1;
                            lookingForEndl = false;
                        }
                        else ++ptr;
                    }
                    lookingForEndl = true;
                }

                if (ptr - m_raw < length) ++ptr;
                else searching = false;
            }
            std::cout << ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl;
        }

        std::string GetMatchingChoice(std::string option) {

            for (unsigned int i = 0; i < options.size(); i++) {

                if (option == options[i]) return configChoices[i];
            }
            return "";
        }

        void GetConfigChoice(std::string option, int * result) {
            
            std::string::size_type sz;
            *result = std::stoi(GetMatchingChoice(option), &sz);
        }
        void GetConfigChoice(std::string option, bool * result) { *result = GetMatchingChoice(option) == "true"; }

    private:

        const char * m_raw;
        std::vector<std::string> options;
        std::vector<std::string> configChoices;
};