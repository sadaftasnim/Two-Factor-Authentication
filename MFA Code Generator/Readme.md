# TOTP Generator: A Time-Based One-Time Password Generator

## Project Description

This project implements a **Time-based One-Time Password (TOTP)** generator in C++ using the TOTP algorithm defined in [RFC 6238](https://tools.ietf.org/html/rfc6238). It generates OTPs based on a Base32-encoded secret key that refreshes every 30 seconds. The OTPs are secure and can be used for multi-factor authentication systems.

---

## Prerequisites

1. **Operating System:**
   - Windows 11

2. **Development Environment:**
   - [Visual Studio 2022](https://visualstudio.microsoft.com/) with the **Desktop Development with C++** workload installed.

3. **Package Manager:**
   - [vcpkg](https://github.com/microsoft/vcpkg) for library management.

4. **Libraries:**
   - OpenSSL (installed via vcpkg).

---

## Algorithm

This program generates a one-time password based on a Base32 secret. Here's how it works:

1. **Input:** 
   - A Base32-encoded secret key.
   - Default parameters: Time step (30 seconds) and OTP length (6 digits).

2. **Base32 Decoding:**
   - The Base32 key is decoded into its binary equivalent.

3. **Time Counter Calculation:**
   - The current Unix time (seconds since 1970) is divided by the time step (30 seconds) to calculate the time counter.

4. **HMAC-SHA1 Calculation:**
   - The time counter is converted into an 8-byte big-endian array.
   - HMAC-SHA1 is applied using the binary secret and the time counter to generate a hash.

5. **Dynamic Truncation:**
   - A 4-byte section of the hash is selected to calculate a 6-digit OTP.

6. **Countdown Timer:**
   - The OTP is displayed with a countdown timer showing the seconds remaining until the next refresh.

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

### Step 2: Install OpenSSL

1. Use vcpkg to install OpenSSL:
   ```bash
   .\vcpkg install openssl:x64-windows
   ```

2. This will install OpenSSL and make it available for use in your Visual Studio projects.

---

### Step 3: Configure Visual Studio 2022

1. Open Visual Studio 2022 and create a new **Console App (C++)** project.
2. Go to **Project** → **Properties** and configure the following settings:
   - Under **VC++ Directories**:
     - Add the path to the OpenSSL `include` directory to **Include Directories**.
     - Add the path to the OpenSSL `lib` directory to **Library Directories**.
   - Under **Linker** → **Input**:
     - Add `libssl.lib` and `libcrypto.lib` to **Additional Dependencies**.
3. Save the changes and build the project.

---

## Running the Program

1. Replace the `base32_secret` in the code with your Base32-encoded secret.
2. Build and run the project in Visual Studio 2022.
3. The console will display the OTP and a countdown timer for the next refresh.

---

## Example Output

```
==============================
       My Secure App
  Account: user@example.com
==============================
OTP: 123456           [ Timer: 30 ]
```
