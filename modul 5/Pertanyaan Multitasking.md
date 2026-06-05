# 📘 Praktikum Sistem Tertanam - Modul 5 Real-Time Operating System

## Percobaan 5A: Multitasking

---

# Pertanyaan Praktikum

1. Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!
2. Bagaimana cara menambahkan task keempat? Jelaskan langkahnya!
3. Modifikasilah program dengan menambah sensor (misalnya potensiometer), lalu gunakan nilainya untuk mengontrol kecepatan LED! Bagaimana hasilnya? Jelaskan program pada file `README.md`.

---

# Jawaban

---

# 1. Apakah ketiga task berjalan secara bersamaan atau bergantian?

Ketiga task pada program FreeRTOS berjalan secara **concurrent** atau multitasking, bukan benar-benar paralel.

Pada Arduino Uno hanya terdapat satu inti prosesor (*single core*), sehingga task sebenarnya dijalankan secara **bergantian dengan sangat cepat** oleh scheduler FreeRTOS.

Karena perpindahan task berlangsung sangat cepat, maka task terlihat seperti berjalan bersamaan.

---

# Mekanisme Kerja Scheduler FreeRTOS

FreeRTOS menggunakan **scheduler** untuk mengatur kapan suatu task dijalankan.

Pada program ini terdapat tiga task:

| Task | Fungsi |
|---|---|
| `TaskBlink1` | Mengontrol LED pertama |
| `TaskBlink2` | Mengontrol LED kedua |
| `Taskprint` | Menampilkan counter di Serial Monitor |

Masing-masing task memiliki:

```cpp
vTaskDelay()
```

yang digunakan untuk memberi jeda sehingga scheduler dapat menjalankan task lain.

Contoh:

```cpp
vTaskDelay(200 / portTICK_PERIOD_MS);
```

Artinya task akan ditunda selama 200 ms dan CPU dapat digunakan oleh task lainnya.

---

# Alur Kerja Multitasking

## Task 1

- LED pin 8 menyala
- Delay 200 ms

## Scheduler berpindah ke Task 2

- LED pin 7 menyala
- Delay 300 ms

## Scheduler berpindah ke Task 3

- Counter bertambah
- Delay 500 ms

Kemudian scheduler akan mengulang proses tersebut terus-menerus.

---

# Kesimpulan Mekanisme

Walaupun terlihat berjalan bersamaan, sebenarnya task berjalan secara:

```text
bergantian sangat cepat (time slicing)
```

dengan bantuan scheduler FreeRTOS.

---

# 2. Cara Menambahkan Task Keempat

Untuk menambahkan task keempat pada FreeRTOS, diperlukan beberapa langkah.

---

# Langkah 1 — Membuat Prototype Function

Tambahkan deklarasi function task baru di bagian atas program.

Contoh:

```cpp
void TaskBlink3(void *pvParameters);
```

---

# Langkah 2 — Membuat Task Menggunakan `xTaskCreate()`

Tambahkan pada fungsi `setup()`:

```cpp
xTaskCreate(
  TaskBlink3,
  "task4",
  128,
  NULL,
  1,
  NULL
);
```

Keterangan parameter:

| Parameter | Fungsi |
|---|---|
| `TaskBlink3` | Nama function task |
| `"task4"` | Nama task |
| `128` | Stack size |
| `NULL` | Parameter task |
| `1` | Priority |
| `NULL` | Task handle |

---

# Langkah 3 — Membuat Isi Function Task

Contoh task baru:

```cpp
void TaskBlink3(void *pvParameters)
{
  pinMode(6, OUTPUT);

  while(1)
  {
    digitalWrite(6, HIGH);
    vTaskDelay(400 / portTICK_PERIOD_MS);

    digitalWrite(6, LOW);
    vTaskDelay(400 / portTICK_PERIOD_MS);
  }
}
```

Task tersebut akan membuat LED pada pin 6 berkedip setiap 400 ms.

---

# Hasil Penambahan Task

Setelah task keempat ditambahkan:

- Scheduler FreeRTOS akan mengatur empat task sekaligus
- Semua task tetap berjalan concurrent
- LED tambahan akan berkedip sesuai delay yang diberikan

---

# 3. Modifikasi Program dengan Potensiometer untuk Mengontrol Kecepatan LED

Pada modifikasi ini ditambahkan:

- Potensiometer sebagai input analog
- Nilai ADC digunakan untuk mengatur kecepatan blinking LED

Semakin besar nilai potensiometer:

```text
LED berkedip semakin lambat
```

Semakin kecil nilai potensiometer:

```text
LED berkedip semakin cepat
```

---

# Source Code Program

