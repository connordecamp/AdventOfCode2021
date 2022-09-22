/*
 * Day 16: Packet Decoder
 */

#include <bits/stdc++.h>

struct Packet {
    uint8_t version = 0;
    uint8_t type = 0;
    
    bool isLiteral = false;
    uint64_t literal = 0;

    std::vector<Packet> subpackets {};
};

uint64_t traversePacketTree(Packet* head) {
    if(!head) return 0;

    uint64_t answer = head->version;

    for(size_t i = 0; i < head->subpackets.size(); i++) {
        answer += traversePacketTree(&head->subpackets[i]);
    }

    return answer;
}

Packet parse(std::vector<uint8_t>& bits);

std::string bytesToBitString(const std::vector<unsigned char>& bytes)
{
    std::stringstream answer;

    auto hexCharToInt = [](unsigned char byte){
        switch(byte) {
        case 'A': return 10;
        case 'B': return 11;
        case 'C': return 12;
        case 'D': return 13;
        case 'E': return 14;
        case 'F': return 15;
        default:
            return byte - '0';
        }
    };

    const std::string HexValues[] {
        "0000", "0001", "0010", "0011",
        "0100", "0101", "0110", "0111",
        "1000", "1001", "1010", "1011",
        "1100", "1101", "1110", "1111"};

    for(unsigned char byte : bytes) {
        byte = hexCharToInt(byte);
        answer << HexValues[byte];
    }

    return answer.str();
}


enum PacketType {
    Literal = 4,
};

uint64_t parseNBits(std::vector<uint8_t>& bits, size_t N)
{
    int result = 0;
    
    for(size_t i = 0; i < N - 1; i++) {
        result |= bits.back();
        bits.pop_back();
        result <<= 1;
    }
    result |= bits.back();
    bits.pop_back();

    return result;
}

uint64_t parseLiteral(std::vector<uint8_t>& bits)
{
    // Parse groups of 5 bits until we find a group
    // that starts with a zero
    constexpr size_t BitGroupSize = 4;
    uint8_t currentBit{};
    std::vector<uint64_t> bitGroups;

    do {
        currentBit = bits.back();
        bits.pop_back();
        bitGroups.push_back(parseNBits(bits, BitGroupSize));       
    }while(currentBit == 1);


    int result = 0;
    size_t shiftAmount = (bitGroups.size() - 1) * BitGroupSize;
    for(uint64_t val : bitGroups) {
        result |= (val << shiftAmount);
        shiftAmount -= BitGroupSize;
    }
    
    return result;
}

std::vector<Packet> parseOperator(std::vector<uint8_t>& bits)
{
    int lengthType = parseNBits(bits, 1);

    bool shouldReadFifteenBits = (lengthType == 0);
    bool shouldReadElevenBits = (lengthType == 1);

    std::vector<Packet> subpackets;

    if(shouldReadFifteenBits) {
        size_t lengthOfAllSubpackets = parseNBits(bits, 15);
        size_t startingPosition = bits.size();

        // (Starting position - current bit vector size) gives us how
        // many bits were processed from the top-level parse() call
        // Keep processing subpackets until we reach the given length

        while((startingPosition - bits.size()) < lengthOfAllSubpackets)
            subpackets.push_back(parse(bits));
        
    }
    else if(shouldReadElevenBits) {
        int number = parseNBits(bits, 11);
        for(int i = 0; i < number; i++)
            subpackets.push_back(parse(bits));
    }

    return subpackets;
}

Packet parse(std::vector<uint8_t>& bits)
{
    Packet p;
    p.version = parseNBits(bits, 3);
    p.type = parseNBits(bits, 3);
    
    if(p.type == PacketType::Literal) {
        p.isLiteral = true;
        p.literal = parseLiteral(bits);
    }else {
        std::vector<Packet> subpackets;
        subpackets = parseOperator(bits);
        p.subpackets = std::move(subpackets);
    }

    return p;
}

int main(int argc, char** argv)
{
    constexpr unsigned NumArgsExpected = 2;
    if(argc != NumArgsExpected) {
        std::cerr << "Usage: " << argv[0] << " <input-file>\n";
        return -1;
    }

    std::ifstream infile(argv[1], std::ios::binary);
    const std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(infile), {});

    for(unsigned char c : buffer)
        std::cout << c;
    std::cout << '\n';

    std::string bitString {bytesToBitString(buffer)};
    std::reverse(bitString.begin(), bitString.end());

    std::vector<uint8_t> bits;
    for(auto c : bitString) {
        bits.push_back(c - '0');
    }

    Packet head;
    head = parse(bits);

    std::cout << "Version sum: " << traversePacketTree(&head) << '\n';

    return 0;
}