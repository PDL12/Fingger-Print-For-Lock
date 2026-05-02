#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

// ================= SETUP =================
void setup()
{
  Serial.begin(9600);
  delay(1000);

  Serial.println("=== MODE ENROLL SIDIK JARI ===");

  finger.begin(57600); // kalau error → ganti 9600

  if (finger.verifyPassword())
  {
    Serial.println("Sensor SIAP");
  }
  else
  {
    Serial.println("Sensor ERROR!");
    while (1);
  }

  Serial.println("Masukkan ID (1 - 127):");
}

// ================= LOOP =================
void loop()
{
  if (Serial.available())
  {
    id = Serial.parseInt();

    if (id == 0)
    {
      Serial.println("ID tidak valid, coba lagi!");
      return;
    }

    Serial.print("Mendaftarkan ID: ");
    Serial.println(id);

    enrollFinger(id);
  }
}

// ================= ENROLL FUNCTION =================
uint8_t enrollFinger(uint8_t id)
{
  int p = -1;

  Serial.println("Tempelkan jari...");
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
  }

  Serial.println("Gambar diambil");

  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK)
  {
    Serial.println("Gagal convert");
    return p;
  }

  Serial.println("Angkat jari...");
  delay(2000);

  Serial.println("Tempel lagi jari yang sama...");
  p = 0;
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
  }

  Serial.println("Gambar kedua diambil");

  p = finger.image2Tz(2);
  if (p != FINGERPRINT_OK)
  {
    Serial.println("Gagal convert ke-2");
    return p;
  }

  p = finger.createModel();
  if (p != FINGERPRINT_OK)
  {
    Serial.println("Gagal membuat model");
    return p;
  }

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK)
  {
    Serial.println("✔ BERHASIL ENROLL!");
  }
  else
  {
    Serial.println("✖ GAGAL MENYIMPAN");
  }

  return p;
}
