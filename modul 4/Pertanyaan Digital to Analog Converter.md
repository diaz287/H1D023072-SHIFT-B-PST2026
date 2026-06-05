# 📘 Praktikum Sistem Tertanam - Modul 4 ADC dan DAC

## Percobaan 4B: Digital to Analog Converter (DAC) / PWM

---

# Pertanyaan Praktikum

1. Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi `analogWrite()`!
2. Apa hubungan antara nilai ADC (0–1023) dan nilai PWM (0–255)?
3. Modifikasilah program agar LED hanya menyala pada rentang kecerahan sedang, yaitu ketika nilai PWM berada pada rentang 50 sampai 200. Jelaskan program pada file `README.md`.

---

# Jawaban

---

# 1. Mengapa LED dapat diatur kecerahannya menggunakan fungsi `analogWrite()`?

LED dapat diatur tingkat kecerahannya menggunakan fungsi `analogWrite()` karena Arduino menggunakan metode PWM (*Pulse Width Modulation*).

PWM merupakan teknik menghasilkan sinyal analog semu menggunakan sinyal digital dengan cara mengatur lebar pulsa (*duty cycle*).

Fungsi:

```cpp
analogWrite(pin, value);
```

digunakan untuk mengirim sinyal PWM ke pin output Arduino.

Nilai PWM berada pada rentang:

```text
0 – 255
```

Keterangan:

| Nilai PWM | Kondisi LED |
|---|---|
| 0 | Mati |
| 127 | Redup/Sedang |
| 255 | Sangat terang |

Semakin besar nilai PWM, maka semakin lama kondisi HIGH dibanding LOW dalam satu periode sinyal. Hal tersebut membuat tegangan rata-rata meningkat sehingga LED terlihat lebih terang.

Sebaliknya, semakin kecil nilai PWM maka LED akan terlihat lebih redup.

---

# 2. Hubungan antara Nilai ADC (0–1023) dan PWM (0–255)

Nilai ADC dan PWM memiliki hubungan dalam proses konversi input analog menjadi output PWM.

## Nilai ADC

Nilai ADC diperoleh dari fungsi:

```cpp
analogRead()
```

dengan rentang:

```text
0 – 1023
```

Karena ADC Arduino Uno memiliki resolusi 10-bit:

```text
2¹⁰ = 1024 data
```

---

## Nilai PWM

Nilai PWM digunakan oleh fungsi:

```cpp
analogWrite()
```

dengan rentang:

```text
0 – 255
```

Karena PWM Arduino menggunakan resolusi 8-bit:

```text
2⁸ = 256 data
```

---

## Konversi ADC ke PWM

Karena rentang ADC dan PWM berbeda, maka diperlukan fungsi `map()` untuk melakukan konversi.

Contoh:

```cpp
pwm = map(nilaiADC, 0, 1023, 0, 255);
```

Artinya:

| Nilai ADC | Nilai PWM |
|---|---|
| 0 | 0 |
| 512 | ±127 |
| 1023 | 255 |

Dengan proses ini, perubahan potensiometer dapat mengatur tingkat kecerahan LED secara bertahap.

---

# 3. Modifikasi Program PWM 50–200

Program dimodifikasi agar LED hanya menyala ketika nilai PWM berada pada rentang:

```text
50 – 200
```

Jika nilai PWM:

- kurang dari `50`
- lebih dari `200`

maka LED akan dimatikan.

---

# Source Code Program

```cpp
#include <Arduino.h>

// ===================== PIN SETUP =====================
const int potPin = A0;
const int ledPin = 9;

// ===================== VARIABEL =====================
int nilaiADC = 0;
int pwm = 0;

void setup() {

  // ===================== OUTPUT SETUP =====================
  pinMode(ledPin, OUTPUT);

  // ===================== SERIAL MONITOR =====================
  Serial.begin(9600);
}

void loop() {

  // ===================== PEMBACAAN ADC =====================
  // Membaca nilai potensiometer
  nilaiADC = analogRead(potPin);

  // ===================== KONVERSI ADC KE PWM =====================
  // Mengubah nilai ADC menjadi PWM
  pwm = map(nilaiADC, 0, 1023, 0, 255);

  // ===================== FILTER RENTANG PWM =====================
  // LED hanya aktif pada PWM 50 - 200
  if (pwm >= 50 && pwm <= 200) {

    analogWrite(ledPin, pwm);

  } else {

    analogWrite(ledPin, 0);
  }

  // ===================== SERIAL MONITOR =====================
  Serial.print("ADC : ");
  Serial.print(nilaiADC);

  Serial.print(" | PWM : ");
  Serial.println(pwm);

  // ===================== STABILISASI =====================
  delay(50);
}
```

