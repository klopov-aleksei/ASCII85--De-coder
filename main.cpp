#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <cstdlib>
#include <cctype>
#include <cstring>

constexpr std::array<unsigned int, 5> POW85{ 85*85*85*85, 85*85*85, 85*85, 85, 1 };

void encodeBuffered() 
{
    std::vector<unsigned char> input{ (std::istreambuf_iterator<char>(std::cin)), std::istreambuf_iterator<char>() };
    size_t i{ 0 };
    while (i < input.size()) 
    {
        unsigned int block{ 0 };
        int n{ 0 };
        for (; n < 4 && i < input.size(); n++, i++) 
        {
            block = (block << 8) | input[i];
        }
        if (n < 4) 
            block <<= (8 * (4 - n));
        if(n == 4 && block == 0) 
        {
            std::cout << 'z';
        } 
        else 
        {
            char encoded[5];
            for (int j = 4; j >= 0; j--) 
            {
                encoded[j] = (block % 85) + 33;
                block /= 85;
            }
            int outCount{ (n < 4) ? n + 1 : 5 };
            std::cout.write(encoded, outCount);
        }
    }
}

void decodeBuffered() 
{
    std::vector<unsigned char> output;
    std::vector<int> group;
    char ch;
    while (std::cin.get(ch)) 
    {
        if (std::isspace(static_cast<unsigned char>(ch)))
            continue;
        if (ch == 'z') 
        {
            if (!group.empty())
            {
                std::cerr << "Ошибка: 'z' встречено в середине группы.\n";
                std::exit(1);
            }
            output.push_back(0);
            output.push_back(0);
            output.push_back(0);
            output.push_back(0);
        } 
        else 
        {
            if (ch < '!' || ch > 'u')
            {
                std::cerr << "Ошибка: Недопустимый символ '" << ch << "'." << '\n';
                std::exit(1);
            }

            group.push_back(ch - 33);
            if (group.size() == 5) 
            {
                // Обработка группы
                unsigned int value{ 0 };
                for (int i = 0; i < 5; i++) 
                {
                    value += group[i] * POW85[i];
                }
                output.push_back((value >> 24) & 0xFF);
                output.push_back((value >> 16) & 0xFF);
                output.push_back((value >> 8)  & 0xFF);
                output.push_back(value & 0xFF);
                group.clear();
            }
        }
    }
    
    if(!group.empty()) {
        int missing = 5 - group.size();
        
        for (int i = 0; i < missing; i++) 
        {
            group.push_back(84);
        }
        unsigned int value{ 0 };
        for (int i = 0; i < 5; i++) 
        {
            value += group[i] * POW85[i];
        }

        int byteCount = group.size() - 1 - missing;
        output.push_back((value >> 24) & 0xFF);
        if (byteCount >= 2) output.push_back((value >> 16) & 0xFF);
        if (byteCount >= 3) output.push_back((value >> 8)  & 0xFF);
    }

    for (unsigned char byte : output)
        std::cout << byte;
}

void encodeStream() 
{
    constexpr size_t chunkSize{ 4096 };
    char inBuffer[chunkSize];
    unsigned char block[4];
    int leftoverCount{ 0 };

    while (true) 
    {
        std::cin.read(inBuffer, chunkSize);
        std::streamsize bytesRead{ std::cin.gcount() };
        if (bytesRead <= 0)
            break;
        size_t pos{ 0 };
        while(pos < static_cast<size_t>(bytesRead)) 
        {
            while(leftoverCount < 4 && pos < static_cast<size_t>(bytesRead)) 
            {
                block[leftoverCount++] = inBuffer[pos++];
            }
            if(leftoverCount == 4) 
            {
                unsigned int value = (block[0] << 24) | (block[1] << 16) | (block[2] << 8) | block[3];
                if(value == 0) 
                {
                    std::cout.put('z');
                } 
                else 
                {
                    char encoded[5];
                    unsigned int temp{ value };
                    for (int j{ 4 }; j >= 0; j--) 
                    {
                        encoded[j] = (temp % 85) + 33;
                        temp /= 85;
                    }
                    std::cout.write(encoded, 5);
                }
                leftoverCount = 0;
            }
        }
    }

    if(leftoverCount > 0) 
    {
        for (int i = leftoverCount; i < 4; i++) 
        {
            block[i] = 0;
        }
        unsigned int value = (block[0] << 24) | (block[1] << 16) |
                             (block[2] << 8) | block[3];
        char encoded[5];
        unsigned int temp = value;
        for (int j = 4; j >= 0; j--) 
        {
            encoded[j] = (temp % 85) + 33;
            temp /= 85;
        }
        std::cout.write(encoded, leftoverCount + 1);
    }
}

