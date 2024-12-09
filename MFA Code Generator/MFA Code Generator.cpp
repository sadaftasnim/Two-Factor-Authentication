#include <iostream>
#include <string>
#include <openssl/hmac.h>  // For HMAC
#include <sstream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <thread>  // For std::this_thread::sleep_for
#include <chrono>  // For time intervals

// Function to decode a Base32-encoded string into binary
std::string decodeBase32(const std::string& base32) {
    std::string decoded;
    int buffer = 0, bits_left = 0;

    for (char c : base32) {
        if (c == ' ') continue;  // Skip spaces
        if (c >= 'A' && c <= 'Z') {
            buffer = (buffer << 5) | (c - 'A');
        }
        else if (c >= '2' && c <= '7') {
            buffer = (buffer << 5) | (c - '2' + 26);
        }
        else {
            std::cerr << "Warning: Invalid Base32 character '" << c << "' skipped." << std::endl;
            continue;  // Skip invalid characters
        }
        bits_left += 5;
        if (bits_left >= 8) {
            bits_left -= 8;
            decoded += static_cast<char>((buffer >> bits_left) & 0xFF);
        }
    }

    return decoded;
}

// Function to validate if the Base32 secret is valid
bool isValidBase32(const std::string& base32) {
    for (char c : base32) {
        if (!((c >= 'A' && c <= 'Z') || (c >= '2' && c <= '7') || c == ' ')) {
            return false;
        }
    }
    return true;
}

// Function to generate OTP
int generateOTP(const std::string& binary_secret, int time_step = 30, int digits = 6) {
    unsigned long long epoch_time = std::time(nullptr);
    unsigned long long counter = epoch_time / time_step;

    // Convert counter to big-endian byte array
    unsigned char counter_bytes[8];
    for (int i = 7; i >= 0; --i) {
        counter_bytes[i] = counter & 0xFF;
        counter >>= 8;
    }

    // HMAC-SHA1 using the binary secret
    unsigned char* hmac_result = HMAC(
        EVP_sha1(), binary_secret.c_str(), binary_secret.size(),
        counter_bytes, sizeof(counter_bytes),
        nullptr, nullptr
    );

    // Dynamic truncation to get a 4-byte integer
    int offset = hmac_result[19] & 0xF;
    unsigned int binary = (
        ((hmac_result[offset] & 0x7F) << 24) |
        ((hmac_result[offset + 1] & 0xFF) << 16) |
        ((hmac_result[offset + 2] & 0xFF) << 8) |
        (hmac_result[offset + 3] & 0xFF)
        );

    // Reduce to desired number of digits
    int otp = binary % static_cast<int>(std::pow(10, digits));
    return otp;
}

int main() {
    // Hardcoded Base32 secret
    std::string base32_secret = "SDA5FTZN3K4P5LWX";
    std::string account_name = "sadaftasnim800@gmail.com";  // Replace with your account name
    std::string issuer = "SADAF TASNIM";  // Replace with your app's name

    // Validate the Base32 secret
    if (!isValidBase32(base32_secret)) {
        std::cerr << "Error: Invalid Base32 secret. Ensure it contains only A-Z and 2-7." << std::endl;
        return 1;
    }

    // Decode Base32 secret to binary
    std::string binary_secret = decodeBase32(base32_secret);

    // Set initial countdown to 30 seconds
    int counter = 30;

    // Generate OTP for the first time
    int otp = generateOTP(binary_secret);

    // Print the initial UI (static part) only once
    std::cout << "==============================\n";
    std::cout << "       " << issuer << "\n";
    std::cout << "  Account: " << account_name << "\n";
    std::cout << "==============================\n";
    //std::cout << "OTP: " << std::setfill('0') << std::setw(6) << otp << "           [ Timer: 30 ]\n";
    std::cout << "==============================\n";

    // Start the countdown and display timer
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Update the timer each second
        --counter;

        // Only update the time remaining, NOT the OTP
        std::cout << "\rOTP: " << std::setfill('0') << std::setw(6) << otp << "           [ Timer: " << counter << " ]" << std::flush;

        // Once counter reaches 0, regenerate OTP and reset timer
        if (counter == 0) {
            counter = 30;
            otp = generateOTP(binary_secret);  // Regenerate OTP

            // Refresh OTP and reset the timer display
            std::cout << "\rOTP: " << std::setfill('0') << std::setw(6) << otp << "           [ Timer: " << counter << " ]" << std::flush;
        }
    }

    return 0;
}
