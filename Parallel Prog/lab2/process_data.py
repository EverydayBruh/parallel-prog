import json
import csv
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec

# Загрузите данные из файла JSON
with open("results.json", "r") as file:
    data = json.load(file)

# Получите список процессоров и соответствующее время выполнения
processors = sorted(data.keys(), key=int)
times = [data[p] for p in processors]

# Рассчитайте ускорение и эффективность
speedup = [times[0] / time for time in times]
efficiency = [s / int(p) for s, p in zip(speedup, processors)]

# Переведите список процессоров в целые числа для построения графиков
processors_int = [int(p) for p in processors]


# Теоретические значения
T1 = times[0]

T_theoretical = [(T1 / int(p)) for p in processors]
S_theoretical = [T1 / time for time in T_theoretical]
E_theoretical = [s / int(p) for s, p in zip(S_theoretical, processors_int)]

# Создайте CSV-файл и запишите в него значения параметров
csv_filename = "performance_data.csv"

with open(csv_filename, mode='w', newline='') as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(['Processors', 'Time', 'Speedup', 'Efficiency'])

    for p, t, s, e in zip(processors, times, speedup, efficiency):
        writer.writerow([p, t, s, e])

# Создаем объект gridspec с разными отношениями размеров ячеек
fig = plt.figure(figsize=(12, 15))
gs = GridSpec(2, 2, width_ratios=[1, 1], height_ratios=[3, 2])


# График времени выполнения с теоретическими значениями
ax1 = plt.subplot(gs[0, :])
ax1.plot(processors_int, times, '-o', color="blue", label='Practical')
ax1.plot(processors_int, T_theoretical, '--', color="orange", label='Theoretical')
ax1.set_xlabel('Number of Processors')
ax1.set_ylabel('Time (seconds)')
ax1.set_title('Execution Time vs Number of Processors')
ax1.legend()
ax1.grid(True)

# График ускорения с теоретическими значениями
ax2 = plt.subplot(gs[2])
ax2.plot(processors_int, speedup, '-o', color="green", label='Practical')
ax2.plot(processors_int, S_theoretical, '--', color="orange", label='Theoretical')
ax2.set_xlabel('Number of Processors')
ax2.set_ylabel('Speedup')
ax2.set_title('Speedup vs Number of Processors')
ax2.legend()
ax2.grid(True)

# График эффективности с теоретическими значениями
ax3 = plt.subplot(gs[3])
ax3.plot(processors_int, efficiency, '-o', color="red", label='Practical')
ax3.plot(processors_int, E_theoretical, '--', color="orange", label='Theoretical')
ax3.set_xlabel('Number of Processors')
ax3.set_ylabel('Efficiency')
ax3.set_title('Efficiency vs Number of Processors')
ax3.legend()
ax3.grid(True)

# Сохранение в файл
plt.tight_layout()
plt.savefig('performance_graphs.png')

# Покажите графики
#plt.show()
