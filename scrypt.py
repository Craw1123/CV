import os
import datetime


# Генерация уникального ID в формате dd.mm.yyyy
unique_id = datetime.datetime.now().strftime("%d.%m.%Y")

# Создание новой папки с уникальным именем
os.makedirs(unique_id, exist_ok=True)

# URL репозитория на GitHub
github_repo_url = "https://github.com/heroku/node-js-getting-started"  

# Клонирование репозитория
os.system(f"git clone {github_repo_url} {unique_id}")

print(f"Репозиторий успешно клонирован в папку: {unique_id}")
os.system(f"docker build -t {unique_id} . --build-arg SOURCE_DIR={unique_id}")

os.system(f"docker run --name container-{unique_id} -d -p 5001:5001 {unique_id}")

