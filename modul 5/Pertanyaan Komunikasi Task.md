# 📘 Praktikum Sistem Tertanam - Modul 5 Real-Time Operating System

## Percobaan 5B: Komunikasi Task

---

# Pertanyaan Praktikum

1. Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!
2. Apakah program ini berpotensi mengalami race condition? Jelaskan!
3. Modifikasilah program dengan menggunakan sensor DHT sesungguhnya sehingga informasi yang ditampilkan dinamis. Bagaimana hasilnya? Jelaskan program pada file `README.md`.

---

# Jawaban

---

# 1. Apakah kedua task berjalan secara bersamaan atau bergantian?

Kedua task pada program FreeRTOS berjalan secara **concurrent (multitasking)**, bukan benar-benar paralel.

Arduino Uno hanya memiliki satu inti prosesor (*single core*), sehingga task dijalankan secara:

```text
bergantian sangat cepat oleh scheduler FreeRTOS
```

Karena perpindahan task berlangsung sangat cepat, maka task terlihat seperti berjalan bersamaan.

---

# Mekanisme Kerja Task pada FreeRTOS

Pada program terdapat dua task utama:

| Task | Fungsi |
|---|---|
| `read_data` | Membaca data sensor |
| `display` | Menampilkan data ke Serial Monitor |

Task-task tersebut dijalankan oleh scheduler FreeRTOS menggunakan teknik:

```text
task scheduling
```

Setiap task akan diberikan waktu CPU untuk dijalankan.

Ketika task mengalami delay atau menunggu queue, scheduler akan memindahkan CPU ke task lainnya.

---

# Mekanisme Queue

Program menggunakan:

```cpp
xQueueSend()
```

untuk mengirim data ke queue.

Dan:

```cpp
xQueueReceive()
```

untuk menerima data dari queue.

Alurnya:

## Task `read_data`

- Membaca data sensor
- Mengirim data ke queue

## Task `display`

- Menunggu data pada queue
- Menampilkan data ke Serial Monitor

---

# Kesimpulan Mekanisme

Walaupun terlihat berjalan bersamaan, sebenarnya kedua task berjalan:

```text
secara bergantian sangat cepat
```

dengan pengaturan scheduler FreeRTOS.

---

# 2. Apakah Program Berpotensi Mengalami Race Condition?

Program ini memiliki kemungkinan race condition yang sangat kecil atau hampir tidak terjadi.

Hal tersebut karena komunikasi antar task dilakukan menggunakan:

```cpp
Queue FreeRTOS
```

Queue pada FreeRTOS dirancang agar aman untuk komunikasi multitasking (*thread-safe*).

---

# Mengapa Queue Mengurangi Race Condition?

Race condition terjadi ketika:

```text
dua task mengakses data yang sama secara bersamaan
```

dan menyebabkan data menjadi tidak konsisten.

Namun pada program ini:

- Task `read_data` hanya mengirim data
- Task `display` hanya menerima data
- Queue mengatur sinkronisasi otomatis

Sehingga akses data menjadi lebih aman.

---

# Fungsi yang Menjaga Sinkronisasi

## Mengirim Data

```cpp
xQueueSend(my_queue, &x, portMAX_DELAY);
```

Digunakan untuk memasukkan data ke queue.

---

## Menerima Data

```cpp
xQueueReceive(my_queue, &x, portMAX_DELAY);
```

Digunakan untuk mengambil data dari queue.

Jika queue kosong, task penerima akan menunggu hingga data tersedia.

---

# Potensi Masalah yang Masih Bisa Terjadi

Walaupun race condition kecil kemungkinan terjadi, masalah berikut tetap dapat muncul:

| Masalah | Penyebab |
|---|---|
| Queue penuh | Data dikirim terlalu cepat |
| Delay task terlalu kecil | CPU terlalu sibuk |
| Stack overflow | Ukuran stack task terlalu kecil |

Namun untuk program sederhana ini, sistem masih berjalan stabil.

---

# 3. Modifikasi Program Menggunakan Sensor DHT

Program dimodifikasi dengan menggunakan sensor DHT11 sehingga data:

- suhu (*temperature*)
- kelembapan (*humidity*)

ditampilkan secara real-time dan dinamis.

---

# Hasil Modifikasi

Hasil percobaan menunjukkan bahwa:

- Data suhu dan kelembapan berubah secara real-time
- Queue berhasil mengirim data antar task
- Task display menampilkan data tanpa mengganggu task pembacaan sensor
- Multitasking FreeRTOS tetap berjalan stabil

Contoh output Serial Monitor:

```text
Temperature = 29°C
Humidity = 70%
```

