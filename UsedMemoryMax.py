import os
import csv
import sys

def sum_of_columns(csv_file):
    with open(csv_file, 'r') as file:
        reader = csv.reader(file, delimiter=';')
        total = 0
        for row in reader:
            try:
                temp = int(row[2]) + int(row[3])
                if temp > total:
                    total = temp
            except ValueError:
                pass
    return total

def main(folder_path):
    if not folder_path:
        folder_path = './'

    for filename in os.listdir(folder_path):
        if filename.endswith('.csv'):
            file_path = os.path.join(folder_path, filename)
            total = sum_of_columns(file_path)
            print(f"Used memory max for {filename}: {total} byte")

if __name__ == "__main__":
    folder_path = sys.argv[1] if len(sys.argv) > 1 else None
    main(folder_path)
