import json
import csv
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec

file_name = "16process"
with open(file_name + ".json", "r") as file:
    data = json.load(file)

# Получите список процессоров и соответствующее время выполнения
processors = sorted(data.keys(), key=int)
times = [data[p] for p in processors]

plt.figure(figsize=(14,8))
bars = plt.bar(processors, times, color='blue', width=0.4)

# Добавление подписей к столбцам
for i, time in enumerate(times):
    plt.text(processors[i], time + 0.5, f'{time:.2f} s', ha='center')

# Добавление сетки
plt.grid(True, which='both', linestyle='--', linewidth=0.5)

plt.xlabel('Processor Number')
plt.ylabel('Run Time (Seconds)')
plt.title('Run Time by Processor')
plt.savefig(file_name + '.png')