Nilai akan berubah sesuai kondisi lingkungan sekitar sensor.

---

# Source Code Program

```cpp
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h>

// ===================== KONFIGURASI DHT =====================
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// ===================== STRUCT DATA =====================
struct readings {
  float temp;
  float hum;
};

// ===================== QUEUE =====================
QueueHandle_t my_queue;

// ===================== TASK FUNCTION =====================
void read_data(void *pvParameters);
void display_data(void *pvParameters);

void setup() {

  Serial.begin(9600);

  // Inisialisasi sensor DHT
  dht.begin();

  // Membuat queue
  my_queue = xQueueCreate(5, sizeof(struct readings));

  // Membuat task
  xTaskCreate(read_data, "read sensor", 128, NULL, 1, NULL);

  xTaskCreate(display_data, "display", 128, NULL, 1, NULL);
}

void loop() {
}

// ===================== TASK MEMBACA SENSOR =====================
void read_data(void *pvParameters)
{
  struct readings data;

  while(1)
  {
    // Membaca suhu dan kelembapan
    data.temp = dht.readTemperature();
    data.hum  = dht.readHumidity();

    // Mengirim data ke queue
    xQueueSend(my_queue, &data, portMAX_DELAY);

    // Delay task
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// ===================== TASK MENAMPILKAN DATA =====================
void display_data(void *pvParameters)
{
  struct readings data;

  while(1)
  {
    // Menerima data dari queue
    if(xQueueReceive(my_queue, &data, portMAX_DELAY) == pdPASS)
    {
      Serial.print("Temperature = ");
      Serial.print(data.temp);
      Serial.println(" C");

      Serial.print("Humidity = ");
      Serial.print(data.hum);
      Serial.println(" %");

      Serial.println("======================");
    }
  }
}
```

---

# Penjelasan Program

---

# 1. Import Library

```cpp
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h>
```

Digunakan untuk:

| Library | Fungsi |
|---|---|
| `Arduino_FreeRTOS.h` | Menjalankan multitasking |
| `queue.h` | Komunikasi antar task |
| `DHT.h` | Mengakses sensor DHT11 |

---

# 2. Konfigurasi Sensor DHT

```cpp
#define DHTPIN 2
#define DHTTYPE DHT11
```

Keterangan:

| Komponen | Fungsi |
|---|---|
| Pin 2 | Data sensor DHT |
| DHT11 | Tipe sensor |

---

# 3. Struktur Data

```cpp
struct readings
```

Digunakan untuk menyimpan:

- suhu
- kelembapan

dalam satu variabel.

---

# 4. Membuat Queue

```cpp
my_queue = xQueueCreate(5, sizeof(struct readings));
```

Queue digunakan untuk mengirim data antar task.

Kapasitas queue:

```text
5 data
```

---

# 5. Task Membaca Sensor

```cpp
read_data()
```

Fungsi:

- Membaca suhu
- Membaca kelembapan
- Mengirim data ke queue

---

# 6. Membaca Suhu dan Kelembapan

```cpp
dht.readTemperature();
dht.readHumidity();
```

Digunakan untuk membaca data sensor DHT11 secara real-time.

---

# 7. Mengirim Data ke Queue

```cpp
xQueueSend()
```

Digunakan agar data sensor dapat dikirim ke task lain dengan aman.

---

# 8. Task Display

```cpp
display_data()
```

Digunakan untuk menerima data dari queue dan menampilkannya ke Serial Monitor.

---

# 9. Menerima Data Queue

```cpp
xQueueReceive()
```

Task display akan menunggu hingga data tersedia pada queue.

---

# 10. Monitoring Serial

```cpp
Serial.print()
```

Digunakan untuk menampilkan:

- suhu
- kelembapan

secara real-time.

---

# Hasil Percobaan

Hasil modifikasi menunjukkan bahwa:

- Sensor DHT11 berhasil membaca suhu dan kelembapan
- Data berubah dinamis sesuai kondisi lingkungan
- Queue FreeRTOS berhasil mengirim data antar task
- Sistem multitasking tetap berjalan stabil tanpa konflik data

---

# Kesimpulan

Pada percobaan ini, FreeRTOS berhasil menjalankan komunikasi antar task menggunakan Queue.

Task pembacaan sensor dan task display berjalan secara concurrent dengan bantuan scheduler FreeRTOS.

Penggunaan Queue membuat komunikasi data menjadi lebih aman dan mengurangi kemungkinan race condition.

Melalui modifikasi menggunakan sensor DHT11, sistem berhasil menampilkan data suhu dan kelembapan secara dinamis dan real-time pada Serial Monitor.
