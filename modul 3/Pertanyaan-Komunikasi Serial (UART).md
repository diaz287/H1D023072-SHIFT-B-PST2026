# 📘 Praktikum Sistem Tertanam - Modul 3 Protokol Komunikasi

# Pertanyaan Praktikum
1. Jelaskan proses dari input keyboard hingga LED menyala/mati!
2. Mengapa digunakan Serial.available() sebelum membaca data? Apa yang terjadi jika baris tersebut dihilangkan?
3. Modifikasi program agar LED berkedip (blink) ketika menerima input '2' dengan kondisi jika ‘2’ aktif maka LED akan terus berkedip sampai perintah selanjutnya diberikan dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!
4. Tentukan apakah menggunakan delay() atau milis()! Jelaskan pengaruhnya terhadap sistem

---

# Jawaban

---

## **1. Jelaskan proses dari input keyboard hingga LED menyala/mati!**

Proses dimulai ketika pengguna mengetikkan karakter pada **Serial Monitor** di Arduino IDE. Data tersebut dikirim ke Arduino melalui komunikasi serial (UART) dalam bentuk karakter.

Arduino kemudian membaca data menggunakan fungsi `Serial.read()`. Nilai yang diterima akan dibandingkan menggunakan percabangan (`if`). Jika data yang diterima sesuai, maka Arduino akan mengontrol pin digital menggunakan fungsi `digitalWrite()`.

Sebagai contoh:

* Input `'1'` → LED menyala (HIGH)
* Input `'0'` → LED mati (LOW)

Dengan demikian, LED dapat merespons perintah dari pengguna secara langsung melalui komunikasi serial.

---

## **2. Mengapa digunakan `Serial.available()` sebelum membaca data? Apa yang terjadi jika dihilangkan?**

Fungsi `Serial.available()` digunakan untuk mengecek apakah terdapat data yang tersedia di buffer serial sebelum dilakukan pembacaan.

Jika fungsi ini digunakan, maka Arduino hanya akan membaca data ketika memang ada data yang masuk.

Namun jika `Serial.available()` dihilangkan:

* Arduino tetap mencoba membaca data meskipun tidak ada input
* Nilai yang terbaca bisa tidak valid (biasanya -1)
* Program dapat berjalan tidak stabil atau menghasilkan output yang salah

Oleh karena itu, `Serial.available()` sangat penting untuk memastikan pembacaan data berjalan dengan benar.

---

## **3. Modifikasi program agar LED berkedip (blink) ketika menerima input '2'**

### **Program Arduino (Modifikasi dari Kode Modul)**

```c
const int PIN_LED = 12;
bool blinkMode = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Ketik '1' untuk menyalakan LED, '0' untuk mematikan LED, '2' untuk blink");
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    char data = Serial.read();

    if (data == '1') {
      digitalWrite(PIN_LED, HIGH);
      blinkMode = false;
      Serial.println("LED ON");
    }
    else if (data == '0') {
      digitalWrite(PIN_LED, LOW);
      blinkMode = false;
      Serial.println("LED OFF");
    }
    else if (data == '2') {
      blinkMode = true;
      Serial.println("LED BLINK");
    }
    else if (data != '\n' && data != '\r') {
      Serial.println("Perintah tidak dikenal");
    }
  }

  // Mode berkedip
  if (blinkMode) {
    digitalWrite(PIN_LED, HIGH);
    delay(500);
    digitalWrite(PIN_LED, LOW);
    delay(500);
  }
}
```

### **Penjelasan Kode**

* `const int PIN_LED = 12;`
  Menentukan pin LED yang digunakan.

* `bool blinkMode = false;`
  Menyimpan status apakah LED dalam mode berkedip.

* `Serial.begin(9600);`
  Menginisialisasi komunikasi serial dengan baud rate 9600 bps.

* `Serial.available()`
  Mengecek apakah ada data yang masuk.

* `Serial.read()`
  Membaca data dari Serial Monitor.

* `if (data == '1')`
  Menyalakan LED dan mematikan mode blink.

* `if (data == '0')`
  Mematikan LED dan mematikan mode blink.

* `if (data == '2')`
  Mengaktifkan mode blink.

* `delay(500)`
  Memberikan jeda agar LED terlihat berkedip.

---

## **4. Tentukan apakah menggunakan `delay()` atau `millis()`! Jelaskan pengaruhnya**

Pada program di atas digunakan `delay()` untuk membuat efek kedipan LED.

Fungsi `delay()` bersifat **blocking**, artinya selama delay berjalan, Arduino tidak dapat membaca input baru dari Serial Monitor. Hal ini membuat sistem menjadi kurang responsif.

Sebagai alternatif, dapat digunakan `millis()` yang bersifat **non-blocking**, sehingga Arduino tetap bisa membaca input sambil menjalankan proses lain.

**Perbandingan:**

* `delay()` → sederhana, tetapi menghambat sistem
* `millis()` → lebih kompleks, tetapi lebih responsif dan real-time

---

## ✅ Kesimpulan Percobaan 3A

Komunikasi UART memungkinkan Arduino menerima input dari pengguna melalui Serial Monitor dan mengontrol perangkat seperti LED. Dengan tambahan fitur blink, sistem menjadi lebih interaktif. Penggunaan metode seperti `millis()` dapat meningkatkan performa sistem dibandingkan `delay()`.

---
