<p align="center">
  <img src="370-project/icons/Cluster Banner.png" alt="Cluster Scheduler Banner" width="800">
</p>
<p align="center">An app designed to help you plan your CSUSM classes efficiently.</p>

---

## 📚 Table of Contents
- [📂 Repository Structure](#-repository-structure)
- [🖥 Requirements to Run](#-requirements-to-run)
- [📥 Installation](#-installation)
- [⚠ Notice](#-notice)
- [🚀 Usage](#-usage)
- [📦 Install Requirements (Developers)](#-install-requirements-for-building-from-source)
- [🐍 Database Update Tool](#-database-update-tool)
- [🛠 Build Instructions](#-build-instructions-for-developers)
- [👥 Contributors](#-contributors)

---

## 📂 Repository Structure

370-project/            # Main app source code (UI, logic, Qt)

├── build/              # Build folder, contains build instructions for Qt

├── icons/              # Icons for various buttons and windows

├── tests/              # Unit Tests

├── Main Palette.xml    # Palette used in software

├── apimap.*            # Api map used to power CSUSM map and directions

├── app_icon.rc         # Shows directory for .exe icno

├── class_info_unit.*   # Class Info frame, shown in "My Schedule List"

├── class_result.*      # Frame for search results

├── clustericon.ico     # .exe icon

├── createclass.*       # Handles Manually Add UI and functionality

├── main.cpp            # Starts the software

├── mainwindow.*        # Handles Main UI and functionality

├── map.qml             # Map data used by apimap.*

├── new_palette.xml     # Alternate Pallete

├── resources.qrc       # Directs to icons/ and map.qml 

├── safestuff.*         # Save data functionality

├── schedulelogic.*     # Schedule confliction algorithm

├── search_window.*     # Handles search UI and functionality

---

## 🖥 Requirements to Run
- Windows 10 or 11 (**64-bit**)
- CPU: Dual-core, 1.5 GHz or faster
- RAM: 512 MB minimum (1 GB+ recommended — more if your computer also doubles as a space heater)
- Display: 800×600 resolution or higher
- Storage: ~90 MB of free space (or roughly 60 billion crabs)
- **Google Chrome** and an active **Internet connection** — required for updating the course database

---

### Optional (but Highly Recommended)
- A keyboard and mouse you haven’t smashed yet
- A CPU that at least *pretends* to know what it’s doing
- One loose stick of RAM
- A black-and-white monitor (or a color one, if you’re feeling fancy)
- That AV cable you’ve been keeping *"just in case"* for the last decade

---

## 📥 Installation
1. Go to the [Releases](https://github.com/rochanehurst/CS370-Team3/releases/latest) page.
2. Click on the **title** of your preferred version.
3. Download the `370-project.zip` file.
4. Once downloaded, **extract** the files.
5. Run `370-project.exe`.
6. Done! 🎉 You’re ready to schedule like a pro.

---

## ⚠ Notice 
<details>
<summary>Click to expand</summary>

> We do not have $300 to spare to have this program validated by Microsoft.  
> You **will** get a virus alert from Windows Security — this is a **false positive**.  
> You are welcome to scan the file with any antivirus software you use and trust.

</details>

---

## 🚀 Usage

1. 🖥 **Launch the Program**
   - Run `370-project.exe` (Windows).
   - The main Cluster Scheduler window will appear.

2. ➕ **Add Classes**
   - Click the **(+)** icon next to the **Your Schedule** title.
   - Choose one of the two options:
     1. **Manually Add Class**
     2. **Search for Class**

3. ✏ **Manually Add Class**
   - Select a building from the dropdown.
   - Enter a name for the class.
   - Select a start time and an end time.
   - Select the days the class occurs.
   - Click **OK** to confirm and add the class to **Your Schedule**.

4. 🔍 **Search for Classes**
   - Use the search bars and dropdowns to filter by:
     - Subject
     - Building
     - Class Name
     - Time
     - Day(s)
   - Combine multiple filters to narrow down results.
   - To add a class from search results, click the **(+)** icon on the far right of the class frame.

5. 📅 **Plan Your Schedule**
   - Add classes to your schedule by:
     - Selecting them from search results
     - Manually adding them
   - The app will automatically:
     - Check for time conflicts
     - Check walking distance between classes

6. 🔄 **Update the Class Database**
   - **Requirements:**
     - Active internet connection  
     - Google Chrome installed  
   - **Usage**
     - Press the **Update** button to the left of the **Your Schedule** title.  
     - The update process takes **3–5 minutes** depending on your computer and internet speed.


💡 **Tip:** You can adjust sections of the app’s interface to focus on your schedule or the map of CSUSM.

---


## 📦 Install Requirements (For Building from Source)
<details>
<summary>Click to expand</summary>
  
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

## 🐍 Database Update Tool
<details>
<summary>Click to expand</summary>
  
Cluster Scheduler includes a small helper program for updating the class database.

### File Location
If building from source, the Python script can be found at:

`370-project/Desktop_Qt_6_9_1_MinGW_64_bit-Release/class_finder/data_extractor`

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

> ⚠ **Important:** Running this tool will replace your current database file with the newest data.
>
> The update process typically takes 3–5 minutes to complete.
</details>

---

## 🛠 Build Instructions (For Developers)
<details>
<summary>Click to expand</summary>

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

## 👥 Contributors
- Cole Leahy (@Crayon-lover)
- Rochane Hurst (@rochanehurst)
- Noah Jumnainpol (@JumGC)
- Raymond Las Pinas (@Rcalp)
- Cassidy Klein (@klein057)
