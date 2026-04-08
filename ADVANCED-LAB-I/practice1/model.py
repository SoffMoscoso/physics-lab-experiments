import tensorflow as tf
from tensorflow import keras

# importa las librerías de tensorflow y keras necesarias para el proyecto,
# facilitan la creación de redes neuronales.

# Assume input shape is (3,) representing x, y, z values of accelerometer
model = keras.Sequential([
    # Crea un modelo secuencial (Sequential). Las capas van unas tras otras.
    keras.layers.InputLayer(input_shape=(3,)),
    # Define la entrada del modelo, cada dato tiene 3 valores
    # acelerometro → x, y, z

    keras.layers.Dense(16, activation='relu'),
    # Dense: capa totalmente conectada
    # 16: número de neuronas
    # relu: función de activación (valores negativos = 0, valores positivos se mantienen)

    keras.layers.Dense(16, activation='relu'),
    # capa similar a la anterior (hace al modelo más potente)

    keras.layers.Dense(3, activation='softmax')
    # 3 output classes for 3 gestures
    # Capa de salida, 3: número de clases, 3 gestos distintos
    # softmax: convierte la salida en probabilidades.
])

# Compilación del modelo
model.compile(
    optimizer='adam',
    loss='sparse_categorical_crossentropy',
    metrics=['accuracy']
)
# ADAM: optimiza pesos automáticamente, es muy usado debido a su eficiencia.
# sparse categorical crossentropy: función de error para clasificación con etiquetas enteras
# metrics=['accuracy']: mide el porcentaje de aciertos

# Entrenamiento
model.fit(train_data, train_labels, epochs=10)
# train_data: datos de entrada (mediciones)
# train_labels: etiquetas (gestos 1, 2 o 3)
# epochs=10: repite el entrenamiento 10 veces (predice, calcula error, ajusta pesos)

# Guarda el modelo
model.save("gesture_model.h5")