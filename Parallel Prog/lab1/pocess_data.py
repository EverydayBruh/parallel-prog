import json
import csv
import matplotlib.pyplot as plt

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

# Постройте графики
plt.figure(figsize=(15, 5))

# График времени выполнения с теоретическими значениями
plt.subplot(1, 3, 1)
plt.plot(processors_int, times, '-o', color="blue", label='Practical')
plt.plot(processors_int, T_theoretical, '--', color="orange", label='Theoretical')
plt.xlabel('Number of Processors')
plt.ylabel('Time (seconds)')
plt.title('Execution Time vs Number of Processors')
plt.legend()
plt.grid(True)

# График ускорения с теоретическими значениями
plt.subplot(1, 3, 2)
plt.plot(processors_int, speedup, '-o', color="green", label='Practical')
plt.plot(processors_int, S_theoretical, '--', color="orange", label='Theoretical')
plt.xlabel('Number of Processors')
plt.ylabel('Speedup')
plt.title('Speedup vs Number of Processors')
plt.legend()
plt.grid(True)

# График эффективности с теоретическими значениями
plt.subplot(1, 3, 3)
plt.plot(processors_int, efficiency, '-o', color="red", label='Practical')
plt.plot(processors_int, E_theoretical, '--', color="orange", label='Theoretical')
plt.xlabel('Number of Processors')
plt.ylabel('Efficiency')
plt.title('Efficiency vs Number of Processors')
plt.legend()
plt.grid(True)

# Сохраните все графики в одном файле
plt.tight_layout()
plt.savefig('performance_graphs.png')

# Покажите графики
plt.show()
