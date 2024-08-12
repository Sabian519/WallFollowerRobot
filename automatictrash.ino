#include <NewPing.h>
#include <Ultrasonic.h>

// Mendefinisikan pin-pin yang digunakan
#define TRIG1 A0 // Pin trigger untuk sensor ultrasonik 1 (kiri)
#define ECHO1 A1 // Pin echo untuk sensor ultrasonik 1 (kiri)
#define TRIG2 A2 // Pin trigger untuk sensor ultrasonik 2 (tengah)
#define ECHO2 A3 // Pin echo untuk sensor ultrasonik 2 (tengah)
#define TRIG3 A4 // Pin trigger untuk sensor ultrasonik 3 (kanan)
#define ECHO3 A5 // Pin echo untuk sensor ultrasonik 3 (kanan)
//#define ENA 9 // Pin enable untuk motor driver
//#define IN1 8 // Pin input 1 untuk motor driver
//#define IN2 10 // Pin input 2 untuk motor driver
//#define IN3 11 // Pin input 3 untuk motor driver
//#define IN4 12 // Pin input 4 untuk motor driver
//#define ENB 13 // Pin enable untuk motor driver

// Mendefinisikan variabel-variabel yang digunakan
long durasi1, durasi2, durasi3; // Variabel untuk menyimpan durasi pulsa dari sensor ultrasonik
int jarak1, jarak2, jarak3; // Variabel untuk menyimpan jarak yang diukur oleh sensor ultrasonik
int kecepatan = 200; // Variabel untuk menyimpan nilai kecepatan motor (0-255)
int jarak_min = 15; // Variabel untuk menyimpan nilai jarak minimum yang diinginkan (cm)
int selisih = 5; // Variabel untuk menyimpan nilai selisih jarak yang diizinkan (cm)
int jarak;
long durasi;
// Fungsi untuk menggerakkan motor kiri ke depan dengan kecepatan tertentu
void maju_kiri(int v) {
  analogWrite(ENA, v); // Mengatur kecepatan motor kiri
  digitalWrite(IN1, HIGH); // Mengatur arah putaran motor kiri
  digitalWrite(IN2, LOW); // Mengatur arah putaran motor kiri
}

// Fungsi untuk menggerakkan motor kanan ke depan dengan kecepatan tertentu
void maju_kanan(int v) {
  analogWrite(ENB, v); // Mengatur kecepatan motor kanan
  digitalWrite(IN3, HIGH); // Mengatur arah putaran motor kanan
  digitalWrite(IN4, LOW); // Mengatur arah putaran motor kanan
}

// Fungsi untuk menggerakkan motor kiri ke belakang dengan kecepatan tertentu
void mundur_kiri(int v) {
  analogWrite(ENA, v); // Mengatur kecepatan motor kiri
  digitalWrite(IN1, LOW); // Mengatur arah putaran motor kiri
  digitalWrite(IN2, HIGH); // Mengatur arah putaran motor kiri
}

// Fungsi untuk menggerakkan motor kanan ke belakang dengan kecepatan tertentu
void mundur_kanan(int v) {
  analogWrite(ENB, v); // Mengatur kecepatan motor kanan
  digitalWrite(IN3, LOW); // Mengatur arah putaran motor kanan
  digitalWrite(IN4, HIGH); // Mengatur arah putaran motor kanan
}

// Fungsi untuk menghentikan motor kiri
void stop_kiri() {
  analogWrite(ENA, 0); // Mengatur kecepatan motor kiri menjadi nol
}

// Fungsi untuk menghentikan motor kanan
void stop_kanan() {
  analogWrite(ENB, 0); // Mengatur kecepatan motor kanan menjadi nol
}

