import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
import tensorflow as tf

PREFIX = "TinyML"


data = pd.read_csv("dataset.csv", header=None)
X = data[[0, 1]].values #columns 1 and 2 as inputs
y = data[2].astype(int).values #column 3 as boolean output (0/1)

scaler = StandardScaler()
scaler.fit_transform(X)
# X = scaler.fit_transform(X)

print("Means of each column (training data):", scaler.mean_)
print("Standard deviations of each column (training data):", scaler.scale_)

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.1)

# Simple classifier model
model = tf.keras.Sequential([
    tf.keras.layers.Input(shape=(2,)),
    tf.keras.layers.Dense(16, activation='relu'),
    tf.keras.layers.Dense(8, activation='relu'),
    tf.keras.layers.Dense(1, activation='sigmoid')
])

model.compile(loss="binary_crossentropy", optimizer="adam", metrics=["accuracy"])
model.fit(X_train, y_train, epochs=100, validation_data=(X_test, y_test))
model.save(PREFIX + '.h5')

loss, accuracy = model.evaluate(X_test, y_test)
print(f"Test Accuracy: {accuracy*100:.2f}%")

# Convert to TFLite
converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
tflite_model = converter.convert()

with open(PREFIX + ".tflite", "wb") as f:
    f.write(tflite_model)

tflite_path = PREFIX + '.tflite'
output_header_path = PREFIX + '.h'

with open(tflite_path, 'rb') as tflite_file:
    tflite_content = tflite_file.read()

hex_lines = [', '.join([f'0x{byte:02x}' for byte in tflite_content[i:i + 12]]) for i in
         range(0, len(tflite_content), 12)]

hex_array = ',\n  '.join(hex_lines)

with open(output_header_path, 'w') as header_file:
    header_file.write('const unsigned char model_arr[] = {\n  ')
    header_file.write(f'{hex_array}\n')
    header_file.write('};\n\n')