```cpp
#include <Arduino_FreeRTOS.h>

// ===================== TASK FUNCTION =====================
void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);
void TaskPotensio(void *pvParameters);

// ===================== PIN =====================
const int led1 = 8;
const int led2 = 7;
const int potPin = A0;

// ===================== VARIABEL GLOBAL =====================
int delayLED = 200;

void setup() {

  Serial.begin(9600);

  // ===================== CREATE TASK =====================
  xTaskCreate(TaskBlink1, "task1", 128, NULL, 1, NULL);

  xTaskCreate(TaskBlink2, "task2", 128, NULL, 1, NULL);

  xTaskCreate(TaskPotensio, "task3", 128, NULL, 1, NULL);

  // ===================== START SCHEDULER =====================
  vTaskStartScheduler();
}

void loop() {
}

// ===================== TASK LED 1 =====================
void TaskBlink1(void *pvParameters)
{
  pinMode(led1, OUTPUT);

  while(1)
  {
    digitalWrite(led1, HIGH);
    vTaskDelay(delayLED / portTICK_PERIOD_MS);

    digitalWrite(led1, LOW);
    vTaskDelay(delayLED / portTICK_PERIOD_MS);
  }
}

// ===================== TASK LED 2 =====================
void TaskBlink2(void *pvParameters)
{
  pinMode(led2, OUTPUT);

  while(1)
  {
    digitalWrite(led2, HIGH);
    vTaskDelay(delayLED / portTICK_PERIOD_MS);

    digitalWrite(led2, LOW);
    vTaskDelay(delayLED / portTICK_PERIOD_MS);
  }
}

// ===================== TASK POTENSIOMETER =====================
void TaskPotensio(void *pvParameters)
{
  int adcValue;

  while(1)
  {
    // Membaca nilai ADC
    adcValue = analogRead(potPin);

    // Mapping ADC menjadi delay
    delayLED = map(adcValue, 0, 1023, 50, 1000);

    // Monitoring Serial
    Serial.print("ADC : ");
    Serial.print(adcValue);

    Serial.print(" | Delay LED : ");
    Serial.println(delayLED);

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
```

---

# Penjelasan Program

---

# 1. Import Library FreeRTOS

```cpp
#include <Arduino_FreeRTOS.h>
```

Digunakan untuk mengaktifkan fitur multitasking FreeRTOS pada Arduino.

---

# 2. Deklarasi Task

```cpp
void TaskBlink1(void *pvParameters);
```

Digunakan untuk mendefinisikan function task yang akan dijalankan scheduler.

---

# 3. Konfigurasi Pin

```cpp
const int led1 = 8;
const int led2 = 7;
const int potPin = A0;
```

Keterangan:

| Pin | Fungsi |
|---|---|
| 8 | LED 1 |
| 7 | LED 2 |
| A0 | Potensiometer |

---

# 4. Variabel Delay Global

```cpp
int delayLED = 200;
```

Digunakan sebagai delay blinking LED yang dapat berubah sesuai nilai potensiometer.

---

# 5. Membuat Task

```cpp
xTaskCreate()
```

Digunakan untuk membuat task baru pada FreeRTOS.

Scheduler akan menjalankan seluruh task secara concurrent.

---

# 6. Task LED

```cpp
digitalWrite()
```

Digunakan untuk mengontrol LED ON/OFF.

Delay blinking diambil dari variabel:

```cpp
delayLED
```

---

# 7. Task Potensiometer

```cpp
adcValue = analogRead(potPin);
```

Digunakan untuk membaca nilai analog dari potensiometer.

---

# 8. Mapping ADC ke Delay

```cpp
delayLED = map(adcValue, 0, 1023, 50, 1000);
```

Konversi:

| ADC | Delay |
|---|---|
| 0 | 50 ms |
| 1023 | 1000 ms |

Artinya:

- Potensiometer kecil → LED cepat
- Potensiometer besar → LED lambat

---

# 9. Monitoring Serial

```cpp
Serial.print()
```

Digunakan untuk menampilkan:

- Nilai ADC
- Delay LED

secara real-time pada Serial Monitor.

---

# Hasil Percobaan

Hasil modifikasi menunjukkan bahwa:

- Potensiometer berhasil mengontrol kecepatan blinking LED
- Kedua LED tetap berjalan multitasking
- Scheduler FreeRTOS tetap bekerja dengan baik
- Perubahan delay berlangsung secara real-time

Semakin besar nilai potensiometer, LED berkedip semakin lambat.

Semakin kecil nilai potensiometer, LED berkedip semakin cepat.

---

# Kesimpulan

Pada percobaan ini, FreeRTOS berhasil menjalankan beberapa task secara concurrent menggunakan scheduler multitasking.

Penambahan task baru dapat dilakukan menggunakan fungsi `xTaskCreate()` dengan membuat function task tambahan.

Modifikasi menggunakan potensiometer berhasil mengontrol kecepatan blinking LED secara real-time melalui pembacaan ADC dan proses mapping delay.

Melalui percobaan ini, konsep multitasking, scheduler, task management, dan integrasi sensor pada sistem embedded berbasis RTOS dapat dipahami dengan baik.
