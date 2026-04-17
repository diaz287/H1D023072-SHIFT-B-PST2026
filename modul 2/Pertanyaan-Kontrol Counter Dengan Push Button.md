# 📘 Praktikum Sistem Tertanam — Modul 2: Pemrograman GPIO - Konrol Counter dengan Push Button

---

## 📝 Daftar Pertanyaan Praktikum

1. Gambarkan rangkaian schematic yang digunakan pada percobaan!
2. Mengapa pada push button digunakan mode `INPUT_PULLUP` pada Arduino Uno? Apa keuntungannya dibandingkan rangkaian biasa?
3. Jika salah satu LED segmen tidak menyala, apa saja kemungkinan penyebabnya dari sisi hardware maupun software?
4. Modifikasi rangkaian dan program dengan dua push button yang berfungsi sebagai penambahan (increment) dan pengurangan (decrement) pada sistem counter dan berikan penjelasan disetiap baris kode nya dalam bentuk `README.md!`

---

## ✅ Jawaban

### 1. Skematik Rangkaian

![Schematic Kontrol Counter](Schematic-Kontrol-Counter-Dengan-Push-Button.png)

---

### 2. Alasan Penggunaan Mode `INPUT_PULLUP` pada Push Button

Mode `INPUT_PULLUP` berfungsi untuk mengaktifkan resistor pull-up yang telah tertanam secara internal di dalam chip Arduino. Dengan konfigurasi ini, pin input secara bawaan akan selalu terbaca sebagai `HIGH`, lalu berpindah ke kondisi `LOW` pada saat push button ditekan dan menghubungkan pin tersebut ke `GND`.

#### Mengapa Diperlukan?

Tanpa `INPUT_PULLUP`, pin input dibiarkan dalam kondisi tidak terdefinisi atau _floating_, sehingga nilai yang terbaca menjadi tidak konsisten dan rentan terhadap gangguan sinyal.

#### Perbandingan dengan Rangkaian Konvensional

| Aspek               | Rangkaian Biasa        | INPUT_PULLUP     |
| ------------------- | ---------------------- | ---------------- |
| Resistor eksternal  | Diperlukan             | Tidak diperlukan |
| Kerumitan rangkaian | Lebih kompleks         | Lebih sederhana  |
| Stabilitas sinyal   | Rentan noise           | Stabil           |
| Logika tombol       | Bergantung konfigurasi | Tekan = `LOW`    |

#### Keunggulan Utama

- Jumlah komponen berkurang karena tidak membutuhkan resistor tambahan
- Skematik rangkaian menjadi jauh lebih bersih dan ringkas
- Pembacaan sinyal lebih terpercaya dan terbebas dari gangguan (_noise_)

> **Kesimpulan:** Penggunaan `INPUT_PULLUP` menjadikan sistem lebih stabil, efisien, dan hemat komponen dibandingkan rangkaian pull-up eksternal konvensional.

---

### 3. Kemungkinan Penyebab Segmen LED Tidak Menyala

Ketika salah satu segmen pada seven segment display tidak berfungsi, investigasi perlu dilakukan dari dua sisi secara bersamaan.

#### 🔧 Sisi Hardware

- Sambungan kabel pada pin segmen tidak terpasang dengan benar atau terlepas
- Resistor pembatas arus mengalami kerusakan atau tidak terpasang pada jalurnya
- Elemen LED pada segmen tersebut secara fisik sudah rusak atau terbakar
- Terjadi kesalahan dalam menghubungkan pin seven segment ke Arduino
- Jalur _common_ (anoda/katoda) tidak tersambung ke titik yang benar

#### 💻 Sisi Software

- Data pada array `digitPattern` mengandung kesalahan nilai untuk segmen tertentu
- Urutan penugasan pin di dalam program tidak sinkron dengan koneksi fisik
- Logika `HIGH`/`LOW` tidak sesuai dengan jenis display yang digunakan (common cathode vs common anode)
- Pin yang bersangkutan belum dideklarasikan sebagai `OUTPUT` di dalam fungsi `setup()`

> **Kesimpulan:** Akar masalah bisa berasal dari sisi perangkat keras maupun perangkat lunak, sehingga keduanya perlu diperiksa secara menyeluruh dan sistematis.

---

### 4. Modifikasi Program: Counter Dua Tombol (Increment & Decrement)

#### 🧩 Konsep Modifikasi

Rangkaian diperluas dengan menambahkan satu push button tambahan, sehingga terdapat dua tombol dengan fungsi yang berlawanan:

- **Tombol 1 (btnUp)** → Menaikkan nilai counter _(increment)_
- **Tombol 2 (btnDown)** → Menurunkan nilai counter _(decrement)_

Kedua tombol dikonfigurasi menggunakan `INPUT_PULLUP`.

#### 💻 Kode Program