---

# Penjelasan Program

---

## 1. Import Library Arduino

```cpp
#include <Arduino.h>
```

Digunakan untuk mengakses fungsi dasar Arduino seperti:

- `analogRead()`
- `analogWrite()`
- `pinMode()`
- `Serial`

---

## 2. Konfigurasi Pin

```cpp
const int potPin = A0;
const int ledPin = 9;
```

Keterangan:

- Pin `A0` digunakan untuk membaca potensiometer
- Pin `9` digunakan sebagai output PWM LED

---

## 3. Inisialisasi Variabel

```cpp
int nilaiADC = 0;
int pwm = 0;
```

Digunakan untuk menyimpan:

- Nilai pembacaan ADC
- Nilai hasil konversi PWM

---

## 4. Setup Sistem

```cpp
pinMode(ledPin, OUTPUT);
Serial.begin(9600);
```

Fungsi:

- Mengatur pin LED sebagai output
- Mengaktifkan Serial Monitor dengan baud rate `9600`

---

## 5. Membaca Nilai Potensiometer

```cpp
nilaiADC = analogRead(potPin);
```

Membaca nilai analog dari potensiometer dengan rentang:

```text
0 – 1023
```

---

## 6. Konversi ADC ke PWM

```cpp
pwm = map(nilaiADC, 0, 1023, 0, 255);
```

Mengubah nilai ADC menjadi nilai PWM.

| ADC | PWM |
|---|---|
| 0 | 0 |
| 1023 | 255 |

---

## 7. Filter Rentang PWM

```cpp
if (pwm >= 50 && pwm <= 200)
```

LED hanya menyala jika PWM berada pada rentang sedang.

### Kondisi:

| Nilai PWM | Kondisi LED |
|---|---|
| < 50 | Mati |
| 50 – 200 | Menyala |
| > 200 | Mati |

Hal ini membuat LED tidak terlalu redup maupun terlalu terang.

---

## 8. Output PWM ke LED

```cpp
analogWrite(ledPin, pwm);
```

Mengirim sinyal PWM ke LED agar tingkat kecerahannya berubah sesuai nilai PWM.

---

## 9. Mematikan LED di Luar Rentang

```cpp
analogWrite(ledPin, 0);
```

Digunakan untuk mematikan LED ketika nilai PWM tidak berada dalam rentang yang ditentukan.

---

## 10. Monitoring Serial

```cpp
Serial.print()
```

Digunakan untuk menampilkan:

- Nilai ADC
- Nilai PWM

secara real-time pada Serial Monitor Arduino IDE.

---

## 11. Delay Stabilitas

```cpp
delay(50);
```

Memberikan jeda agar:

- pembacaan ADC lebih stabil
- tampilan Serial Monitor lebih rapi
- perubahan LED lebih halus

---

# Kesimpulan

Pada percobaan ini, Arduino menggunakan teknik PWM (*Pulse Width Modulation*) untuk mengatur tingkat kecerahan LED melalui fungsi `analogWrite()`.

Nilai ADC dari potensiometer dibaca menggunakan `analogRead()`, kemudian dikonversi menjadi nilai PWM menggunakan fungsi `map()`.

Hasil modifikasi program berhasil membuat LED hanya menyala pada rentang PWM `50 hingga 200`, sehingga LED hanya aktif pada tingkat kecerahan sedang.

Melalui percobaan ini, konsep dasar ADC, PWM, dan hubungan antara input analog dengan output digital semu dapat dipahami dengan baik.
