# 📅 Cluster Scheduler
An app designed to help you plan your CSUSM classes efficiently.

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

## 📦 Install Requirements (For Building from Source)

Before you can build Cluster Scheduler, make sure you have:

- [Qt Community Edition 17.0.0](https://www.qt.io/download)
- A C++17-compatible compiler  
  - **Windows:** MSVC (Visual Studio) or MinGW  
  - **Linux/macOS:** GCC or Clang
- [CMake](https://cmake.org/download/) (minimum version 3.16)
- Git (to clone the repository)
- (Optional) [Qt Creator IDE](https://www.qt.io/product/development-tools) for an easier build process

---

## 🐍 Database Update Tool

Cluster Scheduler includes a small helper program for updating the class database.

### Options to Run It
- **Windows Users (No Python Required):**  
  Use the included `data_extractor.exe` file in the `/tools` folder (or wherever it’s located).
  
- **Using Python Directly:**  
  1. Install [Python 3.10+](https://www.python.org/downloads/).
  2. Install required libraries:
     ```bash
     pip install -r requirements.txt
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


## 🛠 Build Instructions (For Developers)

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
