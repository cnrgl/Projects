import re
import time
import requests
from bs4 import BeautifulSoup
from datetime import datetime


def check_website(url, filter_key, target_strings, target_imp_strings, unwanted_strings, formatted_date):
    filter_date = datetime.strptime(formatted_date, "%Y:%m:%d")
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36"}
    flag = False
    start_time = time.time()
    while True:
        response = requests.get(url, headers=headers)
        if response.status_code == 200:
            soup = BeautifulSoup(response.content, 'html.parser')
            filter_elements = soup.find_all(filter_key)

            for element in filter_elements:
                parent_element = element.parent
                span_date = parent_element.find("div", attrs={"class": "date"})
                date = ''.join(span_date.find_next("span").contents)

                obj_date = re.match(r"(\d{1,2}) (\w{3}) (\d{4})", date)
                day, month, year = obj_date.groups()

                month_mapping = {
                    'Oca': 'Jan', 'Şub': 'Feb', 'Mar': 'Mar',
                    'Nis': 'Apr', 'May': 'May', 'Haz': 'Jun',
                    'Tem': 'Jul', 'Ağu': 'Aug', 'Eyl': 'Sep',
                    'Eki': 'Oct', 'Kas': 'Nov', 'Ara': 'Dec'
                }
                month = month_mapping.get(month, month)
                formatted_date = f'{day} {month} {year}'
                date = datetime.strptime(formatted_date, '%d %b %Y')

                if all(target_string.lower() in element.text.lower() for target_string in target_imp_strings) and \
                        any(target_string.lower() in element.text.lower() for target_string in target_strings) and \
                        not any(ustring.lower() in element.text.lower() for ustring in unwanted_strings) and\
                        filter_date < date:
                    print(f"Found in <h6>: {element.text}, date: {date}")
                    flag = True

        else:
            print(f"Error: {response.status_code}")
        if flag:
            break
        time.sleep(6)
        elapsed_time = time.time() - start_time #wait if an approaching announcement is expected soon
        print(f"waiting... Elapsed:" + "{:.0f}".format(elapsed_time) + " second \n")



if __name__ == "__main__":
    target_strings = ['Araştırma', 'Görevlisi', 'Öğretim']
    target_imp_strings = []
    unwanted_strings = ['Düzeltme', 'Ön', 'Alım']
    check_website("https://www.itu.edu.tr/duyurular", "h6", # search h6 tag in the link with target string constraints
                  target_strings, target_imp_strings, unwanted_strings, "2024:01:01") # Date has to be updated to the starting date
                    # target_imp_strings --> if need an impulsory strings
