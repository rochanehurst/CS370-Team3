from selenium import webdriver
from selenium.webdriver.support.ui import Select
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from bs4 import BeautifulSoup
from pathlib import Path
import pandas as pd
import time
import sys

def clean_field(value: str) -> str:
    if not value:
        return ""
    # Remove quotes
    value = value.replace('"', "")
    # Split by commas and newlines into individual items
    parts = [p.strip() for p in value.replace("\n", ",").split(",")]
    # Deduplicate while preserving order
    seen = []
    for p in parts:
        if p and p not in seen:
            seen.append(p)
    # Join cleaned parts with a pipe |
    return " | ".join(seen)


# Check if running from a .exe or .py file
# Determine system path depending on running file
if getattr(sys, 'frozen', False):
    script_dir = Path(sys.executable).parent
else:
    script_dir = Path(__file__).parent

new_file = script_dir / "data" / "csusm_classes.csv"
subject_file = script_dir / "data" / "subjects.csv"

options = webdriver.ChromeOptions()
options.add_argument("--headless")
driver = webdriver.Chrome(options=options)
driver.get("https://cmsweb.csusm.edu/psp/CSMPRDP/EMPLOYEE/HRMS/c/COMMUNITY_ACCESS.CLASS_SEARCH.GBL?")

subjects = pd.read_csv(subject_file, header=None, names=["Subject"])
rows = []
numOfClass = 0

start_time = time.time()

wait = WebDriverWait(driver, 1200) # Time out after 20 minutes


for subject in subjects["Subject"]:
    # Re-locate iframe after clearing
    driver.switch_to.default_content()
    iframe = wait.until(EC.presence_of_element_located((By.ID, "ptifrmtgtframe")))
    driver.switch_to.frame(iframe)

    # Now find dropdown safely
    subject_dropdown = wait.until(EC.element_to_be_clickable((By.ID, "SSR_CLSRCH_WRK_SUBJECT_SRCH$0")))
    select = Select(subject_dropdown)
    select.select_by_visible_text(subject)

    # Find search button
    search_button = wait.until(EC.element_to_be_clickable((By.ID, "CLASS_SRCH_WRK2_SSR_PB_CLASS_SRCH")))
    search_button.click()

    # Wait for page to load
    result = wait.until(
        EC.any_of(
            EC.presence_of_element_located((By.ID, "DERIVED_CLSMSG_ERROR_TEXT")),
            EC.element_to_be_clickable((By.ID, "#ICSave")),
            EC.presence_of_element_located((By.ID, "win0divSSR_CLSRCH_MTG1$0"))
        )
    )
    # Give the okay to continue
    if result.get_attribute("id") == "win0divSSR_CLSRCH_MTG1$0":
        pass

    # No classes found
    elif result.get_attribute("id") == "DERIVED_CLSMSG_ERROR_TEXT":
        print(f"No classes found for {subject}")
        clear_button = wait.until(EC.element_to_be_clickable((By.ID, "CLASS_SRCH_WRK2_SSR_PB_CLEAR")))
        clear_button.click()

        wait.until(EC.invisibility_of_element_located((By.ID, "DERIVED_CLSMSG_ERROR_TEXT")))
        continue

    # >50 classes warning
    elif result.get_attribute("id") == "#ICSave":
        ok_command = result
        ok_command.click()
        wait.until(EC.presence_of_element_located((By.ID, "win0divSSR_CLSRCH_MTG1$0")))
        pass

    soup = BeautifulSoup(driver.page_source, "html.parser")
    subject_classes = 0
    current_class_name = ""  # track the last seen header

    for section_div in soup.find_all("div", id=lambda x: x and x.startswith("win0divSSR_CLSRSLT_WRK_GROUPBOX2")):
        # Get class name from the header image
        img_tag = section_div.find("img", class_="PTCOLLAPSE")
        if not img_tag:
            continue
        class_name = img_tag.get("alt", "").replace(" Collapsible section", "").strip()

        # Find all class rows under this section
        rows_for_class = section_div.find_all(
            "tr", id=lambda x: x and x.startswith("trSSR_CLSRCH_MTG1")
        )

        for tr in rows_for_class:
            class_number = tr.find("span", id=lambda x: x and x.startswith("MTG_CLASS_NBR"))
            instructor = tr.find("span", id=lambda x: x and x.startswith("MTG_INSTR"))
            location = tr.find("span", id=lambda x: x and x.startswith("MTG_ROOM"))
            times = tr.find("span", id=lambda x: x and x.startswith("MTG_DAYTIME"))

            class_number = class_number.text.strip() if class_number else ""
            instructor = instructor.text.strip() if instructor else ""
            location = location.text.strip() if location else ""
            times = times.text.strip() if times else ""

            if "TBA" in times:
                continue

            rows.append({
                "Major": clean_field(subject.replace(",", "")),
                "Class Name": clean_field(class_name),
                "Class Code": clean_field(class_number),
                "Instructor": clean_field(instructor),
                "Location": clean_field(location),
                "Days & Times": clean_field(times)
            })
            subject_classes += 1


        
    if subject_classes == 0:
        print(f"No classes found for {subject}")
    else:
        print(f"Found {subject_classes} class{'es' if subject_classes != 1 else ''} for {subject}")
        numOfClass += subject_classes

    # click New Search to reset form
    new_search = wait.until(EC.element_to_be_clickable((By.ID, "CLASS_SRCH_WRK2_SSR_PB_NEW_SEARCH")))
    new_search.click()

df = pd.DataFrame(rows)
df.to_csv(new_file, index=False)
print(f"✅ Exported results to {new_file.resolve()}")

end_time = time.time()
time_elapsed = end_time - start_time

print(f"Found {numOfClass} classes\n Program took {time_elapsed:.2f} seconds")
time.sleep(30)
driver.quit()