```cpp
// Deklarasi pin untuk setiap segmen seven segment (a,b,c,d,e,f,g,dp)
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};

// Deklarasi pin untuk kedua push button
const int btnUp   = 3;   // Tombol increment (tambah)
const int btnDown = 2;   // Tombol decrement (kurang)

// Pola nyala segmen untuk angka 0–9 dan huruf A–F (hexadesimal)
byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0},  // 0
  {0,1,1,0,0,0,0,0},  // 1
  {1,1,0,1,1,0,1,0},  // 2
  {1,1,1,1,0,0,1,0},  // 3
  {0,1,1,0,0,1,1,0},  // 4
  {1,0,1,1,0,1,1,0},  // 5
  {1,0,1,1,1,1,1,0},  // 6
  {1,1,1,0,0,0,0,0},  // 7
  {1,1,1,1,1,1,1,0},  // 8
  {1,1,1,1,0,1,1,0},  // 9
  {1,1,1,0,1,1,1,0},  // A
  {0,0,1,1,1,1,1,0},  // B
  {1,0,0,1,1,1,0,0},  // C
  {0,1,1,1,1,0,1,0},  // D
  {1,0,0,1,1,1,1,0},  // E
  {1,0,0,0,1,1,1,0}   // F
};

int currentDigit = 0;    // Nilai awal counter dimulai dari 0

bool lastUp   = HIGH;    // Status tombol UP pada siklus sebelumnya
bool lastDown = HIGH;    // Status tombol DOWN pada siklus sebelumnya

// Fungsi untuk menampilkan angka ke seven segment
void displayDigit(int num) {
  for (int i = 0; i < 8; i++) {
    // Logika dibalik karena menggunakan common anode
    digitalWrite(segmentPins[i], !digitPattern[num][i]);
  }
}

void setup() {
  // Inisialisasi semua pin segmen sebagai OUTPUT
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  // Inisialisasi pin tombol dengan pull-up internal
  pinMode(btnUp,   INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);

  // Tampilkan angka awal (0) saat program pertama berjalan
  displayDigit(currentDigit);
}

void loop() {
  bool upState   = digitalRead(btnUp);    // Baca status tombol UP
  bool downState = digitalRead(btnDown);  // Baca status tombol DOWN

  // Deteksi tombol UP ditekan (transisi HIGH → LOW)
  if (lastUp == HIGH && upState == LOW) {
    delay(200);          // Debounce: cegah pembacaan ganda
    currentDigit++;      // Naikkan nilai counter
    if (currentDigit > 15) currentDigit = 0;  // Wrap-around ke 0 setelah F
    displayDigit(currentDigit);
  }

  // Deteksi tombol DOWN ditekan (transisi HIGH → LOW)
  if (lastDown == HIGH && downState == LOW) {
    delay(200);          // Debounce: cegah pembacaan ganda
    currentDigit--;      // Turunkan nilai counter
    if (currentDigit < 0) currentDigit = 15;  // Wrap-around ke F setelah 0
    displayDigit(currentDigit);
  }

  // Simpan status tombol saat ini sebagai referensi siklus berikutnya
  lastUp   = upState;
  lastDown = downState;
}
```

#### 🔍 Penjelasan Baris per Baris

**1. Deklarasi Pin Segmen**

```cpp
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};
```

Array yang memetakan urutan segmen (a sampai dp) ke pin-pin fisik Arduino. Urutan ini harus disesuaikan dengan koneksi kabel pada rangkaian.

**2. Deklarasi Pin Tombol**

```cpp
const int btnUp = 3;
const int btnDown = 2;
```

Mendefinisikan pin digital untuk masing-masing tombol. Tombol increment pada pin 3, tombol decrement pada pin 2.

**3. Pola Digit Heksadesimal**

```cpp
byte digitPattern[16][8] = { ... };
```

Matriks berisi 16 baris (untuk nilai 0–F) dan 8 kolom (untuk 8 segmen). Nilai `1` berarti segmen tersebut aktif, nilai `0` berarti segmen padam.

**4. Variabel Counter dan Status Tombol**

```cpp
int currentDigit = 0;
bool lastUp = HIGH;
bool lastDown = HIGH;
```

`currentDigit` menyimpan nilai yang sedang ditampilkan. `lastUp` dan `lastDown` mencatat kondisi tombol pada siklus sebelumnya — digunakan untuk mendeteksi momen penekanan (_rising/falling edge_).

**5. Fungsi `displayDigit()`**

```cpp
void displayDigit(int num) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(segmentPins[i], !digitPattern[num][i]);
  }
}
```

Membaca pola dari array `digitPattern` dan mengirimkannya ke pin-pin segmen. Operator `!` digunakan karena display bertipe _common anode_, sehingga logikanya terbalik.

**6. Deteksi Penekanan Tombol**

```cpp
if (lastUp == HIGH && upState == LOW) { ... }
```

Kondisi ini hanya terpenuhi pada momen tepat tombol ditekan (transisi dari `HIGH` ke `LOW`), bukan selama tombol ditahan. Teknik ini disebut _edge detection_.

**7. Batas dan Wrap-around Nilai**

```cpp
if (currentDigit > 15) currentDigit = 0;
if (currentDigit < 0) currentDigit = 15;
```

Memastikan nilai counter selalu berada dalam rentang **0 hingga 15**. Jika melewati batas atas (F), nilai kembali ke 0. Jika melewati batas bawah (0), nilai kembali ke F.

#### 🔁 Alur Eksekusi Program

| Aksi              | Perubahan Nilai  | Tampilan             |
| ----------------- | ---------------- | -------------------- |
| Tekan tombol UP   | `currentDigit++` | Naik satu langkah    |
| Tekan tombol DOWN | `currentDigit--` | Turun satu langkah   |
| Nilai melewati 15 | Reset ke 0       | Kembali ke angka `0` |
| Nilai melewati 0  | Reset ke 15      | Kembali ke huruf `F` |

> **Kesimpulan:** Program ini berhasil mengimplementasikan counter dua arah pada seven segment display dengan memanfaatkan deteksi tepi (_edge detection_) untuk responsivitas tombol yang akurat dan stabil.

---
