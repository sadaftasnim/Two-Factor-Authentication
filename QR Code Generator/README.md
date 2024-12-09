# QR Code Generator

## Project Description

This project implements a **Time-based One-Time Password (TOTP)** generator in C++ that includes generating QR codes for provisioning. The program generates a TOTP-compliant provisioning URI, creates a QR code, and saves it as a PNG file, which can be scanned by authenticator apps like Google Authenticator or Microsoft Authenticator.

---

## Features

- Generates a fixed Base32 secret for TOTP.
- Creates a provisioning URI compliant with the TOTP algorithm defined in [RFC 6238](https://tools.ietf.org/html/rfc6238).
- Generates a QR code from the provisioning URI.
- Saves the QR code as a PNG image using **libpng**.
- Provides the ability to display the secret key and URI for the user.

---

## Prerequisites

1. **Operating System:**
   - Windows 11

2. **Development Environment:**
   - [Visual Studio 2022](https://visualstudio.microsoft.com/) with the **Desktop Development with C++** workload installed.

3. **Libraries:**
   - **OpenSSL** (for cryptographic functions).
   - **libqrencode** (for generating QR codes).
   - **libpng** (for saving QR codes as PNG files).

4. **Package Manager:**
   - [vcpkg](https://github.com/microsoft/vcpkg) for managing the dependencies.

---

## Algorithm

Here’s a step-by-step explanation of how the program works:

1. **Generate Base32 Secret:**
   - A hardcoded Base32 secret is used in this implementation.
   - Example: `"SDA5FTZN3K4P5LWX"`.

2. **Create Provisioning URI:**
   - A URI is generated in the format:  
     ```
     otpauth://totp/{Issuer}:{Account Name}?secret={Secret}&issuer={Issuer}&algorithm=SHA1&digits=6&period=30
     ```
   - Example:  
     ```
     otpauth://totp/SADAF%20TASNIM:sadaftasnim800@gmail.com?secret=SDA5FTZN3K4P5LWX&issuer=SADAF%20TASNIM&algorithm=SHA1&digits=6&period=30
     ```

3. **Generate QR Code:**
   - The provisioning URI is encoded into a QR code using **libqrencode**.
   - The QR code is saved as a PNG image using **libpng**.

---

## Setting Up the Project

### Step 1: Install vcpkg

1. Clone the vcpkg repository:
   ```bash
   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg
   ```

2. Bootstrap vcpkg:
   ```bash
   .\bootstrap-vcpkg.bat
   ```

3. Add vcpkg to your environment:
   ```bash
   .\vcpkg integrate install
   ```

---

### Step 2: Install Required Libraries

1. Install **OpenSSL**, **libqrencode**, and **libpng** using vcpkg:
   ```bash
   .\vcpkg install openssl:x64-windows qrencode:x64-windows libpng:x64-windows
   ```

2. These libraries will be installed and made available for Visual Studio 2022.

---

### Step 3: Configure Visual Studio 2022

1. Open Visual Studio 2022 and create a new **Console App (C++)** project.
2. Go to **Project** → **Properties** and configure the following settings:
   - Under **VC++ Directories**:
     - Add the path to the `include` directories of OpenSSL, libqrencode, and libpng.
   - Under **Linker** → **Input**:
     - Add `libssl.lib`, `libcrypto.lib`, `qrencode.lib`, and `libpng.lib` to **Additional Dependencies**.
3. Save the changes and build the project.

---

## Running the Program

1. Open the project in Visual Studio 2022.
2. Replace `account_name` and `issuer` in the code with your values.
3. Build and run the project.
4. The program will:
   - Display the Base32 secret and the provisioning URI.
   - Save the QR code as `Generated_QR_Code.png`.

---

## Example Output

```
Secret Key: SDA5FTZN3K4P5LWX
Provisioning URI: otpauth://totp/SADAF%20TASNIM:sadaftasnim800@gmail.com?secret=SDA5FTZN3K4P5LWX&issuer=SADAF%20TASNIM&algorithm=SHA1&digits=6&period=30
QR code saved as Generated_QR_Code.png