void decodeStream() 
{
    constexpr size_t chunkSize{ 4096 };
    char inBuffer[chunkSize];
    std::vector<int> group;
    
    while (true) 
    {
        std::cin.read(inBuffer, chunkSize);
        std::streamsize bytesRead = std::cin.gcount();
        if (bytesRead <= 0)
            break;
        for (int i = 0; i < bytesRead; i++) 
        {
            char ch = inBuffer[i];
            if(std::isspace(static_cast<unsigned char>(ch)))
                continue;
            if (ch == 'z') 
            {
                if (!group.empty())
                {
                    std::cerr << "Ошибка: 'z' встречено в середине группы.\n";
                    std::exit(1);
                }
                unsigned char zeros[4] = {0, 0, 0, 0};
                std::cout.write(reinterpret_cast<const char*>(zeros), 4);
            } 
            else 
            {
                if (ch < '!' || ch > 'u')
                {
                    std::cerr << "Ошибка: Недопустимый символ '" << ch << "'." << '\n';
                    std::exit(1);
                }
                group.push_back(ch - 33);
                if(group.size() == 5) 
                {
                    unsigned int value = 0;
                    for (int j = 0; j < 5; j++)
                        value += group[j] * POW85[j];
                    char out[4];
                    out[0] = (value >> 24) & 0xFF;
                    out[1] = (value >> 16) & 0xFF;
                    out[2] = (value >> 8)  & 0xFF;
                    out[3] = value & 0xFF;
                    std::cout.write(out, 4);
                    group.clear();
                }
            }
        }
    }
    if(!group.empty())
    {
        int missing = 5 - group.size();
        for (int i = 0; i < missing; i++) 
        {
            group.push_back(84);
        }
        unsigned int value = 0;
        for (int j = 0; j < 5; j++) 
        {
            value += group[j] * POW85[j];
        }
        int outputBytes{ 5 - missing - 1 };
        char out[4];
        out[0] = (value >> 24) & 0xFF;
        if(outputBytes >= 2) out[1] = (value >> 16) & 0xFF;
        if(outputBytes >= 3) out[2] = (value >> 8)  & 0xFF;
        std::cout.write(out, outputBytes);
    }
}

#ifndef UNIT_TEST

int main(int argc, char* argv[]) 
{
    bool decodeFlag{ false };
    bool streamMode{ false };
    
    for (int i = 1; i < argc; i++) 
    {
        if (std::strcmp(argv[i], "-d") == 0)
            decodeFlag = true;
        else if(std::strcmp(argv[i], "-e") == 0)
            decodeFlag = false;
        else if(std::strcmp(argv[i], "-s") == 0)
            streamMode = true;
        else 
        {
            std::cerr << "Использование: " << argv[0] << " [-e|-d] [-s]" << std::endl;
            return 1;
        }
    }

    
    if(streamMode) 
    {
        if(decodeFlag)
            decodeStream();
        else
            encodeStream();
    } 
    else 
    {
        if(decodeFlag)
            decodeBuffered();
        else
            encodeBuffered();
    }
    std::cout << '\n';

    return 0;
}

#endif