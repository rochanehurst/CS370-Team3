<p align="center">
  <img src="370-project/icons/Cluster Banner.png" alt="Cluster Scheduler Banner" width="800">
</p>
<p align="center">An app designed to help you plan your CSUSM classes efficiently.</p>

---

## 📚 Table of Contents
- [Requirements to Run](#-requirements-to-run)
- [Installation](#-installation)
- [Notice](#-notice-click-to-expand)
- [Install Requirements (Developers)](#-install-requirements-for-building-from-source)
- [Database Update Tool](#-database-update-tool)
- [Build Instructions](#-build-instructions-for-developers)

---

## 🖥 Requirements to Run
- Windows 10 or 11
- A keyboard and mouse you haven't broken... yet
- A CPU that at least pretends to know what it’s doing
- One stick of RAM (Two may be pushing it)
- ~80 MB of free storage space (or roughly 50 billion crabs)

---

## 📥 Installation
1. Go to the [Releases](https://github.com/rochanehurst/CS370-Team3/releases/latest) page.
2. Click on the **title** of your preferred version.
3. Download the `370-project.zip` file.
4. Once downloaded, **extract** the files.
5. Run `370-project.exe`.
6. Done! 🎉 You’re ready to schedule like a pro.

---

<details>
<summary>⚠ Notice (click to expand)</summary>

We do not have $100 to spare to have this program validated by Microsoft.  
You **will** get a virus alert from Windows Security — this is a **false positive**.  
You are welcome to scan the file with any antivirus software you use and trust.

</details>

---

<details>
<summary>## 📦 Install Requirements (For Building from Source)</summary>

Before you can build Cluster Scheduler, make sure you have:

- [Qt Community Edition 17.0.0](https://www.qt.io/download)
- A C++17-compatible compiler  
  - **Windows:** MSVC (Visual Studio) or MinGW  
  - **Linux/macOS:** GCC or Clang
- [CMake](https://cmake.org/download/) (minimum version 3.16)
- Git (to clone the repository)
- (Optional) [Qt Creator IDE](https://www.qt.io/product/development-tools) for an easier build process
</details>

---

<details>
<summary>## 🐍 Database Update Tool</summary>

Cluster Scheduler includes a small helper program for updating the class database.

### File Location
If building from source, the Python script can be found at:
`370-project\Desktop_Qt_6_9_1_MinGW_64_bit-Release\class_finder\data_extractor`

### Options to Run It
- **Windows Users (No Python Required):**  
  Use the included `data_extractor.exe` file in the `/class_finder` folder.
  
- **Using Python Directly:**  
  1. Install [Python 3.10+](https://www.python.org/downloads/).
  2. Install required libraries:
     ```bash
     pip install selenium beautifulsoup4 pandas
     ```
  3. Run the script:
     ```bash
     python data_extractor.py
     ```

### What It Does
- Connects to the CSUSM course catalog
- Downloads the latest class data
- Updates the internal database used by Cluster Scheduler

⚠ **Note:** Running this tool will overwrite your existing database file with the latest data.
</details>

---

<details>
<summary>## 🛠 Build Instructions (For Developers)</summary>

If you want to modify Cluster Scheduler or build it yourself from source, here’s how:

### 1. Clone the Repository
```bash
git clone https://github.com/rochanehurst/CS370-Team3.git
cd CS370-Team3
```
### 2. Install Requirements
(See [Install Requirements](#-install-requirements-for-building-from-source) above.)

### 3. Build via CMake
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
</details>
---

## Contributors
- Cole Leahy (@Crayon-lover)
- Teammate Name 1 (@githubhandle)
- Teammate Name 2 (@githubhandle)
- Teammate Name 3 (@githubhandle)
- Teammate Name 4 (@githubhandle)
