#include <iostream>
#include <string>
#include <openssl/rand.h>  // For generating random secret
#include <qrencode.h>      // For generating QR codes
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdio>  // For fopen_s
#include <png.h>   // Include libpng header(generated QR Code as image)

// Function to generate a base32 secret (16 bytes)
std::string generateBase32Secret(int length) {
    //static const char base32[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    //std::vector<unsigned char> buffer(length);
    //RAND_bytes(buffer.data(), length);  // Generate random bytes using OpenSSL

    //std::string secret;
    //for (int i = 0; i < length; ++i) {
    //    secret += base32[buffer[i] % 32];
    //}
    //return secret;

    //static const char base32[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

    // Hardcoded secret, not random
    std::string secret = "SDA5FTZN3K4P5LWX";  // Fixed secret

    return secret;  // Always returns the same secret
}

// Helper to URL-encode strings
std::string urlEncode(const std::string& value) {
    std::ostringstream encoded;
    for (char c : value) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded << c;
        }
        else {
            encoded << '%' << std::uppercase << std::hex << (int)(unsigned char)c;
        }
    }
    return encoded.str();
}

// Function to generate the provisioning URI (required for scanning with the authenticator app)
std::string generateProvisioningURI(const std::string& secret, const std::string& account_name, const std::string& issuer) {
    std::ostringstream uri;
    uri << "otpauth://totp/"
        << urlEncode(issuer + ":" + account_name)
        << "?secret=" << urlEncode(secret)
        << "&issuer=" << urlEncode(issuer)
        << "&algorithm=SHA1&digits=6&period=30";
    return uri.str();
}

// Function to generate and save the QR code as a PNG file using libpng
void generateQRCode(const std::string& uri, const std::string& output_filename) {
    QRcode* qr = QRcode_encodeString(uri.c_str(), 0, QR_ECLEVEL_L, QR_MODE_8, 1);
    if (qr != nullptr) {
        int scale = 15; // Increased scale factor for better readability
        int margin = 4; // Add a margin around the QR code
        int img_size = (qr->width + margin * 2) * scale;

        FILE* fp = nullptr;
        fopen_s(&fp, output_filename.c_str(), "wb");
        if (fp != nullptr) {
            png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            png_infop info = png_create_info_struct(png);

            if (setjmp(png_jmpbuf(png))) {
                png_destroy_write_struct(&png, &info);
                fclose(fp);
                std::cerr << "Error during PNG creation." << std::endl;
                return;
            }

            png_init_io(png, fp);
            png_set_IHDR(
                png, info, img_size, img_size, 8,
                PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT
            );
            png_write_info(png, info);

            // Write pixel data with scaling and margins
            for (int y = 0; y < img_size; ++y) {
                png_bytep row = (png_bytep)malloc(img_size);
                for (int x = 0; x < img_size; ++x) {
                    int qr_x = (x / scale) - margin;
                    int qr_y = (y / scale) - margin;
                    if (qr_x >= 0 && qr_x < qr->width && qr_y >= 0 && qr_y < qr->width) {
                        row[x] = (qr->data[qr_y * qr->width + qr_x] & 1) ? 0x00 : 0xFF;
                    }
                    else {
                        row[x] = 0xFF; // White margin
                    }
                }
                png_write_row(png, row);
                free(row);
            }

            png_write_end(png, NULL);
            png_destroy_write_struct(&png, &info);
            fclose(fp);

            std::cout << "QR code saved as " << output_filename << std::endl;
        }
        QRcode_free(qr);
    }
    else {
        std::cerr << "Failed to generate QR code." << std::endl;
    }
}

int main() {
    // Example: Generate a secret key, URI, and QR code
    std::string secret = generateBase32Secret(16);  // 16 bytes secret (128 bits)
    std::string account_name = "sadaftasnim800@gmail.com";  // Replace with your account
    std::string issuer = "SADAF TASNIM";  // Replace with your app's name

    std::cout << "Secret Key: " << secret << std::endl;

    std::string uri = generateProvisioningURI(secret, account_name, issuer);
    std::cout << "Provisioning URI: " << uri << std::endl;

    // Generate the QR code and save as PNG file
    generateQRCode(uri, "Generated_QR_Code.png");

    return 0;
}
