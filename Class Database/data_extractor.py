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

wait = WebDriverWait(driver, 60)
short_wait = WebDriverWait(driver, 20)

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

    for tr in soup.find_all("tr", id=lambda x: x and x.startswith("trSSR_CLSRCH_MTG1")):
        class_name_tag = soup.find("img", class_="PTCOLLAPSE")
        if class_name_tag: class_name = class_name_tag["alt"].replace(" Collapsible section", "").strip()
        else: class_name = subject

        class_number = tr.find("span", id=lambda x: x and x.startswith("MTG_CLASS_NBR")).text.strip()
        instructor = tr.find("span", id=lambda x: x and x.startswith("MTG_INSTR")).text.strip()
        location = tr.find("span", id=lambda x: x and x.startswith("MTG_ROOM")).text.strip()
        times = tr.find("span", id=lambda x: x and x.startswith("MTG_DAYTIME")).text.strip()

        rows.append({
            "Major": subject,
            "Class Name": class_name,
            "Class Code": class_number,
            "Instructor": instructor,
            "Location": location,
            "Days & Times": times
        })

        subject_classes += 1
        
    print(f"Found {subject_classes} class{"es" if subject_classes != 1 else ""} for {subject}")
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