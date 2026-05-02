#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

// ================= PIN =================
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#define RELAY_PIN    4
#define LED_MERAH    5
#define LED_BIRU     6
#define BUZZER_PIN   7

#define ACCESS_DELAY 3000   // lebih aman biar solenoid tidak panas

bool sudahAkses = false;

// ================= SETUP =================
void setup()
{
  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);
  pinMode(LED_BIRU, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // HIGH trigger relay → LOW = OFF
  digitalWrite(RELAY_PIN, LOW);

  digitalWrite(LED_MERAH, LOW);
  digitalWrite(LED_BIRU, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  delay(200);

  finger.begin(57600); // kalau error → ganti 9600

  if (!finger.verifyPassword())
  {
    Serial.println("Sensor ERROR!");
    while (1)
    {
      digitalWrite(LED_MERAH, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);
      delay(200);
      digitalWrite(LED_MERAH, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      delay(200);
    }
  }

  Serial.println("Sistem Siap");
}

// ================= LOOP =================
void loop()
{
  int status = getFingerprint();

  if (status > 0 && !sudahAkses)
  {
    Serial.print("Akses ID: ");
    Serial.println(status);

    aksesDiterima();
    sudahAkses = true;
  }
  else if (status == -2)
  {
    aksesDitolak();
  }

  if (status == 0)
  {
    sudahAkses = false;
  }

  delay(50);
}

// ================= FINGERPRINT =================
int getFingerprint()
{
  int p = finger.getImage();

  if (p == FINGERPRINT_NOFINGER) return 0;
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -2;

  return finger.fingerID;
}

// ================= AKSES DITERIMA =================
void aksesDiterima()
{
  digitalWrite(LED_BIRU, HIGH);
  digitalWrite(LED_MERAH, LOW);

  digitalWrite(BUZZER_PIN, HIGH);

  digitalWrite(RELAY_PIN, HIGH); // ON
  delay(ACCESS_DELAY);
  digitalWrite(RELAY_PIN, LOW);  // OFF

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_BIRU, LOW);
}

// ================= AKSES DITOLAK =================
void aksesDitolak()
{
  Serial.println("Akses Ditolak");

  digitalWrite(LED_BIRU, LOW);

  // 🔴 LED merah + buzzer kedip 2x
  for (int i = 0; i < 2; i++)
  {
    digitalWrite(LED_MERAH, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(150);

    digitalWrite(LED_MERAH, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    delay(150);
  }
}
