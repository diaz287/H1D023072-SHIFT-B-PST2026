# 📘 Laporan Praktikum Sistem Tertanam

## Modul 1 - Logika Percabangan

---

## 📝 Daftar Pertanyaan Praktikum

1. Pada kondisi apa program masuk ke blok `if`?
2. Pada kondisi apa program masuk ke blok `else`?
3. Apa fungsi dari perintah `delay(timeDelay)`?
4. Jika program memiliki alur **mati → lambat → cepat → reset (mati)**, ubah menjadi:
   **lambat → cepat → sedang → mati (tanpa reset langsung)** dan jelaskan setiap baris kode.

---

## ✅ Pembahasan dan Jawaban

### 1. Kondisi Pemicu Blok `if`

Program akan mengeksekusi instruksi di dalam blok `if` jika kondisi penentunya bernilai **benar (TRUE)**. Secara spesifik pada program ini:

```cpp
if (timeDelay <= 100)
```

**Detail Penjelasan:**

- Sistem mengevaluasi apakah nilai pada variabel `timeDelay` lebih kecil atau sama dengan **100 milidetik**.
- Jika kondisi tersebut terpenuhi (`TRUE`), maka baris kode di dalam kurung kurawal `if` akan langsung diproses.
- Kondisi ini menandakan bahwa kecepatan kedipan LED telah menyentuh **batas maksimum** yang telah diatur.
- Umumnya, keadaan ini tercapai setelah nilai jeda (delay) terus-menerus dikurangi pada siklus-siklus iterasi sebelumnya.

---

### 2. Kondisi Perpindahan ke Blok `else`

Alur eksekusi akan otomatis berpindah ke blok `else` apabila persyaratan pada `if` **tidak terpenuhi (FALSE)**. Pada skenario ini, hal tersebut terjadi ketika:

```
timeDelay > 100
```

Perintah yang akan dijalankan ketika masuk ke blok `else` adalah:

```cpp
timeDelay -= 100;
```

**Detail Penjelasan:**

- Selama angka `timeDelay` masih lebih besar dari 100 ms, program akan terus melewati blok `if` dan melompat ke `else`.
- Di setiap perulangan, durasi jeda akan **dipotong sebanyak 100 ms**.
- Proses pengurangan yang terus berulang inilah yang membuat kedipan LED secara visual terlihat **semakin cepat** pada setiap siklusnya.

---

### 3. Fungsi Teknis Instruksi `delay(timeDelay)`

**Sintaks:**

```cpp
delay(timeDelay);
```

**Fungsi Utama:**

- Memberikan perintah kepada mikrokontroler untuk **menghentikan sementara** proses eksekusi kode selanjutnya sesuai dengan durasi waktu yang ditetapkan.
- Berperan sebagai pengontrol seberapa lama LED berada dalam fase **menyala (ON)** dan fase **padam (OFF)**.

**Karakteristik Parameter:**

| Perintah      | Durasi             | Efek Visual        |
| ------------- | ------------------ | ------------------ |
| `delay(1000)` | 1000 ms / 1 detik  | Kedipan **lambat** |
| `delay(100)`  | 100 ms / 0,1 detik | Kedipan **cepat**  |

> **Hubungan:** Makin kecil angka parameter di dalam `delay()`, makin cepat pula frekuensi kedipan LED yang dihasilkan, begitu pun sebaliknya.

---

### 4. Modifikasi Logika Program

**Target Alur:** Lambat → Cepat → Sedang → Mati Total _(Tanpa Reset Otomatis)_

#### 💻 Kode Program (Arduino C++)

```cpp
const int ledPin = 6;    // Menentukan pin 6 sebagai jalur output untuk LED
int timeDelay = 1000;    // Menetapkan jeda awal sebesar 1 detik (mode lambat)

void setup() {
  pinMode(ledPin, OUTPUT);  // Mengonfigurasi ledPin sebagai pin keluaran (OUTPUT)
}

void loop() {
  // --- Fase LED Menyala ---
  digitalWrite(ledPin, HIGH);
  delay(timeDelay);          // LED menyala selama durasi nilai timeDelay

  // --- Fase LED Padam ---
  digitalWrite(ledPin, LOW);
  delay(timeDelay);          // LED padam selama durasi nilai timeDelay

  // --- Logika Pengaturan Transisi Kecepatan ---
  if (timeDelay <= 200) {         // Jika kedipan sudah sangat cepat (jeda sangat singkat)
    timeDelay = 500;              // Ubah jeda menjadi kecepatan menengah (sedang)

  } else if (timeDelay <= 500) {  // Jika program sedang berada pada kecepatan menengah
    timeDelay = 0;                // Jadikan nilai delay 0 sebagai indikator pemicu berhentinya sistem

  } else {                        // Jika program masih dalam fase kecepatan lambat
    timeDelay -= 200;             // Potong nilai jeda perlahan agar kedipan semakin cepat
  }

  // --- Mekanisme Penghentian Sistem Permanen ---
  if (timeDelay == 0) {
    digitalWrite(ledPin, LOW);  // Pastikan LED dalam kondisi mati sepenuhnya
    while(true);                // Terjebak dalam loop tak terbatas agar program berhenti dan tidak reset
  }
}
```

#### 🔍 Analisis Jalannya Program

| Fase                   | Kondisi            | Keterangan                                              |
| ---------------------- | ------------------ | ------------------------------------------------------- |
| **Inisiasi**           | `timeDelay = 1000` | Kedipan lambat dan santai                               |
| **Akselerasi**         | `else` aktif       | Jeda dipotong 200 ms tiap perulangan, LED semakin cepat |
| **Transisi ke Sedang** | `timeDelay <= 200` | Delay didongkrak ke 500 ms (kecepatan menengah)         |
| **Persiapan Berhenti** | `else if` aktif    | `timeDelay` diubah menjadi 0                            |
| **Terminasi**          | `timeDelay == 0`   | `while(true)` mengunci sistem, LED mati permanen        |

---

## 🎯 Kesimpulan

- Penerapan struktur percabangan `if-else-if` terbukti sangat efektif untuk mengendalikan perubahan nilai variabel kontrol secara dinamis bergantung pada situasi yang sedang berjalan.
- Dalam sistem ini, variabel `timeDelay` bertindak sebagai **parameter fundamental** yang mendikte karakteristik visual dari LED.
- Modifikasi kode berhasil menciptakan alur program yang sesuai dengan urutan:

  **Lambat → Cepat → Sedang → Mati**

  dan sukses mencegah sistem melakukan reset berkat adanya metode penguncian mikrokontroler menggunakan **infinite loop** (`while(true)`).
