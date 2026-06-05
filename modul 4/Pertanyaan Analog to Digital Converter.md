# 📘 Praktikum Sistem Tertanam - Modul 4 ADC dan DAC

## Percobaan 4A: Analog to Digital Converter (ADC)

---

# Pertanyaan Praktikum

1. Apa fungsi perintah `analogRead()` pada rangkaian praktikum ini?
2. Mengapa diperlukan fungsi `map()` dalam program tersebut?
3. Modifikasi program agar servo hanya bergerak dalam rentang 30° hingga 150°, meskipun potensiometer tetap memiliki rentang ADC 0–1023. Jelaskan program pada file `README.md`.

---

# Jawaban

---

# 1. Fungsi `analogRead()` pada Rangkaian Praktikum

Fungsi `analogRead()` digunakan untuk membaca nilai tegangan analog dari potensiometer yang terhubung ke pin analog Arduino.

Potensiometer menghasilkan tegangan yang berubah-ubah sesuai posisi putarannya. Arduino kemudian mengubah tegangan tersebut menjadi data digital menggunakan modul ADC (Analog to Digital Converter).

Format penggunaan:

```cpp
analogRead(pin);
```

Pada praktikum ini digunakan:

```cpp
val = analogRead(potensioPin);
```

Artinya Arduino membaca nilai analog dari potensiometer yang terhubung pada pin `A0`.

Nilai ADC yang dihasilkan memiliki rentang:

- `0` → tegangan 0V
- `1023` → tegangan 5V

Nilai tersebut kemudian digunakan untuk menentukan sudut pergerakan servo motor.

## Contoh Pembacaan ADC

| Nilai ADC | Tegangan | Posisi Potensiometer |
|---|---|---|
| 0 | 0V | Minimum |
| 512 | ±2.5V | Tengah |
| 1023 | 5V | Maksimum |

---

# 2. Fungsi `map()` dalam Program

Fungsi `map()` digunakan untuk mengubah rentang nilai ADC menjadi rentang sudut servo.

Nilai dari `analogRead()` memiliki rentang:

```text
0 – 1023
```

Sedangkan servo motor bekerja pada rentang:

```text
0° – 180°
```

Karena kedua rentang berbeda, maka diperlukan konversi nilai menggunakan fungsi `map()`.

Format fungsi:

```cpp
map(nilai, fromLow, fromHigh, toLow, toHigh);
```

Contoh penggunaan:

```cpp
pos = map(val, 0, 1023, 0, 180);
```

Artinya:

- ADC `0` → Servo `0°`
- ADC `1023` → Servo `180°`

Tanpa fungsi `map()`, nilai ADC tidak dapat langsung digunakan untuk mengontrol sudut servo dengan benar.

---

# 3. Modifikasi Program Servo 30° hingga 150°

Program dimodifikasi agar servo hanya bergerak pada rentang:

```text
30° – 150°
```

meskipun potensiometer tetap menghasilkan nilai ADC:

```text
0 – 1023
```

---

# Source Code Program

```cpp
#include <Servo.h>

// ===================== OBJECT SERVO =====================
Servo myservo;

// ===================== PIN SETUP =====================
const int potensioPin = A0;
const int servoPin = 9;

// ===================== VARIABEL =====================
int val = 0;
int pos = 0;

void setup() {

  // Menghubungkan servo ke pin PWM
  myservo.attach(servoPin);

  // Mengaktifkan komunikasi serial
  Serial.begin(9600);
}

void loop() {

  // ===================== PEMBACAAN ADC =====================
  // Membaca nilai potensiometer
  val = analogRead(potensioPin);

  // ===================== KONVERSI DATA =====================
  // Mengubah nilai ADC menjadi sudut 30° - 150°
  pos = map(val, 0, 1023, 30, 150);

  // ===================== OUTPUT SERVO =====================
  // Menggerakkan servo sesuai hasil mapping
  myservo.write(pos);

  // ===================== MONITORING DATA =====================
  Serial.print("ADC Potensio : ");
  Serial.print(val);

  Serial.print(" | Sudut Servo : ");
  Serial.println(pos);

  // ===================== STABILISASI =====================
  // Delay agar servo bergerak stabil
  delay(15);
}
```

---

# Penjelasan Program

## 1. Import Library Servo

```cpp
#include <Servo.h>
```

Library `Servo.h` digunakan untuk mengontrol motor servo pada Arduino.

---

## 2. Membuat Objek Servo

```cpp
Servo myservo;
```

Digunakan untuk membuat objek servo dengan nama `myservo`.

---

## 3. Konfigurasi Pin

```cpp
const int potensioPin = A0;
const int servoPin = 9;
```

- Pin `A0` digunakan untuk membaca potensiometer
- Pin `9` digunakan sebagai sinyal PWM servo

---

## 4. Inisialisasi Servo dan Serial Monitor

```cpp
myservo.attach(servoPin);
Serial.begin(9600);
```

Fungsi:

- Menghubungkan servo ke pin 9
- Mengaktifkan Serial Monitor dengan baud rate `9600`

---

## 5. Membaca Nilai ADC

```cpp
val = analogRead(potensioPin);
```

Digunakan untuk membaca nilai analog dari potensiometer.

Rentang pembacaan:

```text
0 – 1023
```

---

## 6. Konversi Nilai ADC ke Sudut Servo

```cpp
pos = map(val, 0, 1023, 30, 150);
```

Mengubah nilai ADC menjadi sudut servo dengan rentang:

| Nilai ADC | Sudut Servo |
|---|---|
| 0 | 30° |
| 1023 | 150° |

Tujuan pembatasan sudut ini adalah:

- Mengurangi getaran servo
- Mencegah servo bergerak terlalu ekstrem
- Menjaga kestabilan sistem

---

## 7. Menggerakkan Servo

```cpp
myservo.write(pos);
```

Digunakan untuk mengirim nilai sudut ke servo motor.

Servo akan bergerak sesuai hasil konversi nilai ADC.

---

## 8. Menampilkan Data pada Serial Monitor

```cpp
Serial.print()
```

Digunakan untuk menampilkan:

- Nilai ADC potensiometer
- Sudut servo hasil mapping

pada Serial Monitor Arduino IDE secara real-time.

---

## 9. Delay Stabilitas Sistem

```cpp
delay(15);
```

Memberikan jeda agar:

- Servo bergerak lebih stabil
- Pembacaan ADC tidak terlalu cepat
- Mengurangi jitter pada servo

---

# Kesimpulan

Pada percobaan ini, Arduino menggunakan modul ADC untuk membaca sinyal analog dari potensiometer melalui fungsi `analogRead()`. Nilai ADC yang diperoleh kemudian dikonversi menggunakan fungsi `map()` agar sesuai dengan rentang sudut servo motor.

Hasil modifikasi program berhasil membatasi pergerakan servo hanya pada rentang `30° hingga 150°`, meskipun input potensiometer tetap berada pada rentang penuh `0–1023`.

Dengan percobaan ini, konsep dasar ADC serta proses konversi sinyal analog menjadi kontrol aktuator dapat dipahami dengan baik.