// Fungsi untuk mengukur jarak dengan sensor ultrasonik
int baca_jarak(int trig, int echo) {
  digitalWrite(trig, LOW); // Mengirim pulsa rendah ke pin trigger
  delayMicroseconds(2); // Menunggu 2 mikrodetik
  digitalWrite(trig, HIGH); // Mengirim pulsa tinggi ke pin trigger
  delayMicroseconds(10); // Menunggu 10 mikrodetik
  digitalWrite(trig, LOW); // Mengirim pulsa rendah ke pin trigger
  long durasi = pulseIn(echo, HIGH); // Mengukur durasi pulsa yang diterima oleh pin echo
  int jarak = durasi * 0.034 / 2; // Menghitung jarak berdasarkan durasi pulsa
  return jarak; // Mengembalikan nilai jarak
}

// Fungsi untuk mengikuti dinding dengan sensor ultrasonik
void ikuti_dinding() {
  jarak1 = baca_jarak(TRIG1, ECHO1); // Membaca jarak dari sensor ultrasonik 1 (kiri)
  jarak2 = baca_jarak(TRIG2, ECHO2); // Membaca jarak dari sensor ultrasonik 2 (tengah)
  jarak3 = baca_jarak(TRIG3, ECHO3); // Membaca jarak dari sensor ultrasonik 3 (kanan)

  // Jika jarak tengah lebih besar dari jarak minimum
  if (jarak2 > jarak_min) {
    // Jika jarak kiri lebih besar dari jarak kanan
    if (jarak1 > jarak3) {
      // Jika jarak kiri lebih besar dari jarak minimum ditambah selisih
      if (jarak1 > jarak_min + selisih) {
        // Membelokkan robot ke kiri dengan kecepatan berbeda
        maju_kiri(kecepatan / 2);
        maju_kanan(kecepatan);
      }
      // Jika jarak kiri lebih kecil dari jarak minimum dikurangi selisih
      else if (jarak1 < jarak_min - selisih) {
        // Membelokkan robot ke kanan dengan kecepatan berbeda
        maju_kiri(kecepatan);
        maju_kanan(kecepatan / 2);
      }
      // Jika jarak kiri dalam rentang jarak minimum
      else {
        // Melaju lurus dengan kecepatan sama
        maju_kiri(kecepatan);
        maju_kanan(kecepatan);
      }
    }
    // Jika jarak kanan lebih besar dari jarak kiri
    else {
      // Jika jarak kanan lebih besar dari jarak minimum ditambah selisih
      if (jarak3 > jarak_min + selisih) {
        // Membelokkan robot ke kanan dengan kecepatan berbeda
        maju_kiri(kecepatan);
        maju_kanan(kecepatan / 2);
      }
      // Jika jarak kanan lebih kecil dari jarak minimum dikurangi selisih
      else if (jarak3 < jarak_min - selisih) {
        // Membelokkan robot ke kiri dengan kecepatan berbeda
        maju_kiri(kecepatan / 2);
        maju_kanan(kecepatan);
      }
      // Jika jarak kanan dalam rentang jarak minimum
      else {
        // Melaju lurus dengan kecepatan sama
        maju_kiri(kecepatan);
        maju_kanan(kecepatan);
      }
    }
  }
  // Jika jarak tengah lebih kecil dari jarak minimum
  else {
    // Jika jarak kiri lebih besar dari jarak kanan
    if (jarak1 > jarak3) {
      // Membelokkan robot ke kiri dengan mundur kanan
      stop_kiri();
      mundur_kanan(kecepatan);
    }
    // Jika jarak kanan lebih besar dari jarak kiri
    else {
      // Membelokkan robot ke kanan dengan mundur kiri
      stop_kanan();
      mundur_kiri(kecepatan);
    }
  }
}

// Fungsi setup yang dijalankan sekali saat awal program
void setup() {
  // Mengatur pin-pin sebagai input atau output
  Serial.begin(9600);
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(TRIG3, OUTPUT);
  pinMode(ECHO3, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
}

// Fungsi loop yang dijalankan terus-menerus selama program berjalan
void loop() {
  // Memanggil fungsi ikuti_dinding
  ikuti_dinding();
}