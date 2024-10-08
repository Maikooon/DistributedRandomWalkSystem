#include <iostream>
#include <fstream>
#include "jwt-cpp/jwt.h"

bool verifyJWT(const std::string &token, const std::string &secret_key, uint32_t &RWer_id)
{
    std::cout << "Received JWT: " << token << std::endl;
    try
    {
        auto decoded = jwt::decode(token);

        std::cout << "Decoded payload: " << decoded.get_payload() << std::endl;

        auto verifier = jwt::verify()
                            .allow_algorithm(jwt::algorithm::hs256{secret_key})
                            // ここに検証したい値を入れる必要があるということ
                            .with_claim("id", jwt::claim(std::string{"12345"}));

        verifier.verify(decoded);

        RWer_id = std::stoi(decoded.get_payload_claim("id").as_string());
        std::cout << "Extracted RWer_id: " << RWer_id << std::endl;

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return false;
    }
}

int main()
{
    std::string secret_key = "my_secret_key";
    std::string token;

    // トークンをファイルから読み込む
    std::ifstream infile("token.txt");
    if (infile.is_open())
    {
        std::getline(infile, token);
        infile.close();
        std::cout << "Token loaded from token.txt" << std::endl;
    }
    else
    {
        std::cerr << "Error opening file to load token." << std::endl;
        return 1;
    }

    uint32_t RWer_id = 0;

    std::cout << "RwID: " << token << std::endl;
    // トークン検証
    std::cout << token << std::endl;
    if (verifyJWT("eyJhbGciOiJIUzI1NiJ9.eyJpZCI6IjEyMzQ1In0.3WpGvhzGul4RS_lRr7Xj44CXgD63t3mkZMZ4tfmkDtI", secret_key, RWer_id))
    {
        std::cout << "Token verified successfully. RWer_id: " << RWer_id << std::endl;
    }
    else
    {
        std::cout << "Token verification failed." << std::endl;
    }

    return 0;
}

//  g++ -std=c++11 verify_jwt.cpp -o verify_jwt -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto