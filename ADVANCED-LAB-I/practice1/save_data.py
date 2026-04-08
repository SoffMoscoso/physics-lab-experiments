import serial
import numpy as np

ser = serial.Serial("/dev/ttyUSB0", 115200, timeout=1)
ser.write(b"a")  # envía 'a' para iniciar la transmisión de datos
data = []

for i in range(300):  # número de muestras
    line = ser.readline().decode().strip()
    print(line) # para monitorear
    values = list(map(float, line.split(',')))
    data.append(values)

print(data)

labels = [0]*300 + [1]*300 + [2]*300

train_data = np.array(data)
train_labels = np.array(labels)

# normalizar
train_data = train_data / 10.0

# guardar en txt
np.savetxt("train_data_2.txt", train_data)
np.savetxt("train_labels_2.txt", train_labels, fmt="%d")